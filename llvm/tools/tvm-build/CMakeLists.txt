add_custom_target(tvm-build ALL
  DEPENDS ${CMAKE_BINARY_DIR}/bin/tvm-build ${CMAKE_BINARY_DIR}/bin/tvm-build++)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/bin/tvm-build
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/tvm-build.py
    ${CMAKE_BINARY_DIR}/bin/tvm-build
  DEPENDS tvm-build.py
  COMMENT "Copying tvm-build script")

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/bin/tvm-build++
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/tvm-build++.py
    ${CMAKE_BINARY_DIR}/bin/tvm-build++
  DEPENDS tvm-build++.py
  COMMENT "Copying tvm-build++ script")
