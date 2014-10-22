// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/JSClassDefinition.hpp"
#include "JavaScriptCoreCPP/RAII/JSClassDefinitionBuilder.hpp"
#include "JavaScriptCoreCPP/RAII/JSClass.hpp"

namespace JavaScriptCoreCPP { namespace RAII {

JSClassDefinition::JSClassDefinition(const JSClassDefinitionBuilder& builder)
		: attributes_(builder.attributes_)
		, class_name_(builder.class_name_)
		, parent_class_ptr_(builder.parent_class_ptr_)
		, static_values_(builder.static_values_)
		, static_functions_(builder.static_functions_)
		, initialize_callback_(builder.initialize_callback_)
		, finalize_callback_(builder.finalize_callback_)
		, has_property_callback_(builder.has_property_callback_)
		, get_property_callback_(builder.get_property_callback_)
		, set_property_callback_(builder.set_property_callback_)
		, delete_property_callback_(builder.delete_property_callback_)
		, get_property_names_callback_(builder.get_property_names_callback_)
		, call_as_function_callback_(builder.call_as_function_callback_)
		, call_as_constructor_callback_(builder.call_as_constructor_callback_)
		, has_instance_callback_(builder.has_instance_callback_)
		, convert_to_type_callback_(builder.convert_to_type_callback_) {
}

}} // namespace JavaScriptCoreCPP { namespace RAII {
