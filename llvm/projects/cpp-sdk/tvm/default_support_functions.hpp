#pragma once

// ==================== Support methods =========================== //
#define DEFAULT_SUPPORT_FUNCTIONS(TVM_INTERFACE, TVM_REPLAY_PROTECTION)                                    \
  unsigned msg_pubkey_ = 0;                                                                                \
  __always_inline void set_msg_pubkey(unsigned pubkey) { msg_pubkey_ = pubkey; }                           \
  __always_inline unsigned tvm_pubkey() const { return msg_pubkey_; }                                      \
  __always_inline unsigned msg_pubkey() const { return msg_pubkey_; }                                      \
  std::variant<cell, slice> msg_slice_;                                                                    \
  __always_inline void set_msg_slice(slice sl) { msg_slice_ = sl; }                                        \
  __always_inline void set_msg_slice(cell cl) { msg_slice_ = cl; }                                         \
  __always_inline slice msg_slice() {                                                                      \
    if (isa<cell>(msg_slice_))                                                                             \
      msg_slice_ = std::get_known<cell>(msg_slice_).ctos();                                                \
    return std::get_known<slice>(msg_slice_);                                                              \
  }                                                                                                        \
  std::optional<address> int_sender_;                                                                      \
  Grams int_value_;                                                                                        \
  __always_inline std::pair<address, Grams> int_sender_and_value() {                                       \
    address sender = int_sender();                                                                         \
    return { sender, int_value_ };                                                                         \
  }                                                                                                        \
  __always_inline Grams int_value() {                                                                      \
    int_sender();                                                                                          \
    return int_value_;                                                                                     \
  }                                                                                                        \
  __always_inline address int_sender() {                                                                   \
    if (!int_sender_) {                                                                                    \
      auto parsed_msg = parse<schema::int_msg_info>(parser(msg_slice()), error_code::bad_incoming_msg);    \
      int_sender_ = incoming_msg(parsed_msg).int_sender();                                                 \
      int_value_ = parsed_msg.value.grams;                                                                 \
    }                                                                                                      \
    return *int_sender_;                                                                                   \
  }                                                                                                        \
  __always_inline void set_int_sender(lazy<MsgAddressInt> val) {                                           \
    int_sender_ = val;                                                                                     \
  }                                                                                                        \
  std::optional<lazy<MsgAddressExt>> ext_sender_;                                                          \
  __always_inline lazy<MsgAddressExt> ext_sender() {                                                       \
    if (!ext_sender_) {                                                                                    \
      auto parsed_msg = parse<schema::ext_in_msg_info>(parser(msg_slice()), error_code::bad_incoming_msg); \
      ext_sender_ = incoming_msg(parsed_msg).ext_sender();                                                 \
    }                                                                                                      \
    return *ext_sender_;                                                                                   \
  }                                                                                                        \
  __always_inline void set_ext_sender(lazy<MsgAddressExt> val) {                                           \
    ext_sender_ = val;                                                                                     \
  }                                                                                                        \
  unsigned int_return_flag_ = DEFAULT_MSG_FLAGS;                                                           \
  std::optional<address> suicide_addr_;                                                                    \
  __always_inline std::optional<address> suicide_addr() {                                                  \
    return suicide_addr_;                                                                                  \
  }                                                                                                        \
   __always_inline void suicide(address addr) {                                                            \
    suicide_addr_ = addr;                                                                                  \
  }                                                                                                        \
  __always_inline unsigned int_return_flag() const { return int_return_flag_; }                            \
  __always_inline void set_int_return_flag(unsigned flag) { int_return_flag_ = flag; }                     \
  unsigned int_return_value_ = 0;                                                                          \
  __always_inline unsigned int_return_value() const { return int_return_value_; }                          \
  __always_inline void set_int_return_value(unsigned val) { int_return_value_ = val; }                     \
  std::optional<uint32> return_func_id_;                                                                   \
  __always_inline std::optional<uint32> return_func_id() const { return return_func_id_; }                 \
  __always_inline void set_return_func_id(uint32 v) { return_func_id_ = v; }                               \
  persistent_data_header_t<TVM_INTERFACE, TVM_REPLAY_PROTECTION> header_;                                  \
  __always_inline                                                                                          \
  void set_persistent_data_header(                                                                         \
    persistent_data_header_t<TVM_INTERFACE, TVM_REPLAY_PROTECTION> header) {                               \
    header_ = header;                                                                                      \
  }

