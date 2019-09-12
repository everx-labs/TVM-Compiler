#!/usr/bin/env python

from __future__ import print_function

import argparse
import os
import re
import sys
import tempfile

def eprint(*args, **kwargs):
  print(*args, file=sys.stderr, **kwargs)

def parseArgs():
  parser = argparse.ArgumentParser()

  parser.add_argument('--entry', help='Entry function name')
  parser.add_argument('--no-trace', default=False, action="store_true", help='Disable trace during execution')
  parser.add_argument('--stdlibc-path', default="", help='Path to stdlibc')

  return parser.parse_args()

def createInputAsmFile():
  stdin_contents      = sys.stdin.read()
  tmp_file, file_name = tempfile.mkstemp()

  with os.fdopen(tmp_file, 'w') as tmp:
    tmp.write(stdin_contents)

  return file_name

def getPathToLinker():
  linker_name = "tvm_linker"

  for path in os.environ["PATH"].split(os.pathsep):
    file_name = os.path.join(path, linker_name)
    if os.path.isfile(file_name) and os.access(file_name, os.X_OK):
      return file_name

  return None

def getPathToStdlibC(linker_path, subdir=""):
  linker_bin_dir = os.path.dirname(linker_path)
  stdlibc_path   = linker_bin_dir + "/" + subdir + "stdlib_c.tvm"

  return stdlibc_path

def getContractDetails(linker_stdout, entry_fn):
  contract_base_name = None
  entry_addr         = None

  for line in linker_stdout.split("\n"):
    contract_base_name_re = r'.*Saved contract to file (.*).tvc.*'
    entry_addr_re         = r'.*Function {} *: id=([^,]*).*'.format(entry_fn)

    if re.match(contract_base_name_re, line):
      contract_base_name = re.sub(contract_base_name_re, '\\1', line)

    if re.match(entry_addr_re, line):
      entry_addr = re.sub(entry_addr_re, '\\1', line)

  if not contract_base_name or not entry_addr_re:
    return None

  return {"contract": contract_base_name, "entry": entry_addr}

def linkProgram(linker_path, stdlibc_path, asm_file_path, entry_fn):
  cmd_line = "{} compile {} --debug --lib {}".format(linker_path, asm_file_path, stdlibc_path)
  
  with os.popen(cmd_line) as stream:
    linker_stdout    = stream.read()
    contract_details = getContractDetails(linker_stdout, entry_fn)

    if not contract_details:
      pad_width = 80
      eprint("Can't link program\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n".format(
      	"#" * pad_width,
      	open(asm_file_path, 'r').read(),
      	"#" * pad_width,
      	cmd_line,
      	"#" * pad_width,
      	linker_stdout,
      	"#" * pad_width))
      sys.exit(1)

    return contract_details

  return None

def runProgram(linker_path, contract_details, need_trace):
  cmd_line = "{} test {} --decode-c6 --body 00{}".format(linker_path, contract_details["contract"],
  	contract_details["entry"])

  if need_trace:
  	cmd_line += " --trace"

  with os.popen(cmd_line) as stream:
    linker_stdout = stream.read()

    if linker_stdout.find("TEST COMPLETED") == -1:
      pad_width = 80
      eprint("Contract runtime error:\n{}\n{} with entry {}\n{}\n{}\n{}\n{}\n{}\n".format(
      	"#" * pad_width,
      	contract_details["contract"],
      	contract_details["entry"],
      	"#" * pad_width,
      	cmd_line,
      	"#" * pad_width,
      	linker_stdout,
      	"#" * pad_width))
      sys.exit(1)

    print(linker_stdout)

def main():
  args = parseArgs()

  linker_path = getPathToLinker()

  if not linker_path:
  	eprint("tvm_linker has not been found")
  	sys.exit(1)

  stdlibc_path = args.stdlibc_path
  if stdlibc_path == "":
    stdlibc_path = getPathToStdlibC(linker_path)
    if not os.path.isfile(stdlibc_path):
      stdlibc_path = getPathToStdlibC(linker_path, "../../")

  if not os.path.isfile(stdlibc_path):
  	eprint("stdlib_c.tvm has not been found")
  	sys.exit(1)

  asm_file_path = createInputAsmFile()

  contract_details = linkProgram(linker_path, stdlibc_path, asm_file_path, args.entry)

  if not contract_details:
  	eprint("internal error: can't get contract details")
  	sys.exit(1)

  runProgram(linker_path, contract_details, need_trace=not args.no_trace)

if __name__=='__main__':
  main()
