
if(NOT "E:/Temp/TON-Compiler-LLVM13/llvm/projects/ton-compiler/tvm_linker/src/tvm_linker-stamp/tvm_linker-gitinfo.txt" IS_NEWER_THAN "E:/Temp/TON-Compiler-LLVM13/llvm/projects/ton-compiler/tvm_linker/src/tvm_linker-stamp/tvm_linker-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: 'E:/Temp/TON-Compiler-LLVM13/llvm/projects/ton-compiler/tvm_linker/src/tvm_linker-stamp/tvm_linker-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "E:/Temp/TON-Compiler-LLVM13/llvm/projects/ton-compiler/tvm_linker/src/tvm_linker"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'E:/Temp/TON-Compiler-LLVM13/llvm/projects/ton-compiler/tvm_linker/src/tvm_linker'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe"  clone --no-checkout --config "advice.detachedHead=false" "https://github.com/tonlabs/TVM-linker/" "tvm_linker"
    WORKING_DIRECTORY "E:/Temp/TON-Compiler-LLVM13/llvm/projects/ton-compiler/tvm_linker/src"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/tonlabs/TVM-linker/'")
endif()

execute_process(
  COMMAND "C:/Program Files/Git/cmd/git.exe"  checkout master --
  WORKING_DIRECTORY "E:/Temp/TON-Compiler-LLVM13/llvm/projects/ton-compiler/tvm_linker/src/tvm_linker"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'master'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe"  submodule update --recursive --init 
    WORKING_DIRECTORY "E:/Temp/TON-Compiler-LLVM13/llvm/projects/ton-compiler/tvm_linker/src/tvm_linker"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'E:/Temp/TON-Compiler-LLVM13/llvm/projects/ton-compiler/tvm_linker/src/tvm_linker'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "E:/Temp/TON-Compiler-LLVM13/llvm/projects/ton-compiler/tvm_linker/src/tvm_linker-stamp/tvm_linker-gitinfo.txt"
    "E:/Temp/TON-Compiler-LLVM13/llvm/projects/ton-compiler/tvm_linker/src/tvm_linker-stamp/tvm_linker-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'E:/Temp/TON-Compiler-LLVM13/llvm/projects/ton-compiler/tvm_linker/src/tvm_linker-stamp/tvm_linker-gitclone-lastrun.txt'")
endif()

