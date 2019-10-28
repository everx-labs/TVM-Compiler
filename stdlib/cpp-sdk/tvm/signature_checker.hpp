#pragma once

namespace tvm {

class __attribute__((tvm_tuple)) signature_checker {
public:
  explicit signature_checker(slice msg_body) {
    auto [rem_body, sign_slice] = __builtin_tvm_ldrefrtos(msg_body.get());
    auto [signature, rem_pub_key] = __builtin_tvm_ldslice(sign_slice, 512);
    public_key_ = tvm::parser(rem_pub_key).ldu(256);
    auto hash = __builtin_tvm_hashsu(rem_body);
    verified_ = __builtin_tvm_chksignu(hash, signature, public_key_);
  }
  unsigned public_key() const { return public_key_; }
  bool verified() const { return verified_; }
private:
  unsigned public_key_;
  bool verified_;
};

} // namespace tvm

