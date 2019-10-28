#pragma once

namespace tvm {

struct error_code {
  static constexpr int persistent_data_parse_error = 51;
  static constexpr int custom_data_parse_error = 52;
  static constexpr int bad_tupled_variant_kind = 53;
};

} // namespace tvm
