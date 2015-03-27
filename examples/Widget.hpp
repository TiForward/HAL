/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _HAL_EXAMPLES_WIDGET_HPP_
#define _HAL_EXAMPLES_WIDGET_HPP_

#include "HAL/HAL.hpp"
#include <string>

using namespace HAL;

/*!
 @class
 
 @discussion This is an example of how to create a JavaScript object
 implemented by a C++ class.
 */
class Widget : public JSExportObject, public JSExport<Widget> {
  
public:
  
  /*!
   @method
   
   @abstract This is the constructor used by JSContext::CreateObject
   to create a Widget instance and add it to a JavaScript execution
   context.
   
   @param js_context The JavaScriptCore execution context that your
   JavaScript object will execute in.
   
   @param arguments An optional list of JSValues to initialize your
   JavaScript object with as the result of being called in a
   JavaScript 'new' expression.
   */
  Widget(const JSContext& js_context) HAL_NOEXCEPT;
  
  std::string get_name() const                  HAL_NOEXCEPT;
  void        set_name(const std::string& name) HAL_NOEXCEPT;
  
  std::int32_t get_number() const                    HAL_NOEXCEPT;
  void         set_number(const std::int32_t number) HAL_NOEXCEPT;
  
  static double get_pi() HAL_NOEXCEPT;
  
  std::string sayHello();

  std::string testMemberObjectProperty() const HAL_NOEXCEPT;
  std::string testMemberArrayProperty()  const HAL_NOEXCEPT;
  std::string testCallAsFunction(JSObject& this_object)       HAL_NOEXCEPT;
  
  virtual ~Widget()                HAL_NOEXCEPT;
  Widget(const Widget&)            HAL_NOEXCEPT;
  Widget(Widget&&)                 HAL_NOEXCEPT;
  Widget& operator=(const Widget&) HAL_NOEXCEPT;
  Widget& operator=(Widget&&)      HAL_NOEXCEPT;
  void swap(Widget&)               HAL_NOEXCEPT;
  
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
	
  JSValue js_get_name() const              HAL_NOEXCEPT;
  bool    js_set_name(const JSValue& name) HAL_NOEXCEPT;
  
  JSValue js_get_number() const                HAL_NOEXCEPT;
  bool    js_set_number(const JSValue& number) HAL_NOEXCEPT;
  
  JSValue js_get_pi() const;
  
  JSValue js_get_value() const                HAL_NOEXCEPT;
  bool    js_set_value(const JSValue& value) HAL_NOEXCEPT;
  
  JSValue js_sayHello(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_sayHelloWithCallback(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_helloLambda(const std::vector<JSValue>& arguments, JSObject& this_object);
  
  JSValue js_testMemberObjectProperty(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_testMemberArrayProperty(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_testMemberNullProperty(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_testMemberUndefinedProperty(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_testMemberBooleanProperty(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_testMemberNumberProperty(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_testMemberStringProperty(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_testCallAsFunction(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_testMemberDateProperty(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_testMemberErrorProperty(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_testMemberRegExpProperty(const std::vector<JSValue>& arguments, JSObject& this_object);
  
  JSValue js_testException(const std::vector<JSValue>& arguments, JSObject& this_object);
  JSValue js_testNestedException(const std::vector<JSValue>& arguments, JSObject& this_object);

  static uint32_t constructor_count__;
private:
  
  std::string name__;
  int         number__;
  
  static double pi__;
  
  JSValue    jsvalue__;
  JSValue    jsnull__;
  JSValue    jsundefined__;
  JSBoolean  jsboolean__;
  JSNumber   jsnumber__;
  JSValue    jsstring__;
  JSObject   jsobject__;
  JSArray    jsarray__;
  JSFunction jsfunction__;
  JSDate     jsdate__;
  JSError    jserror__;
  JSRegExp   jsregexp__;
  JSObject   hello_callback__;

};

inline
void swap(Widget& first, Widget& second) HAL_NOEXCEPT {
  first.swap(second);
}

#endif // _HAL_EXAMPLES_WIDGET_HPP_
