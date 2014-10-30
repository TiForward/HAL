/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSNATIVEOBJECTCALLBACKS_HPP_
#define _JAVASCRIPTCORECPP_JSNATIVEOBJECTCALLBACKS_HPP_

#include "JavaScriptCoreCPP/JSNativeClassAttribute.hpp"
#include "JavaScriptCoreCPP/JSPropertyNameAccumulator.hpp"
#include "JavaScriptCoreCPP/JSValue.hpp"
#include <vector>

namespace JavaScriptCoreCPP { namespace RAII {

class JSString;
class JSObject;
class JSValue;

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
  
  @param object A non-const reference to the native object backing the
  JavaScript object.
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
    void Finalize();
  };

  You would define the callback like this:

  FinalizeCallback callback(&Foo::Finalize);

  @param object A non-const reference to the native object backing the
  JavaScript object.
*/
template<typename T>
using FinalizeCallback = std::function<void(T&)>;
	
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
    JSObject Constructor(const std::vector<JSValue>& arguments);
  };
  
  You would define the callback like this:

  CallAsConstructorCallback callback(&Foo::Constructor);
	  
  If your callback were invoked by the JavaScript expression
  'new myConstructor()', then 'myConstructor' is the instance of Foo
  being called.

  @param object A non-const reference to the native object backing the
  JavaScript object.

  @param arguments A JSValue array of arguments to pass to the
  constructor.

  @result Return a JSObject that is the constructor's return value.
*/
template<typename T>
using CallAsConstructorCallback = std::function<JSObject(T&, const std::vector<JSValue>&)>;

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
    bool HasInstance(const JSValue& possible_instance) const;
  };

  You would define the callback like this:

  HasInstanceCallback callback(&Foo::HasInstance);

  If your callback were invoked by the JavaScript expression
  'someValue instanceof myObject', then 'myObject' is the instanceof
  of Foo being called and 'someValue' is the possible_instance
  parameter.

  @param object A const reference to the native object backing the
  JavaScript object.

  @param possible_instance The JSValue being tested to determine if it
  is an instance of object.

  @result Return true to indicate object is an 'instanceof'
  possible_instance.
*/
template<typename T>
using HasInstanceCallback = std::function<bool(const T&, const JSValue&)>;

/*! 
  @typedef GetNamedPropertyCallback

  @abstract The callback to invoke when getting a property's value
  from a JavaScript object.

  @discussion For example, given this class definition:
  
  class Foo {
    JSValue GetBar() const;
  };

  You would define the callback like this:

  GetNamedPropertyCallback callback(&Foo::GetBar);


  @param object A const reference to the native object backing the
  JavaScript object.

  @result Return the property's value if object has the property,
  otherwise return JSUndefined.
*/
template<typename T>
using GetNamedPropertyCallback = std::function<JSValue(const T&)>;
	
/*! 
  @typedef SetNamedPropertyCallback

  @abstract The callback to invoke when setting a property's value on
  a JavaScript object.

  @discussion For example, given this class definition:
  
  class Foo {
    bool SetBar(const JSValue& value);
  };

  You would define the callback like this:

  SetNamedPropertyCallback callback(&Foo::SetBar);

  @param object A non-const reference to the native object backing the
  JavaScript object.

  @param value A JSValue to use as the property's value.

  @result Return true to indicate that the property was set.
*/
template<typename T>
using SetNamedPropertyCallback = std::function<bool(T&, const JSValue&)>;

/*! 
  @typedef GetPropertyCallback

  @abstract The callback to invoke when getting a property's value
  from a JavaScript object.

  @discussion For example, given this class definition:
  
  class Foo {
    JSValue GetProperty(const JSString& property_name) const;
  };

  You would define the callback like this:

  GetPropertyCallback callback(&Foo::GetProperty);

  @param object A const reference to the native object backing the
  JavaScript object.

  @param property_name A JSString containing the name of the property
  to get.

  @result Return the property's value if object has the property.
  Return JSUndefined to forward the request to properties provided by
  the GetNamedPropertyCallback and/or SetNamedPropertyCallback
  callbacks (if any), then properties vended by the class' parent
  class chain, then properties belonging to the JavaScript object's
  prototype chain.
*/
template<typename T>
using GetPropertyCallback = std::function<JSValue(const T&, const JSString&)>;
	
/*! 
  @typedef SetPropertyCallback

  @abstract The callback to invoke when setting a property's value on
  a JavaScript object.

  @discussion For example, given this class definition:
  
  class Foo {
    bool SetProperty(const JSString& property_name, const JSValue& value);
  };

  You would define the callback like this:

  SetPropertyCallback callback(&Foo::SetProperty);

  @param object A non-const reference to the native object backing the
  JavaScript object.

  @param property_name A JSString containing the name of the property
  to set.

  @param value A JSValue to use as the property's value.

  @result Return true to indicate that the property was set. Return
  false to forward the request to properties provided by the
  GetNamedPropertyCallback and/or SetNamedPropertyCallback callbacks
  (if any), then properties vended by the class' parent class chain,
  then properties belonging to the JavaScript object's prototype
  chain.
*/
template<typename T>
using SetPropertyCallback = std::function<bool(T&, const JSString&, const JSValue&)>;

