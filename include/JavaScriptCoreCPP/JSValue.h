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

class JSValue final : public std::enable_shared_from_this<JSValue> {

public:

    // Create the Undefined value.
    explicit JSValue(const JSContext_ptr_t& context_ptr);
    
    explicit JSValue(bool value, const JSContext_ptr_t& context_ptr);

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
    
    operator JSString() const;

    static long ctorCounter() {
        return ctorCounter_;
    }
    
    static long dtorCounter() {
        return dtorCounter_;
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

