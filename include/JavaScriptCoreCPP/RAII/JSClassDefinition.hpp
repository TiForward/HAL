/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITION_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITION_HPP_


#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include "JavaScriptCoreCPP/RAII/JSStaticValue.hpp"
#include "JavaScriptCoreCPP/RAII/JSStaticFunction.hpp"
#include <set>
#include <memory>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

enum class JSClassAttributes {
	None                 = kJSClassAttributeNone,
	NoAutomaticPrototype = kJSClassAttributeNoAutomaticPrototype
};

class JSClass;
class JSClassBuilder;

/*!
  @class

  @abstract This is a private helper class used by JSClassBuilder and
  JSClass that contains the properties and callbacks that define a
  type of JavaScript object. The only way to get an instance of a
  JSClassDefinition by calling get_js_class_definition() on a JSClass
  instance.
*/
class JSClassDefinition final	{
	
 public:

	JSClassDefinition() = delete;
	~JSClassDefinition() = default;

	JSClassDefinition(const JSClassDefinition& rhs) = default;
	JSClassDefinition(JSClassDefinition&& rhs) = default;

	JSClassDefinition& operator=(const JSClassDefinition& rhs) = default;
	JSClassDefinition& operator=(JSClassDefinition&& rhs) = default;

	JSString                            get_class_name()                   const { return class_name_; }
	std::set<JSClassAttributes>         get_attributes()                   const { return attributes_; }
	std::shared_ptr<JSClass>            get_parent_class_ptr()             const { return parent_class_ptr_; }
	std::set<JSStaticValue>             get_static_values()                const { return static_values_; }
	std::set<JSStaticFunction>          get_static_functions()             const { return static_functions_; }
	JSObject::InitializeCallback        get_initialize_callback()          const { return initialize_callback_; }
	JSObject::FinalizeCallback          get_finalize_callback()            const { return finalize_callback_; }
	JSObject::HasPropertyCallback       get_has_property_callback()        const { return has_property_callback_; }
	JSObject::GetPropertyCallback       get_get_property_callback()        const { return get_property_callback_; }
	JSObject::SetPropertyCallback       get_set_property_callback()        const { return set_property_callback_; }
	JSObject::DeletePropertyCallback    get_delete_property_callback()     const { return delete_property_callback_; }
	JSObject::GetPropertyNamesCallback  get_get_property_names_callback()  const { return get_property_names_callback_; }
	JSObject::CallAsFunctionCallback    get_call_as_function_callback()    const { return call_as_function_callback_; }
	JSObject::CallAsConstructorCallback get_call_as_constructor_callback() const { return call_as_constructor_callback_; }
	JSObject::HasInstanceCallback       get_has_instance_callback()        const { return has_instance_callback_; }
	JSObject::ConvertToTypeCallback     get_convert_to_type_callback()     const { return convert_to_type_callback_; }

 private:

	// Only the JSClassBuilder can create instances of
	// JSClassDefinition.
	JSClassDefinition(const JSClassBuilder& builder);

	// For interoperability with the JavaScriptCore C API.
	operator ::JSClassDefinition const*() const {
		return &js_class_definition_;
	}

	// Return true if the two JSClassDefinitions are equal.
	friend bool operator==(const JSClassDefinition& lhs, const JSClassDefinition& rhs);

	// Define a strict weak ordering for two JSClassDefinitions.
	friend bool operator<(const JSClassDefinition& lhs, const JSClassDefinition& rhs);

	friend class JSClassBuilder;
	friend class JSClass;

	JSString                            class_name_;
	std::string                         class_name_for_js_class_definition_;
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
	::JSClassDefinition                 js_class_definition_ = kJSClassDefinitionEmpty;
};

// Return true if the two JSClassDefinitions are equal.
bool operator==(const JSClassDefinition& lhs, const JSClassDefinition& rhs);
// Return true if the two JSString are not equal.

inline
bool operator!=(const JSClassDefinition& lhs, const JSClassDefinition& rhs) {
	return ! (lhs == rhs);
}

// Define a strict weak ordering for two JSClassDefinitions.
bool operator<(const JSClassDefinition& lhs, const JSClassDefinition& rhs);

inline
bool operator>(const JSClassDefinition& lhs, const JSClassDefinition& rhs) {
	return rhs < lhs;
}

inline
bool operator<=(const JSClassDefinition& lhs, const JSClassDefinition& rhs) {
	return !(lhs > rhs);
}

inline
bool operator>=(const JSClassDefinition& lhs, const JSClassDefinition& rhs) {
	return !(lhs < rhs);
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITION_HPP_
