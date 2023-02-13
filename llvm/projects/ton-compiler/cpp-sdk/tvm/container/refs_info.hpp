/// References info about complex element to re-use free references in intrusive containers

#pragma once

#include <boost/hana/ext/std/tuple.hpp>

#include <optional>
#include <tvm/schema/make_parser.hpp>
#include <tvm/tuple_stack.hpp>
#include <tvm/schema/estimate_element.hpp>
#include <tvm/schema/chain_tuple.hpp>
#include <tvm/container/tuple_array.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/flatten.hpp>
#include <boost/hana/append.hpp>
#include <boost/hana/size.hpp>
#include <boost/hana/for_each.hpp>

namespace tvm { namespace refs_info {

template<unsigned BeginRef, typename RefsTup>
struct cell_info {
  /// Occupied references count
  static constexpr unsigned refs_count = decltype(hana::size(RefsTup{}))::value;
  /// Free references count
  static constexpr unsigned free_refs = cell::max_refs - refs_count;

  /// Begin ref number in external references array
  static constexpr unsigned begin_ref = BeginRef;
  /// End ref number in external references array
  static constexpr unsigned end_ref = BeginRef + free_refs;

  /// References hana::tuple (ref_info_elem/ref_info_child)
  using tup = RefsTup;
};

struct ref_info_elem {};

template<typename T>
struct ref_info_child {
  using tup = T;
};

template<class _Tp>
struct is_ref : std::false_type {};
template<class _Tp>
struct is_ref<ref<_Tp>> : std::true_type {};

template<class _Tp>
struct extract_subref {};
template<class _Tp>
struct extract_subref<ref<_Tp>> {
  using type = _Tp;
};

template<class _Tp>
struct is_child : std::false_type {};
template<class _Tp>
struct is_child<ref_info_child<_Tp>> : std::true_type {};

template<class _Tp>
struct extract_child {};
template<class _Tp>
struct extract_child<ref_info_child<_Tp>> {
  using type = _Tp;
};

template<typename LinearTup>
constexpr auto process_refs(LinearTup LTup) {
  BOOST_HANA_CONSTEXPR_LAMBDA auto proc_ref = [](auto Tup, auto E) {
    using T = decltype(E);
    if constexpr (is_ref<T>::value) {
      using SubT = typename extract_subref<T>::type;
      using ProcessedSub = decltype(process_refs(std::declval<SubT>()));
      using elem_t = ref_info_child<ProcessedSub>;
      return hana::append(Tup, hana::type_c<elem_t>);
    } else {
      using est = estimate_element<T>;
      // TODO: support optional-like ref elements
      static_assert(est::min_refs == est::max_refs);
      static_assert(est::max_refs < 2);
      if constexpr (est::max_refs > 0)
        return hana::append(Tup, hana::type_c<ref_info_elem>);
      else
        return Tup;
    }
  };
  return hana::fold_left(LTup, hana::make_tuple(), proc_ref);
};

template<class T> struct ref_info_printer;

template<class T>
struct map_ref_printer {
  static constexpr auto value = ref_info_printer<T>::value;
};
template<class Elem>
struct map_ref_printer<hana::tuple<Elem>> {
  static constexpr auto value = ref_info_printer<typename Elem::type>::value;
};
template<class Elem, class... Elems>
struct map_ref_printer<hana::tuple<Elem, Elems...>> {
  static constexpr auto value = ref_info_printer<typename Elem::type>::value + ","_s + map_ref_printer<hana::tuple<Elems...>>::value;
};
template<>
struct map_ref_printer<hana::tuple<>> {
  static constexpr auto value = ""_s;
};

template<class T>
struct ref_info_printer {
  static constexpr auto value = "unknown"_s;
};
template<>
struct ref_info_printer<ref_info_elem> {
  static constexpr auto value = "*"_s;
};
template<typename Tup>
struct ref_info_printer<ref_info_child<Tup>> {
  static constexpr auto value = "ref("_s + map_ref_printer<Tup>::value + ")"_s;
};

template<class LinearTup>
struct RefsMap {
  //cell_info_tup
};

template<typename RefsTup>
constexpr unsigned calc_free_refs() {
  constexpr unsigned my_free_refs = cell::max_refs - decltype(hana::size(RefsTup{}))::value;
  BOOST_HANA_CONSTEXPR_LAMBDA auto child_free_refs = [](unsigned count, auto E) {
    using T = typename decltype(E)::type;
    if constexpr (is_child<T>::value) {
      using SubT = typename extract_child<T>::type;
      return count + calc_free_refs<SubT>();
    } else {
      return count;
    }
  };
  return my_free_refs + hana::fold_left(RefsTup{}, 0, child_free_refs);
}

template<class Header, class Element>
struct intrusive_elem_info {
  using hdr_est = estimate_element<Header>;
  using data_tup_t = to_std_tuple_t<Element>;
  using LinearTup = decltype(make_chain_tuple<hdr_est::max_bits, 0>(data_tup_t{}));
  using refs_map = decltype(process_refs(std::declval<LinearTup>()));

