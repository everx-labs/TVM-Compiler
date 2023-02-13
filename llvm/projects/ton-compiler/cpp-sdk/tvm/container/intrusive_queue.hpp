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
#include <tvm/contract.hpp>

namespace tvm {

struct header {
  // Current active (last pushed) sub-element
  // nref is increased only if last pushed ref is full of its level
  uint8 nref;
  // Element id
  uint32 id;
};
struct __attribute__((tvm_tuple)) decomposed {
  unsigned    nref;     // decomposed header nref
  unsigned    id;       // decomposed header id
  __tvm_slice body_sl;  // decomposed body slice
  __tvm_tuple refs_tup; // decomposed refs

  header            hdr() const { return { uint8(nref), uint32(id) }; }
  slice             sl() const { return body_sl; }
  tuple_array<cell> refs() const { return tuple_array<cell>(refs_tup); }
};

/// Intrusive queue - re-uses free references in element cells for inter-element links
template<class Element>
class intrusive_queue {
public:
  using value_type = Element;
  using value_tup = tuple<Element>;
  using elem_info = refs_info::intrusive_elem_info<header, Element>;
  using this_type = intrusive_queue<value_type>;

  bool   empty() const { return size_ == 0; }
  uint32 last_id() const { return last_id_; }
  uint32 size() const { return size_; }
  uint8  root_lvl() const { return root_lvl_; }

  std::pair<header, value_tup> top_info() const {
    if (!rbegin_)
      return {};
    return parse_elem(rbegin_.get());
  }

  static std::pair<unsigned, this_type> push_impl(this_type v, value_tup val) {
    __builtin_tvm_printstr("__push_impl");
    __builtin_tvm_logflush();
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
  unsigned push(value_tup val) {
    auto [rv, new_this] = push_impl(*this, val);
    *this = new_this;
    return rv;
  }

  /// go deep at first elements and gather path
  __attribute__((noinline))
  static std::pair<cell, tuple_stack<cell>> go_deep_zero(cell cur) {
    __builtin_tvm_printstr("__go_deep_zero");
    __builtin_tvm_logflush();
    tuple_stack<cell> path;
    while (auto next = sub_elem(cur, 0u8)) {
      path.push(cur);
      cur = next.get();
    }
    return { cur, path };
  }

  /// go deep at first elements and gather path of `decomposed` tuples
  __attribute__((noinline))
  static std::pair<cell, tuple_stack<__tvm_tuple>> go_deep_zero2(cell cur) {
    __builtin_tvm_printstr("__go_deep_z2");
    __builtin_tvm_logflush();
    tuple_stack<__tvm_tuple> path;
    while (true) {
      auto [next, dec] = sub_elem2(cur, 0u8);
      if (!next) break;
      path.push(entuple(dec));
      cur = next.get();
    }
    return { cur, path };
  }

  __attribute__((noinline))
  static std::pair<std::optional<value_tup>, this_type> pop_opt_impl(this_type v) {
    __builtin_tvm_printstr("__pop_opt_impl");
    __builtin_tvm_logflush();
    if (!v.rbegin_)
      return { {}, v };
    auto [cur, path] = go_deep_zero2(v.rbegin_.get());
    auto [hdr, rv] = parse_elem(cur);
    v.rbegin_ = unroll_path_pop(path);
    if (--v.size_ == 0)
      v.root_lvl_ = 0;
    return { rv, v };
  }

  __attribute__((noinline))
  static this_type pop_impl(this_type v) {
    __builtin_tvm_printstr("__pop_impl");
    __builtin_tvm_logflush();
    require(v.rbegin_, error_code::iterator_overflow);
    auto [cur, path] = go_deep_zero2(v.rbegin_.get());
    v.rbegin_ = unroll_path_pop(path);
    if (--v.size_ == 0)
      v.root_lvl_ = 0;
    return v;
  }

  /// Pop element from the queue (pop front) and return the element if any
  std::optional<value_tup> pop_opt() {
    auto [rv, new_this] = pop_opt_impl(*this);
    *this = new_this;
    return rv;
  }

  /// Pop element from the queue (pop front). `iterator_overflow` error if empty.
  void pop() {
    *this = pop_impl(*this);
  }

  /// Returns null value_tup if empty
  static std::pair<unsigned, value_tup> front_with_idx_opt_impl(this_type v) {
    __builtin_tvm_printstr("__front1");
    __builtin_tvm_logflush();
    if (!v.rbegin_)
      return {};
    auto [cur, path] = go_deep_zero(v.rbegin_.get());
    auto [hdr, rv] = parse_elem(cur.get());
    return std::make_pair(hdr.id.get(), rv);
  }

  /// Get front element from queue with its index. Doesn't erase element from the queue.
  std::pair<unsigned, value_tup> front_with_idx_opt() const {
    return front_with_idx_opt_impl(*this);
  }

  static this_type change_front_impl(this_type v, value_tup val) {
    __builtin_tvm_printstr("__change_front");
    __builtin_tvm_logflush();
    require(!!v.rbegin_, error_code::iterator_overflow);
    auto [cur, path] = go_deep_zero(v.rbegin_.get());
    v.rbegin_ = unroll_path_change_front(path, cur.get(), val);
    return v;
  }

  /// Change front element in the queue.
  void change_front(value_tup val) {
    *this = change_front_impl(*this, val);
  }

  /// Find element by index
  opt<value_tup> find(unsigned idx) const {
    __builtin_tvm_printstr("__find");
    __builtin_tvm_logflush();
    if (!rbegin_ || idx > last_id_)
      return {};
    optcell cur = rbegin_;
    auto [hdr, refs] = decompose(cur.get());
    while (true) {
      if (hdr.id == idx) {
        // Element is found
        return parse_elem(cur.get()).second;
      } else if (hdr.id.get() < idx) {
        // Child elements have lesser id than parent, so we can't find such an element
        return {};
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
            break;
          }
        }
        if (!child_found)
          return {};
      }
    }
    return {};
  }

