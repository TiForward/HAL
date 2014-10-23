/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSBUILDER_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSBUILDER_HPP_


#include "JavaScriptCoreCPP/RAII/JSClassDefinition.hpp"
#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include <set>
#include <memory>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class

  @abstract This is a builder that creates JSClass instances with
  user-defined properties and callbacks that define a custom type of
  JavaScript object. All properties except class_name are optional,
  and all callbacks may be nullptr.

  @discussion Adding JSStaticValue and JSStaticFunction instances to a
  JSClassBuilder are the simplest and most efficient means for vending
  custom properties since they autmatically service requests like
  get_property_callback, set_property_callback, and
  get_property_names_callback. The other property access callbacks are
  required only to implement unusual properties, like array indexes,
  whose names are not known at compile-time.
  
  Standard JavaScript practice calls for storing function objects in
  prototypes so that they can be shared. The instances of JSClass
  created by a JSClassBuilder follows this idiom, instantiating
  objects with a shared, automatically generated prototype containing
  the class's function objects.

  To override this, the JSClassAttributes::NoAutomaticPrototype
  attribute can be set which specifies that a JSClass should not
  automatically generate such a prototype so that objects created by
  the JSClass have only the default object prototype, and every object
  gets its own copy of the class's properties.
  
  Setting any callback to nullptr specifies that the default object
  callback should substitute, except in the case of
  has_property_callback, where it specifies that
  get_property_names_callback should substitute.
*/
class JSClassBuilder final {
	
 public:
	
	/**
	 * Create an empty builder.
	 */
	JSClassBuilder(const JSString& class_name) : class_name_(class_name) {
	}
	
	JSClassBuilder() = delete;;
	~JSClassBuilder() = default;

	JSClassBuilder(const JSClassBuilder& rhs) = default;
	JSClassBuilder(JSClassBuilder&& rhs) = default;

	JSClassBuilder& operator=(const JSClassBuilder& rhs) = default;
	JSClassBuilder& operator=(JSClassBuilder&& rhs) = default;

	/*!
	  @method
	  
	  @abstract Return the class's name.
	  
	  @result The class's name.
	*/
	JSString get_class_name() {
		return class_name_;
	}
	
	/*!
	  @method
	  
	  @abstract Set the class's name.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_class_name(const JSString& class_name) {
		class_name_ = class_name;
		return *this;
	}
	
	/*!
	  @method
	  
	  @abstract Return the JSClassAttributes that describe the
	  characteristics of this class.
	  
	  @result The JSClassAttributes that describe the characteristics of
	  this class.
	*/
	std::set<JSClassAttributes> get_attributes() const {
		return attributes_;
	}

	/*!
	  @method
	  
	  @abstract Set the JSClassAttributes that describe the
	  characteristics of this class.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_attributes(const std::set<JSClassAttributes>& attributes) {
		attributes_ = attributes;
		return *this;
	}
	
	/*!
	  @method
	  
	  @abstract Return the parent of the JSClass. A nullptr means to use
	  the default object class.
	  
	  @result The parent of the JSClass
	*/
	std::shared_ptr<JSClass> get_parent_class_ptr() const {
		return parent_class_ptr_;
	}

	/*!
	  @method
	  
	  @abstract Set the parent of the JSClass. A nullptr means to use
	  the default object class.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_parent_class_ptr(const std::shared_ptr<JSClass>& parent_class_ptr) {
		parent_class_ptr_ = parent_class_ptr;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the current set of of JSStaticValues that
	  describe the class's statically declared value properties.
  
	  @result The current set of of JSStaticValues that describe the
	  class's statically declared value properties.
	*/
	std::set<JSStaticValue> get_static_values() const {
		return static_values_;
	}

	/*!
	  @method

	  @abstract Replace the current set of of JSStaticValues that
	  describe the class's statically declared value properties.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_static_values(const std::set<JSStaticValue>& static_values) {
		static_values_ = static_values;
		return *this;
	}
	
	/*!
	  @method

	  @abstract Return the current set of of JSStaticFunctions that
	  describe the class's statically declared function properties.

	  @result The current set of of JSStaticFunctions that describe the
	  class's statically declared function properties.
	*/
	std::set<JSStaticFunction> get_static_functions() const {
		return static_functions_;
	}

