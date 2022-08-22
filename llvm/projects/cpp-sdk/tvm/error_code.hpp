#pragma once

namespace tvm {

struct error_code {
  // solidity conformant error codes
  // (https://www.notion.so/tonlabs/306a53e50ea44623a999f2f199fd0f57?v=7f61664dee434e27a65d9c3565b27cb1)
  static constexpr int invalid_signature           = 40;
  static constexpr int wrong_public_call           = 41; // requested method not found
  static constexpr int bad_incoming_msg            = 48; // invalid inbound message
  // custom error codes

  static constexpr int persistent_data_parse_error = 51;
  static constexpr int custom_data_parse_error     = 52;
  static constexpr int bad_tupled_variant_kind     = 53;
  static constexpr int unsupported_call_method     = 54;
  static constexpr int unsupported_bounced_msg     = 55;
  static constexpr int no_persistent_data          = 56;

  static constexpr int wrong_answer_id             = 58;
  static constexpr int unexpected_answer           = 59;

  static constexpr int replay_attack_check         = 60;
  static constexpr int bad_arguments               = 61;
  static constexpr int constructor_double_call     = 62;
  static constexpr int method_called_without_init  = 63;
  static constexpr int iterator_overflow           = 64;
  static constexpr int empty_container             = 65;

  // test/temp error codes
  static constexpr int non_empty_bits_at_cell_wrap  = 66;
  static constexpr int non_single_refs_at_cell_wrap = 67;
  static constexpr int no_pubkey                    = 68;
};

} // namespace tvm
