/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSCLASS_HPP_
#define _JAVASCRIPTCORECPP_JSCLASS_HPP_

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#include <cstddef>
#include <unordered_map>
#include <utility>

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

extern "C" {
  struct JSClassRef;
  struct JSClassDefinition;
}

namespace JavaScriptCoreCPP {

/*!
  @class

  @discussion A JSClass is an RAII wrapper around a JSClassRef, the
  JavaScriptCore C API representation of a JavaScript class that
  defines JavaScript objects implemented in C.

  JSClass is a base class for interoperability with the JavaScriptCore
  C API. See JSClassBuilder to create a JSClass based on a C++ class.
  
  An instance of JSClass may be passed to the JSContextGroup
  constructor to create a custom JavaScript global object for all
  contexts in that group.

  JavaScript objects based on a JSClass may be created by the
  JSContext::CreateObject member functions.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSClass : public detail::JSPerformanceCounter<JSClass> {
#else
class JSClass {
#endif
  
 public:

  /*!
    @method
    
    @abstract Return the name of this JSClass.
    
    @result The name of this JSClass.
  */
  virtual std::string get_name() final {
    return name__;
  }
  
  /*!
    @method
    
    @abstract Return the version number of this JSClass.
    
    @result The version number of this JSClass.
  */
  virtual int get_version() final {
    return version__;
  }

  virtual ~JSClass();
  JSClass(const JSClass&);
  JSClass(JSClass&&);
  JSClass& JSClass::operator=(const JSClass&) = delete;
  JSClass& JSClass::operator=(JSClass&&) = delete;
  JSClass& operator=(JSClass);
  void swap(JSClass& other) noexcept;
  
 protected:

  /*!
    @method
    
    @abstract Create a JSClass that defines the behavior of JavaScript
    objects based on this JSClass. This constructor is for
    interoperability with the JavaScriptCore C API. See JSClassBuilder
    to create a JSClass that is based on a C++ class.

    @param js_class_definition The JSClassDefinition that defines the
    JSClass.
  */
  JSClass(const JSClassDefinition& js_class_definition);
  
 private:

  // These classes need access to operator JSClassRef().
  friend class JSContext;
  friend class JSValue;  // for IsObjectOfClass

  // For interoperability with the JavaScriptCore C API.
  operator JSClassRef() const {
    return js_class_ref__;
  }

  /*!
    @method
    
    @abstract Return the default JSClass that defines no properties or
    callbacks. This is only used by JSContext to create empty
    JSObjects.

    @result The default JSClass that defines no properties or
    callbacks
  */
  static JSClass EmptyJSClass();

  // Prevent heap based objects.
  static void * operator new(std::size_t);     // #1: To prevent allocation of scalar objects
  static void * operator new [] (std::size_t); // #2: To prevent allocation of array of objects
  
  // The JSObject constructor needs access to operator JSClassRef().
  friend class JSObject;

  JSClassRef  js_class_ref__ { nullptr };
  std::string name__         { "Default" };
  int         version__;

#undef  JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD
#undef  JAVASCRIPTCORECPP_JSCLASS_STATIC_LOCK_GUARD
#ifdef  JAVASCRIPTCORECPP_THREAD_SAFE
                                                                    std::recursive_mutex              mutex__;
#define JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD        std::lock_guard<std::recursive_mutex>        lock(mutex__)
  static                                                            std::recursive_mutex              static_mutex__;
#define JAVASCRIPTCORECPP_JSCLASS_STATIC_LOCK_GUARD std::lock_guard<std::recursive_mutex> static_lock(static_mutex__);
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE

  static void ThrowRuntimeErrorIfJSClassAlreadyExists();

  static JSClass                               empty_js_class_;
  static std::unordered_map<JSString, JSClass> js_class_map_;
};

} // namespace JavaScriptCoreCPP {

namespace std {
using JavaScriptCoreCPP::JSClass;
template<>
void swap<JSContextGroup>(JSClass& first, JSClass& second) noexcept {
  first.swap(second);
}
}  // namespace std

#endif // _JAVASCRIPTCORECPP_JSCLASS_HPP_
