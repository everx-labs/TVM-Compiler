#ifndef TON_SDK_ARGUMENTS_H
#define TON_SDK_ARGUMENTS_H

#include "tvm.h"

#define DESERIALIZE_SLICE_UNSIGNED(argument, size) (Deserialize_Unsigned_Impl (&(argument), size))
#define DESERIALIZE_SLICE_COMPLEX(argument, type) (Deserialize_##type##_Impl (&(argument)))

#endif