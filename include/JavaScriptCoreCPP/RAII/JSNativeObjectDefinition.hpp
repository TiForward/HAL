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
	
 private:

	// Only the JSNativeObjectBuilder can create instances of
	// JSNativeObjectDefinition.
	JSNativeObjectDefinition(const JSNativeObjectBuilder<T>& builder);

	template<typename U>
	friend class JSNativeObjectBuilder;

	JSString                                                                name_;
	std::string                                                             class_name_for_js_class_definition_;
	std::unordered_set<JSNativeObjectAttribute>                             attributes_;
	std::shared_ptr<JSNativeObject<T>>                                      parent_ptr_                          { nullptr };
	std::unordered_map<std::string, JSNativeObjectValuePropertyCallback<T>> value_property_callback_map_;
	std::unordered_set<JSNativeObjectFunctionPropertyCallback<T>>           function_property_callback_map_;
	InitializeCallback<T>                                                   initialize_callback_                 { nullptr };
	FinalizeCallback<T>                                                     finalize_callback_                   { nullptr };
	HasPropertyCallback<T>                                                  has_property_callback_               { nullptr };
	GetPropertyCallback<T>                                                  get_property_callback_               { nullptr };
	SetPropertyCallback<T>                                                  set_property_callback_               { nullptr };
	DeletePropertyCallback<T>                                               delete_property_callback_            { nullptr };
	GetPropertyNamesCallback<T>                                             get_property_names_callback_         { nullptr };
	CallAsFunctionCallback<T>                                               call_as_function_callback_           { nullptr };
	CallAsConstructorCallback<T>                                            call_as_constructor_callback_        { nullptr };
	HasInstanceCallback<T>                                                  has_instance_callback_               { nullptr };
	ConvertToTypeCallback<T>                                                convert_to_type_callback_            { nullptr };
	JSClass                                                                 js_class_;

	// For interoperability with the JavaScriptCore C API.
	JSClassDefinition                                                       js_class_definition_ = kJSClassDefinitionEmpty;
	std::vector<JSStaticValue>                                              js_static_value_vector_;
	std::vector<JSStaticFunction>                                           js_static_function_vector_;

};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTDEFINITION_HPP_
