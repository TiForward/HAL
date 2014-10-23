/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_



namespace JavaScriptCoreCPP { namespace RAII {

template<typename T>
class JSNativeObject {
	
 public:
	
	/*! 
	  @typedef      InitializeCallback
	  @abstract     The callback invoked when a JavaScript object is first created.
	  @param object A reference to the native object that backs the JavaScript object.
	  @discussion   If you named your function Initialize, you would declare it like this:
	  
	  void Initialize(T& object);
	  
	  Unlike the other object callbacks, the initialize callback is
	  called on the least derived class (the parent class) first, and
	  the most derived class last.
	*/
	using InitializeCallback = std::function<void(T&)>;

	JSNativeObject(const JSContext& js_context) : js_context_(js_context) {
	}

	virtual ~JSNativeObject() {
	}

 private:

	JSContext js_context_;
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_

