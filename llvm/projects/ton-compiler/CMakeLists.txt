set(TVM_RUNTIMES
  stdlib_cpp.tvm
  stdlib_c.tvm
)

set(TVM_SUPPLEMENTS
  abi_parser.py
)

add_custom_target(tvm-runtimes)

install(FILES ${TVM_RUNTIMES}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  COMPONENT tvm-runtimes)

if (NOT CMAKE_CONFIGURATION_TYPES)
  add_custom_target(install-tvm-runtimes
                    DEPENDS tvm-runtimes
                    COMMAND "${CMAKE_COMMAND}"
                            -DCMAKE_INSTALL_COMPONENT=tvm-runtimes
                            -P "${CMAKE_BINARY_DIR}/cmake_install.cmake")
  add_custom_target(install-tvm-runtimes-stripped DEPENDS install-tvm-runtimes)
endif()

add_custom_target(tvm-supplements ALL DEPENDS ${TVM_SUPPLEMENTS})

install(FILES ${TVM_SUPPLEMENTS}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
  COMPONENT tvm-supplements)

if (NOT CMAKE_CONFIGURATION_TYPES)
  add_custom_target(install-tvm-supplements
                    DEPENDS tvm-supplements
                    COMMAND "${CMAKE_COMMAND}"
                            -DCMAKE_INSTALL_COMPONENT=tvm-supplements
                            -P "${CMAKE_BINARY_DIR}/cmake_install.cmake")
  # Stripping is a no-op for scripts
  add_custom_target(install-tvm-supplements-stripped DEPENDS install-tvm-supplements)
endif()

add_subdirectory(cpp-sdk)
