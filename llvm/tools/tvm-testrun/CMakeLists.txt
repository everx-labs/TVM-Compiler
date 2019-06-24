add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/bin/tvm-testrun
                   COMMAND ${CMAKE_COMMAND} -E make_directory
                     ${CMAKE_BINARY_DIR}/bin
                   COMMAND ${CMAKE_COMMAND} -E copy
                     ${CMAKE_CURRENT_SOURCE_DIR}/tvm-testrun.py
                     ${CMAKE_BINARY_DIR}/bin/tvm-testrun
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tvm-testrun.py)
add_custom_target(tvm-testrun ALL DEPENDS ${CMAKE_BINARY_DIR}/bin/tvm-testrun)

