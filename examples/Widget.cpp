/**
 * HAL
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
  JSExport<Widget>::SetClassVersion(1);
  JSExport<Widget>::AddValueProperty("name"       , std::mem_fn(&Widget::get_name)  , std::mem_fn(&Widget::set_name));
  JSExport<Widget>::AddValueProperty("number"     , std::mem_fn(&Widget::get_number), std::mem_fn(&Widget::set_number));
  JSExport<Widget>::AddValueProperty("pi"         , std::mem_fn(&Widget::pi));
  JSExport<Widget>::AddFunctionProperty("sayHello", std::mem_fn(&Widget::sayHello));
}

double Widget::pi__ = 3.141592653589793;


Widget::Widget(const JSContext& js_context) HAL_NOEXCEPT
: JSExportObject(js_context)
, name__("world")
, number__(42) {
  HAL_LOG_DEBUG("Widget ctor");
}

Widget::Widget(const Widget& rhs, const std::vector<JSValue>& arguments) HAL_NOEXCEPT
: JSExportObject(rhs, arguments)
, name__(rhs.name__)
, number__(rhs.number__) {
  HAL_LOG_DEBUG("Widget JavaScript constructor");
  assert(arguments.size() >= 2);
  const auto _0 = arguments.at(0);
  const auto _1 = arguments.at(1);
  assert(_0.IsString());
  assert(_1.IsNumber());
  name__   = static_cast<JSString>(_0);
  number__ = static_cast<int32_t>(_1);
  
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

JSValue Widget::sayHello(const std::vector<JSValue>& arguments, JSObject& this_object) {
  std::ostringstream os;
  os << "Hello";
  
  if (!name__.empty()) {
    os << ", " << name__;
  }
  
  os << ". Your number is " << number__ << ".";
  
  return get_context().CreateString(os.str());
}

std::string Widget::get_name_native() const {
  return name__;
}

void Widget::set_name_native(const std::string& name) {
  name__ = name;
}

int Widget::get_number_native() const {
  return number__;
}

void Widget::set_number_native(int number) {
  number__ = number;
}

Widget::~Widget() HAL_NOEXCEPT {
  HAL_LOG_DEBUG("Widget dtor");
}

Widget::Widget(const Widget& rhs) HAL_NOEXCEPT
: JSExportObject(rhs.get_context())
, name__(rhs.name__)
, number__(rhs.number__) {
  HAL_LOG_DEBUG("Widget copy ctor");
}

Widget::Widget(Widget&& rhs) HAL_NOEXCEPT
: JSExportObject(rhs.get_context())
, name__(rhs.name__)
, number__(rhs.number__) {
  HAL_LOG_DEBUG("Widget move ctor");
}

Widget& Widget::operator=(const Widget& rhs) HAL_NOEXCEPT {
  JSExportObject::operator=(rhs);
  name__   = rhs.name__;
  number__ = rhs.number__;
  HAL_LOG_DEBUG("Widget copy assign");
  return *this;
}

Widget& Widget::operator=(Widget&& rhs) HAL_NOEXCEPT {
  swap(rhs);
  HAL_LOG_DEBUG("Widget move assign");
  return *this;
}

void Widget::swap(Widget& other) HAL_NOEXCEPT {
  JSExportObject::swap(other);
  using std::swap;
  
  // By swapping the members of two classes, the two classes are
  // effectively swapped.
  swap(name__  , other.name__);
  swap(number__, other.number__);
}
