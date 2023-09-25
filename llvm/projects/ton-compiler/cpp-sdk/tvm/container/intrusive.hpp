#pragma once

#include <tvm/container/tuple_array.hpp>
#include <tvm/tuple_stack.hpp>

namespace tvm { namespace intrusive {

#if 0
#define INTR_LOG(x) __builtin_tvm_printstr(x); \
                    __builtin_tvm_logflush();
#else
#define INTR_LOG(x)
#endif

/// Element header
struct header {
  // Current active (last pushed) sub-element
  // nref is increased only if last pushed ref is full of its level.
  // IT IS NOT REFERENCES COUNT, ITS *FULL* REFERENCES COUNT.
  uint8 nref;
  // Element id
  uint32 id;
};

/// Element decomposed info
struct __attribute__((tvm_tuple)) decomposed {
  unsigned    nref;     ///< header nref
  unsigned    id;       ///< header id
  unsigned    num;      ///< number in parent refs
  __tvm_slice body_sl;  ///< body slice
  __tvm_tuple refs_tup; ///< refs

  header             hdr() const { return { uint8(nref), uint32(id) }; }
  slice              sl() const { return body_sl; }
  tuple_array<cell> refs() const { return tuple_array<cell>(refs_tup); }
  std::pair<header, tuple_array<cell>> hdr_and_refs() const {
    return { hdr(), refs() };
  }
};

/// Intrusive logic (helper functions)
template<typename Element>
struct logic {
  using elem_info = refs_info::intrusive_elem_info<header, Element>;
  using value_type = Element;
  using value_tup = tuple<Element>;
  using path_t = tuple_stack<__tvm_tuple>;
  using this_t = logic<Element>;

  /// Decompose element - extract header and references to other elements
  __attribute__((noinline))
  static decomposed decompose(cell cl, unsigned num) {
    INTR_LOG("__decomIt");
    parser p(cl.ctos());
    auto [hdr, =p] = parse_continue<header>(p);
    require(!!hdr, error_code::integer_overflow);
    auto refs = refs_info::extract_refs<elem_info>(p.sl());
    return { hdr->nref.get(), hdr->id.get(), num, p.sl(), refs };
  }

  /// Build new element
  __attribute__((noinline))
  static cell build_elem(header hdr, value_tup val) {
    INTR_LOG("__build_elIt");
    return build_chain_static(std::make_pair(hdr, val.unpack()));
  }

  __always_inline
  static cell build_elem(uint32 id, value_tup val) {
    return build_elem(header{ .nref = uint8(elem_info::max_refs), .id = id }, val);
  }

  /// Build new root element
  __always_inline
  static cell build_elem(uint32 id, value_tup val, cell child) {
    return rebuild2(build_elem(id, val), tuple_array<cell>(child), { .nref = 1u8, .id = id } );
  }

  __attribute__((noinline))
  static cell build_elem(header hdr, value_tup val, tuple_array<cell> refs) {
    return rebuild2(build_elem(hdr.id, val), refs, hdr);
  }

  /// Rebuild element with a new references and new header
  __attribute__((noinline))
  static cell rebuild(slice sl, tuple_array<cell> refs, header hdr) {
    INTR_LOG("__rebuildIt");
    return refs_info::rebuild_refs<elem_info>(build(hdr), sl, refs);
  }

  /// Rebuild element with a new references and new header
  __always_inline
  static cell rebuild2(cell old, tuple_array<cell> refs, header hdr) {
    parser p(old.ctos());
    auto [old_hdr, =p] = parse_continue<header>(p);
    return rebuild(p.sl(), refs, hdr);
  }

  /// Sub element #nref
  __always_inline
  static opt<decomposed> sub_elem(decomposed parent, unsigned ref_num) {
    if (ref_num >= parent.refs().size())
      return {};
    return decompose(parent.refs()[ref_num], ref_num);
  }

  /// go deep at first elements and gather path of `decomposed` tuples
  __attribute__((noinline))
  static std::pair<tuple<decomposed>, path_t> go_deep_zero(cell parent, unsigned ref_num, path_t path) {
    INTR_LOG("__go_deep0It");
    auto cur = decompose(parent, ref_num);
    while (true) {
      auto next = sub_elem(cur, 0);
      if (!next) break;
      path.push(entuple(cur));
      cur = *next;
    }
    return { entuple(cur), path };
  }

