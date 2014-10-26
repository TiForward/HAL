/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_DETAIL_JSNATIVEOBJECTDEFINITION_HPP_
#define _JAVASCRIPTCORECPP_RAII_DETAIL_JSNATIVEOBJECTDEFINITION_HPP_

#include "JavaScriptCoreCPP/RAII/JSNativeObjectCallbacks.hpp"
#include "JavaScriptCoreCPP/RAII/detail/JSNativeObjectFunctionPropertyCallback.hpp"
#include "JavaScriptCoreCPP/RAII/detail/JSNativeObjectValuePropertyCallback.hpp"
#include <unordered_set>

namespace JavaScriptCoreCPP { namespace RAII {

template<typename T>
class JSNativeObject;

template<typename T>
class JSNativeObjectBuilder;

}} // namespace JavaScriptCoreCPP { namespace RAII {


namespace JavaScriptCoreCPP { namespace detail {

template<typename T>
using JSNativeObjectValuePropertyCallbackMap_t = std::unordered_map<std::string, JSNativeObjectValuePropertyCallback<T>>;
		
template<typename T>
using JSNativeObjectFunctionPropertyCallbackMap_t = std::unordered_map<std::string, JSNativeObjectFunctionPropertyCallback<T>>;

using namespace JavaScriptCoreCPP::RAII;

/*!
  @class

  @abstract This is a private helper class used by
  JSNativeObjectBuilder and JSNativeObject that contains the
  properties and callbacks that define a type of JavaScript object
  backed by a native object.

  This class is immutable and thread-safe.
*/
template<typename T>
class JSNativeObjectDefinition final	{

 public:
	
	~JSNativeObjectDefinition() {
	}

	// Copy constructor.
	JSNativeObjectDefinition(const JSNativeObjectDefinition& rhs)
			: name_(rhs.name_)
			, class_name_for_js_class_definition_(name_)
			, attributes_(rhs.attributes_)
			, parent_ptr_(rhs.parent_ptr_)
			, value_property_callback_map_(rhs.value_property_callback_map_)
			, function_property_callback_map_(rhs.function_property_callback_map_)
			, initialize_callback_(rhs.initialize_callback_)
			, finalize_callback_(rhs.finalize_callback_)
			, has_property_callback_(rhs.has_property_callback_)
			, get_property_callback_(rhs.get_property_callback_)
			, set_property_callback_(rhs.set_property_callback_)
			, delete_property_callback_(rhs.delete_property_callback_)
			, get_property_names_callback_(rhs.get_property_names_callback_)
			, call_as_function_callback_(rhs.call_as_function_callback_)
			, call_as_constructor_callback_(rhs.call_as_constructor_callback_)
			, has_instance_callback_(rhs.has_instance_callback_)
			, convert_to_type_callback_(rhs.convert_to_type_callback_)
			, js_class_(rhs.js_class_)
			, js_class_definition_(kJSClassDefinitionEmpty)
			, js_static_values_(CreateJSStaticValueVector())
			, js_static_functions_(CreateJSStaticFunctionVector()) {

		InitializeJSClassDefinition();
		js_class_ = JSClass(&js_class_definition_);
	}
	
	// Move constructor.
	JSNativeObjectDefinition(JSNativeObjectDefinition&& rhs)
			: name_(rhs.name_)
			, class_name_for_js_class_definition_(name_)
			, attributes_(rhs.attributes_)
			, parent_ptr_(rhs.parent_ptr_)
			, value_property_callback_map_(rhs.value_property_callback_map_)
			, function_property_callback_map_(rhs.function_property_callback_map_)
			, initialize_callback_(rhs.initialize_callback_)
			, finalize_callback_(rhs.finalize_callback_)
			, has_property_callback_(rhs.has_property_callback_)
			, get_property_callback_(rhs.get_property_callback_)
			, set_property_callback_(rhs.set_property_callback_)
			, delete_property_callback_(rhs.delete_property_callback_)
			, get_property_names_callback_(rhs.get_property_names_callback_)
			, call_as_function_callback_(rhs.call_as_function_callback_)
			, call_as_constructor_callback_(rhs.call_as_constructor_callback_)
			, has_instance_callback_(rhs.has_instance_callback_)
			, convert_to_type_callback_(rhs.convert_to_type_callback_)
			, js_class_(rhs.js_class_)
			, js_class_definition_(rhs.js_class_definition_)
			, js_static_values_(rhs.js_static_values_)
			, js_static_functions_(rhs.js_static_functions_) {
	}
  
