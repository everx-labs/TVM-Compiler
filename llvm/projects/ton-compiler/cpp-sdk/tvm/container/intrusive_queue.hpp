#pragma once

#include <optional>
#include <tvm/schema/make_parser.hpp>
#include <tvm/tuple_stack.hpp>
#include <tvm/container/tuple_array.hpp>
#include <tvm/schema/estimate_element.hpp>
#include <tvm/schema/chain_tuple.hpp>
#include <tvm/container/refs_info.hpp>
#include <tvm/dict_array.hpp>
#include <tvm/suffixes.hpp>

namespace tvm {

/// Intrusive queue - re-uses free references in element cells for inter-element links
template<class Element>
class intrusive_queue {
public:
  struct header {
    // Current active (last pushed) sub-element
    // nref is increased only if last pushed ref is full of its level
    uint8 nref;
    // Element id
    uint32 id;
  };
  using value_type = Element;
  using elem_info = refs_info::intrusive_elem_info<header, Element>;
  using this_type = intrusive_queue<value_type>;

  bool   empty() const { return size_ == 0; }
  uint32 last_id() const { return last_id_; }
  uint32 size() const { return size_; }
  uint8  root_lvl() const { return root_lvl_; }

  std::pair<header, value_type> top_info() const {
    if (!rbegin_)
      return {};
    return parse_elem(rbegin_.get());
  }

  static std::pair<unsigned, this_type> push_impl(this_type v, value_type val) {
    ++v.size_;
    ++v.last_id_;
    // First element in the queue
    if (!v.rbegin_) {
      v.rbegin_ = build_elem(v.last_id_, val);
      return { v.last_id_.get(), v };
    }
    cell old = v.rbegin_.get();
    auto hdr = parse<header>(old.ctos());
    // If rbegin is full of his lvl, new rbegin is set and old will be first ref of the new rbegin.
    // nref for new root will be 1, nref for other cells unchanged.
    // root_lvl is increased.
    if (is_full_node(hdr.nref)) {
      v.rbegin_ = build_elem(v.last_id_, val, old);
      v.root_lvl_++;
      return { v.last_id_.get(), v };
    }

    // Otherwise, we need recursively get sub-element from nref
    tuple_stack<cell> path;
    path.push(build_elem(v.last_id_, val, old));

    auto cur = old;
    auto cur_lvl = v.root_lvl_;
    while(true) {
      auto hdr = get_header(cur);
      if (is_full_node(hdr.nref)) {
        v.rbegin_ = unroll_path(path, cur_lvl);
        break;
      } else {
        require(cur_lvl > 0, error_code::iterator_overflow);
        path.push(cur);
        cur = sub_elem(cur, hdr.nref).get();
        cur_lvl--;
        if (cur.isnull()) {
          v.rbegin_ = unroll_path(path, cur_lvl);
          break;
        }
        continue;
      }
    }
    return { v.last_id_.get(), v };
  }

  /// Push element in the queue (push back). Returns new element's id.
  unsigned push(value_type val) {
    auto [rv, new_this] = push_impl(*this, val);
    *this = new_this;
    return rv;
  }

  /// Pop element from the queue (pop front) and return the element if any
  std::optional<value_type> pop_opt() {
    if (!rbegin_)
      return {};
    optcell cur = rbegin_;
    tuple_stack<cell> path;
    while (true) {
      auto next = sub_elem(cur.get(), 0u8);
      if (!next) {
        auto [hdr, rv] = parse_elem(cur.get());
        rbegin_ = unroll_path_pop(path);
        if (--size_ == 0)
          root_lvl_ = 0;
        return rv;
      } else {
        path.push(cur.get());
        cur = next;
      }
    }
  }

  /// Pop element from the queue (pop front). `iterator_overflow` error if empty.
  void pop() {
    auto rv = pop_opt();
    require(!!rv, error_code::iterator_overflow);
  }

  /// Get front element from queue with its index. Doesn't erase element from the queue.
  std::optional<std::pair<unsigned, value_type>> front_with_idx_opt() const {
    if (!rbegin_)
      return {};
    optcell cur = rbegin_;
    while (auto next = sub_elem(cur.get(), 0u8)) {
      cur = next;
    }
    auto [hdr, rv] = parse_elem(cur.get());
    return std::make_pair(hdr.id.get(), rv);
  }

  /// Change front element in the queue.
  void change_front(value_type v) {
    require(!!rbegin_, error_code::iterator_overflow);
    optcell cur = rbegin_;
    tuple_stack<cell> path;
    while (auto next = sub_elem(cur.get(), 0u8)) {
      path.push(cur.get());
      cur = next;
    }
    rbegin_ = unroll_path_change_front(path, cur.get(), v);
  }

