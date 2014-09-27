// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#ifndef _Titanium_Mobile_JSValue_h_
#define _Titanium_Mobile_JSValue_h_

#include "JavaScriptCoreCPP/JSContext.h"
#include "JavaScriptCoreCPP/JSString.h"
#include <JavaScriptCore/JavaScript.h>
#include <memory>
#include <string>
#include <iostream>
#include <atomic>
#include  <cstdint>

/*!
  @interface
  @discussion A JSValue is a reference to a value within the JavaScript object space of a
  JSVirtualMachine. All instances of JSValue originate from a JSContext and
  hold a strong reference to this JSContext. As long as any value associated with
  a particular JSContext is retained, that JSContext will remain alive.
  Where an instance method is invoked upon a JSValue, and this returns another
  JSValue, the returned JSValue will originate from the same JSContext as the
  JSValue on which the method was invoked.
 
  All JavaScript values are associated with a particular JSVirtualMachine
  (the associated JSVirtualMachine is available indirectly via the context
  property). An instance of JSValue may only be passed as an argument to
  methods on instances of JSValue and JSContext that belong to the same
  JSVirtualMachine - passing a JSValue to a method on an object originating
  from a different JSVirtualMachine will result in an C++ exception
  being raised.
*/
class JSValue final : public std::enable_shared_from_this<JSValue> {

public:
    
  // Factory function for creating a smart pointer to a JSValue. This is the
  // public API to create a JSValue.
  template <typename... Ts>
      static JSValue_ptr_t create(Ts&&... params) {
    return JSValue_ptr_t(new JSValue(std::forward<Ts>(params)...), deleter{});
  }
    
  /*!
    @method
    @abstract Create a JavaScript value from a bool primitive.
    @param value
    @param context The JSContext in which the resulting JSValue will be created.
    @result The new JSValue representing the equivalent bool value.
  */
  static JSValue_ptr_t valueWithBoolInContext(bool value, const JSContext_ptr_t& context_ptr) {
    return JSValue::create(JSValueMakeBoolean(static_cast<::JSGlobalContextRef>(*context_ptr), value), context_ptr);
  }
    
  /*!
    @method
    @abstract Create a JavaScript value from a double primitive.
    @param value
    @param context The JSContext in which the resulting JSValue will be created.
    @result The new JSValue representing the equivalent double value.
  */
  static JSValue_ptr_t valueWithDoubleInContext(double value, const JSContext_ptr_t& context_ptr) {
    return JSValue::create(JSValueMakeNumber(static_cast<::JSGlobalContextRef>(*context_ptr), value), context_ptr);
  }

  /*!
    @method
    @abstract Create a JavaScript value from an <code>int32_t</code> primitive.
    @param value
    @param context The JSContext in which the resulting JSValue will be created.
    @result The new JSValue representing the equivalent int32_t value.
  */
  static JSValue_ptr_t valueWithInt32InContext(int32_t value, const JSContext_ptr_t& context_ptr) {
    return JSValue::create(JSValueMakeNumber(static_cast<::JSGlobalContextRef>(*context_ptr), value), context_ptr);
  }
    
  /*!
    @method
    @abstract Create a JavaScript value from a <code>uint32_t</code> primitive.
    @param value
    @param context The JSContext in which the resulting JSValue will be created.
    @result The new JSValue representing the equivalent uint32_t value.
  */
  static JSValue_ptr_t valueWithUInt32InContext(uint32_t value, const JSContext_ptr_t& context_ptr) {
    return JSValue::create(JSValueMakeNumber(static_cast<::JSGlobalContextRef>(*context_ptr), value), context_ptr);
  }

  /*!
    @method
    @abstract Create a JavaScript value from a <code>std::string</code>.
    @param value
    @param context The JSContext in which the resulting JSValue will be created.
    @result The new JSValue representing the equivalent std::string value.
  */
  static JSValue_ptr_t valueWithStringInContext(const std::string& value, const JSContext_ptr_t& context_ptr) {
    return JSValue::create(JSValueMakeString(static_cast<::JSGlobalContextRef>(*context_ptr), JSStringRef(JSString(value))), context_ptr);
  }

  /*!
    @method
    @abstract Create a new, empty JavaScript object.
    @param context The JSContext in which the resulting object will be created.
    @result The new JavaScript object.
  */
  static JSValue_ptr_t valueWithNewObjectInContext(const JSContext_ptr_t& context_ptr) {
    return JSValue::create(JSObjectMake(static_cast<::JSGlobalContextRef>(*context_ptr), nullptr, nullptr), context_ptr);
  }
    
  /*!
    @method
    @abstract Create a new, empty JavaScript array.
    @param context The JSContext in which the resulting array will be created.
    @result The new JavaScript array.
  */
  static JSValue_ptr_t valueWithNewArrayInContext(const JSContext_ptr_t& context_ptr) {
    return JSValue::create(JSObjectMakeArray(static_cast<::JSGlobalContextRef>(*context_ptr), 0, nullptr, nullptr), context_ptr);
  }