  /// Parse element
  __attribute__((noinline))
  static value_tup parse_elem(slice body_sl) {
    INTR_LOG("__parse_elIt");
    using est = estimate_element<header>;
    return entuple(parse_chain_static<value_type, est::max_bits>(parser(body_sl)));
  }

  /// Parse element from cell
  __attribute__((noinline))
  static std::pair<header, value_tup> parse_elem(cell cl) {
    parser p(cl.ctos());
    [[maybe_unused]] auto [hdr, =p] = parse_continue<header>(p);
    require(!!hdr, error_code::iterator_overflow);
    return { *hdr, parse_elem(p.sl()) };
  }

  /// Iterator for intrusive container
  class iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = tuple<Element>;
    using difference_type = int;
    //using pointer = value_type*;
    //using reference = value_type&;

    path_t            path;
    tuple<decomposed> cur;
    uint32            sz;
    value_type        modified;

    static iterator create_begin(cell root) {
      if (root.isnull())
        return {};
      auto [dec, path] = go_deep_zero(root, 0, path_t{});
      return iterator{path, dec, 0u32, {}};
    }
    static iterator create_end() { return {}; }

    iterator& operator++() {
      *this = go_next(*this);
      return *this;
    }
    iterator operator++(int) {
      return go_next(*this);
    }

    unsigned idx() const { return cur.index<unsigned>(offsetof(decomposed, id)); }

    value_type val() const {
      auto sl = cur.index<slice>(offsetof(decomposed, body_sl));
      return parse_elem(sl);
    }

    value_type operator*() const {
      return val();
    }

    void modify(value_type v) {
      modified = v;
    }

    std::pair<unsigned, value_type> idx_and_val() const { return { idx(), val() }; }

    bool is_end() const { return cur.isnull(); }

    /// Iterate to the next element
    static iterator go_next(iterator v) {
      require(!v.is_end(), error_code::iterator_overflow);
      if (v.path.empty())
        return {};
      tuple<decomposed> parent(v.path.top());
      unsigned next_ref = v.cur.index<unsigned>(offsetof(decomposed, num)) + 1;
      auto parent_refs = parent.index<tuple_array<cell>>(offsetof(decomposed, refs_tup));
      if (next_ref < parent_refs.size()) {
        // go_sibling - right and max down
        cell cl = parent_refs[next_ref];
        auto [deep, deep_path] = go_deep_zero(cl, next_ref, v.path);
        v.cur = deep;
        v.path = deep_path;
      } else {
        // go_parent - up
        v.cur = tuple<decomposed>(*v.path.extract());
      }
      ++v.sz;
      return v;
    }
  };

  /// Unroll with erasing left references (before \p it) and storing modified \p it
  __attribute__((noinline))
  static cell unroll_drop_left(iterator it) {
    INTR_LOG("__unroll_dropL");
    auto v = it.cur.unpack();
    cell last = it.modified.isnull() ? rebuild(v.sl(), {}, v.hdr()) : build_elem(v.hdr(), it.modified);
    unsigned rnum = v.num;

    while (auto cur = it.path.extract()) {
      auto v = tuple<decomposed>(*cur).unpack();
      auto refs = v.refs();
      refs.drop_front(rnum);
      refs.set_at(0, last);
      last = rebuild(v.sl(), refs, v.hdr());
      rnum = v.num;
    }
    return last;
  }

  // Unroll with updating zero reference
  __attribute__((noinline))
  static cell unroll_set_zero(path_t path, cell last) {
    INTR_LOG("__unroll_set0It");
    while (auto cur_opt = path.extract()) {
      auto cur = tuple<decomposed>(*cur_opt).unpack();
      auto refs = cur.refs();
      refs.set_at(0, last);
      last = rebuild(cur.sl(), refs, cur.hdr());
    }
    return last;
  }

  // Unroll path of affected elements and create modified cells (for pop operation)
  static cell unroll_path_pop(path_t path) {
    INTR_LOG("__unroll_pop");
    require(!path.empty(), error_code::iterator_overflow);
    // First element should be rebuilt without first reference
    auto leaf = tuple<decomposed>(*path.extract()).unpack();
    auto refs = leaf.refs();
    refs.pop_front();
    cell last = rebuild(leaf.sl(), refs, leaf.hdr());
    return unroll_set_zero(path, last);
  }

  // Unroll path of affected elements and create modified cells (for change_front operation)
  static cell unroll_path_change_front(path_t path, tuple<decomposed> leaf, value_tup v) {
    INTR_LOG("__unroll_front");
    auto id = leaf.index<unsigned>(offsetof(decomposed, id));
    cell last = build_elem(id, v);
    return unroll_set_zero(path, last);
  }

