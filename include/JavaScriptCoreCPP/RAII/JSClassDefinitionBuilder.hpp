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
	
	// TODO: provide getters and setters.

	JSClassDefinition build() {
		JSClassDefinition js_class_definition(*this);
		// TODO validate js_class_definition.
		return js_class_definition;
	}
	
 private:

	friend class JSClassDefinition;

	// Required parameters
	JSString                          class_name_;
	
	// Optional parameters - initialized to default values
	std::set<JSClassAttributes>       attributes_;
	std::unique_ptr<JSClass>          parent_class_ptr_;
	std::set<JSStaticValue>           static_values_;
	std::set<JSStaticValue>           static_functions_;
	JSObjectInitializeCallback        initialize_callback_          { nullptr };
	JSObjectFinalizeCallback          finalize_callback_            { nullptr };
	JSObjectHasPropertyCallback       has_property_callback_        { nullptr };
	JSObjectGetPropertyCallback       get_property_callback_        { nullptr };
	JSObjectSetPropertyCallback       set_property_callback_        { nullptr };
	JSObjectDeletePropertyCallback    delete_property_callback_     { nullptr };
	JSObjectGetPropertyNamesCallback  get_property_names_callback_  { nullptr };
	JSObjectCallAsFunctionCallback    call_as_function_callback_    { nullptr };
	JSObjectCallAsConstructorCallback call_as_constructor_callback_ { nullptr };
	JSObjectHasInstanceCallback       has_instance_callback_        { nullptr };
	JSObjectConvertToTypeCallback     convert_to_type_callback_     { nullptr };
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITION_HPP_
