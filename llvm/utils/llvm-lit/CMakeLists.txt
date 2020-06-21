get_property(LLVM_LIT_CONFIG_MAP GLOBAL PROPERTY LLVM_LIT_CONFIG_MAP)

get_llvm_lit_path(LIT_BASE_DIR LIT_FILE_NAME)

set(LLVM_SOURCE_DIR ${LLVM_MAIN_SRC_DIR})

if(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
  foreach(BUILD_MODE ${CMAKE_CONFIGURATION_TYPES})
    string(REPLACE ${CMAKE_CFG_INTDIR} ${BUILD_MODE} bi ${LIT_BASE_DIR})
    set(bi "${bi}/${LIT_FILE_NAME}")
    configure_file(
      llvm-lit.in
      ${bi}
      )
  endforeach()
else()
  set(BUILD_MODE .)
  configure_file(
    llvm-lit.in
    ${LIT_BASE_DIR}/${LIT_FILE_NAME}
    )
endif()

#TVM local begin
if (NOT CMAKE_CONFIGURATION_TYPES)
  add_custom_target(install-llvm-lit
                    COMMAND "${CMAKE_COMMAND}"
                            -DCMAKE_INSTALL_COMPONENT=llvm-lit
                            -P "${CMAKE_BINARY_DIR}/cmake_install.cmake")
  add_custom_target(install-llvm-lit-stripped DEPENDS install-llvm-lit)
endif()

add_custom_target(llvm-lit ALL DEPENDS ${LIT_BASE_DIR}/${LIT_FILE_NAME})

install(FILES ${LIT_BASE_DIR}/${LIT_FILE_NAME}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
  COMPONENT llvm-lit)
#TVM local end