  // Maximum references from one element to another elements
  static constexpr unsigned max_refs = calc_free_refs<refs_map>();
};

/// For each cell in the tree, extract "extra" refs (free)
template<typename RefsInfo>
tuple_array<cell> extract_refs_impl(tuple_array<cell> arr, slice sl) {
  constexpr unsigned sz = decltype(hana::size(RefsInfo{}))::value;
  if constexpr (sz < cell::max_refs) {
    unsigned refs_sz = sl.srefs();
    if (refs_sz > sz) {
      slice extra_sl = __builtin_tvm_scutlast(sl, 0, refs_sz - sz);
      parser p(extra_sl);
      for (unsigned i = sz; i < refs_sz; ++i) {
        arr.push_back(p.ldref());
      }
    }
  }
  parser p(sl);
  BOOST_HANA_CONSTEXPR_LAMBDA auto f = [&p](tuple_array<cell> st, auto E) {
    using T = typename decltype(E)::type;
    if constexpr (is_child<T>::value) {
      using SubT = typename extract_child<T>::type;
      return extract_refs_impl<SubT>(st, p.ldrefrtos());
    } else {
      p.ldref();
      return st;
    }
  };
  return hana::fold_left(RefsInfo{}, arr, f);
}

/// For each cell in the tree, extract "extra" refs (free)
template<typename RefsInfo>
tuple_array<cell> extract_refs(slice sl) {
  return extract_refs_impl<typename RefsInfo::refs_map>({}, sl);
}

/// We need to convert element from old to a copy with modified references
/// First we need to process children cells.
/// For tail cell we keeping all bits and native references (cutting all extra refs).
/// Placing it in the builder. And then add external refs. And return finalized new cell.
///
template<typename RefsInfo>
cell rebuild_refs_impl(builder root, slice old, tuple_array<cell> refs, unsigned begin_ref) {
  constexpr unsigned sz = decltype(hana::size(RefsInfo{}))::value;
  constexpr unsigned free_sz = cell::max_refs - sz;
  slice only_bits = __builtin_tvm_scutfirst(old, old.sbits(), 0);
  root.stslice(only_bits);
  parser p(old);

  unsigned cur_ref = begin_ref + free_sz;
  auto f = [&root, &p, refs, &cur_ref](auto E) {
    using T = typename decltype(E)::type;
    if constexpr (is_child<T>::value) {
      using SubT = typename extract_child<T>::type;
      root.stref(rebuild_refs_impl<SubT>(builder(), p.ldrefrtos(), refs, cur_ref));
      constexpr unsigned child_free_refs = calc_free_refs<SubT>();
      cur_ref += child_free_refs;
    } else {
      root.stref(p.ldref());
    }
  };
  hana::for_each(RefsInfo{}, f);

  unsigned i_sz = std::min(begin_ref + free_sz, refs.size());
  /// Inserting external refs
  for (unsigned i = begin_ref; i < i_sz; ++i) {
    root.stref(refs[i]);
  }
  return root.endc();
}

/// We need to convert element from old to a copy with modified references
/// First we need to process children cells.
/// For tail cell we keeping all bits and native references (cutting all extra refs).
/// Placing it in the builder. And then add external refs. And return finalized new cell.
template<typename RefsInfo>
cell rebuild_refs(builder root, slice old, tuple_array<cell> refs) {
  return rebuild_refs_impl<typename RefsInfo::refs_map>(root, old, refs, 0);
}

}} // namespace refs_info