  // Unroll path of affected elements and create modified cells (for find_and_modify operation)
  __attribute__((noinline))
  static cell unroll_path_modify(
    path_t            path,          // Path of decomposed parents (root->parent.lvl1->parent.lvl2->...->parent) in stack form.
    value_tup         leaf_modified, // Leaf with modified data
    unsigned          leaf_refnum,   // Leaf refnum
    header            leaf_hdr,      // Leaf header
    tuple_array<cell> leaf_refs      // Leaf refs
  ) {
    INTR_LOG("__unroll_mod");

    cell last = build_elem(leaf_hdr, leaf_modified, leaf_refs);
    unsigned last_refnum = leaf_refnum;

    while (auto cur_opt = path.extract()) {
      auto cur = tuple<decomposed>(*cur_opt).unpack();
      auto refs = cur.refs();
      refs.set_at(last_refnum, last);
      last = rebuild(cur.sl(), refs, cur.hdr());
      last_refnum = cur.num;
    }
    return last;
  }

  // Unroll path of affected elements and create modified cells (for erase_if operation)
  __attribute__((noinline))
  static cell unroll_path_erase(
    path_t    path,          // Path of decomposed parents (root->parent.lvl1->parent.lvl2->...->parent) in stack form.
    optcell   rebuilt_child, // Re-built child cell (last child of the deleted element)
    unsigned  leaf_refnum    // Leaf refnum
  ) {
    INTR_LOG("__unroll_erase");
    if (path.empty())
      return rebuilt_child.get();
    auto cur = tuple<decomposed>(*path.extract()).unpack();
    cell last;
    unsigned last_refnum = cur.num;
    if (rebuilt_child) {
      INTR_LOG("__unroll_er1");
      auto refs = cur.refs();
      // if leaf (deleted element) has child refs, we need to replace leaf with its last child
      refs.set_at(leaf_refnum, rebuilt_child.get());
      last = rebuild(cur.sl(), refs, cur.hdr());
    } else {
      INTR_LOG("__unroll_er2");
      auto [hdr, refs] = cur.hdr_and_refs();
      // otherwise, we need to remove reference to this element
      refs.erase_at(leaf_refnum);
      last = rebuild(cur.sl(), refs, { .nref = uint8(refs.size()), .id = hdr.id });
    }

    while (auto cur_opt = path.extract()) {
      auto cur = tuple<decomposed>(*cur_opt).unpack();
      auto refs = cur.refs();
      refs.set_at(last_refnum, last);
      last = rebuild(cur.sl(), refs, cur.hdr());
      last_refnum = cur.num;
    }
    return last;
  }

  /// In case of erasing non-empty element, we need to move its last child to replace the element.
  /// And when we moving non-empty last child, we need to move its last child upper etc...
  /// Returns null if `cur` is an empty element (no child elements)
  __attribute__((noinline))
  static optcell move_last_child_up(decomposed cur) {
    INTR_LOG("__move_last");
    path_t path;
    // first going down and preparing path of last child elements (which still have its own child elements)
    do {
      auto refs = cur.refs();
      auto sz = refs.size();
      if (sz) {
        path.push(entuple(cur));
        cur = decompose(refs[sz - 1], 0);
        continue;
      }
      break;
    } while(true);
    if (path.empty())
      return {};
    // Now we have path of non-empty last child elements in `path` and leaf element in `cur`

    // First, rebuilding leaf element with first parent's refs without one (back)
    auto leaf = cur;
    auto parent = tuple<decomposed>(*path.extract()).unpack();
    auto [parent_hdr, parent_refs] = parent.hdr_and_refs();
    parent_refs.pop_back();
    cell last = rebuild(leaf.sl(), parent_refs, { .nref = uint8(parent_refs.size()), .id = leaf.hdr().id });
    auto ch = parent;

    // In loop, rebuilding current element with its parent's refs with last ref changed to 'last'
    while (auto parent_opt = path.extract()) {
      auto parent = tuple<decomposed>(*parent_opt).unpack();
      auto [parent_hdr, parent_refs] = parent.hdr_and_refs();
      parent_refs.pop_back();
      parent_refs.push_back(last);
      last = rebuild(ch.sl(), parent_refs, { .nref = parent_hdr.nref, .id = ch.hdr().id });
      ch = parent;
    }
    return { last };
  }
};

}} // namespace tvm::intrusive

