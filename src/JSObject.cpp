// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/JSObject.h"

std::atomic<long> JSObject::ctorCounter_ { 0 };
std::atomic<long> JSObject::dtorCounter_ { 0 };


// Create the Undefined value.
JSObject::JSObject(const JSContext_ptr_t& context_ptr) :
    JSObject(JSObjectMake(static_cast<::JSGlobalContextRef>(*context_ptr), nullptr, nullptr), context_ptr)
{
  std::clog << "JSValue: ctor called (JSContext_ptr_t)" << std::endl;
}

JSObject::JSObject(::JSObjectRef object, const JSContext_ptr_t& context_ptr) :
    object_(object),
    context_ptr_(context_ptr)
{
  std::clog << "JSObject: ctor called (JSObjectRef, JSContext_ptr_t)" << std::endl;
  //JSValueProtect(*context_ptr_, object_);
  ++ctorCounter_;
}

JSObject::~JSObject() {
  std::clog << "JSObject: dtor called" << std::endl;
  JSValueUnprotect(static_cast<::JSGlobalContextRef>(*context_ptr_), object_);
  ++dtorCounter_;
}

// Create a copy of another JSObject.
JSObject::JSObject(const JSObject& rhs) :
    JSObject(rhs.object_, rhs.context_ptr_)
{
  std::clog << "JSObject: copy ctor called" << std::endl;
}

// Create a copy of another JSObject by assignment.
JSObject& JSObject::operator=(const JSObject& rhs) {
  std::clog << "JSObject: operator= called" << std::endl;
    
  if ( this == &rhs ) {
    return *this;
  }
    
  // Release the resource we are replacing.
  JSValueUnprotect(static_cast<::JSGlobalContextRef>(*context_ptr_), object_);
    
  object_      = rhs.object_;
  context_ptr_ = rhs.context_ptr_;
    
  // Retain the resource we copying.
  JSValueProtect(static_cast<::JSGlobalContextRef>(*context_ptr_), object_);
    
  return *this;
}
