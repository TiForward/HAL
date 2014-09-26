// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/JSVirtualMachine.h"
#include <iostream>

std::atomic<long> JSVirtualMachine::ctorCounter_ { 0 };
std::atomic<long> JSVirtualMachine::dtorCounter_ { 0 };

JSVirtualMachine::JSVirtualMachine() :
    contextGroup_(JSContextGroupCreate())
{
  std::clog << "JSVirtualMachine: ctor called" << std::endl;
  ++ctorCounter_;
}

JSVirtualMachine::~JSVirtualMachine() {
  std::clog << "JSVirtualMachine: dtor called" << std::endl;
  JSContextGroupRelease(contextGroup_);
  ++dtorCounter_;
}

// Create a copy of another JSVirtualMachine.
JSVirtualMachine::JSVirtualMachine(const JSVirtualMachine& rhs) :
    contextGroup_(JSContextGroupRetain(rhs.contextGroup_))
{
  std::clog << "JSVirtualMachine: copy ctor called" << std::endl;
  ++ctorCounter_;
}

// Create a copy of another JSVirtualMachine by assignment.
JSVirtualMachine& JSVirtualMachine::operator=(const JSVirtualMachine& rhs) {
  std::clog << "JSVirtualMachine: operator= called" << std::endl;
    
  if ( this == &rhs ) {
    return *this;
  }
    
  // Release the resource we are replacing.
  JSContextGroupRelease(contextGroup_);
    
  // Retain the resource we are copying.
  contextGroup_ = JSContextGroupRetain(rhs.contextGroup_);
    
  return *this;
}
