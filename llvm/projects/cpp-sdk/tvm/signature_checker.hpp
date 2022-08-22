#pragma once

namespace tvm {

template<unsigned err_code>
class __attribute__((tvm_tuple)) signature_checker {
public:
  explicit signature_checker(slice msg_body) {
    require(!msg_body.srempty(), err_code);
    parser p_msg(msg_body);
    slice sign_slice = p_msg.ldrefrtos();
    require(!sign_slice.empty(), err_code);

    parser p_sign(sign_slice);
    slice signature = p_sign.ldslice(512);
    public_key_ = p_sign.ldu(256);
    p_sign.ends();

    auto hash = __builtin_tvm_hashsu(p_msg.sl());
    verified_ = __builtin_tvm_chksignu(hash, signature, public_key_);
    modified_slice_ = p_msg.sl();
  }
  unsigned public_key() const { return public_key_; }
  bool verified() const { return verified_; }
  slice modified_slice() const { return modified_slice_; }

  // Returns public key and rest of the slice is succeeded
  // Throws err_code if error
  inline static std::tuple<unsigned, slice> check(slice msg_body) {
    signature_checker sig_check(msg_body);
    tvm_assert(sig_check.verified(), err_code);
    return { sig_check.public_key(), sig_check.modified_slice() };
  }
private:
  unsigned public_key_;
  bool verified_;
  slice modified_slice_;
};

class signature_checker_v2 {
public:
  // msg_sl - slice from header begin
  inline static bool check(slice msg_sl, slice signature, schema::uint256 pubkey) {
    auto hash = __builtin_tvm_hashsu(msg_sl.get());
    return __builtin_tvm_chksignu(hash, signature, pubkey.get());
  }
};

} // namespace tvm

