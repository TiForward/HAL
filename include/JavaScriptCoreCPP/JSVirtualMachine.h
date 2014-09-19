// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#ifndef _Titanium_Mobile_JSVirtualMachine_h_
#define _Titanium_Mobile_JSVirtualMachine_h_

#include <JavaScriptCore/JavaScript.h>
#include <memory>
#include <atomic>

/*!
 @interface
 @discussion An instance of JSVirtualMachine represents a single JavaScript "object space"
 or set of execution resources. Thread safety is supported by locking the
 virtual machine, with concurrent JavaScript execution supported by allocating
 separate instances of JSVirtualMachine.
 */
class JSVirtualMachine;
typedef std::shared_ptr<JSVirtualMachine> JSVirtualMachine_ptr_t;

class JSVirtualMachine final : public std::enable_shared_from_this<JSVirtualMachine> {

public:
    
    // Factory function for creating a smart pointer to a JSVirtualMachine. This
    // is the public API to create a JSVirtualMachine.
    template <typename... Ts>
    static JSVirtualMachine_ptr_t create(Ts&&... params) {
        return JSVirtualMachine_ptr_t(new JSVirtualMachine(std::forward<Ts>(params)...), deleter{});
    }
    
    // Implicit convertsion to C API.
    operator JSContextGroupRef() const {
        return contextGroup_;
    }
    
    static long ctorCounter() {
        return ctorCounter_;
    }
    
    static long dtorCounter() {
        return dtorCounter_;
    }

private:
    
    JSVirtualMachine();
    
    ~JSVirtualMachine();
    
    // Create a copy of another JSVirtualMachine.
    JSVirtualMachine(const JSVirtualMachine& rhs);
    
    // Create a copy of another JSVirtualMachine by assignment.
    JSVirtualMachine& operator=(const JSVirtualMachine& rhs);
    
private:
    
    // This struct only exists so that a custom deleter can be passed to
    // std::shared_ptr<JSVirtualMachine> while keeping the JSVirtualMachine
    // destructor private.
    struct deleter {
        void operator()(JSVirtualMachine* ptr) {
            delete ptr;
        }
    };
    
    ::JSContextGroupRef contextGroup_ { nullptr };
    
    static std::atomic<long> ctorCounter_;
    static std::atomic<long> dtorCounter_;
};

#endif // _Titanium_Mobile_JSVirtualMachine_h_
