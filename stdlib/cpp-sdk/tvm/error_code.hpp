#pragma once

namespace tvm {

struct error_code {
  static constexpr int persistent_data_parse_error = 51;
  static constexpr int custom_data_parse_error     = 52;
  static constexpr int bad_tupled_variant_kind     = 53;
  static constexpr int unsupported_call_method     = 54;
  static constexpr int wrong_public_call           = 55;
  static constexpr int no_persistent_data          = 56;
  static constexpr int bad_incoming_msg            = 57;
  static constexpr int bad_signature               = 58;
  static constexpr int replay_attack_check         = 59;
  static constexpr int bad_arguments               = 60;
};

} // namespace tvm
