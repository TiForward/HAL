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
class JSClassDefinitionBuilder;

/*!
  @class

  @abstract.......................... This class contains the
                                      properties and callbacks that
                                      define a type of JavaScript
                                      object. All properties except
                                      class_name are optional, and all
                                      callbacks may be nullptr.

  @field attributes.................. An optional set of
                                      JSClassAttributes that describe
                                      the characteristics of this
                                      class.
                                      

  @field class_name.................. The class's name.

  @field parent_class................ An optional JSClass to set as
                                      the class's parent class. Pass
                                      nullptr to use the default
                                      object class.

  @field static_values............... An optional JSStaticValue array
                                      containing the class's
                                      statically declared value
                                      properties. Pass nullptr to
                                      specify no statically declared
                                      value properties.
  
  @field static_functions............ An optional JSStaticFunction
                                      array containing the class's
                                      statically declared function
                                      properties. Pass nullptr to
                                      specify no statically declared
                                      function properties.

  @field initialize_callback......... An optional callback to invoke
                                      when an object is first
                                      created. Use this callback to
                                      initialize the object.
  
  @field finalize_callback........... An optional callback to invoke
                                      when an object is finalized
                                      (prepared for garbage
                                      collection). Use this callback
                                      to release resources allocated
                                      for the object, and perform
                                      other cleanup.
  
  @field has_property_callback....... An optional callback invoked
                                      when determining whether an
                                      object has a property. Passing
                                      nullptr will delegate to
                                      get_property_callback. The
                                      has_property_callback enables
                                      optimization in cases where only
                                      a property's existence needs to
                                      be known, not its value, and
                                      computing its value is
                                      expensive.
  
  @field get_property_callback....... An optional callback to invoke
                                      when getting a property's value.
  
  @field set_property_callback....... An optional callback to invoke
                                      when setting a property's value.
  
  @field delete_property_callback.... An optional callback to invoke
                                      when deleting a property.
  
  @field get_property_names_callback. An optional callback to invoke
                                      when collecting the names of an
                                      object's properties.
  
  @field call_as_function_callback... An optional callback to invoke
                                      when an object is called as a
                                      function.
  
  @field call_as_constructor_callback An optional callback to invoke
                                      when an object is used as a
                                      constructor in a 'new'
                                      expression. However, if you do
                                      provide this callback then you
                                      must also provide the
                                      has_instance_callback as well.
  
  @field has_instance_callback....... An optional callback to invoke
                                      when an object is used as the
                                      target of an 'instanceof'
                                      expression. However, if you do
                                      provide this callback then you
                                      must also provide the
                                      call_as_constructor_callback as
                                      well.
  
  @field convert_to_type_callback.... An optional callback to invoke
                                      when converting an object to a
                                      particular JavaScript type.

  @discussion The static_values and static_functions arrays are the
  simplest and most efficient means for vending custom
  properties. These statically declared properties autmatically
  service requests like get_property_callback, set_property_callback,
  and get_property_names_callback. Property access callbacks are
  required only to implement unusual properties, like array indexes,
  whose names are not known at compile-time.
  
  Standard JavaScript practice calls for storing function objects in
  prototypes, so they can be shared. The default JSClass constructor
  follows this idiom, instantiating objects with a shared,
  automatically generated prototype containing the class's function
  objects. The JSClassAttributes::NoAutomaticPrototype attribute
  specifies that a JSClass should not automatically generate such a
  prototype. The resulting JSClass instantiates objects with the
  default object prototype, and gives each instance object its own
  copy of the class's function objects.
  
  A nullptr callback specifies that the default object callback should
  substitute, except in the case of has_property_callback, where it
  specifies that get_property_names_callback should substitute.
*/
class JSClassDefinition final	{
	
 public:

	JSClassDefinition(const JSClassDefinitionBuilder& builder);

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
    
	// For interoperability with the JavaScriptCore C API.
	operator ::JSClassDefinition const*() const {
		return &js_class_definition_;
	}

	// Return true if the two JSClassDefinitions are equal.
	friend bool operator==(const JSClassDefinition& lhs, const JSClassDefinition& rhs);

	// Define a strict weak ordering for two JSClassDefinitions.
	friend bool operator<(const JSClassDefinition& lhs, const JSClassDefinition& rhs);

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