	/*!
	  @method

	  @abstract Replace the current set of of JSStaticFunctions that
	  describe the class's statically declared function properties.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_static_functions(const std::set<JSStaticFunction>& static_functions) {
		static_functions_ = static_functions;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when an object is first
	  created. Use this callback to initialize the object.
  
	  @result The callback to invoke when an object is first created.
	*/
	JSObject::InitializeCallback get_initialize_callback() const {
		return initialize_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when an object is first
	  created. Use this callback to initialize the object.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_initialize_callback(const JSObject::InitializeCallback& initialize_callback) {
		initialize_callback_ = initialize_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when an object is
	  finalized (prepared for garbage collection). Use this callback to
	  release resources allocated for the object, and perform other
	  cleanup.
  	  
	  @result The callback to invoke when an object is finalized
	  (prepared for garbage collection).
	*/
	JSObject::FinalizeCallback get_finalize_callback() const {
		return finalize_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when an object is finalized
	  (prepared for garbage collection). Use this callback to release
	  resources allocated for the object, and perform other cleanup.
  	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_finalize_callback(const JSObject::FinalizeCallback& finalize_callback) {
		finalize_callback_ = finalize_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when determining whether
	  an object has a property. If this callback is missing (i.e. the
	  returned value is a nullptr) then the object will delegate to
	  get_property_callback. The has_property_callback enables
	  optimization in cases where only a property's existence needs to
	  be known, not its value, and computing its value is expensive.
  
	  @result The callback to invoke when determining whether an object
	  has a property.
	*/
	JSObject::HasPropertyCallback get_has_property_callback() const {
		return has_property_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when determining whether an
	  object has a property. If this callback is missing (i.e. if you
	  pass nullptr) then the object will delegate to
	  get_property_callback. The has_property_callback enables
	  optimization in cases where only a property's existence needs to
	  be known, not its value, and computing its value is expensive.
  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_has_property_callback(const JSObject::HasPropertyCallback& has_property_callback) {
		has_property_callback_ = has_property_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when getting a property's
	  value.
  	  
	  @result The callback to invoke when getting a property's value.
	*/
	JSObject::GetPropertyCallback get_get_property_callback() const {
		return get_property_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when getting a property's
	  value.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_get_property_callback(const JSObject::GetPropertyCallback& get_property_callback) {
		get_property_callback_ = get_property_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when setting a property's
	  value.
  	  
	  @result The callback to invoke when setting a property's value.
	*/
	JSObject::SetPropertyCallback get_set_property_callback() const {
		return set_property_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when setting a property's
	  value.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_set_property_callback(const JSObject::SetPropertyCallback& set_property_callback) {
		set_property_callback_ = set_property_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when deleting a property.
  	  
	  @result The callback to invoke when deleting a property.
	*/
	JSObject::DeletePropertyCallback get_delete_property_callback() const {
		return delete_property_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when deleting a property.
  	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_delete_property_callback(const JSObject::DeletePropertyCallback& delete_property_callback) {
		delete_property_callback_ = delete_property_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when collecting the names
	  of an object's properties.
  	  
	  @result The callback to invoke when collecting the names of an
	  object's properties
	*/
	JSObject::GetPropertyNamesCallback get_get_property_names_callback() const {
		return get_property_names_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when collecting the names of
	  an object's properties.
  	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_get_property_names_callback(const JSObject::GetPropertyNamesCallback& get_property_names_callback) {
		get_property_names_callback_ = get_property_names_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when an object is called
	  as a function.
  
	  @result The callback to invoke when an object is called as a
	  function.
	*/
	JSObject::CallAsFunctionCallback get_call_as_function_callback() const {
		return call_as_function_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when an object is called as a
	  function.
  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_call_as_function_callback(const JSObject::CallAsFunctionCallback& call_as_function_callback) {
		call_as_function_callback_ = call_as_function_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when an object is used as
	  a constructor in a 'new' expression. However, if you do provide
	  this callback then you must also provide the has_instance_callback
	  as well.
  
	  @result The callback to invoke when an object is used as a
	  constructor in a 'new' expression.
	*/
	JSObject::CallAsConstructorCallback get_call_as_constructor_callback() const {
		return call_as_constructor_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when an object is used as a
	  constructor in a 'new' expression. However, if you do provide this
	  callback then you must also provide the has_instance_callback as
	  well.
  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_call_as_constructor_callback(const JSObject::CallAsConstructorCallback& call_as_constructor_callback) {
		call_as_constructor_callback_ = call_as_constructor_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when an object is used as
	  the target of an 'instanceof' expression. However, if you do
	  provide this callback then you must also provide the
	  call_as_constructor_callback as well.

	  @result The callback to invoke when an object is used as the
	  target of an 'instanceof' expression.
	*/
	JSObject::HasInstanceCallback get_has_instance_callback() const {
		return has_instance_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when an object is used as the
	  target of an 'instanceof' expression. However, if you do provide
	  this callback then you must also provide the
	  call_as_constructor_callback as well.

	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_has_instance_callback(const JSObject::HasInstanceCallback& has_instance_callback) {
		has_instance_callback_ = has_instance_callback;
		return *this;
	}
	
	/*!
	  @method

	  @abstract Return the callback to invoke when converting an object
	  to a particular JavaScript type.

	  @result The callback to invoke when converting an object to a
	  particular JavaScript type.
	*/
	JSObject::ConvertToTypeCallback get_convert_to_type_callback() const {
		return convert_to_type_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when converting an object to
	  a particular JavaScript type.

	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder& set_convert_to_type_callback(const JSObject::ConvertToTypeCallback& convert_to_type_callback) {
		convert_to_type_callback_ = convert_to_type_callback;
		return *this;
	}
	
	/*!
	  @method

	  @abstract Create and return a JSClass instance with all of the
	  properties and callbacks specified in the builder.

	  @result A JSClass instance with all of the properties and
	  callbacks specified in the builder.
	*/
	JSClass build() {
		// TODO validate js_class_definition.
		return JSClass(*this);
	}
	
 private:

	JSClassDefinition get_js_class_definition() const {
		JSClassDefinition js_class_definition(*this);
		return js_class_definition;
	}

	friend class JSClassDefinition;
	friend class JSClass;

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

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSBUILDER_HPP_
