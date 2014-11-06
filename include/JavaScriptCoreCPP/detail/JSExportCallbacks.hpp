/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCALLBACKS_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCALLBACKS_HPP_

// #include "JavaScriptCoreCPP/JSString.hpp"
// #include "JavaScriptCoreCPP/JSObject.hpp"
#include <vector>

namespace JavaScriptCoreCPP {
class JSString;
class JSValue;
class JSObject;
}


namespace JavaScriptCoreCPP { namespace detail {

/*! 
  @typedef GetNamedValuePropertyCallback

  @abstract The callback to invoke when getting a property's value
  from a JavaScript object.

  @discussion For example, given this class definition:
  
  class Foo {
    JSValue GetBar() const;
  };

  You would define the callback like this:

  GetNamedValuePropertyCallback callback(&Foo::GetBar);

  @param 1 A const reference to the C++ object that returns the
  property's value.

  @result Return the named property's value.
*/
template<typename T>
using GetNamedValuePropertyCallback = std::function<JSValue(const T&)>;
	
/*! 
  @typedef SetNamedValuePropertyCallback

  @abstract The callback to invoke when setting a property's value on
  a JavaScript object.

  @discussion For example, given this class definition:
  
  class Foo {
    bool SetBar(JSObject object, const JSValue&& value);
  };

  You would define the callback like this:

  SetNamedValuePropertyCallback callback(&Foo::SetBar);

  @param 1 A non-const reference to the C++ object that sets the
  property's value.

  @param 2 A const rvalue reference to the property's value.

  @result Return true to indicate that the property was set.
*/
template<typename T>
using SetNamedValuePropertyCallback = std::function<bool(T&, const JSValue&&)>;

/*! 
  @typedef CallNamedFunctionCallback

  @abstract The callback to invoke when a JavaScript object is called
  as a function.

  @discussion For example, given this class definition:
  
  class Foo {
    JSValue Hello(const std::vector<JSValue>&& arguments, JSObject&& this_object);
  };

  You would define the callback like this:

  CallNamedFunctionCallback callback(&Foo::Hello);
  
  In the JavaScript expression 'myObject.myFunction()', then
  'myFunction' is the instance of Foo being called, and this_object
  would be set to 'myObject'.

  @param 1 A non-const reference to the C++ object that provides the
  JavaScript function call behavior.

  @param 2 A const rvalue reference to the JSValue array of arguments
  to pass to the function.

  @param 3 An rvalue reference to the "this" JavaScript object.

  @result Return the function's value.
*/
template<typename T>
using CallNamedFunctionCallback = std::function<JSValue(T&, const std::vector<JSValue>&&, JSObject&&)>;
	
/*! 
  @typedef InitializeCallback

  @abstract The callback to invoke when a JavaScript object is first
  created. Unlike the other object callbacks, the initialize callback
  is called on the least derived object (the parent object) first, and
  the most derived object last, analogous to the way C++ constructors
  work in a class hierarchy.
  
  For example, given this class definition:
  
  class Foo {
    void Initialize();
  };

  You would define the callback like this:
  
  InitializeCallback callback(&Foo::Initialize);
  
  @param 1 A non-const reference to the C++ object being initialized.
*/
template<typename T>
using InitializeCallback = std::function<void(T&)>;
	
/*! 
  @typedef FinalizeCallback

  @abstract The callback to invoke when a JavaScript object is
  finalized (prepared for garbage collection). This callback is
  invoked immediately before your C++ class destructor. An object may
  be finalized on any thread.
  
  @discussion The finalize callback is called on the most derived
  object first, and the least derived object (the parent object) last,
  analogous to that way C++ destructors work in a class hierarchy.
	  
  You must not call any function that may cause a garbage collection
  or an allocation of a garbage collected object from within a
  FinalizeCallback. This basically means don't create any object whose
  class name begins with JS (e.g. JSString, JSValue, JSObject, etc.)
  and don't call any methods on such objects that you may already have
  a reference to.
	  
  For example, given this class definition:

  class Foo {
    void Finalize(void* native_object_ptr);
  };

  You would define the callback like this:

  FinalizeCallback callback(&Foo::Finalize);

  @param 1 A non-const reference to the C++ object being finalized.

  @param 2 A pointer to the native object being finalized.
*/
template<typename T>
using FinalizeCallback = std::function<void(T&, void*)>;
	
/*! 
  @typedef CallAsFunctionCallback

  @abstract The callback to invoke when a JavaScript object is called
  as a function. If this callback does not exist, then calling your
  object as a function will throw a JavaScript exception.

  @discussion For example, given this class definition:
  
  class Foo {
    JSValue CallAsFunction(const std::vector<JSValue>&& arguments, JSObject&& this_object);
  };

  You would define the callback like this:

  CallAsFunctionCallback callback(&Foo::CallAsFunction);
  
  In the JavaScript expression 'myObject.myFunction()', then
  'myFunction' is the instance of Foo being called, and this_object
  would be set to 'myObject'.

  @param 1 A non-const reference to the C++ object that provides the
  JavaScript function call behavior.

  @param 2 A const rvalue reference to the JSValue array of arguments
  to pass to the function.

  @param 3 An rvalue reference to the "this" JavaScript object.

  @result Return the function's value.
*/
template<typename T>
using CallAsFunctionCallback = std::function<JSValue(T&, const std::vector<JSValue>&&, JSObject&&)>;

/*! 
  @typedef CallAsConstructorCallback

  @abstract The callback to invoke when a JavaScript object is used as
  a constructor in a 'new' expression. If you provide this callback
  then you must also provide the HasInstanceCallback as well.

  @discussion If this callback doest not exist, then using your object
  as a constructor in a 'new' expression will throw a JavaScript
  exception.
  
  For example, given this class definition:
  
  class Foo {
    JSObject CallAsConstructor(const std::vector<JSValue>&& arguments);
  };
  
  You would define the callback like this:

  CallAsConstructorCallback callback(&Foo::CallAsConstructor);
	  
  If your callback were invoked by the JavaScript expression
  'new myConstructor()', then 'myConstructor' is the instance of Foo
  being called.

  @param 1 A non-const reference to the C++ object that provides the
  JavaScript constructor 'new' behavior.

  @param 2 A const rvalue reference to the JSValue array of arguments
  to pass to the constructor.

  @result Return a JSObject that is the constructor's return value.
*/
template<typename T>
using CallAsConstructorCallback = std::function<JSObject(T&, const std::vector<JSValue>&&)>;

/*! 
  @typedef HasInstanceCallback

  @abstract The callback to invoke when a JavaScript object is used as
  the target of an 'instanceof' expression. If you provide this
  callback then you must also provide the CallAsConstructorCallback as
  well.

  @discussion If this callback does not exist, then 'instanceof'
  expressions that target your object will return false.

  For example, given this class definition:
  
  class Foo {
    bool HasInstance(const JSValue&& possible_instance) const;
  };

  You would define the callback like this:

  HasInstanceCallback callback(&Foo::HasInstance);

  If your callback were invoked by the JavaScript expression
  'someValue instanceof myObject', then 'myObject' is the instance of
  Foo being called and 'someValue' is the possible_instance parameter.

  @param 1 A non-const reference to the C++ object that provides the
  JavaScript 'instanceof' behavior.

  @param 2 A const rvalue reference to the JSValue being tested to
  determine if it is an instance of parameter 1.

  @result Return true to indicate parameter 2 is an 'instanceof'
  parameter 1.
*/
template<typename T>
using HasInstanceCallback = std::function<bool(const T&, const JSValue&&)>;

/*! 
  @typedef ConvertToTypeCallback

  @abstract The callback to invoke when converting a JavaScript object
  to another JavaScript type. This function is only invoked when
  converting a JavaScript object to a number or a string. A JavaScript
  object converted to boolean is 'true.' A JavaScript object converted
  to a JavaScript object is itself.

  @discussion For example, given this class definition:
  
  class Foo {
    JSValue ConvertToType(const JSValue::Type&& type) const;
  };

  You would define the callback like this:
  
  ConvertToTypeCallback callback(&Foo::ConvertToType);

  @param 1 A non-const reference to the C++ object that provides the
  JavaScript conversion behavior.

  @param 2 A const rvalue reference to the JSValue::Type specifying
  the JavaScript type to convert to.

  @result Return the objects's converted value. Return JSUndefined to
  forward the reqeust to the class' parent class chain, then the
  JavaScript object's prototype chain.
*/
// template<typename T>
// using ConvertToTypeCallback = std::function<JSValue(const T&, ::JavaScriptCoreCPP::JSValue::Type&&)>;

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCALLBACKS_HPP_
