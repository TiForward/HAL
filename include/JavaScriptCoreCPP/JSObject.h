// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#ifndef _Titanium_Mobile_JSObject_h_
#define _Titanium_Mobile_JSObject_h_

#include "JavaScriptCoreCPP/JSValue.h"
#include <JavaScriptCore/JavaScript.h>
#include <memory>
#include <iostream>
#include <atomic>

class JSObject;
typedef std::shared_ptr<JSObject> JSObject_ptr_t;

class JSObject final : public std::enable_shared_from_this<JSObject> {
    
public:
    
  // Factory function for creating a smart pointer to a JSObject. This is the
  // public API to create a JSObject.
  template <typename... Ts>
      static JSObject_ptr_t create(Ts&&... params) {
    return JSObject_ptr_t(new JSObject(std::forward<Ts>(params)...), deleter{});
  }
    
  //    JSValue_ptr_t prototype() const {
  //        JSValueRef value = JSObjectGetPrototype(*context_ptr_, object_);
  //    }

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

  JSObject() = delete;
    
  JSObject(const JSContext_ptr_t& context_ptr_t);
    
  JSObject(::JSObjectRef object, const JSContext_ptr_t& context_ptr);

  ~JSObject();
    
  // Create a copy of another JSContext.
  JSObject(const JSObject& rhs);
    
  // Create a copy of another JSContext by assignment.
  JSObject& operator=(const JSObject& rhs);

    
  // This struct only exists so that a custom deleter can be passed to
  // std::shared_ptr<JSObject> while keeping the JSObject destructor
  // private.
  struct deleter {
    void operator()(JSObject* ptr) {
      delete ptr;
    }
  };

  JSContext_ptr_t context_ptr_;
  ::JSObjectRef   object_;
    
  static std::atomic<long> ctorCounter_;
  static std::atomic<long> dtorCounter_;
};


#endif // _Titanium_Mobile_JSObject_h_

