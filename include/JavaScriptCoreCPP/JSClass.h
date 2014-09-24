// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#ifndef _Titanium_Mobile_JSClass_h_
#define _Titanium_Mobile_JSClass_h_

#include <JavaScriptCore/JavaScript.h>
#include <memory>
#include <iostream>
#include <atomic>

class JSClass;
typedef std::shared_ptr<JSClass> JSClass_ptr_t;

class JSClass final : public std::enable_shared_from_this<JSClass> {
    
public:
    
    // Factory function for creating a smart pointer to a JSClass. This is the
    // public API to create a JSClass.
    template <typename... Ts>
    static JSClass_ptr_t create(Ts&&... params) {
        return JSClass_ptr_t(new JSClass(std::forward<Ts>(params)...), deleter{});
    }

    static long ctorCounter() {
        return ctorCounter_;
    }
    
    static long dtorCounter() {
        return dtorCounter_;
    }
    
protected:
    
    virtual void initialize() {
        
    }
    
private:

    JSClass();
    
    ~JSClass();
    
    // Create a copy of another JSContext.
    JSClass(const JSClass& rhs);
    
    // Create a copy of another JSContext by assignment.
    JSClass& operator=(const JSClass& rhs);

    
    // This struct only exists so that a custom deleter can be passed to
    // std::shared_ptr<JSClass> while keeping the JSClass destructor
    // private.
    struct deleter {
        void operator()(JSClass* ptr) {
            delete ptr;
        }
    };

    void JSObjectInitializeCallback(JSContextRef ctx, JSObjectRef object);
    void JSObjectFinalizeCallback(JSObjectRef object);
    bool JSObjectHasPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName);
    JSValueRef JSObjectGetPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception);
    bool JSObjectSetPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception);
    bool JSObjectDeletePropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception);
    void JSObjectGetPropertyNamesCallback(JSContextRef ctx, JSObjectRef object, JSPropertyNameAccumulatorRef propertyNames);
    JSValueRef JSObjectCallAsFunctionCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    JSObjectRef JSObjectCallAsConstructorCallback(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    bool JSObjectHasInstanceCallback(JSContextRef ctx, JSObjectRef constructor, JSValueRef possibleInstance, JSValueRef* exception);
    JSValueRef  JSObjectConvertToTypeCallback(JSContextRef ctx, JSObjectRef object, JSType type, JSValueRef* exception);
    
    ::JSClassDefinition classDefinition_;
    ::JSClassRef        class_;
    
    static std::atomic<long> ctorCounter_;
    static std::atomic<long> dtorCounter_;
};


#endif // _Titanium_Mobile_JSClass_h_

