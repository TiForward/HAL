/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITIONBUILDER_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITIONBUILDER_HPP_


#include "JavaScriptCoreCPP/RAII/JSClassDefinition.hpp"
#include <set>
#include <memory>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

class JSClass;

class JSClassDefinitionBuilder final {
	
 public:
	
	/**
	 * Create an empty builder.
	 */
	JSClassDefinitionBuilder(const JSString& class_name) : class_name_(class_name) {
	}
	
	JSClassDefinitionBuilder() = delete;;
	~JSClassDefinitionBuilder() = default;

	JSClassDefinitionBuilder(const JSClassDefinitionBuilder& rhs) = default;
	JSClassDefinitionBuilder(JSClassDefinitionBuilder&& rhs) = default;

	JSClassDefinitionBuilder& operator=(const JSClassDefinitionBuilder& rhs) = default;
	JSClassDefinitionBuilder& operator=(JSClassDefinitionBuilder&& rhs) = default;

	JSString get_class_name() {
		return class_name_;
	}
	
	JSClassDefinitionBuilder& set_class_name(const JSString& class_name) {
		class_name_ = class_name;
		return *this;
	}
	
	std::set<JSClassAttributes> get_attributes() const {
		return attributes_;
	}

	JSClassDefinitionBuilder& set_attributes(const std::set<JSClassAttributes>& attributes) {
		attributes_ = attributes;
		return *this;
	}
	
	std::shared_ptr<JSClass> get_parent_class_ptr() const {
		return parent_class_ptr_;
	}

	JSClassDefinitionBuilder& set_parent_class_ptr(const std::shared_ptr<JSClass>& parent_class_ptr) {
		parent_class_ptr_ = parent_class_ptr;
		return *this;
	}

	std::set<JSStaticValue> get_static_values() const {
		return static_values_;
	}

	JSClassDefinitionBuilder& set_static_values(const std::set<JSStaticValue>& static_values) {
		static_values_ = static_values;
		return *this;
	}
	
	std::set<JSStaticFunction> get_static_functions() const {
		return static_functions_;
	}

	JSClassDefinitionBuilder& set_static_functions(const std::set<JSStaticFunction>& static_functions) {
		static_functions_ = static_functions;
		return *this;
	}

	JSObject::InitializeCallback get_initialize_callback() const {
		return initialize_callback_;
	}

	JSClassDefinitionBuilder& set_initialize_callback(const JSObject::InitializeCallback& initialize_callback) {
		initialize_callback_ = initialize_callback;
		return *this;
	}

	JSObject::FinalizeCallback get_finalize_callback() const {
		return finalize_callback_;
	}

	JSClassDefinitionBuilder& set_finalize_callback(const JSObject::FinalizeCallback& finalize_callback) {
		finalize_callback_ = finalize_callback;
		return *this;
	}

	JSObject::HasPropertyCallback get_has_property_callback() const {
		return has_property_callback_;
	}

	JSClassDefinitionBuilder& set_has_property_callback(const JSObject::HasPropertyCallback& has_property_callback) {
		has_property_callback_ = has_property_callback;
		return *this;
	}

	JSObject::GetPropertyCallback get_get_property_callback() const {
		return get_property_callback_;
	}

	JSClassDefinitionBuilder& set_get_property_callback(const JSObject::GetPropertyCallback& get_property_callback) {
		get_property_callback_ = get_property_callback;
		return *this;
	}

	JSObject::SetPropertyCallback get_set_property_callback() const {
		return set_property_callback_;
	}

	JSClassDefinitionBuilder& set_set_property_callback(const JSObject::SetPropertyCallback& set_property_callback) {
		set_property_callback_ = set_property_callback;
		return *this;
	}

	JSObject::DeletePropertyCallback get_delete_property_callback() const {
		return delete_property_callback_;
	}

	JSClassDefinitionBuilder& set_delete_property_callback(const JSObject::DeletePropertyCallback& delete_property_callback) {
		delete_property_callback_ = delete_property_callback;
		return *this;
	}

	JSObject::GetPropertyNamesCallback get_get_property_names_callback() const {
		return get_property_names_callback_;
	}

	JSClassDefinitionBuilder& set_get_property_names_callback(const JSObject::GetPropertyNamesCallback& get_property_names_callback) {
		get_property_names_callback_ = get_property_names_callback;
		return *this;
	}

	JSObject::CallAsFunctionCallback get_call_as_function_callback() const {
		return call_as_function_callback_;
	}

	JSClassDefinitionBuilder& set_call_as_function_callback(const JSObject::CallAsFunctionCallback& call_as_function_callback) {
		call_as_function_callback_ = call_as_function_callback;
		return *this;
	}

	JSObject::CallAsConstructorCallback get_call_as_constructor_callback() const {
		return call_as_constructor_callback_;
	}

	JSClassDefinitionBuilder& set_call_as_constructor_callback(const JSObject::CallAsConstructorCallback& call_as_constructor_callback) {
		call_as_constructor_callback_ = call_as_constructor_callback;
		return *this;
	}

	JSObject::HasInstanceCallback get_has_instance_callback() const {
		return has_instance_callback_;
	}

	JSClassDefinitionBuilder& set_has_instance_callback(const JSObject::HasInstanceCallback& has_instance_callback) {
		has_instance_callback_ = has_instance_callback;
		return *this;
	}
	
	JSObject::ConvertToTypeCallback get_convert_to_type_callback() const {
		return convert_to_type_callback_;
	}

	JSClassDefinitionBuilder& set_convert_to_type_callback(const JSObject::ConvertToTypeCallback& convert_to_type_callback) {
		convert_to_type_callback_ = convert_to_type_callback;
		return *this;
	}
	
	JSClassDefinition build() {
		JSClassDefinition js_class_definition(*this);
		// TODO validate js_class_definition.
		return js_class_definition;
	}
	
 private:

	friend class JSClassDefinition;

	// Required parameters
	JSString                            class_name_;
	
	// Optional parameters - initialized to default values
	std::set<JSClassAttributes>         attributes_;
	std::shared_ptr<JSClass>            parent_class_ptr_             { nullptr };
	std::set<JSStaticValue>             static_values_;
	std::set<JSStaticFunction>          static_functions_;
	JSObject::InitializeCallback        initialize_callback_          { nullptr };
	JSObject::FinalizeCallback          finalize_callback_            { nullptr };
	JSObject::HasPropertyCallback       has_property_callback_        { nullptr };
	JSObject::GetPropertyCallback       get_property_callback_        { nullptr };
	JSObject::SetPropertyCallback       set_property_callback_        { nullptr };
	JSObject::DeletePropertyCallback    delete_property_callback_     { nullptr };
	JSObject::GetPropertyNamesCallback  get_property_names_callback_  { nullptr };
	JSObject::CallAsFunctionCallback    call_as_function_callback_    { nullptr };
	JSObject::CallAsConstructorCallback call_as_constructor_callback_ { nullptr };
	JSObject::HasInstanceCallback       has_instance_callback_        { nullptr };
	JSObject::ConvertToTypeCallback     convert_to_type_callback_     { nullptr };
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITION_HPP_
