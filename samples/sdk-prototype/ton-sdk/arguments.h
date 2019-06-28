#ifndef TON_SDK_ARGUMENTS_H
#define TON_SDK_ARGUMENTS_H

#include "tvm.h"

#define DESERIALIZE_ARGUMENT_SIGNED(size) (Deserialize_Signed_Impl (size))
#define DESERIALIZE_ARGUMENT_UNSIGNED(size) (Deserialize_Unsigned_Impl (size))
#define DESERIALIZE_ARGUMENT_COMPLEX(type) (Deserialize_##type##_Impl ())

#endif