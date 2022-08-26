TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

SOURCES +=
INCLUDEPATH += ../../

HEADERS += \
    ../Console.hpp \
    ../ElementRef.hpp \
    ../assert.hpp \
    ../awaiting_responses_map.hpp \
    ../big_dict_traits.hpp \
    ../big_queue.hpp \
    ../builder.hpp \
    ../call_wrapper.hpp \
    ../cell.hpp \
    ../container/intrusive_queue.hpp \
    ../container/refs_info.hpp \
    ../container/tuple_array.hpp \
    ../contract.hpp \
    ../contract_handle.hpp \
    ../debot.hpp \
    ../default_support_functions.hpp \
    ../dict_array.hpp \
    ../dict_base.hpp \
    ../dict_map.hpp \
    ../dict_set.hpp \
    ../dict_traits.hpp \
    ../dictionary.hpp \
    ../dictionary_array_iterator.hpp \
    ../dictionary_const_iterator.hpp \
    ../dictionary_map_const_iterator.hpp \
    ../dictionary_map_iterator.hpp \
    ../emit.hpp \
    ../error_code.hpp \
    ../from_dictionary.hpp \
    ../func_id.hpp \
    ../globals.hpp \
    ../incoming_msg.hpp \
    ../is_tuple.hpp \
    ../log_sequence.hpp \
    ../message_flags.hpp \
    ../msg_address.hpp \
    ../msg_address.inline.hpp \
    ../numeric_limits.hpp \
    ../parser.hpp \
    ../persistent_data.hpp \
    ../persistent_data_header.hpp \
    ../queue.hpp \
    ../reflection.hpp \
    ../replay_attack_protection/rolling_ids.hpp \
    ../replay_attack_protection/timestamp.hpp \
    ../resumable.hpp \
    ../schema/abiv1.hpp \
    ../schema/abiv2.hpp \
    ../schema/basics.hpp \
    ../schema/build_chain_static.hpp \
    ../schema/builder/dynamic_field_builder.hpp \
    ../schema/builder/struct_builder.hpp \
    ../schema/builder/variant_builder.hpp \
    ../schema/chain_builder.hpp \
    ../schema/chain_fold.hpp \
    ../schema/chain_tuple.hpp \
    ../schema/chain_tuple_printer.hpp \
    ../schema/estimate_element.hpp \
    ../schema/get_bitsize.hpp \
    ../schema/is_expandable.hpp \
    ../schema/json-abi-gen.hpp \
    ../schema/make_builder.hpp \
    ../schema/make_parser.hpp \
    ../schema/message.hpp \
    ../schema/parse_chain_static.hpp \
    ../schema/parser/dynamic_field_parser.hpp \
    ../schema/parser/struct_parser.hpp \
    ../schema/parser/variant_parser.hpp \
    ../sequence.hpp \
    ../signature_checker.hpp \
    ../slice.hpp \
    ../small_dict_map.hpp \
    ../small_dict_map_const_iterator.hpp \
    ../small_dict_traits.hpp \
    ../smart_contract_info.hpp \
    ../smart_switcher.hpp \
    ../static_print.hpp \
    ../string.hpp \
    ../suffixes.hpp \
    ../to_dictionary.hpp \
    ../to_std_tuple.hpp \
    ../to_struct.hpp \
    ../tree_cell.hpp \
    ../tuple.hpp \
    ../tuple_stack.hpp \
    ../unpackfirst_caller.hpp \
    ../untuple_caller.hpp
