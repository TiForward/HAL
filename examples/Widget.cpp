/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "Widget.hpp"

#include <functional>
#include <sstream>

void Widget::JSExportInitialize() {
  SetClassVersion(1);
  AddValueProperty("name"       , std::mem_fn(&Widget::get_name)  , std::mem_fn(&Widget::set_name));
  AddValueProperty("number"     , std::mem_fn(&Widget::get_number), std::mem_fn(&Widget::set_number));
  AddValueProperty("pi"         , std::mem_fn(&Widget::pi));
  AddFunctionProperty("sayHello", std::mem_fn(&Widget::sayHello));
}

double Widget::pi__ = 3.141592653589793;


Widget::Widget(const JSContext& js_context) JAVASCRIPTCORECPP_NOEXCEPT
: JSExport<Widget>(js_context)
, name__("world")
, number__(42) {
  JAVASCRIPTCORECPP_LOG_DEBUG("Widget ctor");
}

JSValue Widget::get_name() const {
  return get_context().CreateString(name__);
}

bool Widget::set_name(const JSValue& value) {
  bool result = false;
  if (value.IsString()) {
    name__ = static_cast<std::string>(value);
    result = true;
  }
  return result;
}

JSValue Widget::get_number() const {
  return get_context().CreateNumber(number__);
}

bool Widget::set_number(const JSValue& value) {
  bool result = false;
  if (value.IsNumber()) {
    number__ = static_cast<int32_t>(value);
    result = true;
  }
  return result;
}

JSValue Widget::pi() const {
  return get_context().CreateNumber(pi__);
}

JSValue Widget::sayHello(const std::vector<JSValue>& arguments) {
  std::ostringstream os;
  os << "Hello";
  
  if (!name__.empty()) {
    os << ", " << name__;
  }
  
  os << ". Your number is " << number__ << ".";
  
  return get_context().CreateString(os.str());
}

Widget::~Widget() JAVASCRIPTCORECPP_NOEXCEPT {
  JAVASCRIPTCORECPP_LOG_DEBUG("Widget dtor");
}

Widget::Widget(const Widget& rhs) JAVASCRIPTCORECPP_NOEXCEPT
: JSExport<Widget>(rhs.get_context())
, name__(rhs.name__)
, number__(rhs.number__) {
  JAVASCRIPTCORECPP_LOG_DEBUG("Widget copy ctor");
}

Widget::Widget(Widget&& rhs) JAVASCRIPTCORECPP_NOEXCEPT
: JSExport<Widget>(rhs.get_context())
, name__(rhs.name__)
, number__(rhs.number__) {
  JAVASCRIPTCORECPP_LOG_DEBUG("Widget move ctor");
}

Widget& Widget::operator=(const Widget& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
  JSExport<Widget>::operator=(rhs);
  name__   = rhs.name__;
  number__ = rhs.number__;
  JAVASCRIPTCORECPP_LOG_DEBUG("Widget copy assign");
  return *this;
}

Widget& Widget::operator=(Widget&& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
  swap(rhs);
  JAVASCRIPTCORECPP_LOG_DEBUG("Widget move assign");
  return *this;
}

void Widget::swap(Widget& other) JAVASCRIPTCORECPP_NOEXCEPT {
  JSExport<Widget>::swap(other);
  using std::swap;
  
  // By swapping the members of two classes, the two classes are
  // effectively swapped.
  swap(name__  , other.name__);
  swap(number__, other.number__);
}
