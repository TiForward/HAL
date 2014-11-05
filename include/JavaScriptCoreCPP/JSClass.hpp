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

#include <cstdint>
#include <memory>

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

namespace JavaScriptCoreCPP { namespace detail {
class JSClassPimpl;
}}

extern "C" struct JSClassRef;

namespace JavaScriptCoreCPP {

/*!
  @class

  @discussion A JSClass is an RAII wrapper around a JSClassRef, the
  JavaScriptCore C API representation of a JavaScript class that
  defines JavaScript objects implemented in C.

  A JSClass wraps a C++ class and seamlessly integrates it into the
  JavaScriptCore runtime.  The only way to create a JSClass is through
  the use of a JSClassBuilder, so please see that class for more
  details.

  An instance of JSClass may be passed to the JSContextGroup
  constructor to create a custom JavaScript global object for all
  contexts in that group.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSClass final : public detail::JSPerformanceCounter<JSClass> {
#else
class JSClass final {
#endif
  
 public:

	static JSClass& EmptyJSClass();

  /*!
    @method
    
    @abstract Return the name of this JSClass.
    
    @result The name of this JSClass.
  */
	std::string get_name() const;
  
  /*!
    @method
    
    @abstract Return the version number of this JSClass.
    
    @result The version number of this JSClass.
  */
	std::uint32_t get_version() const;

	JSClass() = delete;
	~JSClass() = default;
	JSClass(const JSClass&) = default;
	JSClass(JSClass&&) = default;
	JSClass& operator=(const JSClass&) = default;
  JSClass& operator=(JSClass&&) = default;
  
 private:
  
  // Only a JSClassBuilder can create a JSClass.
  template<typename T>
  friend class JSClassBuilder;

  explicit JSClass(std::shared_ptr<detail::JSClassPimpl> js_class_pimpl_ptr);

  // These classes need access to operator JSClassRef().
  friend class JSContext;
  friend class JSValue;  // for IsObjectOfClass
  friend class JSObject; // for constructor

  // For interoperability with the JavaScriptCore C API.
  operator JSClassRef() const;

  // Prevent heap based objects.
  static void * operator new(std::size_t);     // #1: To prevent allocation of scalar objects
  static void * operator new [] (std::size_t); // #2: To prevent allocation of array of objects
  
  std::shared_ptr<detail::JSClassPimpl> js_class_pimpl_ptr__;

  static const JSClass                  empty_js_class__;
		  
#undef  JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD
#ifdef  JAVASCRIPTCORECPP_THREAD_SAFE
                                                             std::recursive_mutex       mutex__
#define JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock(mutex__)
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSCLASS_HPP_