  /// Find and modify. func must be `opt<Elem> func(Elem elem)`.
  template<typename Func>
  bool find_and_modify(unsigned idx, Func func) {
    __builtin_tvm_printstr("__find_modify");
    __builtin_tvm_logflush();
    require(rbegin_ && idx <= last_id_, error_code::iterator_overflow);
    optcell cur = rbegin_;
    tuple_stack<cell> path;
    tuple_stack<unsigned> path_ids;
    auto [hdr, refs] = decompose(cur.get());
    while (true) {
      if (hdr.id.get() == idx) {
        // Element is found
        auto elem = parse_elem(cur.get()).second;
        // If modified is empty, we need to delete element, otherwise - we need to modify element and fixup its parents
        auto modified = func(elem);
        if (size_ == 1) {
          if (modified) {
            rbegin_ = build_elem(hdr.id, *modified);
          } else {
            rbegin_ = cell{};
            size_ = 0;
            root_lvl_ = 0;
          }
          return true;
        }
        if (modified) {
          cell v = build_elem(hdr.id, value_tup(*modified));
          v = rebuild(v, refs, hdr);
          rbegin_ = unroll_path_modify(path, path_ids, v);
        } else {
          // last child of cur, rebuilt in its place (with all nested last child elements promoted recursively)
          optcell rebuilt_child = move_last_child_up(cur.get());
          rbegin_ = unroll_path_erase(path, path_ids, rebuilt_child);
        }
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

  /// Find element by index and erase if func(elem) is true.
  /// Returns element value if the element was deleted
  template<typename Func>
  value_tup erase_if(unsigned idx, Func func) {
    __builtin_tvm_printstr("__erase_if");
    __builtin_tvm_logflush();
    require(rbegin_ && idx <= last_id_, error_code::iterator_overflow);
    optcell cur = rbegin_;
    tuple_stack<cell> path;
    tuple_stack<unsigned> path_ids;
    auto [hdr, refs] = decompose(cur.get());
    while (true) {
      if (hdr.id.get() == idx) {
        // Element is found
        auto elem = parse_elem(cur.get()).second;
        if (!func(elem))
          return {};
        if (size_ == 1) {
          rbegin_ = cell{};
          size_ = 0;
          root_lvl_ = 0;
          return elem;
        }
        // last child of cur, rebuilt in its place (with all nested last child elements promoted recursively)
        optcell rebuilt_child = move_last_child_up(cur.get());
        rbegin_ = unroll_path_erase(path, path_ids, rebuilt_child);
        return elem;
      } else if (hdr.id.get() < idx) {
        // Child elements have lesser id than parent, so we can't find such an element
        return {};
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
          return {};
      }
    }
    return {};
  }

  __attribute__((noinline))
  static dict_array<value_type> subpart_impl(this_type v, unsigned begin_id, unsigned end_id) {
    dict_array<value_type> rv;
    if (!v.rbegin_)
      return rv;
    tuple_stack<cell> stack;
    stack.push(v.rbegin_.get());
    uint32 last_id(0);
    while (optcell cl = stack.extract()) {
      auto [hdr, refs] = decompose(cl.get());
      if (refs.empty() || (last_id + 1 == hdr.id)) {
        last_id = hdr.id;
        rv.push_back(parse_elem(cl.get()).second.unpack());
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
  dict_array<value_type> subpart(unsigned begin_id, unsigned end_id) const {
    return subpart_impl(*this, begin_id, end_id);
  }

private:
  // Unroll with updating zero reference
  __attribute__((noinline))
  static cell unroll_set_zero(tuple_stack<cell> path, cell last) {
    __builtin_tvm_printstr("__unroll_set_0");
    __builtin_tvm_logflush();
    while (optcell cur = path.extract()) {
      auto [hdr, refs] = decompose(cur.get());
      refs.set_at(0, last);
      last = rebuild(cur.get(), refs, hdr);
    }
    return last;
  }

  // Unroll with updating zero reference
  __attribute__((noinline))
  static cell unroll_set_zero2(tuple_stack<__tvm_tuple> path, cell last) {
    __builtin_tvm_printstr("__unroll_set_02");
    __builtin_tvm_logflush();
    while (auto cur_opt = path.extract()) {
      tuple<decomposed> cur_tup(*cur_opt);
      decomposed cur = cur_tup.unpack();
      auto refs = cur.refs();
      refs.set_at(0, last);
      last = rebuild2(cur.sl(), refs, cur.hdr());
    }
    return last;
  }

  // Unroll path of affected elements and create modified cells (for pop operation)
  static cell unroll_path_pop(tuple_stack<__tvm_tuple> path) {
    __builtin_tvm_printstr("__unroll_pop");
    __builtin_tvm_logflush();
    if (path.empty())
      return {};
    // First element should be rebuilt without first reference
    tuple<decomposed> leaf_t(*path.extract());
    decomposed leaf = leaf_t.unpack();
    auto refs = leaf.refs();
    refs.pop_front();
    cell last = rebuild2(leaf.sl(), refs, leaf.hdr());
    return unroll_set_zero2(path, last);
  }

  // Unroll path of affected elements and create modified cells (for change_front operation)
  static cell unroll_path_change_front(tuple_stack<cell> path, cell leaf, value_tup v) {
    __builtin_tvm_printstr("__unroll_front");
    __builtin_tvm_logflush();
    auto [leaf_hdr, leaf_refs] = decompose(leaf);
    cell last = build_elem(leaf_hdr.id, v);
    return unroll_set_zero(path, last);
  }

  // Unroll path of affected elements and create modified cells (for erase_if operation)
  __attribute__((noinline))
  static cell unroll_path_erase(
    tuple_stack<cell>     path,         // Path of parent cells (root->parent.lvl1->parent.lvl2->...->parent) in stack form.
    tuple_stack<unsigned> path_ids,     // Path of parent ref ids (# of parent.lvl1 ref in root | # of parent.lvl2 ref in parent.lvl1 | ...) in stack form.
    optcell               rebuilt_child // Re-built child cell (last child of the deleted element)
  ) {
    __builtin_tvm_printstr("__unroll_erase");
    __builtin_tvm_logflush();
    require(!path.empty(), error_code::iterator_overflow);
    cell parent = path.extract().get();
    auto parent_idx = path_ids.extract();
    require(!!parent_idx, error_code::iterator_overflow);
    cell last;
    auto [parent_hdr, parent_refs] = decompose(parent);
    if (rebuilt_child) {
      // if leaf (deleted element) has child refs, we need to replace leaf with its last child
      parent_refs.set_at(*parent_idx, rebuilt_child.get());
      last = rebuild(parent, parent_refs, parent_hdr);
    } else {
      // otherwise, we need to remove reference to this element
      parent_refs.erase_at(*parent_idx);
      last = rebuild(parent, parent_refs, { .nref = parent_hdr.nref - 1, .id = parent_hdr.id });
    }

    while (optcell cur = path.extract()) {
      auto parent_idx = path_ids.extract();
      require(!!parent_idx, error_code::iterator_overflow);

      auto [hdr, refs] = decompose(cur.get());
      refs.set_at(*parent_idx, last);
      last = rebuild(cur.get(), refs, hdr);
    }
    return last;
  }
  // Unroll path of affected elements and create modified cells (for find_and_modify operation)
  __attribute__((noinline))
  static cell unroll_path_modify(
    tuple_stack<cell>     path,         // Path of parent cells (root->parent.lvl1->parent.lvl2->...->parent) in stack form.
    tuple_stack<unsigned> path_ids,     // Path of parent ref ids (# of parent.lvl1 ref in root | # of parent.lvl2 ref in parent.lvl1 | ...) in stack form.
    cell                  leaf_modified // Leaf with modified data
  ) {
    __builtin_tvm_printstr("__unroll_mod");
    __builtin_tvm_logflush();
    require(!path.empty(), error_code::iterator_overflow);
    cell parent = path.extract().get();
    auto parent_idx = path_ids.extract();
    require(!!parent_idx, error_code::iterator_overflow);
    auto [parent_hdr, parent_refs] = decompose(parent);
    parent_refs.set_at(*parent_idx, leaf_modified.get());
    cell last = rebuild(parent, parent_refs, parent_hdr);

    while (optcell cur = path.extract()) {
      auto parent_idx = path_ids.extract();
      require(!!parent_idx, error_code::iterator_overflow);

      auto [hdr, refs] = decompose(cur.get());
      refs.set_at(*parent_idx, last);
      last = rebuild(cur.get(), refs, hdr);
    }
    return last;
  }

  /// In case of erasing non-empty element, we need to move its last child to replace the element.
  /// And when we moving non-empty last child, we need to move its last child upper etc...
  /// Returns null if `cur` is an empty element (no child elements)
  __attribute__((noinline))
  static optcell move_last_child_up(cell cur) {
    __builtin_tvm_printstr("__move_last");
    __builtin_tvm_logflush();
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
    __builtin_tvm_printstr("__unroll");
    __builtin_tvm_logflush();
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
  __always_inline
  static optcell sub_elem(cell cl, uint8 nref) {
    if (is_full_node(nref))
      return {};
    auto [h, refs] = decompose(cl);
    if (nref >= refs.size())
      return {};
    return { refs[nref.get()] };
  }

  /// Sub element #nref
  __always_inline
  static std::pair<optcell, decomposed> sub_elem2(cell cl, uint8 nref) {
    if (is_full_node(nref))
      return {};
    auto dec = decompose2(cl);
    if (nref >= dec.refs().size())
      return {};
    return { optcell{ dec.refs()[nref.get()] }, dec };
  }

  /// Get header
  __always_inline
  static header get_header(cell cl) {
    return parse<header>(cl.ctos());
  }

  /// Build new (leaf) element
  __attribute__((noinline))
  static cell build_elem(uint32 id, value_tup val) {
    __builtin_tvm_printstr("__build_elem");
    __builtin_tvm_logflush();
    return build_chain_static(std::make_pair(header{ .nref = uint8(elem_info::max_refs), .id = id }, val.unpack()));
  }

  /// Build new root element
  __always_inline
  static cell build_elem(uint32 id, value_tup val, cell child) {
    return rebuild(build_elem(id, val), tuple_array<cell>(child), { .nref = 1u8, .id = id } );
  }

  /// Parse element from cell
  __attribute__((noinline))
  static std::pair<header, value_tup> parse_elem(cell cl) {
    __builtin_tvm_printstr("__parse_elem");
    __builtin_tvm_logflush();
    parser p(cl.ctos());
    [[maybe_unused]] auto [hdr, =p] = parse_continue<header>(p);
    require(!!hdr, error_code::iterator_overflow);
    using est = estimate_element<header>;
    return { *hdr, value_tup(parse_chain_static<value_type, est::max_bits>(p)) };
  }

  /// Decompose element - extract header and references to other elements
  __attribute__((noinline))
  static std::pair<header, tuple_array<cell>> decompose(cell cl) {
    __builtin_tvm_printstr("__decompose");
    __builtin_tvm_logflush();
    parser p(cl.ctos());
    auto [hdr, =p] = parse_continue<header>(p);
    auto refs = refs_info::extract_refs<elem_info>(p.sl());
    return { *hdr, refs };
  }

  /// Decompose element - extract header and references to other elements
  __attribute__((noinline))
  static decomposed decompose2(cell cl) {
    __builtin_tvm_printstr("__decom2");
    __builtin_tvm_logflush();
    parser p(cl.ctos());
    auto [hdr, =p] = parse_continue<header>(p);
    require(!!hdr, error_code::integer_overflow);
    auto refs = refs_info::extract_refs<elem_info>(p.sl());
    return { hdr->nref.get(), hdr->id.get(), p.sl(), refs };
  }

  /// Rebuild element with a new references and new header
  __attribute__((noinline))
  static cell rebuild(cell old, tuple_array<cell> refs, header hdr) {
    __builtin_tvm_printstr("__rebuild");
    __builtin_tvm_logflush();
    parser p(old.ctos());
    auto [old_hdr, =p] = parse_continue<header>(p);
    return refs_info::rebuild_refs<elem_info>(build(hdr), p.sl(), refs);
  }

  /// Rebuild element with a new references and new header
  __attribute__((noinline))
  static cell rebuild2(slice sl, tuple_array<cell> refs, header hdr) {
    __builtin_tvm_printstr("__rebuild2");
    __builtin_tvm_logflush();
    return refs_info::rebuild_refs<elem_info>(build(hdr), sl, refs);
  }

  /// Is it a full node of this lvl
  __always_inline
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

