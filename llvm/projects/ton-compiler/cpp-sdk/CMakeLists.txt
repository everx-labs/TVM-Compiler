set(TVM_CXX_HEADERS
  boost
  std
  tvm
  sha256_literal
)

add_custom_target(tvm-cxx-headers ALL DEPENDS ${TVM_CXX_HEADERS})

install(DIRECTORY ${TVM_CXX_HEADERS}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  COMPONENT tvm-cxx-headers)

if (NOT CMAKE_CONFIGURATION_TYPES)
  add_custom_target(install-tvm-cxx-headers
                    DEPENDS tvm-cxx-headers
                    COMMAND "${CMAKE_COMMAND}"
                            -DCMAKE_INSTALL_COMPONENT=tvm-cxx-headers
                            -P "${CMAKE_BINARY_DIR}/cmake_install.cmake")
  add_custom_target(install-tvm-cxx-headers-stripped DEPENDS install-tvm-cxx-headers)
endif()
