/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTDEFINITION_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTDEFINITION_HPP_

#include "JavaScriptCoreCPP/RAII/JSNativeObjectCallbacks.hpp"
#include "JavaScriptCoreCPP/RAII/JSNativeObjectFunctionPropertyCallback.hpp"
#include "JavaScriptCoreCPP/RAII/JSNativeObjectValuePropertyCallback.hpp"
#include <unordered_set>

namespace JavaScriptCoreCPP { namespace RAII {

template<typename T>
class JSNativeObject;

template<typename T>
class JSNativeObjectBuilder;

/*!
  @class

  @abstract This is a private helper class used by
  JSNativeObjectBuilder and JSNativeObject that contains the
  properties and callbacks that define a type of JavaScript object
  backed by a native object.
*/
template<typename T>
class JSNativeObjectDefinition final	{
	
 public:
	
	JSNativeObjectDefinition() = delete;
	~JSNativeObjectDefinition() = default;
	
	JSNativeObjectDefinition(const JSNativeObjectDefinition& rhs) = default;
	JSNativeObjectDefinition(JSNativeObjectDefinition&& rhs) = default;
	
	JSNativeObjectDefinition& operator=(const JSNativeObjectDefinition& rhs) = default;
	JSNativeObjectDefinition& operator=(JSNativeObjectDefinition&& rhs) = default;
	
	JSString                                                      get_class_name()                          const { return class_name_; }
	std::unordered_set<JSNativeObjectAttributes>                  get_attributes()                          const { return attributes_; }
	std::shared_ptr<JSNativeObject<T>>                            get_parent_ptr()                          const { return parent_ptr_; }
	std::unordered_set<JSNativeObjectValuePropertyCallback<T>>    get_value_property_callbacks()            const { return value_property_callbacks_; }
	std::unordered_set<JSNativeObjectFunctionPropertyCallback<T>> get_function_property_callbacks()         const { return function_property_callbacks_; }
	InitializeCallback<T>                                         get_initialize_callback()                 const { return initialize_callback_; }
	FinalizeCallback<T>                                           get_finalize_callback()                   const { return finalize_callback_; }
	HasPropertyCallback<T>                                        get_has_property_callback()               const { return has_property_callback_; }
	GetPropertyCallback<T>                                        get_get_property_callback()               const { return get_property_callback_; }
	SetPropertyCallback<T>                                        get_set_property_callback()               const { return set_property_callback_; }
	DeletePropertyCallback<T>                                     get_delete_property_callback()            const { return delete_property_callback_; }
	GetPropertyNamesCallback<T>                                   get_get_property_names_callback()         const { return get_property_names_callback_; }
	CallAsFunctionCallback<T>                                     get_call_as_function_callback()           const { return call_as_function_callback_; }
	CallAsFunctionWithThisCallback<T>                             get_call_as_function_with_this_callback() const { return call_as_function_with_this_callback_; }
	CallAsConstructorCallback<T>                                  get_call_as_constructor_callback()        const { return call_as_constructor_callback_; }
	HasInstanceCallback<T>                                        get_has_instance_callback()               const { return has_instance_callback_; }
	ConvertToTypeCallback<T>                                      get_convert_to_type_callback()            const { return convert_to_type_callback_; }

 private:

	// Only the JSNativeObjectBuilder can create instances of
	// JSNativeObjectDefinition.
	JSNativeObjectDefinition(const JSNativeObjectBuilder<T>& builder);

	template<typename U>
	friend class JSNativeObjectBuilder;

	// Return true if the two JSNativeObjectDefinitions are equal.
	template<typename U>
	friend bool operator==(const JSNativeObjectDefinition<U>& lhs, const JSNativeObjectDefinition<U>& rhs);

	// Define a strict weak ordering for two JSNativeObjectDefinitions.
	template<typename U>
	friend bool operator<(const JSNativeObjectDefinition<U>& lhs, const JSNativeObjectDefinition<U>& rhs);

	JSString                                                      class_name_;
	std::string                                                   class_name_for_js_class_definition_;
	std::unordered_set<JSNativeObjectAttributes>                  attributes_;
	std::shared_ptr<JSNativeObject<T>>                            parent_ptr_                          { nullptr };
	std::unordered_set<JSNativeObjectValuePropertyCallback<T>>    value_property_callbacks_;
	std::unordered_set<JSNativeObjectFunctionPropertyCallback<T>> function_property_callbacks_;
	InitializeCallback<T>                                         initialize_callback_                 { nullptr };
	FinalizeCallback<T>                                           finalize_callback_                   { nullptr };
	HasPropertyCallback<T>                                        has_property_callback_               { nullptr };
	GetPropertyCallback<T>                                        get_property_callback_               { nullptr };
	SetPropertyCallback<T>                                        set_property_callback_               { nullptr };
	DeletePropertyCallback<T>                                     delete_property_callback_            { nullptr };
	GetPropertyNamesCallback<T>                                   get_property_names_callback_         { nullptr };
	CallAsFunctionCallback<T>                                     call_as_function_callback_           { nullptr };
	CallAsFunctionWithThisCallback<T>                             call_as_function_with_this_callback_ { nullptr };
	CallAsConstructorCallback<T>                                  call_as_constructor_callback_        { nullptr };
	HasInstanceCallback<T>                                        has_instance_callback_               { nullptr };
	ConvertToTypeCallback<T>                                      convert_to_type_callback_            { nullptr };
};

// Return true if the two JSNativeObjectDefinitions are equal.
template<typename T>
bool operator==(const JSNativeObjectDefinition<T>& lhs, const JSNativeObjectDefinition<T>& rhs) {
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
bool operator!=(const JSNativeObjectDefinition<T>& lhs, const JSNativeObjectDefinition<T>& rhs) {
	return ! (lhs == rhs);
}

// Define a strict weak ordering for two JSNativeObjectDefinitions.
template<typename T>
bool operator<(const JSNativeObjectDefinition<T>& lhs, const JSNativeObjectDefinition<T>& rhs) {
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
bool operator>(const JSNativeObjectDefinition<T>& lhs, const JSNativeObjectDefinition<T>& rhs) {
	return rhs < lhs;
}

template<typename T>
bool operator<=(const JSNativeObjectDefinition<T>& lhs, const JSNativeObjectDefinition<T>& rhs) {
	return !(lhs > rhs);
}

template<typename T>
bool operator>=(const JSNativeObjectDefinition<T>& lhs, const JSNativeObjectDefinition<T>& rhs) {
	return !(lhs < rhs);
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTDEFINITION_HPP_
