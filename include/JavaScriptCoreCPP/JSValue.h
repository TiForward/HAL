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
    
    // Create the Undefined value.
    explicit JSValue(const JSContext_ptr_t& context_ptr);
    
    /*!
     @method
     @abstract Create a JavaScript value from a BOOL primitive.
     @param value
     @param context The JSContext in which the resulting JSValue will be created.
     @result The new JSValue representing the equivalent boolean value.
     */
    explicit JSValue(bool value, const JSContext_ptr_t& context_ptr);

    /*!
     @method
     @abstract Create a JavaScript value from an <code>int32_t</code> primitive.
     @param value
     @param context The JSContext in which the resulting JSValue will be created.
     @result The new JSValue representing the equivalent boolean value.
     */
    explicit JSValue(int32_t value, const JSContext_ptr_t& context_ptr);
    
    /*!
     @method
     @abstract Create a JavaScript value from a double primitive.
     @param value
     @param context The JSContext in which the resulting JSValue will be created.
     @result The new JSValue representing the equivalent boolean value.
     */
    explicit JSValue(double value, const JSContext_ptr_t& context_ptr);

    JSValue(::JSValueRef value, const JSContext_ptr_t& context_ptr);
    
    ~JSValue();
        
    // Create a copy of another JSValue.
    JSValue(const JSValue& rhs);
        
    // Create a copy of another JSValue by assignment.
    JSValue& operator=(const JSValue& rhs);

    operator bool() const {
        return JSValueToBoolean(*context_ptr_, value_);
    }
    
    operator double() const;
    
    operator int32_t() const;
    
    // This implements ToUInt32, defined in ECMA-262 9.6.
    operator uint32_t() const  {
        // As commented in the spec, the operation of ToInt32 and ToUint32 only differ
        // in how the result is interpreted; see NOTEs in sections 9.5 and 9.6.
        return this->operator int32_t();
    }

    operator JSString() const;

    static long ctorCounter() {
        return ctorCounter_;
    }
    
    static long dtorCounter() {
        return dtorCounter_;
    }

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
    
private:
    
    // Return true if the two JSValues are equal.
    friend bool operator==( const JSValue& lhs, const JSValue& rhs );

    JSValue() = delete;
    
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

