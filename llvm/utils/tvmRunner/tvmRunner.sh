#!/bin/sh

# parse arguments 
while [ -n "$1" ]; do # while loop starts
   case "$1" in
   -r)
      result=$2
      shift
      ;;
   -s)
      file=$(realpath $2)
      echo "Source file \"$file\""
      shift
      ;;
   -f)
      func=$2
      echo "Function to execute: $func"
      shift
      ;;
   *) break ;;

   esac
   shift
done

# check if source file is passed
if [ -z "$file" ]; then
   echo "Source file is not present"
   echo "Usage: $(echo $0 | grep -Eo "[a-zA-Z_]*\.sh") -s <source .ll file> [-f <function to execute>] [-r <expected RegExp result>] [arg1 [arg2 [...]]]"
   exit 1
fi

# check if file has expected suffix
if [ "$(echo $file | grep -E -o ".ll$")" != ".ll" ]; then
   echo "Expecting .ll file"
   exit 2
fi

# asm file
asm_file=`echo $file | grep -E -o "^([\.]*)?([A-Za-zА-Яа-я0-9_/\-]*)"`.asm
if [ -f "$asm_file" ]; then
   rm -f $asm_file
fi

# result file
r_file=`echo $file | grep -E -o "^([\.]*)?([A-Za-zА-Яа-я0-9_/\-]*)"`.res
if [ -f "$r_file" ]; then
   rm -f $r_file
fi

# compile source
llc < $file -march=tvm > $asm_file

# find linker folder
cfg=$(echo $0 | grep -E -o "^([\.]*)?([A-Za-z0-9_/\-]*)").cfg
if [ -f "$cfg" ]; then
   linker_path=`cat $cfg | grep "linker_path=" | sed -r -e "s/linker_path=//g"`
else
   linker_path=$(find ~/ -name "tvm_linker" | grep -E "^(/[A-Za-z0-9А-Яа-я\-\_]*)*/TVM-linker/tvm_linker$")
   echo "linker_path=$linker_path" > $cfg
fi
 # check that linker found
if [ -z "$linker_path" ]; then
   echo "tvm_linker wasn't been found"
   echo "Skipping..."
   rm -f $cfg
   exit 3
fi

# compile contract
c_comp=$(tvm_linker --lib $linker_path/stdlib_arg.tvm --debug $asm_file)
#echo "$c_comp"
contract=$(echo "$c_comp" | grep -Pzo "[A-Fa-f0-9]{64,64}")
echo "Compiled contract: $contract"
if [ "$(echo "$c_comp" | grep -Pzo "General-purpose functions:[\r\n]+[- ]+[\r\n]+[- ]+")" != "" ]; then
   echo "No external function in contract"
   exit 4
fi
# executing contract
res=
if [ -z "$func" ]; then
   # executing with no function specified
   res=$($linker_path/target/debug/tvm_linker $contract --debug)
else
   # getting function id
   funcid=$(echo "$c_comp" | grep -E "^Function $func" | grep -Eo "[A-Fa-f0-9]{8,8}")
   if [ "$funcid" = "" ]; then
      echo "Function id not found for function \"$func\""
      exit 6
   fi
   # processing additional parameters
   args=$@
   fargs=
   for i in $args; do
      fargs=$fargs$i
   done
   echo "Arguments: $fargs"
   # executing
   res=$($linker_path/targer/debug/tvm_linker $contract --debug test --body 00$funcid$fargs)
fi
# parse exit code
ec=$(echo "$res" | grep "TVM terminated with exit code" | grep -Eo "[0-9]*")
if [ "$ec" != "0" ]; then
   # non zero exit code
   echo "Failed with exit code $ec"
   exit "$ec"
fi
echo "Exit code: $ec"
if [ -z "$result" ]; then
   exit "$ec"
fi
if [ "$(echo "$res" | grep -Eo "$result")" != "" ]; then
   echo "$(echo "$res" | grep -E "$result")"
   exit 0
fi
echo "Expected by RegExp result not found"
exit 5