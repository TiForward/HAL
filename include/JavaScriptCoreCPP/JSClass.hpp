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

#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#include <cstddef>
#include <unordered_map>
#include <utility>

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
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
  C API. See JSNativeClassBuilder to create a JSClass based on a C++
  class.
  
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
    
    @abstract Return the JSClass with the given name.

    @param The name of the JSClass to return.
    
    @result The JSClass with the given name.
    
    @throws std::invalid_argument if a JSClass with the given name
    does not exist.
  */
  static JSClass GetClassWithName(const JSString& class_name);

  /*!
    @method
    
    @abstract Return the number of JSClasses created.

    @result The number of JSClasses created.
  */
  static std::size_t GetClassCount();

  /*!
    @method
    
    @abstract Return the names of the JSClasses created.

    @result The names of the JSClasses created.
  */
  static std::vector<std::string> GetClassNames();

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
    interoperability with the JavaScriptCore C API. See
    JSNativeClassBuilder to create a JSClass that is based on a C++
    class.

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

  // Prevent heap based objects.
  static void * operator new(std::size_t);     // #1: To prevent allocation of scalar objects
  static void * operator new [] (std::size_t); // #2: To prevent allocation of array of objects
  
  // The JSObject constructor needs access to operator JSClassRef().
  friend class JSObject;

  JSClassRef  js_class_ref__ { nullptr };
  std::string name__         { "Default" };
  int         version__;

#undef  JAVASCRIPTCORECPP_JSCLASS_MUTEX_TYPE
#undef  JAVASCRIPTCORECPP_JSCLASS_MUTEX_NAME_PREFIX
#undef  JAVASCRIPTCORECPP_JSCLASS_MUTEX_NAME
#undef  JAVASCRIPTCORECPP_JSCLASS_MUTEX
#ifdef  JAVASCRIPTCORECPP_THREAD_SAFE
#define JAVASCRIPTCORECPP_JSCLASS_MUTEX_TYPE        std::recursive_mutex
#define JAVASCRIPTCORECPP_JSCLASS_MUTEX_NAME_PREFIX js_class
#define JAVASCRIPTCORECPP_JSCLASS_MUTEX_NAME        JAVASCRIPTCORECPP_JSCLASS_MUTEX_NAME##_mutex_
#define JAVASCRIPTCORECPP_JSCLASS_MUTEX             JAVASCRIPTCORECPP_JSCLASS_MUTEX_TYPE JAVASCRIPTCORECPP_JSCLASS_MUTEX_NAME
#else
#define JAVASCRIPTCORECPP_JSCLASS_MUTEX
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
  JAVASCRIPTCORECPP_JSCLASS_MUTEX;

  static void ThrowRuntimeErrorIfJSClassAlreadyExists();
  static std::unordered_map<JSString, JSClass> js_class_map_;

#undef  JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX_TYPE
#undef  JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX_NAME_PREFIX
#undef  JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX_NAME
#undef  JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX
#ifdef  JAVASCRIPTCORECPP_THREAD_SAFE
#define JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX_TYPE        std::recursive_mutex
#define JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX_NAME_PREFIX js_class_static
#define JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX_NAME        JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX_NAME_PREFIX##_mutex_
#define JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX             JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX_TYPE JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX_NAME
#else
#define JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
  static JAVASCRIPTCORECPP_JSCLASS_STATIC_MUTEX;
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSCLASS_HPP_
