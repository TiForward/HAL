/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPIMPL_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPIMPL_HPP_

namespace JavaScriptCoreCPP { namespace detail {

template<typename T>
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSNativeObjectPimpl : public detail::JSPerformanceCounter<JSNativeObjectPimpl> {
#else
class JSNativeObjectPimpl {
#endif
	
 private:

	// We are exclusively owned by the JSNativeObject that creates us.
	template<typename T>
	friend class JSNativeObject;

	template<typename T>
	JSNativeObjectPimpl(JSNativeObject<T>* js_native_object_ptr) {
		js_native_object_ptr__ = js_native_object_ptr;
	}
	
	JSNativeObjectPimpl() = delete;
	~JSNativeObjectPimpl() = default;
	
	// Copy constructor.
	JSNativeObjectPimpl(const JSNativeObjectPimpl& rhs) = default;
	
	// Move constructor.
	JSNativeObjectPimpl(JSNativeObjectPimpl&& rhs) = default;
	
	// Copy assignment operator.
	JSNativeObjectPimpl& operator=(const JSNativeObjectPimpl&) = default;
	
	// Move assignment operator.
	JSNativeObjectPimpl& operator=(JSNativeObjectPimpl&&) = default;

 private:

	template<typename T>
	JSNativeObject<T>* js_native_object_ptr__;
};

	
}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPIMPL_HPP_
