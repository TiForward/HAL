/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _HAL_JSEXPORTOBJECT_HPP_
#define _HAL_JSEXPORTOBJECT_HPP_

#include "HAL/JSExport.hpp"
#include "HAL/JSContext.hpp"
#include "HAL/JSString.hpp"
#include "HAL/JSValue.hpp"

#include <vector>
#include <unordered_set>

namespace HAL { namespace detail {
  template<typename T>
  class JSExportClass;
}}

namespace HAL {
  
  /*!
   @class
   
   @discussion A JSExportObject is a convenience base class for
   integrating your own C++ classes into JavaScriptCore.
   */
  class HAL_EXPORT JSExportObject : public JSExport<JSExportObject> HAL_PERFORMANCE_COUNTER2(JSExportObject) {
    
  public:
    
    /*!
     @method
     
     @abstract Determine whether this JavaScript object has a
     property.
     
     @param property_name The name of the property to set.
     
     @result true if this JavaScript object has the property.
     */
    virtual bool HasProperty(const JSString& property_name) const HAL_NOEXCEPT final;
    
    /*!
     @method
     
     @abstract Return a property of this JavaScript object.
     
     @param property_name The name of the property to get.
     
     @result The property's value if this JavaScript object has the
     property, otherwise JSUndefined.
     */
    virtual JSValue GetProperty(const JSString& property_name) const HAL_NOEXCEPT final;
    
    /*!
     @method
     
     @abstract Set a property on this JavaScript object with an
     optional set of attributes.
     
     @param property_name The name of the property to set.
     
     @param value The value of the the property to set.
     
     @param attributes An optional set of property attributes to give
     to the property.
     
     @result true if the the property was set.
     */
    virtual bool SetProperty(const JSString& property_name, const JSValue& property_value) HAL_NOEXCEPT final;
    
    /*!
     @method
     
     @abstract Delete a property from this JavaScript object.
     
     @param property_name The name of the property to delete.
     
     @result true if the property was deleted.
     */
    virtual bool DeleteProperty(const JSString& property_name) HAL_NOEXCEPT final;
    
    /*!
     @method
     
     @abstract Return the names of this JavaScript object's enumerable
     properties.
     
     @result A JSPropertyNameArray containing the names object's
     enumerable properties.
     */
    virtual std::vector<JSString> GetEnumerablePropertyNames() const HAL_NOEXCEPT final;
    
    /*!
     @method
     
     @abstract Call this JavaScript object as a function. By default
     this method simply returns JSUndefined to the caller. However,
     your derived class can override this method to provide custom
     'CallAsFunction' behavior.
     
     @discussion In the JavaScript expression 'myObject.myFunction()',
     the 'this_object' parameter will be set to 'myObject' and this
     JavaScript object is 'myFunction'.
     
     @param arguments The arguments passed to the function.
     
     @param this_object The JavaScript object to use as 'this'.
     
     @result Return the function's return value.
     */
    virtual JSValue CallAsFunction(const std::vector<JSValue>& arguments, JSObject this_object);
    
    /*!
     @method
     
     @abstract Return this JavaScript object's prototype.
     
     @result This JavaScript object's prototype.
     */
    virtual JSValue GetPrototype() const HAL_NOEXCEPT final;
    
    /*!
     @method
     
     @abstract Sets this JavaScript object's prototype.
     
     @param value The value to set as this JavaScript object's
     prototype.
     */
    virtual bool SetPrototype(const JSValue& js_value) HAL_NOEXCEPT final;

    /*!
     @method
     
     @abstract Return the execution context of this JavaScript value.
     
     @result The the execution context of this JavaScript value.
     */
    virtual JSContext get_context() const HAL_NOEXCEPT final;
    
    /*!
     @method
     
     @abstract This mandatory constructor is invoked when your
     JavaScript object is created not as the result of a JavaScript
     'new' expression.
     
     @param js_context The JSContext in which your JavaScript object
     is created.
     */
    JSExportObject(const JSContext& js_context) HAL_NOEXCEPT;
    
    /*!
     @method
     
     @abstract This mandatory constructor is invoked when your
     JavaScript object is created as the result of being called in a
     JavaScript 'new' expression.
     
     @param arguments An optional list of JSValues to initialize your
     JavaScript object with as the result of being called in a
     JavaScript 'new' expression.
     */
    JSExportObject(const JSExportObject&, const std::vector<JSValue>& arguments) HAL_NOEXCEPT;
    virtual ~JSExportObject()  HAL_NOEXCEPT;
    JSExportObject(const JSExportObject&)            = default;
    JSExportObject& operator=(const JSExportObject&) = default;
#ifdef TITANIUM_MOVE_CTOR_AND_ASSIGN_DEFAULT_ENABLE
    JSExportObject(JSExportObject&&)                 = default;
    JSExportObject& operator=(JSExportObject&&)      = default;
#endif

    void swap(JSExportObject&) HAL_NOEXCEPT;
    
    static void JSExportInitialize();
    
  private:
    
    /*!
     @method
     
     @abstract Collect the names of this JavaScript object's
     properties.
     
     @discussion Derived classes should provide only the names of
     properties that this JavaScript object provides through the
     GetProperty and SetProperty methods. Other property names are
     automatically added from properties vended by the JavaScript
     object's parent class chain and properties belonging to the
     JavaScript object's prototype chain.
     
     @param accumulator A JavaScript property name accumulator in
     which to accumulate the names of this JavaScript object's
     properties. Use JSPropertyNameAccumulator::AddName to add
     property names to the accumulator. Property name accumulators are
     used by JavaScript for...in loops.
     */
    virtual void GetPropertyNames(const JSPropertyNameAccumulator& accumulator) const HAL_NOEXCEPT final;
    
    JSObject js_object__;
    
#undef  HAL_JSEXPORTOBJECT_LOCK_GUARD
#ifdef  HAL_THREAD_SAFE
    std::recursive_mutex mutex__;
#define HAL_JSEXPORTOBJECT_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock(mutex__)
#else
#define HAL_JSEXPORTOBJECT_LOCK_GUARD
#endif  // HAL_THREAD_SAFE
  };
  
  inline
  void swap(JSExportObject& first, JSExportObject& second) HAL_NOEXCEPT {
    first.swap(second);
  }
  
} // namespace HAL {

#endif // _HAL_JSEXPORTOBJECT_HPP_
