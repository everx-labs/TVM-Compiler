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

  bool   empty() const { return size_ == 0; }
  uint32 size() const { return size_; }
  uint8  root_lvl() const { return root_lvl_; }

  std::pair<header, value_type> top_info() const {
    if (!rbegin_)
      return {};
    return parse_elem(rbegin_.get());
  }

  /// Push element in the queue (push back)
  void push(value_type val) {
    ++size_;
    // First element in the queue
    if (!rbegin_) {
      rbegin_ = build_elem(size_, val);
      return;
    }
    cell old = rbegin_.get();
    auto hdr = parse<header>(old.ctos());
    // If rbegin is full of his lvl, new rbegin is set and old will be first ref of the new rbegin.
    // nref for new root will be 1, nref for other cells unchanged.
    // root_lvl is increased.
    if (is_full_node(hdr)) {
      rbegin_ = build_elem(size_, val, old);
      root_lvl_++;
      return;
    }

    // Otherwise, we need recursively get sub-element from nref
    tuple_stack<cell> path;
    path.push(build_elem(size_, val, old));

    auto cur = old;
    auto cur_lvl = root_lvl_;
    while(true) {
      auto hdr = get_header(cur);
      if (is_full_node(hdr)) {
        rbegin_ = unroll_path(path, cur_lvl);
        break;
      } else {
        require(cur_lvl > 0, 98);
        path.push(cur);
        cur = sub_elem(cur, hdr).get();
        cur_lvl--;
        continue;
      }
    }
  }

  std::optional<value_type> pop_opt() {
  }

  void pop() {
  }

  dict_array<value_type> subpart(unsigned begin_id, unsigned end_id) {
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
  // Unroll path of affected elements and create modified cells
  static cell unroll_path(tuple_stack<cell> path, uint8 lvl) {
    // ===== 2 custom step for leaf and first parent ===== //
    require(path.size() >= 2, 99);
    optcell leaf = path.extract();
    optcell par1 = path.extract();
    require(leaf && par1, 99);
    // If it is a leaf of lvl 0 - means it is a full node (nref = max_refs).
    auto [leaf_hdr, leaf_refs] = decompose(leaf.get());
    cell new_leaf = rebuild(leaf.get(), {}, { .nref = uint8((lvl == 0) ? elem_info::max_refs : 0), .id = leaf_hdr.id });
    leaf_refs.push_back(new_leaf);
    auto [old_hdr, old_refs] = decompose(par1.get());
    cell new_par1 = rebuild(par1.get(), leaf_refs, { .nref = leaf_hdr.nref + 1, .id = old_hdr.id });
    cell new_child = new_par1;
    old_refs.pop_back();
    old_refs.push_back(new_child);
    if (!path.empty()) {
      // ===== loop for other elements in the path ===== //
      while (auto cl = path.extract()) {
        auto [hdr, refs] = decompose(cl.get());
        new_child = rebuild(cl.get(), old_refs, { .nref = old_hdr.nref, .id = hdr.id });
        refs.pop_back();
        refs.push_back(new_child);
        old_refs = refs;
        old_hdr = hdr;
      }
    }
    return new_child;
  }

  /// Sub element #nref
  static optcell sub_elem(cell cl, header hdr) {
    if (is_full_node(hdr))
      return {};
    auto [h, refs] = decompose(cl);
    if (hdr.nref >= refs.size())
      return {};
    return { refs[hdr.nref.get()] };
  }

  /// Get header
  static header get_header(cell cl) {
    return parse<header>(cl.ctos());
  }

  /// Build new (leaf) element
  static cell build_elem(uint32 id, value_type val) {
    return build_chain_static(std::make_pair(header{ .nref = uint8(elem_info::max_refs), .id = id }, val));
  }

  /// Build new root element
  static cell build_elem(uint32 id, value_type val, cell child) {
    return rebuild(build_elem(id, val), tuple_array<cell>(child), { .nref = 1u8, .id = id } );
  }

  static std::pair<header, value_type> parse_elem(cell cl) {
    parser p(cl.ctos());
    [[maybe_unused]] auto [hdr, =p] = parse_continue<header>(p);
    require(!!hdr, 99);
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
  static bool is_full_node(header hdr) {
    return hdr.nref >= elem_info::max_refs;
  }

public:
  uint32  size_;
  uint8   root_lvl_;
  optcell rbegin_;
};

} // namespace tvm

