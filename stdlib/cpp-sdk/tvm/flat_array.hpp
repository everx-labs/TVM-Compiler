#pragma once

namespace tvm {

// Array represented in flat sequence of elements
template<class Element, unsigned KeyLen>
class flat_array {
public:
  Element operator[](unsigned idx) const {
    auto [slice, succ] = dict_.dictuget(idx, KeyLen);
    tvm_assert(succ, error_code::custom_data_parse_error);
    return parse<Element>(slice);
  }
private:
  uint_t<32> size_;
  dictionary dict_;
};

} // namespace tvm

