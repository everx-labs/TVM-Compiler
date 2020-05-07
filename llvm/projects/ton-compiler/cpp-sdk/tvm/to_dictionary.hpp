#pragma once

#include <tuple>

#include <tvm/dictionary.hpp>
#include <tvm/schema/make_builder.hpp>
#include <tvm/persistent_data.hpp>
#include <tvm/to_std_tuple.hpp>

namespace tvm {

template<class Tuple, int Index>
struct dictionary_filler {
  inline static dictionary process(Tuple tup, dictionary dict) {
    constexpr int idx = Index - 1;
    dict = dictionary_filler<Tuple, idx>::process(tup, dict);
    auto val = std::get<idx>(tup);
    if (val.changed()) {
      auto sl = schema::build(val.get()).make_slice();
      dict.dictiset(sl, Index, 64);
    }
    return dict;
  }
};
template<class Tuple>
struct dictionary_filler<Tuple, 0> {
  inline static dictionary process(Tuple, dictionary dict) {
    return dict;
  }
};

// Fills dictionary by tuple vals
// tup<0> -> dict<key 0>, tup<1> -> dict<key -1>, tup<2> -> dict<key -2>
template<class... Types>
dictionary to_dictionary(std::tuple<Types...> tup, dictionary dict) {
  return dictionary_filler<std::tuple<Types...>, sizeof...(Types)>::process(tup, dict);
}

template<class Type>
dictionary to_dictionary(Type val, dictionary dict) {
  return to_dictionary(to_std_tuple(val), dict);
}

} // namespace tvm
