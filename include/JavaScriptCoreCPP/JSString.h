// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#ifndef _Titanium_Mobile_JSString_h_
#define _Titanium_Mobile_JSString_h_

#include <JavaScriptCore/JavaScript.h>
#include <string>
#include <memory>
#include <iostream>
#include <atomic>

/*!
 @interface
 @discussion A JSString is an immutable, thread-safe representation of a
 JavaScript string.
 
 */
class JSString final : public std::enable_shared_from_this<JSString> {
public:
	
    // Constructs an empty JSString, with a length of zero characters.
    JSString();
    
    JSString(JSStringRef string);
	
    JSString(const std::string& string);
	
    ~JSString();
	
    // Create a copy of another JSString.
    JSString(const JSString& rhs);
	
    // Create a copy of another JSProxy by assignment.
    JSString& operator=(const JSString& rhs);
	
    size_t size() const {
        return JSStringGetLength(string_);
    }
    
    explicit operator std::string() const;
	
    // Explicit conversion to C API.
    explicit operator JSStringRef() const {
        return string_;
    }
    
	static long ctorCounter() {
		return ctorCounter_;
	}
	
	static long dtorCounter() {
		return dtorCounter_;
	}
	
 private:
	
    // Return true if the two JSStrings are equal.
    friend bool operator==( const JSString& lhs, const JSString& rhs );

    ::JSStringRef string_;
	
    static std::atomic<long> ctorCounter_;
    static std::atomic<long> dtorCounter_;
};

// Return true if the two JSStrings are equal.
inline
bool operator==( const JSString& lhs, const JSString& rhs ) {
    return JSStringIsEqual(lhs.string_, rhs.string_);
}

// Return true if the two JSStrings are not equal.
inline
bool operator!=( const JSString& lhs, const JSString& rhs ) {
    return ! ( lhs == rhs ) ;
}

inline
std::ostream& operator << (std::ostream& ostream, const JSString& string) {
    ostream << std::string(string);
    return ostream;
}

#endif // _Titanium_Mobile_JSString_h_

