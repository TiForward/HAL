// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/JSValue.h"

std::atomic<long> JSValue::ctorCounter_ { 0 };
std::atomic<long> JSValue::dtorCounter_ { 0 };

/*
 * C++'s idea of a reinterpret_cast lacks sufficient cojones.
 */
template<typename ToType, typename FromType>
inline ToType bitwise_cast(FromType from)
{
    static_assert(sizeof(FromType) == sizeof(ToType), "bitwise_cast size of FromType and ToType must be equal!");
    union {
        FromType from;
        ToType to;
    } u;
    u.from = from;
    return u.to;
}

// Create the Undefined value.
JSValue::JSValue(const JSContext_ptr_t& context_ptr) :
JSValue(JSValueMakeUndefined(*context_ptr), context_ptr)
{
    std::clog << "JSValue: ctor called (JSContext_ptr_t)" << std::endl;
}

JSValue::JSValue(bool value, const JSContext_ptr_t& context_ptr) :
JSValue(JSValueMakeBoolean(*context_ptr, value), context_ptr)
{
    std::clog << "JSValue: ctor called (bool, JSContext_ptr_t)" << std::endl;
}

JSValue::JSValue(int32_t value, const JSContext_ptr_t& context_ptr) :
JSValue(JSValueMakeNumber(*context_ptr, value), context_ptr)
{
    std::clog << "JSValue: ctor called (int, JSContext_ptr_t)" << std::endl;
}

JSValue::JSValue(double value, const JSContext_ptr_t& context_ptr) :
JSValue(JSValueMakeNumber(*context_ptr, value), context_ptr)
{
    std::clog << "JSValue: ctor called (double, JSContext_ptr_t)" << std::endl;
}

JSValue::JSValue(::JSValueRef value, const JSContext_ptr_t& context_ptr) :
value_(value),
context_ptr_(context_ptr)
{
    std::clog << "JSValue: ctor called (JSValueRef, JSContext_ptr_t)" << std::endl;
    JSValueProtect(*context_ptr_, value_);
    ++ctorCounter_;
}

JSValue::~JSValue() {
    std::clog << "JSValue: dtor called" << std::endl;
    JSValueUnprotect(*context_ptr_, value_);
    ++dtorCounter_;
}

// Create a copy of another JSValue.
JSValue::JSValue(const JSValue& rhs) :
JSValue(rhs.value_, rhs.context_ptr_)
{
    std::clog << "JSValue: copy ctor called" << std::endl;
}

// Create a copy of another JSValue by assignment.
JSValue& JSValue::operator=(const JSValue& rhs) {
    std::clog << "JSValue: operator= called" << std::endl;
    
    if ( this == &rhs ) {
        return *this;
    }
    
    // Release the resource we are replacing.
    JSValueUnprotect(*context_ptr_, value_);
    
    value_       = rhs.value_;
    context_ptr_ = rhs.context_ptr_;
    
    // Retain the resource we copying.
    JSValueProtect(*context_ptr_, value_);
    
    return *this;
}

JSValue::operator double() const {
    ::JSValueRef exception = 0;
    double result = JSValueToNumber(*context_ptr_, value_, &exception);
    if (exception) {
        context_ptr_ -> notifyException(exception);
        return std::numeric_limits<double>::quiet_NaN();
    }
    
    return result;
}

// This in the ToInt32 operation is defined in section 9.5 of the ECMA-262 spec.
// Note that this operation is identical to ToUInt32 other than to interpretation
// of the resulting bit-pattern (as such this metod is also called to implement
// ToUInt32).
//
// The operation can be descibed as round towards zero, then select the 32 least
// bits of the resulting value in 2s-complement representation.
JSValue::operator int32_t() const
{
    const double number(double(*this));
    int64_t bits = bitwise_cast<int64_t>(number);
    int32_t exp = (static_cast<int32_t>(bits >> 52) & 0x7ff) - 0x3ff;
    
    // If exponent < 0 there will be no bits to the left of the decimal point
    // after rounding; if the exponent is > 83 then no bits of precision can be
    // left in the low 32-bit range of the result (IEEE-754 doubles have 52 bits
    // of fractional precision).
    // Note this case handles 0, -0, and all infinte, NaN, & denormal value.
    if (exp < 0 || exp > 83)
        return 0;
    
    // Select the appropriate 32-bits from the floating point mantissa.  If the
    // exponent is 52 then the bits we need to select are already aligned to the
    // lowest bits of the 64-bit integer representation of tghe number, no need
    // to shift.  If the exponent is greater than 52 we need to shift the value
    // left by (exp - 52), if the value is less than 52 we need to shift right
    // accordingly.
    int32_t result = (exp > 52)
    ? static_cast<int32_t>(bits << (exp - 52))
    : static_cast<int32_t>(bits >> (52 - exp));
    
    // IEEE-754 double precision values are stored omitting an implicit 1 before
    // the decimal point; we need to reinsert this now.  We may also the shifted
    // invalid bits into the result that are not a part of the mantissa (the sign
    // and exponent bits from the floatingpoint representation); mask these out.
    if (exp < 32) {
        int32_t missingOne = 1 << exp;
        result &= missingOne - 1;
        result += missingOne;
    }
    
    // If the input value was negative (we could test either 'number' or 'bits',
    // but testing 'bits' is likely faster) invert the result appropriately.
    return bits < 0 ? -result : result;
}

JSValue::operator JSString() const {
    ::JSValueRef exception = 0;
    ::JSStringRef result = JSValueToStringCopy(*context_ptr_, value_, &exception);
    if (exception) {
        context_ptr_ -> notifyException(exception);
        return JSString("");
    }
    
    return JSString(result);
}

