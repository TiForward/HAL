/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITION2_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITION2_HPP_


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

class JSClass2;
template<typename T>
class JSClassBuilder2;

/*!
  @class

  @abstract This is a private helper class used by JSClassBuilder and
  JSClass that contains the properties and callbacks that define a
  type of JavaScript object. The only way to get an instance of a
  JSClassDefinition by calling get_js_class_definition() on a JSClass
  instance.
*/
template<typename T>
class JSClassDefinition2 final	{
	
 public:

	JSClassDefinition2() = delete;
	~JSClassDefinition2() = default;

	JSClassDefinition(const JSClassDefinition2& rhs) = default;
	JSClassDefinition(JSClassDefinition2&& rhs) = default;

	JSClassDefinition& operator=(const JSClassDefinition2& rhs) = default;
	JSClassDefinition& operator=(JSClassDefinition2&& rhs) = default;

	JSString                            get_class_name()                   const { return class_name_; }
	std::set<JSClassAttributes>         get_attributes()                   const { return attributes_; }
	std::shared_ptr<JSClass2>           get_parent_class_ptr()             const { return parent_class_ptr_; }
	std::set<JSStaticValue>             get_static_values()                const { return static_values_; }
	std::set<JSStaticFunction>          get_static_functions()             const { return static_functions_; }
	JSCustomObject::InitializeCallback        get_initialize_callback()          const { return initialize_callback_; }
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

	// Only the JSClassBuilder can create instances of JSClass.
	template<typename T>
	JSClassDefinition2(const JSClassBuilder2<T>& builder);

	// For interoperability with the JavaScriptCore C API.
	operator ::JSClassDefinition const*() const {
		return &js_class_definition_;
	}

	// Return true if the two JSClassDefinitions are equal.
	template<typename T>
	friend bool operator==(const JSClassDefinition<T>& lhs, const JSClassDefinition<T>& rhs);

	// Define a strict weak ordering for two JSClassDefinitions.
	template<typename T>
	friend bool operator<(const JSClassDefinition<T>& lhs, const JSClassDefinition<T>& rhs);

	friend class JSClass2;
	template<typename T>
	friend class JSClassBuilder2;

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
template<typename T>
bool operator==(const JSClassDefinition2<T>& lhs, const JSClassDefinition<T>& rhs) {
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

template<typename T>
bool operator!=(const JSClassDefinition<T>& lhs, const JSClassDefinition<T>& rhs) {
	return ! (lhs == rhs);
}

// Define a strict weak ordering for two JSClassDefinitions.
template<typename T>
bool operator<(const JSClassDefinition<T>& lhs, const JSClassDefinition<T>& rhs) {
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

template<typename T>
bool operator>(const JSClassDefinition<T>& lhs, const JSClassDefinition<T>& rhs) {
	return rhs < lhs;
}

template<typename T>
bool operator<=(const JSClassDefinition<T>& lhs, const JSClassDefinition<T>& rhs) {
	return !(lhs > rhs);
}

template<typename T>
bool operator>=(const JSClassDefinition<T>& lhs, const JSClassDefinition<T>& rhs) {
	return !(lhs < rhs);
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITION_HPP_
