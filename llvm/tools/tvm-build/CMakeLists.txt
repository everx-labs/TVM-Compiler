set(TVM_BUILD_SCRIPTS
  tvm-build.py
  tvm-build++.py
)

foreach(s ${TVM_BUILD_SCRIPTS})
  set(src ${CMAKE_CURRENT_SOURCE_DIR}/${s})
  set(dst ${CMAKE_BINARY_DIR}/bin/${s})
  add_custom_command(OUTPUT ${dst}
    COMMAND ${CMAKE_COMMAND} -E copy ${src} ${dst}
    DEPENDS ${src}
    COMMENT "Copying ${s} script")
  list(APPEND out_build_scripts ${dst})
endforeach()

add_custom_target(tvm-build ALL DEPENDS ${out_build_scripts})

install(FILES ${out_build_scripts}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
  COMPONENT tvm-build)

if (NOT CMAKE_CONFIGURATION_TYPES)
  add_custom_target(install-tvm-build
                    DEPENDS tvm-build
                    COMMAND "${CMAKE_COMMAND}"
                            -DCMAKE_INSTALL_COMPONENT=tvm-build
                            -P "${CMAKE_BINARY_DIR}/cmake_install.cmake")
  # Stripping is a no-op for scripts
  add_custom_target(install-tvm-build-stripped DEPENDS install-tvm-build)
endif()
