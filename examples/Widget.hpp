/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_EXAMPLES_WIDGET_HPP_
#define _JAVASCRIPTCORECPP_EXAMPLES_WIDGET_HPP_

#include "JavaScriptCoreCPP/JavaScriptCoreCPP.hpp"
#include <string>

using namespace JavaScriptCoreCPP;

/*!
 @class
 
 @discussion This is an example of how to create a JavaScript object
 implemented by a C++ class.
 */
class Widget : public JSExport<Widget>, public JSExportObject {
  
public:
  
  /*!
   @method
   
   @abstract Define how your JavaScript objects appear to
   JavaScriptCore.
   
   @discussion JavaScriptCoreCPP will call this function exactly once
   just before your first JavaScript object is created.
   */
  static void JSExportInitialize();
  
  /*!
   @method
   
   @abstract This is the constructor used by JSContext::CreateObject
   to create a Widget instance and add it to a JavaScript execution
   context.
   
   @param js_context The JavaScriptCore execution context that your
   JavaScript object will execute in.
   */
  Widget(const JSContext& js_context)                          JAVASCRIPTCORECPP_NOEXCEPT;
  Widget(const Widget&, const std::vector<JSValue>& arguments) JAVASCRIPTCORECPP_NOEXCEPT;
  
  JSValue get_name() const;
  bool    set_name(const JSValue& name);
  
  JSValue get_number() const;
  bool    set_number(const JSValue& number);
  
  JSValue pi() const;
  
  JSValue sayHello(const std::vector<JSValue>& arguments);
  
  std::string get_name_native() const;
  void        set_name_native(const std::string& name);
  
  int         get_number_native() const;
  void        set_number_native(int number);
  
  
  virtual ~Widget()                JAVASCRIPTCORECPP_NOEXCEPT;
  Widget(const Widget&)            JAVASCRIPTCORECPP_NOEXCEPT;
  Widget(Widget&&)                 JAVASCRIPTCORECPP_NOEXCEPT;
  Widget& operator=(const Widget&) JAVASCRIPTCORECPP_NOEXCEPT;
  Widget& operator=(Widget&&)      JAVASCRIPTCORECPP_NOEXCEPT;
  void swap(Widget&)               JAVASCRIPTCORECPP_NOEXCEPT;

 private:
  
  std::string name__;
  int         number__;
  
  static double pi__;
  
};

inline
void swap(Widget& first, Widget& second) JAVASCRIPTCORECPP_NOEXCEPT {
  first.swap(second);
}

#endif // _JAVASCRIPTCORECPP_EXAMPLES_WIDGET_HPP_
