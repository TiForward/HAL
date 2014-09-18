// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#ifndef _Titanium_Mobile_JSString_h_
#define _Titanium_Mobile_JSString_h_

#include <JavaScriptCore/JavaScript.h>
#include <string>
#include <iostream>
#include <atomic>

/*!
 @interface
 @discussion A JSString is an immutable, thread-safe representaton of a
 JavaScript string.
 
 */
class JSString final : public std::enable_shared_from_this<JSString> {
public:
	
    JSString(JSStringRef string);
	
    JSString(const std::string& string);
	
    ~JSString();
	
    // Create a copy of another JSString.
    JSString(const JSString& rhs);
	
    // Create a copy of another JSProxy by assignment.
    JSString& operator=(const JSString& rhs);
	
    operator std::string() const;
	
    // Explicit convertsion to C API.
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

    JSString() = delete;
	
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

