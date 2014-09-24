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
 from a different JSVirtualMachine will result in an Objective-C exception
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
        return JSValue::create(JSValueMakeBoolean(*context_ptr, value), context_ptr);
    }
    
    /*!
     @method
     @abstract Create a JavaScript value from a double primitive.
     @param value
     @param context The JSContext in which the resulting JSValue will be created.
     @result The new JSValue representing the equivalent double value.
     */
    static JSValue_ptr_t valueWithDoubleInContext(double value, const JSContext_ptr_t& context_ptr) {
        return JSValue::create(JSValueMakeNumber(*context_ptr, value), context_ptr);
    }

    /*!
     @method
     @abstract Create a JavaScript value from an <code>int32_t</code> primitive.
     @param value
     @param context The JSContext in which the resulting JSValue will be created.
     @result The new JSValue representing the equivalent int32_t value.
     */
    static JSValue_ptr_t valueWithInt32InContext(int32_t value, const JSContext_ptr_t& context_ptr) {
        return JSValue::create(JSValueMakeNumber(*context_ptr, value), context_ptr);
    }
    
    /*!
     @method
     @abstract Create a JavaScript value from a <code>uint32_t</code> primitive.
     @param value
     @param context The JSContext in which the resulting JSValue will be created.
     @result The new JSValue representing the equivalent uint32_t value.
     */
    static JSValue_ptr_t valueWithUInt32InContext(uint32_t value, const JSContext_ptr_t& context_ptr) {
        return JSValue::create(JSValueMakeNumber(*context_ptr, value), context_ptr);
    }

    /*!
     @method
     @abstract Create a JavaScript value from a <code>std::string</code>.
     @param value
     @param context The JSContext in which the resulting JSValue will be created.
     @result The new JSValue representing the equivalent std::string value.
     */
    static JSValue_ptr_t valueWithStringInContext(const std::string& value, const JSContext_ptr_t& context_ptr) {
        return JSValue::create(JSValueMakeString(*context_ptr, JSStringRef(JSString(value))), context_ptr);
    }

    /*!
     @method
     @abstract Create a new, empty JavaScript object.
     @param context The JSContext in which the resulting object will be created.
     @result The new JavaScript object.
     */
    static JSValue_ptr_t valueWithNewObjectInContext(const JSContext_ptr_t& context_ptr) {
        return JSValue::create(JSObjectMake(*context_ptr, nullptr, nullptr), context_ptr);
    }
    
    /*!
     @method
     @abstract Create a new, empty JavaScript array.
     @param context The JSContext in which the resulting array will be created.
     @result The new JavaScript array.
     */
    static JSValue_ptr_t valueWithNewArrayInContext(const JSContext_ptr_t& context_ptr) {
        return JSValue::create(JSObjectMakeArray(*context_ptr, 0, nullptr, nullptr), context_ptr);
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
        JSValueRef arguments[2] = { JSValueMakeString(*context_ptr, JSStringRef(JSString(message))) };
        return JSValue::create(JSObjectMakeError(*context_ptr, 1, arguments, nullptr), context_ptr);
    }
    
    /*!
     @method
     @abstract Create the JavaScript value <code>null</code>.
     @param context The JSContext to which the resulting JSValue belongs.
     @result The JSValue representing the JavaScript value <code>null</code>.
     */
    static JSValue_ptr_t valueWithNullInContext(const JSContext_ptr_t& context_ptr) {
        return JSValue::create(JSValueMakeNull(*context_ptr), context_ptr);
    }

    /*!
     @method
     @abstract Create the JavaScript value <code>undefined</code>.
     @param context The JSContext to which the resulting JSValue belongs.
     @result The JSValue representing the JavaScript value <code>undefined</code>.
     */
    static JSValue_ptr_t valueWithUndefinedInContext(const JSContext_ptr_t& context_ptr) {
        return JSValue::create(JSValueMakeUndefined(*context_ptr), context_ptr);
    }

    /*!
     @method
     @abstract Convert a JSValue to a boolean.
     @discussion The JSValue is converted to a boolean according to the rules specified
     by the JavaScript language.
     @result The boolean result of the conversion.
     */
    operator bool() const {
        return JSValueToBoolean(*context_ptr_, value_);
    }
    
    /*!
     @method
     @abstract Convert a JSValue to a double.
     @discussion The JSValue is converted to a number according to the rules specified
     by the JavaScript language.
     @result The double result of the conversion.
     */
    operator double() const;
    
    /*!
     @method
     @abstract Convert a JSValue to an <code>int32_t</code>.
     @discussion The JSValue is converted to an integer according to the rules specified
     by the JavaScript language.
     @result The <code>int32_t</code> result of the conversion.
     */
    operator int32_t() const;
    
    /*!
     @method
     @abstract Convert a JSValue to a <code>uint32_t</code>.
     @discussion The JSValue is converted to an integer according to the rules specified
     by the JavaScript language (implements ToUInt32, defined in ECMA-262 9.6).
     @result The <code>uint32_t</code> result of the conversion.
     */
    operator uint32_t() const  {
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
//    operator std::string() const {
//    }

    operator JSString() const;

    bool isUndefined() const {
        return JSValueIsUndefined(*context_ptr_, value_);
    }
    
    bool isNull() const {
        return JSValueIsNull(*context_ptr_, value_);
    }
    
    bool isBoolean() const {
        return JSValueIsBoolean(*context_ptr_, value_);
    }
    
    bool isNumber() const {
        return JSValueIsNumber(*context_ptr_, value_);
    }
    
    bool isString() const {
        return JSValueIsString(*context_ptr_, value_);
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
    ostream << JSString(value);
    return ostream;
}

#endif // _Titanium_Mobile_JSValue_h_

