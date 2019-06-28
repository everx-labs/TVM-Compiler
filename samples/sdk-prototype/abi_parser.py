import json
import re

errors = []

def convert_type (abi_type):
    g = re.match (r'(u?)int(\d+)', abi_type)
    if g:
        type = g.group (1)
        command = ("%s" % ("Unsigned" if type=="u" else "Signed"), g.group (2))
        return ('unsigned' if type == 'u' else 'int', command);

    if abi_type == 'void':
        return ('void', ('',''))

    errors.append ("Type %s is not supported yet\n" % abi_type)
    return ("int", ("Signed", 256))

contract_json = open("contract.abi")
data = json.loads("".join ([x for x in contract_json]))
contract_json.close()

header = [
    '#ifndef CONTRACT_H',
    '#define CONTRACT_H',
    '',
    '#include "ton-sdk/tvm.h"',
    ''
]
wrapper = [
    '#include "ton-sdk/tvm.h"',
    '#include "ton-sdk/arguments.h"',
    '#include "ton-sdk/messages.h"',
    '#include "contract.h"',
    ''
]

abi_version = data['ABI version']
for func in data['functions']:
    outputs_type = 'void'
    outputs_command = ''
    if 'outputs' in func and len(func['outputs']) > 0:
        (outputs_type, outputs_command) = convert_type (func['outputs'][0]['type'])
        if len(func['outputs']) != 1:
            errors.append ('Function may have 0 or 1 return values')

    inputs = func['inputs']
    name = func['name']

    header_inputs = [];
    for inp in inputs:
        header_inputs.append (outputs_type + ' ' + inp['name'])

    header_str = "%s %s_Impl (%s);" % (outputs_type, name, ", ".join(header_inputs));
    header.append(header_str);

    wrapper_header = "void %s () {" % name;
    wrapper.append(wrapper_header);
    for inp in inputs:
        (inputs_type, inputs_command) = convert_type (inp['type'])
        wrapper.append("    int %s_Deserialized = Deserialize_%s_Impl (%s);" % (inp['name'], inputs_command[0], inputs_command[1]))

    main_arguments = []
    for inp in inputs:
        main_arguments.append("%s_Deserialized" % inp['name'])

    if (outputs_type != 'void'):
        wrapper.append ("    %s res = %s_Impl (%s);" % (outputs_type, name, ", ".join(main_arguments)))
        wrapper.append ("    build_external_output_common_message ();")
        wrapper.append ("    Serialize_%s_Impl (res, %s);" % (outputs_command[0], outputs_command[1]))
        wrapper.append ("    send_raw_message (0);")
    else:
        wrapper.append ("    %s_Impl (%s);" % (name, ", ".join(main_arguments, )))

    wrapper.append ("}");
    wrapper.append ("");

header.append ('');
header.append ('#endif');
header.append ('');

f = open("contract.h", "wt");
f.write ("\n".join (header));
f.close ();

f = open("contract_wrapper.cpp", "wt");
f.write ("\n".join (wrapper));
f.close ();

f = open("contract.err", "wt");
f.write ("\n".join (errors));
f.close ();
