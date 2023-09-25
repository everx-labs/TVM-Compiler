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

#include "intrusive.hpp"

namespace tvm {

/// Intrusive queue - re-uses free references in element cells for inter-element links
template<class Element>
class intrusive_queue {
public:
  using value_type = Element;
  using value_tup = tuple<Element>;
  using header = intrusive::header;
  using decomposed = intrusive::decomposed;
  using elem_info = refs_info::intrusive_elem_info<header, Element>;
  using this_type = intrusive_queue<value_type>;
  using intr_logic = intrusive::logic<Element>;
  using iterator = typename intr_logic::iterator;
  using path_t = typename intr_logic::path_t;

  bool   empty() const { return size_ == 0; }
  uint32 last_id() const { return last_id_; }
  uint32 size() const { return size_; }
  uint8  root_lvl() const { return root_lvl_; }

  std::pair<header, value_tup> top_info() const {
    if (!rbegin_)
      return {};
    return intr_logic::parse_elem(rbegin_.get());
  }

  static std::pair<unsigned, this_type> push_impl(this_type v, value_tup val) {
    INTR_LOG("__push_impl");
    ++v.size_;
    ++v.last_id_;
    // First element in the queue
    if (!v.rbegin_) {
      v.rbegin_ = intr_logic::build_elem(v.last_id_, val);
      return { v.last_id_.get(), v };
    }
    cell old = v.rbegin_.get();
    auto hdr = get_header(old);
    // If rbegin is full of his lvl, new rbegin is set and old will be first ref of the new rbegin.
    // nref for new root will be 1, nref for other cells unchanged.
    // root_lvl is increased.
    if (is_full_node(hdr.nref)) {
      v.rbegin_ = intr_logic::build_elem(v.last_id_, val, old);
      v.root_lvl_++;
      return { v.last_id_.get(), v };
    }

    // Otherwise, we need recursively get sub-element from nref
    tuple_stack<cell> path;
    path.push(intr_logic::build_elem(v.last_id_, val, old));

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

  __attribute__((noinline))
  static std::pair<std::optional<value_tup>, this_type> pop_opt_impl(this_type v) {
    INTR_LOG("__pop_opt_impl");
    if (!v.rbegin_)
      return { {}, v };
    auto [cur, path] = intr_logic::go_deep_zero(v.rbegin_.get(), 0, path_t{});
    auto sl = cur.index<slice>(offsetof(decomposed, body_sl));
    auto rv = intr_logic::parse_elem(sl);
    if (path.empty()) {
      v.reset();
      return { rv, v };
    }
    v.rbegin_ = intr_logic::unroll_path_pop(path);
    return { rv, v };
  }

  __attribute__((noinline))
  static this_type pop_impl(this_type v) {
    INTR_LOG("__pop_impl");
    require(v.rbegin_, error_code::iterator_overflow);
    [[maybe_unused]] auto [cur, path] = intr_logic::go_deep_zero(v.rbegin_.get(), 0, path_t{});
    if (path.empty()) {
      v.reset();
      return v;
    }
    v.rbegin_ = intr_logic::unroll_path_pop(path);
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
    INTR_LOG("__front1");
    if (!v.rbegin_)
      return {};
    auto [cur, path] = intr_logic::go_deep_zero(v.rbegin_.get(), 0, {});
    auto sl = cur.index<slice>(offsetof(decomposed, body_sl));
    auto id = cur.index<unsigned>(offsetof(decomposed, id));
    auto rv = intr_logic::parse_elem(sl);
    return std::make_pair(id, rv);
  }

  /// Get front element from queue with its index. Doesn't erase element from the queue.
  std::pair<unsigned, value_tup> front_with_idx_opt() const {
    return front_with_idx_opt_impl(*this);
  }

  static this_type change_front_impl(this_type v, value_tup val) {
    INTR_LOG("__change_front");
    require(!!v.rbegin_, error_code::iterator_overflow);
    auto [leaf, path] = intr_logic::go_deep_zero(v.rbegin_.get());
    v.rbegin_ = intr_logic::unroll_path_change_front(path, leaf, val);
    return v;
  }

  /// Change front element in the queue.
  void change_front(value_tup val) {
    *this = change_front_impl(*this, val);
  }

  /// Find element by index
  opt<value_tup> find(unsigned idx) const {
    INTR_LOG("__find");
    if (!rbegin_ || idx > last_id_)
      return {};
    optcell cur = rbegin_;
    auto dec = intr_logic::decompose(cur.get(), 0);
    while (true) {
      auto hdr = dec.hdr();
      if (hdr.id == idx) {
        // Element is found
        return intr_logic::parse_elem(dec.sl());
      } else if (hdr.id.get() < idx) {
        // Child elements have lesser id than parent, so we can't find such an element
        return {};
      } else {
        auto refs = dec.refs();
        // We need to find first child with hdr.id >= idx
        bool child_found = false;
        for (unsigned i = 0, sz = refs.size(); i < sz; ++i) {
          auto ch = intr_logic::decompose(refs[i], i);
          if (ch.hdr().id >= idx) {
            child_found = true;
            dec = ch;
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
    INTR_LOG("__find_modify");
    require(rbegin_ && idx <= last_id_, error_code::iterator_overflow);
    optcell cur = rbegin_;
    path_t path;
    auto dec = intr_logic::decompose(cur.get(), 0);
    while (true) {
      auto [hdr, refs] = dec.hdr_and_refs();
      if (hdr.id.get() == idx) {
        INTR_LOG("__elem_found");
        // Element is found
        auto elem = intr_logic::parse_elem(dec.sl());
        // If modified is empty, we need to delete element, otherwise - we need to modify element and fixup its parents
        auto modified = func(elem);
        if (size_ == 1) {
          if (modified)
            rbegin_ = intr_logic::build_elem(hdr.id, *modified);
          else
            reset();
          INTR_LOG("__find_mod1");
          return true;
        }
        if (modified) {
          INTR_LOG("__find_mod2.1");
          rbegin_ = intr_logic::unroll_path_modify(path, value_tup(*modified), dec.num, hdr, refs);
        } else {
          INTR_LOG("__find_mod2.2");
          // last child of cur, rebuilt in its place (with all nested last child elements promoted recursively)
          optcell rebuilt_child = intr_logic::move_last_child_up(dec);
          rbegin_ = intr_logic::unroll_path_erase(path, rebuilt_child, dec.num);
          --size_;
        }
        INTR_LOG("__find_mod2");
        return true;
      } else if (hdr.id.get() < idx) {
        // Child elements have lesser id than parent, so we can't find such an element
        INTR_LOG("__find_mod3");
        return false;
      } else {
        // We need to find first child with hdr.id >= idx
        bool child_found = false;
        for (unsigned i = 0, sz = refs.size(); i < sz; ++i) {
          auto cl = refs[i];
          auto hdr = get_header(cl);
          if (hdr.id >= idx) {
            child_found = true;
            path.push(entuple(dec));
            INTR_LOG("__ch_found");
            dec = intr_logic::decompose(cl, i);
            break;
          }
        }
        if (!child_found) {
          INTR_LOG("__find_mod4");
          return false;
        }
      }
    }
    INTR_LOG("__find_mod5");
    return false;
  }

  /// Find element by index and erase if func(elem) is true.
  /// Returns element value if the element was deleted
  template<typename Func>
  value_tup erase_if(unsigned idx, Func func) {
    INTR_LOG("__erase_if");
    require(rbegin_ && idx <= last_id_, error_code::iterator_overflow);
    optcell cur = rbegin_;
    path_t path;
    auto dec = intr_logic::decompose(cur.get(), 0);
    while (true) {
      auto [hdr, refs] = dec.hdr_and_refs();
      if (hdr.id.get() == idx) {
        // Element is found
        auto elem = intr_logic::parse_elem(dec.sl());
        if (!func(elem))
          return {};
        if (size_ == 1) {
          reset();
          return elem;
        }
        // last child of cur, rebuilt in its place (with all nested last child elements promoted recursively)
        optcell rebuilt_child = intr_logic::move_last_child_up(dec);
        rbegin_ = intr_logic::unroll_path_erase(path, rebuilt_child, dec.num);
        --size_;
        return elem;
      } else if (hdr.id.get() < idx) {
        // Child elements have lesser id than parent, so we can't find such an element
        return {};
      } else {
        // We need to find first child with hdr.id >= idx
        bool child_found = false;
        for (unsigned i = 0; i < refs.size(); ++i) {
          auto cl = refs[i];
          auto hdr = get_header(cl);
          if (hdr.id >= idx) {
            child_found = true;
            path.push(entuple(dec));
            dec = intr_logic::decompose(cl, i);
            break;
          }
        }
        if (!child_found)
          return {};
      }
    }
    return {};
  }

  /// Erase all elements before it: [begin, it) and save modified it
  void erase_until_and_modify(iterator it) {
    INTR_LOG("__erase_blk");
    if (it.is_end()) { // last element deleted
      reset();
      return;
    }
    rbegin_ = intr_logic::unroll_drop_left(it);
    size_ -= it.sz;
  }

  iterator begin() {
    return iterator::create_begin(rbegin_.get());
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
      auto ch = intr_logic::decompose(cl.get(), 0);
      auto hdr = ch.hdr();
      auto refs = ch.refs();
      // We going up if refs is empty (leaf elem) or when we processing non-empty element second time (in back path, after its children)
      // We detecting back path if at least first child is processed - last_id > first child id
      if (refs.empty() || (last_id >= get_header(refs[0]).id)) {
        last_id = hdr.id;
        rv.push_back(intr_logic::parse_elem(cl.get()).second.unpack());
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
  __attribute__((noinline))
  static dict_array<std::pair<uint32, value_type>> subpart2_impl(this_type v, unsigned begin_id, unsigned end_id) {
    dict_array<std::pair<uint32, value_type>> rv;
    if (!v.rbegin_)
      return rv;
    tuple_stack<cell> stack;
    stack.push(v.rbegin_.get());
    uint32 last_id(0);
    while (optcell cl = stack.extract()) {
      auto ch = intr_logic::decompose(cl.get(), 0);
      auto hdr = ch.hdr();
      auto refs = ch.refs();
      // We going up if refs is empty (leaf elem) or when we processing non-empty element second time (in back path, after its children)
      // We detecting back path if at least first child is processed - last_id > first child id
      if (refs.empty() || (last_id >= get_header(refs[0]).id)) {
        last_id = hdr.id;
        rv.push_back({hdr.id, intr_logic::parse_elem(cl.get()).second.unpack()});
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
  dict_array<std::pair<uint32, value_type>> subpart2(unsigned begin_id, unsigned end_id) const {
    return subpart2_impl(*this, begin_id, end_id);
  }
  __attribute__((noinline))
  static dict_array<std::pair<uint32, uint32>> graph_impl(this_type v) {
    dict_array<std::pair<uint32, uint32>> rv;
    if (!v.rbegin_)
      return rv;
    tuple_stack<cell> stack;
    stack.push(v.rbegin_.get());
    while (optcell cl = stack.extract()) {
      auto ch = intr_logic::decompose(cl.get(), 0);
      auto [hdr, refs] = ch.hdr_and_refs();
      unsigned sz = refs.size();
      for (unsigned i = 0; i < sz; ++i)
        stack.push(refs[sz - i - 1]);
      for (unsigned i = 0; i < sz; ++i)
        rv.push_back({hdr.id, get_header(refs[i]).id});
    }
    return rv;
  }
  dict_array<std::pair<uint32, uint32>> graph() const {
    return graph_impl(*this);
  }

private:
  // Unroll path of affected elements and create modified cells (for push operation)
  __attribute__((noinline))
  static cell unroll_path(tuple_stack<cell> path, uint8 lvl) {
    INTR_LOG("__unroll");
    // ===== 2 custom step for leaf and first parent ===== //
    require(path.size() >= 2, error_code::iterator_overflow);
    optcell leaf = path.extract();
    optcell par1 = path.extract();
    require(leaf && par1, error_code::iterator_overflow);
    // If it is a leaf of lvl 0 - means it is a full node (nref = max_refs).
    auto dec = intr_logic::decompose(leaf.get(), 0);
    auto [leaf_hdr, leaf_refs] = dec.hdr_and_refs();
    cell new_leaf = intr_logic::rebuild(dec.sl(), {}, { .nref = uint8((lvl == 0) ? elem_info::max_refs : 0), .id = leaf_hdr.id });
    leaf_refs.push_back(new_leaf);
    auto dec_old = intr_logic::decompose(par1.get(), 0);
    auto [old_hdr, old_refs] = dec_old.hdr_and_refs();
    if (lvl == 0) ++leaf_hdr.nref;
    cell new_par1 = intr_logic::rebuild(dec_old.sl(), leaf_refs, { .nref = leaf_hdr.nref, .id = old_hdr.id });
    cell new_child = new_par1;
    old_refs.pop_back();
    old_refs.push_back(new_child);
    auto prev_hdr = leaf_hdr;
    if (!path.empty()) {
      // ===== loop for other elements in the path ===== //
      while (auto cl = path.extract()) {
        auto cur = intr_logic::decompose(cl.get(), 0);
        auto [hdr, refs] = cur.hdr_and_refs();
        if (is_full_node(prev_hdr.nref)) ++old_hdr.nref;
        new_child = intr_logic::rebuild(cur.sl(), old_refs, { .nref = old_hdr.nref, .id = hdr.id });
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
    auto [h, refs] = intr_logic::decompose(cl, 0).hdr_and_refs();
    if (nref >= refs.size())
      return {};
    return { refs[nref.get()] };
  }

  /// Get header
  __always_inline
  static header get_header(cell cl) {
    return parse<header>(cl.ctos());
  }

  /// Is it a full node of this lvl
  __always_inline
  static bool is_full_node(uint8 nref) {
    return nref >= elem_info::max_refs;
  }

  /// Reset to empty
  __always_inline
  void reset() {
    rbegin_ = cell{};
    size_ = 0;
    root_lvl_ = 0;
  }

public:
  uint32  last_id_;
  uint32  size_;
  uint8   root_lvl_;
  optcell rbegin_;
};

} // namespace tvm

