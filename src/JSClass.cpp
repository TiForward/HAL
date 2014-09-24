// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/JSClass.h"

std::atomic<long> JSClass::ctorCounter_ { 0 };
std::atomic<long> JSClass::dtorCounter_ { 0 };


JSClass::JSClass() {
    std::clog << "JSClass: ctor called" << std::endl;
    
    classDefinition_ = kJSClassDefinitionEmpty;
    
    //classDefinition_.initialize = JSObjectInitializeCallback;
    
    class_ = JSClassCreate(&classDefinition_);
    
    ++ctorCounter_;
}

JSClass::~JSClass() {
    std::clog << "JSClass: dtor called" << std::endl;
    JSClassRelease(class_);
    ++dtorCounter_;
}

// Create a copy of another JSClass.
JSClass::JSClass(const JSClass& rhs) :
classDefinition_(rhs.classDefinition_),
class_(rhs.class_)
{
    std::clog << "JSClass: copy ctor called" << std::endl;
}

// Create a copy of another JSClass by assignment.
JSClass& JSClass::operator=(const JSClass& rhs) {
    std::clog << "JSClass: operator= called" << std::endl;
    
    if ( this == &rhs ) {
        return *this;
    }
    
    // Release the resource we are replacing.
    JSClassRelease(class_);
    
    classDefinition_ = rhs.classDefinition_;
    class_           = rhs.class_;
    
    // Retain the resource we copying.
    JSClassRetain(class_);
    
    return *this;
}

void JSClass::JSObjectInitializeCallback(JSContextRef ctx, JSObjectRef object) {
    // TODO
}

void JSClass::JSObjectFinalizeCallback(JSObjectRef object) {
    // TODO
}

bool JSClass::JSObjectHasPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName) {
    // TODO
    return false;
}

JSValueRef JSClass::JSObjectGetPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
    // TODO
    return nullptr;
}

bool JSClass::JSObjectSetPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception) {
    // TODO
    return false;
}

bool JSClass::JSObjectDeletePropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
    // TODO
    return false;
}

void JSClass::JSObjectGetPropertyNamesCallback(JSContextRef ctx, JSObjectRef object, JSPropertyNameAccumulatorRef propertyNames) {
    // TODO
}

JSValueRef JSClass::JSObjectCallAsFunctionCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
    // TODO
    return nullptr;
}

JSObjectRef JSClass::JSObjectCallAsConstructorCallback(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
    // TODO
    return nullptr;
}

bool JSClass::JSObjectHasInstanceCallback(JSContextRef ctx, JSObjectRef constructor, JSValueRef possibleInstance, JSValueRef* exception) {
    // TODO
    return false;
}

JSValueRef JSClass::JSObjectConvertToTypeCallback(JSContextRef ctx, JSObjectRef object, JSType type, JSValueRef* exception) {
    // TODO
    return nullptr;
}