  // Create a copy of another JSNativeObjectDefinition by assignment. This is a unified
	// assignment operator that fuses the copy assignment operator,
  // X& X::operator=(const X&), and the move assignment operator,
  // X& X::operator=(X&&);
	JSNativeObjectDefinition& operator=(JSNativeObjectDefinition rhs) {
		swap(*this, rhs);
    return *this;
  }
  
	friend void swap(JSNativeObjectDefinition& first, JSNativeObjectDefinition& second) noexcept {
		// enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(first.name_                              , second.name_);
    swap(first.class_name_for_js_class_definition_, second.class_name_for_js_class_definition_);
    swap(first.attributes_                        , second.attributes_);
    swap(first.parent_ptr_                        , second.parent_ptr_);
    swap(first.value_property_callback_map_       , second.value_property_callback_map_);
    swap(first.function_property_callback_map_    , second.function_property_callback_map_);
    swap(first.initialize_callback_               , second.initialize_callback_);
    swap(first.finalize_callback_                 , second.finalize_callback_);
    swap(first.has_property_callback_             , second.has_property_callback_);
    swap(first.get_property_callback_             , second.get_property_callback_);
    swap(first.set_property_callback_             , second.set_property_callback_);
    swap(first.delete_property_callback_          , second.delete_property_callback_);
    swap(first.get_property_names_callback_       , second.get_property_names_callback_);
    swap(first.call_as_function_callback_         , second.call_as_function_callback_);
    swap(first.call_as_constructor_callback_      , second.call_as_constructor_callback_);
    swap(first.has_instance_callback_             , second.has_instance_callback_);
    swap(first.convert_to_type_callback_          , second.convert_to_type_callback_);
    swap(first.js_class_                          , second.js_class_);
    swap(first.js_class_definition_               , second.js_class_definition_);
    swap(first.js_static_values_                  , second.js_static_values_);
    swap(first.js_static_functions_               , second.js_static_functions_);
	}
	
 private:

	// Only the JSNativeObjectBuilder can create instances of
	// JSNativeObjectDefinition.
	JSNativeObjectDefinition(const JSNativeObjectBuilder<T>& builder);

	std::vector<JSStaticValue>    CreateJSStaticValueVector();
	std::vector<JSStaticFunction> CreateJSStaticFunctionVector();
	void InitializeJSClassDefinition();
	
	template<typename U>
	friend class JSNativeObject;

	template<typename U>
	friend class JSNativeObjectBuilder;

	JSString                                       name_;
	std::string                                    class_name_for_js_class_definition_;
	std::unordered_set<JSNativeObjectAttribute>    attributes_;
	std::shared_ptr<JSNativeObject<T>>             parent_ptr_                          { nullptr };
	JSNativeObjectValuePropertyCallbackMap_t<T>    value_property_callback_map_;
	JSNativeObjectFunctionPropertyCallbackMap_t<T> function_property_callback_map_;
	InitializeCallback<T>                          initialize_callback_                 { nullptr };
	FinalizeCallback<T>                            finalize_callback_                   { nullptr };
	HasPropertyCallback<T>                         has_property_callback_               { nullptr };
	GetPropertyCallback<T>                         get_property_callback_               { nullptr };
	SetPropertyCallback<T>                         set_property_callback_               { nullptr };
	DeletePropertyCallback<T>                      delete_property_callback_            { nullptr };
	GetPropertyNamesCallback<T>                    get_property_names_callback_         { nullptr };
	CallAsFunctionCallback<T>                      call_as_function_callback_           { nullptr };
	CallAsConstructorCallback<T>                   call_as_constructor_callback_        { nullptr };
	HasInstanceCallback<T>                         has_instance_callback_               { nullptr };
	ConvertToTypeCallback<T>                       convert_to_type_callback_            { nullptr };
	JSClass                                        js_class_;
	
	// For interoperability with the JavaScriptCore C API.
	JSClassDefinition                              js_class_definition_;
	std::vector<JSStaticValue>                     js_static_values_;
	std::vector<JSStaticFunction>                  js_static_functions_;
};


// For hash functions for std::unordered_set<JSNativeObjectAttribute>
// and interoperability with the JavaScriptCore C API.
inline
JSClassAttributes ToJSClassAttributes(const std::unordered_set<JSNativeObjectAttribute>& attributes) {
	using property_attribute_underlying_type = std::underlying_type<JSNativeObjectAttribute>::type;
	std::bitset<2> class_attributes;
	for (auto class_attribute : attributes) {
		const auto bit_position = static_cast<property_attribute_underlying_type>(class_attribute);
		class_attributes.set(bit_position);
	}
	
	return static_cast<property_attribute_underlying_type>(class_attributes.to_ulong());
}

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_RAII_DETAIL_JSNATIVEOBJECTDEFINITION_HPP_
