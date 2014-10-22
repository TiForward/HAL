// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/JSClassDefinition.hpp"
#include "JavaScriptCoreCPP/RAII/JSClassBuilder.hpp"
#include "JavaScriptCoreCPP/RAII/JSClass.hpp"

namespace JavaScriptCoreCPP { namespace RAII {

JSClassDefinition::JSClassDefinition(const JSClassBuilder& builder)
		: attributes_(builder.attributes_)
		, class_name_(builder.class_name_)
		, class_name_for_js_class_definition_(class_name_)
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

bool operator==(const JSClassDefinition& lhs, const JSClassDefinition& rhs) {
	if (lhs.class_name_ != rhs.class_name_) {
		return false;
	}

	if (lhs.attributes_ != rhs.attributes_) {
		return false;
	}

	if (lhs.parent_class_ptr_ != rhs.parent_class_ptr_) {
		return false;
	}

	if (lhs.static_values_ != rhs.static_values_) {
		return false;
	}

	if (lhs.static_functions_ != rhs.static_functions_) {
		return false;
	}

	// initialize_callback_
	if (lhs.initialize_callback_ && !rhs.initialize_callback_) {
		return false;
	}
	
	if (!lhs.initialize_callback_ && rhs.initialize_callback_) {
		return false;
	}

	// finalize_callback_
	if (lhs.finalize_callback_ && !rhs.finalize_callback_) {
		return false;
	}
	
	if (!lhs.finalize_callback_ && rhs.finalize_callback_) {
		return false;
	}

	// has_instance_callback_
	if (lhs.has_property_callback_ && !rhs.has_property_callback_) {
		return false;
	}
	
	if (!lhs.has_property_callback_ && rhs.has_property_callback_) {
		return false;
	}

	// get_property_names_callback_
	if (lhs.get_property_callback_ && !rhs.get_property_callback_) {
		return false;
	}
	
	if (!lhs.get_property_callback_ && rhs.get_property_callback_) {
		return false;
	}

	// set_property_names_callback_
	if (lhs.set_property_callback_ && !rhs.set_property_callback_) {
		return false;
	}

	if (!lhs.set_property_callback_ && rhs.set_property_callback_) {
		return false;
	}

	// delete_property_callback_
	if (lhs.delete_property_callback_ && !rhs.delete_property_callback_) {
		return false;
	}

	if (!lhs.delete_property_callback_ && rhs.delete_property_callback_) {
		return false;
	}

	// get_property_names_callback_
	if (lhs.get_property_names_callback_ && !rhs.get_property_names_callback_) {
		return false;
	}

	if (!lhs.get_property_names_callback_ && rhs.get_property_names_callback_) {
		return false;
	}

	// call_as_function_callback_
	if (lhs.call_as_function_callback_ && !rhs.call_as_function_callback_) {
		return false;
	}

	if (!lhs.call_as_function_callback_ && rhs.call_as_function_callback_) {
		return false;
	}

	// call_as_constructor_callback_
	if (lhs.call_as_constructor_callback_ && !rhs.call_as_constructor_callback_) {
		return false;
	}

	if (!lhs.call_as_constructor_callback_ && rhs.call_as_constructor_callback_) {
		return false;
	}

	// has_instance_callback_
	if (lhs.has_instance_callback_ && !rhs.has_instance_callback_) {
		return false;
	}

	if (!lhs.has_instance_callback_ && rhs.has_instance_callback_) {
		return false;
	}

	// convert_to_type_callback_
	if (lhs.convert_to_type_callback_ && !rhs.convert_to_type_callback_) {
		return false;
	}

	if (!lhs.convert_to_type_callback_ && rhs.convert_to_type_callback_) {
		return false;
	}

	return true;
}

bool operator<(const JSClassDefinition& lhs, const JSClassDefinition& rhs) {
	if (lhs.class_name_for_js_class_definition_ < rhs.class_name_for_js_class_definition_) {
		return true;
	}

	if (lhs.attributes_ < rhs.attributes_) {
		return true;
	}

	if (lhs.parent_class_ptr_ < rhs.parent_class_ptr_) {
		return true;
	}

	if (lhs.static_values_ < rhs.static_values_) {
		return true;
	}

	return lhs.static_functions_ < rhs.static_functions_;
}

}} // namespace JavaScriptCoreCPP { namespace RAII {
