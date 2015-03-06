/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _HAL_EXAMPLES_OTHERWIDGET_HPP_
#define _HAL_EXAMPLES_OTHERWIDGET_HPP_

#include "HAL/HAL.hpp"
#include <string>

using namespace HAL;

/*!
 @class
 
 @discussion This is an example of how to create a JavaScript object
 implemented by a C++ class.
 */
class OtherWidget : public JSExportObject, public JSExport<OtherWidget> {
  
public:
  
  /*!
   @method
   
   @abstract This is the constructor used by JSContext::CreateObject
   to create a OtherWidget instance and add it to a JavaScript execution
   context.
   
   @param js_context The JavaScriptCore execution context that your
   JavaScript object will execute in.
   
   @param arguments An optional list of JSValues to initialize your
   JavaScript object with as the result of being called in a
   JavaScript 'new' expression.
   */
  OtherWidget(const JSContext& js_context) HAL_NOEXCEPT;
  
  virtual ~OtherWidget()                HAL_NOEXCEPT;
  OtherWidget(const OtherWidget&)            HAL_NOEXCEPT;
  OtherWidget(OtherWidget&&)                 HAL_NOEXCEPT;
  OtherWidget& operator=(const OtherWidget&) HAL_NOEXCEPT;
  OtherWidget& operator=(OtherWidget&&)      HAL_NOEXCEPT;
  void swap(OtherWidget&)               HAL_NOEXCEPT;
  
  /*!
   @method
   
   @abstract Define how your JavaScript objects appear to
   JavaScriptCore.
   
   @discussion HAL will call this function exactly once
   just before your first JavaScript object is created.
   */
  static void JSExportInitialize();
	
  virtual void postInitialize(JSObject& js_object) override;
  virtual void postCallAsConstructor(const JSContext& js_context, const std::vector<JSValue>& arguments) override;
	
private:
  
};

inline
void swap(OtherWidget& first, OtherWidget& second) HAL_NOEXCEPT {
  first.swap(second);
}

#endif // _HAL_EXAMPLES_OTHERWIDGET_HPP_
