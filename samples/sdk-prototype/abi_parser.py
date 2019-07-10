import json
import sys
import re

errors = []
contract_name = sys.argv[1]

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

contract_json = open("%s.abi" % contract_name)
data = json.loads("".join ([x for x in contract_json]))
contract_json.close()

header = [
    ('#ifndef __%s_H' % contract_name),
    ('#define __%s_H' % contract_name),
    '',
    '#include "ton-sdk/tvm.h"',
    ''
]
wrapper = [
    '#include "ton-sdk/tvm.h"',
    '#include "ton-sdk/arguments.h"',
    '#include "ton-sdk/messages.h"',
    ('#include "%s.h"' % contract_name),
    ''
]

abi_version = data['ABI version']
for func in data['functions']:
    name = func['name']

    outputs_type = 'void'
    outputs_command = ''
    if 'outputs' in func and len(func['outputs']) > 0:
        (outputs_type, outputs_command) = convert_type (func['outputs'][0]['type'])
        if len(func['outputs']) != 1:
            errors.append ('Function %s may have 0 or 1 return values' % name)

    inputs = func['inputs']
    signed = func['signed']

    if name [-11:] == "_authorized":
        if signed != 'true':
            errors.append ('Function %s ends with _authorized, so attribute "signed" must be set to true')
    else:
        if signed != 'false':
            errors.append ('Function %s does not end with _authorized, so attribute "signed" must be set to false')

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
        wrapper.append ("    build_external_output_common_message_header ();")
        wrapper.append ("    Serialize_%s_Impl (res, %s);" % (outputs_command[0], outputs_command[1]))
        wrapper.append ("    send_raw_message (0);")
    else:
        wrapper.append ("    %s_Impl (%s);" % (name, ", ".join(main_arguments, )))

    wrapper.append ("}");
    wrapper.append ("");

header.append ('');
header.append ('#endif');
header.append ('');

f = open(("%s.h" % contract_name), "wt");
f.write ("\n".join (header));
f.close ();

f = open(("%s_wrapper.c" % contract_name), "wt");
f.write ("\n".join (wrapper));
f.close ();

f = open(("%s.err" % contract_name), "wt");
f.write ("\n".join (errors));
f.close ();

exit (0 if len(errors)==0 else 1);