  /*!
    @method
    @abstract Create a new JavaScript regular expression object.
    @param pattern The regular expression pattern.
    @param flags The regular expression flags.
    @param context The JSContext in which the resulting regular expression object will be created.
    @result The new JavaScript regular expression object.
  */
  static JSValue_ptr_t valueWithNewRegularExpressionFromPatternAndFlagsInContext(const std::string& pattern, const std::string& flags, const JSContext_ptr_t& context_ptr);
    
  /*!
    @method
    @abstract Create a new JavaScript error object.
    @param message The error message.
    @param context The JSContext in which the resulting error object will be created.
    @result The new JavaScript error object.
  */
  static JSValue_ptr_t valueWithNewErrorFromMessageInContext(const std::string& message, const JSContext_ptr_t& context_ptr) {
    JSValueRef arguments[2] = { JSValueMakeString(static_cast<::JSGlobalContextRef>(*context_ptr), JSStringRef(JSString(message))) };
    return JSValue::create(JSObjectMakeError(static_cast<::JSGlobalContextRef>(*context_ptr), 1, arguments, nullptr), context_ptr);
  }
    
  /*!
    @method
    @abstract Create the JavaScript value <code>null</code>.
    @param context The JSContext to which the resulting JSValue belongs.
    @result The JSValue representing the JavaScript value <code>null</code>.
  */
  static JSValue_ptr_t valueWithNullInContext(const JSContext_ptr_t& context_ptr) {
    return JSValue::create(JSValueMakeNull(static_cast<::JSGlobalContextRef>(*context_ptr)), context_ptr);
  }

  /*!
    @method
    @abstract Create the JavaScript value <code>undefined</code>.
    @param context The JSContext to which the resulting JSValue belongs.
    @result The JSValue representing the JavaScript value <code>undefined</code>.
  */
  static JSValue_ptr_t valueWithUndefinedInContext(const JSContext_ptr_t& context_ptr) {
    return JSValue::create(JSValueMakeUndefined(static_cast<::JSGlobalContextRef>(*context_ptr)), context_ptr);
  }

  /*!
    @method
    @abstract Convert a JSValue to a boolean.
    @discussion The JSValue is converted to a boolean according to the rules specified
    by the JavaScript language.
    @result The boolean result of the conversion.
  */
  explicit operator bool() const {
    return JSValueToBoolean(static_cast<::JSGlobalContextRef>(*context_ptr_), value_);
  }
    
  /*!
    @method
    @abstract Convert a JSValue to a double.
    @discussion The JSValue is converted to a number according to the rules specified
    by the JavaScript language.
    @result The double result of the conversion.
  */
  explicit operator double() const;
    
  /*!
    @method
    @abstract Convert a JSValue to an <code>int32_t</code>.
    @discussion The JSValue is converted to an integer according to the rules specified
    by the JavaScript language.
    @result The <code>int32_t</code> result of the conversion.
  */
  explicit operator int32_t() const;
    
  /*!
    @method
    @abstract Convert a JSValue to a <code>uint32_t</code>.
    @discussion The JSValue is converted to an integer according to the rules specified
    by the JavaScript language (implements ToUInt32, defined in ECMA-262 9.6).
    @result The <code>uint32_t</code> result of the conversion.
  */
  explicit operator uint32_t() const  {
    // As commented in the spec, the operation of ToInt32 and ToUint32 only differ
    // in how the result is interpreted; see NOTEs in sections 9.5 and 9.6.
    return this->operator int32_t();
  }

  /*!
    @method
    @abstract Convert a JSValue to a NSString.
    @discussion The JSValue is converted to a string according to the rules specified
    by the JavaScript language.
    @result The NSString containing the result of the conversion.
  */
  explicit operator std::string() const {
    return static_cast<std::string>(this->operator JSString());
  }

  explicit operator JSString() const;

  /*!
    @method
    @abstract Convert a JSValue to a NSDate.
    @discussion The value is converted to a number representing a time interval
    since 1970 which is then used to create a new NSDate instance.
    @result The NSDate created using the converted time interval.
  */
  //    - (NSDate *)toDate;
    
  /*!
    @method
    @abstract Convert a JSValue to a NSArray.
    @discussion If the value is <code>null</code> or <code>undefined</code> then <code>nil</code> is returned.
    If the value is not an object then a JavaScript TypeError will be thrown.
    The property <code>length</code> is read from the object, converted to an unsigned
    integer, and an NSArray of this size is allocated. Properties corresponding
    to indicies within the array bounds will be copied to the array, with
    JSValues converted to equivalent C++ objects as specified.
    @result The NSArray containing the recursively converted contents of the
    converted JavaScript array.
  */
  //    - (NSArray *)toArray;
    