/*! 
  @typedef DeletePropertyCallback

  @abstract The callback to invoke when deleting a property from a
  JavaScript object.

  @discussion For example, given this class definition:
  
  class Foo {
    bool DeleteProperty(const JSString& property_name);
  };

  You would define the callback like this:

  DeletePropertyCallback callback(&Foo::DeleteProperty);

  @param object A non-const reference to the native object backing the
  JavaScript object.

  @param property_name A JSString containing the name of the property
  to delete.

  @result Return true to indicate that the property was
  deleted. Return false to forward the request to properties provided
  by the GetNamedPropertyCallback and/or SetNamedPropertyCallback
  callbacks (if any), then properties vended by the class' parent
  class chain, then properties belonging to the JavaScript object's
  prototype chain.
*/
template<typename T>
using DeletePropertyCallback = std::function<bool(T&, const JSString&)>;

/*! 
  @typedef GetPropertyNamesCallback

  @abstract The callback to invoke when collecting the names of a
  JavaScript object's properties.

  @discussion The GetPropertyNamesCallback only needs to provide the
  property names provided by the GetPropertyCallback and/or
  SetPropertyCallback callbacks (if any). Other property names are
  automatically added from properties provided by the
  GetNamedPropertyCallback and/or SetNamedPropertyCallback callbacks
  (if any), those properties vended by the class' parent class chain
  and properties belonging to the JavaScript object's prototype chain.

  For example, given this class definition:

  class Foo {
    void GetPropertyNames(const JSPropertyNameAccumulator& accumulator) const;
  };

  You would define the callback like this:

  GetPropertyNamesCallback callback(&Foo::GetPropertyNames);

  @param object A const reference to the native object backing the
  JavaScript object.

  @param accumulator A JavaScript property name accumulator in which
  to accumulate the names of object's properties. Use
  JSPropertyNameAccumulator::AddName to add property names to
  accumulator. Property name accumulators are used by JavaScript
  for...in loops.
*/
template<typename T>
using GetPropertyNamesCallback = std::function<void(const T&, const JSPropertyNameAccumulator&)>;

/*! 
  @typedef HasPropertyCallback

  @abstract The callback to invoke when determining whether a
  JavaScript object has a property.
  
  @discussion The HasPropertyCallback enables optimization in cases
  where only a property's existence needs to be known, not its value,
  and computing its value is expensive. If the HasPropertyCallback
  doesn't exist, then the GetPropertyCallback will be used instead.
  
  For example, given this class definition:
  
  class Foo {
    bool HasProperty(const JSString& property_name) const;
  };

  You would define the callback like this:

  HasPropertyCallback callback(&Foo::HasProperty);

  @param object A const reference to the native object backing the
  JavaScript object.

  @param property_name A JSString containing the name of the property
  to look up.

  @result Return true to indicate that object has the property. Return
  false to forward the reqeust to properties added by the
  GetNamedPropertyCallback and/or SetNamedPropertyCallback callbacks
  (if any), then properties vended by the class' parent class chain,
  then properties belonging to the JavaScript object's prototype
  chain.
*/
template<typename T>
using HasPropertyCallback = std::function<bool(const T&, const JSString&)>;
	
/*! 
  @typedef CallAsFunctionCallback

  @abstract The callback to invoke when a JavaScript object is called
  as a function. If this callback does not exist, then calling your
  object as a function will throw a JavaScript exception.

  @discussion For example, given this class definition:
  
  class Foo {
    JSValue Bar(const std::vector<JSValue>& arguments, const JSObject& this_object);
  };

  You would define the callback like this:

  CallAsFunctionWithThisCallback callback(&Foo::Bar);
  
  In the JavaScript expression 'myObject.myFunction()', then
  'myFunction' is the instance of Foo being called, and this_object
  would be set to 'myObject'.

  @param object A non-const reference to the native object backing the
  JavaScript object.

  @param arguments A JSValue array of arguments to pass to the
  function.

  @param this_object The object to use as "this".

  @result Return the function's return value.
*/
template<typename T>
using CallAsFunctionCallback = std::function<JSValue(T&, const std::vector<JSValue>&, const JSObject&)>;
	
/*! 
  @typedef ConvertToTypeCallback

  @abstract The callback to invoke when converting a JavaScript object
  to another JavaScript type. This function is only invoked when
  converting an object to a number or a string. An object converted to
  boolean is 'true.' An object converted to object is itself.

  @discussion For example, given this class definition:
  
  class Foo {
    JSValue ConvertToType(const JSValue::Type& type) const;
  };

  You would define the callback like this:
  
  ConvertToTypeCallback callback(&Foo::ConvertToType);

  @param object A const reference to the native object backing the
  JavaScript object.

  @param type A JSValue::Type specifying the JavaScript type to
  convert to.

  @result Return the objects's converted value. Return JSUndefined to
  forward the reqeust to the class' parent class chain, then the
  JavaScript object's prototype chain.
*/
template<typename T>
using ConvertToTypeCallback = std::function<JSValue(const T&, const JSValue::Type&)>;

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_JSNATIVEOBJECTCALLBACKS_HPP_