  /// Find element by index and erase if func(elem) is true.
  /// Returns true if the element was deleted
  template<typename Func>
  bool erase_if(unsigned idx, Func func) {
    require(rbegin_ && idx <= last_id_, error_code::iterator_overflow);
    optcell cur = rbegin_;
    tuple_stack<cell> path;
    tuple_stack<unsigned> path_ids;
    auto [hdr, refs] = decompose(cur.get());
    while (true) {
      if (hdr.id.get() == idx) {
        // Element is found
        if (!func(parse_elem(cur.get()).second))
          return false;
        if (size_ == 1) {
          rbegin_ = cell{};
          size_ = 0;
          root_lvl_ = 0;
          return true;
        }
        // last child of cur, rebuilt in its place (with all nested last child elements promoted recursively)
        optcell rebuilt_child = move_last_child_up(cur.get());
        rbegin_ = unroll_path_erase(path, path_ids, cur.get(), rebuilt_child);
        return true;
      } else if (hdr.id.get() < idx) {
        // Child elements have lesser id than parent, so we can't find such an element
        return false;
      } else {
        // We need to find first child with hdr.id >= idx
        bool child_found = false;
        for (unsigned i = 0; i < refs.size(); ++i) {
          cell ch = refs[i];
          auto [ch_hdr, ch_refs] = decompose(ch);
          if (ch_hdr.id >= idx) {
            hdr = ch_hdr;
            refs = ch_refs;
            child_found = true;
            path.push(cur.get());
            path_ids.push(i); // we need to save it to use during unroll (remove/fix reference)
            break;
          }
        }
        if (!child_found)
          return false;
      }
    }
    return false;
  }

  dict_array<value_type> subpart(unsigned begin_id, unsigned end_id) const {
    dict_array<value_type> rv;
    if (!rbegin_)
      return rv;
    tuple_stack<cell> stack;
    stack.push(rbegin_.get());
    uint32 last_id(0);
    while (optcell cl = stack.extract()) {
      auto [hdr, refs] = decompose(cl.get());
      if (refs.empty() || (last_id + 1 == hdr.id)) {
        last_id = hdr.id;
        rv.push_back(parse_elem(cl.get()).second);
      } else {
        stack.push(cl.get());
        unsigned sz = refs.size();
        for (unsigned i = 0; i < sz; ++i) {
          stack.push(refs[sz - i - 1]);
        }
      }
    }
    return rv;
  }

private:
  // Unroll path of affected elements and create modified cells (for pop operation)
  __attribute__((noinline))
  static cell unroll_path_pop(tuple_stack<cell> path) {
    if (path.empty())
      return {};
    // First element should be rebuilt without first reference
    optcell leaf = path.extract();
    auto [leaf_hdr, leaf_refs] = decompose(leaf.get());
    leaf_refs.pop_front();
    cell last = rebuild(leaf.get(), leaf_refs, leaf_hdr);
    while (optcell cur = path.extract()) {
      auto [hdr, refs] = decompose(cur.get());
      refs.set_at(0, last);
      last = rebuild(cur.get(), refs, hdr);
    }
    return last;
  }

  // Unroll path of affected elements and create modified cells (for change_front operation)
  static cell unroll_path_change_front(tuple_stack<cell> path, cell leaf, value_type v) {
    auto [leaf_hdr, leaf_refs] = decompose(leaf);
    cell last = build_elem(leaf_hdr.id, v);
    while (optcell cur = path.extract()) {
      auto [hdr, refs] = decompose(cur.get());
      refs.set_at(0, last);
      last = rebuild(cur.get(), refs, hdr);
    }
    return last;
  }

  // Unroll path of affected elements and create modified cells (for erase_if operation)
  __attribute__((noinline))
  static cell unroll_path_erase(tuple_stack<cell> path, tuple_stack<unsigned> path_ids, cell leaf, optcell rebuilt_child) {
    require(!path.empty(), error_code::iterator_overflow);
    cell parent = path.extract().get();
    auto parent_idx = path_ids.extract();
    auto [leaf_hdr, leaf_refs] = decompose(leaf);
    cell last;
    auto [parent_hdr, parent_refs] = decompose(parent);
    if (rebuilt_child) {
      // if leaf (deleted element) has child refs, we need to replace leaf with its last child
      parent_refs.set_at(parent_idx, rebuilt_child.get());
      last = rebuild(parent, parent_refs, parent_hdr);
    } else {
      // otherwise, we need to remove reference to this element
      parent_refs.erase_at(parent_idx);
      last = rebuild(parent, parent_refs, { .nref = parent_hdr.nref - 1, .id = parent_hdr.id });
    }

    while (optcell cur = path.extract()) {
      auto parent_idx = path_ids.extract();
      require(!!parent_idx, error_code::iterator_overflow);

      auto [hdr, refs] = decompose(cur.get());
      refs.set_at(parent_idx, last);
      last = rebuild(cur, refs, hdr);
    }
    return last;
  }

