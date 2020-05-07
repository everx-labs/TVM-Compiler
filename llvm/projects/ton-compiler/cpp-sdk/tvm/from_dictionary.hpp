#pragma once

#include <tuple>

#include <tvm/dictionary.hpp>
#include <tvm/schema/make_builder.hpp>
#include <tvm/persistent_data.hpp>
#include <tvm/to_std_tuple.hpp>
#include <tvm/to_struct.hpp>

namespace tvm {

template<class Tuple, int Index>
struct dictionary_reader {
  inline static Tuple process(Tuple tup, dictionary dict) {
    constexpr int idx = Index - 1;
    std::get<idx>(tup).set_dict(dict, Index);
    return dictionary_reader<Tuple, idx>::process(tup, dict);
  }
};
template<class Tuple>
struct dictionary_reader<Tuple, 0> {
  inline static Tuple process(Tuple tup, dictionary) {
    return tup;
  }
};

// Initializes tuple of persistent<T> vals by dictionary and key
// tup<0> -> dict<key 0>, tup<1> -> dict<key -1>, tup<2> -> dict<key -2>
template<class... Types>
std::tuple<Types...> from_dictionary(std::tuple<Types...> tup, dictionary dict) {
  return dictionary_reader<std::tuple<Types...>, sizeof...(Types)>::process(tup, dict);
}

template<class Type>
Type from_dictionary(Type val, dictionary dict) {
  auto tup = from_dictionary(to_std_tuple(val), dict);
  return to_struct<Type>(tup);
}

} // namespace tvm