  /*!
    @method
    @abstract Convert a JSValue to a NSDictionary.
    @discussion If the value is <code>null</code> or <code>undefined</code> then <code>nil</code> is returned.
    If the value is not an object then a JavaScript TypeError will be thrown.
    All enumerable properties of the object are copied to the dictionary, with
    JSValues converted to equivalent C++ objects as specified.
    @result The NSDictionary containing the recursively converted contents of
    the converted JavaScript object.
  */
  //    - (NSDictionary *)toDictionary;


  /*!
   @methodgroup Accessing Properties
   */
  /*!
   @method
   @abstract Access a property of a JSValue.
   @result The JSValue for the requested property or the JSValue <code>undefined</code>
   if the property does not exist.
   */
  JSValue_ptr_t valueForProperty(const std::string& property) const;
  
  /*!
   @method
   @abstract Set a property on a JSValue.
   */
  //void setValueForProperty(bool value, const std::string& property);
  
  /*!
   @method
   @abstract Delete a property from a JSValue.
   @result YES if deletion is successful, NO otherwise.
   */
  //bool deleteProperty(const std::string& property);
  
  /*!
   @method
   @abstract Check if a JSValue has a property.
   @discussion This method has the same function as the JavaScript operator <code>in</code>.
   @result Returns YES if property is present on the value.
   */
  bool hasProperty(const std::string& property) const;
  
  /*!
   @method
   @abstract Define properties with custom descriptors on JSValues.
   @discussion This method may be used to create a data or accessor property on an object.
   This method operates in accordance with the Object.defineProperty method in the
   JavaScript language.
   */
  //void definePropertyWithDescriptor(const std::string& property, int descriptor);
  
  /*!
   @method
   @abstract Access an indexed (numerical) property on a JSValue.
   @result The JSValue for the property at the specified index.
   Returns the JavaScript value <code>undefined</code> if no property exists at that index.
   */
  //JSValue_ptr_t valueAtIndex(size_t index);
  
  /*!
   @method
   @abstract Set an indexed (numerical) property on a JSValue.
   @discussion For JSValues that are JavaScript arrays, indices greater than
   UINT_MAX - 1 will not affect the length of the array.
   */
  //void setValueAtIndex(bool value, size_t index);
  
  
  
  bool isUndefined() const {
    return JSValueIsUndefined(static_cast<::JSGlobalContextRef>(*context_ptr_), value_);
  }
    
  bool isNull() const {
    return JSValueIsNull(static_cast<::JSGlobalContextRef>(*context_ptr_), value_);
  }
    
  bool isBoolean() const {
    return JSValueIsBoolean(static_cast<::JSGlobalContextRef>(*context_ptr_), value_);
  }
    
  bool isNumber() const {
    return JSValueIsNumber(static_cast<::JSGlobalContextRef>(*context_ptr_), value_);
  }
    
  bool isString() const {
    return JSValueIsString(static_cast<::JSGlobalContextRef>(*context_ptr_), value_);
  }
    
  bool isObject() const {
    return JSValueIsObject(static_cast<::JSGlobalContextRef>(*context_ptr_), value_);
  }

  static long ctorCounter() {
    return ctorCounter_;
  }
    
  static long dtorCounter() {
    return dtorCounter_;
  }
    
private:

  JSValue() = delete;
    
  JSValue(::JSValueRef value, const JSContext_ptr_t& context_ptr);
    
  ~JSValue();
    
  // Create a copy of another JSValue.
  JSValue(const JSValue& rhs);
    
  // Create a copy of another JSValue by assignment.
  JSValue& operator=(const JSValue& rhs);
    
private:
    
  friend class JSContext;

  // Return true if the two JSValues are equal.
  friend bool operator==( const JSValue& lhs, const JSValue& rhs );

  // This struct only exists so that a custom deleter can be passed to
  // std::shared_ptr<JSValue> while keeping the JSValue destructor
  // private.
  struct deleter {
    void operator()(JSValue* ptr) {
      delete ptr;
    }
  };

  ::JSValueRef    value_;
  JSContext_ptr_t context_ptr_;
    
  static std::atomic<long> ctorCounter_;
  static std::atomic<long> dtorCounter_;
};

// Return true if the two JSValues are equal.
inline
bool operator==( const JSValue& lhs, const JSValue& rhs ) {
  if (lhs.context_ptr_ != rhs.context_ptr_) {
    return false;
  }
    
  return JSValueIsStrictEqual(JSGlobalContextRef(*lhs.context_ptr_), lhs.value_, rhs.value_);
}

// Return true if the two JSValues are not equal.
inline
bool operator!=( const JSValue& lhs, const JSValue& rhs ) {
  return ! ( lhs == rhs ) ;
}

inline
std::ostream& operator << (std::ostream& ostream, const JSValue& value) {
  ostream << static_cast<JSString>(value);
  return ostream;
}

inline
std::ostream& operator << (std::ostream& ostream, const JSValue_ptr_t& value_ptr) {
  ostream << *value_ptr;
  return ostream;
}

#endif // _Titanium_Mobile_JSValue_h_