  /// In case of erasing non-empty element, we need to move its last child to replace the element.
  /// And when we moving non-empty last child, we need to move its last child upper etc...
  /// Returns null if `cur` is an empty element (no child elements)
  __attribute__((noinline))
  static optcell move_last_child_up(cell cur) {
    tuple_stack<cell> path;
    header hdr;
    // first going down and preparing path of last child elements (which still have its own child elements)
    do {
      auto [=hdr, refs] = decompose(cur);
      auto sz = refs.size();
      if (sz) {
        path.push(cur);
        cur = refs[sz - 1];
        continue;
      }
      break;
    } while(true);
    if (path.empty())
      return {};
    // Now we have path of non-empty last child elements in `path` and leaf element in `cur`

    // First, rebuilding leaf element with first parent's refs without one (back)
    cell leaf = cur;
    optcell parent = path.extract();
    auto [parent_hdr, parent_refs] = decompose(parent.get());
    parent_refs.pop_back();
    cell last = rebuild(leaf, parent_refs, { .nref = parent_hdr.nref - 1, .id = hdr.id });
    cur = parent.get();
    hdr = parent_hdr;
    // In loop, rebuilding current element with its parent's refs with last ref changed to 'last'
    while (optcell parent = path.extract()) {
      auto [parent_hdr, parent_refs] = decompose(parent.get());
      parent_refs.pop_back();
      parent_refs.push_back(last);
      last = rebuild(cur, parent_refs, { .nref = parent_hdr.nref, .id = hdr.id });
      hdr = parent_hdr;
    }
    return { last };
  }

  // Unroll path of affected elements and create modified cells (for push operation)
  __attribute__((noinline))
  static cell unroll_path(tuple_stack<cell> path, uint8 lvl) {
    // ===== 2 custom step for leaf and first parent ===== //
    require(path.size() >= 2, error_code::iterator_overflow);
    optcell leaf = path.extract();
    optcell par1 = path.extract();
    require(leaf && par1, error_code::iterator_overflow);
    // If it is a leaf of lvl 0 - means it is a full node (nref = max_refs).
    auto [leaf_hdr, leaf_refs] = decompose(leaf.get());
    cell new_leaf = rebuild(leaf.get(), {}, { .nref = uint8((lvl == 0) ? elem_info::max_refs : 0), .id = leaf_hdr.id });
    leaf_refs.push_back(new_leaf);
    auto [old_hdr, old_refs] = decompose(par1.get());
    if (lvl == 0) ++leaf_hdr.nref;
    cell new_par1 = rebuild(par1.get(), leaf_refs, { .nref = leaf_hdr.nref, .id = old_hdr.id });
    cell new_child = new_par1;
    old_refs.pop_back();
    old_refs.push_back(new_child);
    auto prev_hdr = leaf_hdr;
    if (!path.empty()) {
      // ===== loop for other elements in the path ===== //
      while (auto cl = path.extract()) {
        auto [hdr, refs] = decompose(cl.get());
        if (is_full_node(prev_hdr.nref)) ++old_hdr.nref;
        new_child = rebuild(cl.get(), old_refs, { .nref = old_hdr.nref, .id = hdr.id });
        refs.pop_back();
        refs.push_back(new_child);
        prev_hdr = old_hdr;
        old_refs = refs;
        old_hdr = hdr;
      }
    }
    return new_child;
  }

  /// Sub element #nref
  static optcell sub_elem(cell cl, uint8 nref) {
    if (is_full_node(nref))
      return {};
    auto [h, refs] = decompose(cl);
    if (nref >= refs.size())
      return {};
    return { refs[nref.get()] };
  }

  /// Get header
  __attribute__((noinline))
  static header get_header(cell cl) {
    return parse<header>(cl.ctos());
  }

  /// Build new (leaf) element
  __attribute__((noinline))
  static cell build_elem(uint32 id, value_type val) {
    return build_chain_static(std::make_pair(header{ .nref = uint8(elem_info::max_refs), .id = id }, val));
  }

  /// Build new root element
  __attribute__((noinline))
  static cell build_elem(uint32 id, value_type val, cell child) {
    return rebuild(build_elem(id, val), tuple_array<cell>(child), { .nref = 1u8, .id = id } );
  }

  /// Parse element from cell
  __attribute__((noinline))
  static std::pair<header, value_type> parse_elem(cell cl) {
    parser p(cl.ctos());
    [[maybe_unused]] auto [hdr, =p] = parse_continue<header>(p);
    require(!!hdr, error_code::iterator_overflow);
    using est = estimate_element<header>;
    return { *hdr, parse_chain_static<value_type, est::max_bits>(p) };
  }

  /// Decompose element - extract header and references to other elements
  __attribute__((noinline))
  static std::pair<header, tuple_array<cell>> decompose(cell cl) {
    parser p(cl.ctos());
    auto [hdr, =p] = parse_continue<header>(p);
    auto refs = refs_info::extract_refs<elem_info>(p.sl());
    return { *hdr, refs };
  }

  /// Rebuild element with a new references and new header
  __attribute__((noinline))
  static cell rebuild(cell old, tuple_array<cell> refs, header hdr) {
    parser p(old.ctos());
    auto [old_hdr, =p] = parse_continue<header>(p);
    return refs_info::rebuild_refs<elem_info>(build(hdr), p.sl(), refs);
  }

  // Is it a full node of this lvl
  static bool is_full_node(uint8 nref) {
    return nref >= elem_info::max_refs;
  }

public:
  uint32  last_id_;
  uint32  size_;
  uint8   root_lvl_;
  optcell rbegin_;
};

} // namespace tvm

