// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#ifndef _Titanium_Mobile_JSContext_h_
#define _Titanium_Mobile_JSContext_h_

#include "JavaScriptCoreCPP/JSVirtualMachine.h"
#include <JavaScriptCore/JavaScript.h>
#include <memory>
#include <atomic>
#include <functional>

class JSValue;
typedef std::shared_ptr<JSValue> JSValue_ptr_t;

class JSContext;
typedef std::shared_ptr<JSContext> JSContext_ptr_t;

class JSContext final : public std::enable_shared_from_this<JSContext> {

public:
    
    // Factory function for creating a smart pointer to a JSContext. This is the
    // public API to create a JSContext.
    template <typename... Ts>
    static JSContext_ptr_t create(Ts&&... params) {
        return JSContext_ptr_t(new JSContext(std::forward<Ts>(params)...), deleter{});
    }

    // Return the JSVirtualMachine this object was constructed with.
    JSVirtualMachine_ptr_t virtualMachine() const {
        return virtualMachine_ptr_;
    }
    
    JSValue_ptr_t globalObject();
    
    JSValue_ptr_t evaluateScript(const std::string& script);
    
    typedef std::function<void(const JSContext_ptr_t&, const JSValue_ptr_t&)> exceptionHandler_t;
    
    void setExceptionHandler(const exceptionHandler_t& exceptionHandler) {
        exceptionHandler_ = exceptionHandler;
    };
    
    exceptionHandler_t exceptionHandler() const {
        return exceptionHandler_;
    };
    
    // Implicit convertsion to C API.
    operator ::JSGlobalContextRef() const {
        return context_;
    }
    
    static long ctorCounter() {
        return ctorCounter_;
    }
    
    static long dtorCounter() {
        return dtorCounter_;
    }
    
private:

    JSContext();

    JSContext(const JSVirtualMachine_ptr_t& virtualMachine_ptr);
    
    ~JSContext();
    
    // Create a copy of another JSContext.
    JSContext(const JSContext& rhs);
    
    // Create a copy of another JSContext by assignment.
    JSContext& operator=(const JSContext& rhs);
    
private:
    
    friend class JSValue;
    
    // This struct only exists so that a custom deleter can be passed to
    // std::shared_ptr<JSContext> while keeping the JSContext destructor
    // private.
    struct deleter {
        void operator()(JSContext* ptr) {
            delete ptr;
        }
    };
    
    JSValue_ptr_t valueFromNotifyException(::JSValueRef exceptionValue);
    
    void notifyException(::JSValueRef value);
    
    JSVirtualMachine_ptr_t virtualMachine_ptr_ { nullptr };
    ::JSGlobalContextRef   context_            { nullptr };
    std::weak_ptr<JSValue> exception_ptr_;
    exceptionHandler_t     exceptionHandler_   { [](const JSContext_ptr_t& context_ptr, const JSValue_ptr_t& exception_ptr) { context_ptr->exception_ptr_ = exception_ptr; } };
    
    static std::atomic<long> ctorCounter_;
    static std::atomic<long> dtorCounter_;
};

#endif // _Titanium_Mobile_JSContext_h_
