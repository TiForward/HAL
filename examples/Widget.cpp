/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "Widget.hpp"

#include <functional>
#include <sstream>

double Widget::pi__ = 3.141592653589793;


Widget::Widget(const JSContext& js_context, const std::vector<JSValue>& arguments) HAL_NOEXCEPT
: JSExportObject(js_context, arguments)
, name__("world")
, number__(42) {
  HAL_LOG_DEBUG("Widget:: ctor ", this);
  if (arguments.size() >= 1) {
    const auto _0 = arguments.at(0);
    assert(_0.IsString());
    name__   = static_cast<JSString>(_0);
  }
  
  if (arguments.size() >= 2) {
    const auto _1 = arguments.at(1);
    assert(_1.IsNumber());
    number__ = static_cast<int32_t>(_1);
  }
}

Widget::~Widget() HAL_NOEXCEPT {
  HAL_LOG_DEBUG("Widget:: dtor ", this);
}

Widget::Widget(const Widget& rhs) HAL_NOEXCEPT
: JSExportObject(rhs.get_context())
, name__(rhs.name__)
, number__(rhs.number__) {
  HAL_LOG_DEBUG("Widget:: copy ctor ", this);
}

Widget::Widget(Widget&& rhs) HAL_NOEXCEPT
: JSExportObject(rhs.get_context())
, name__(rhs.name__)
, number__(rhs.number__) {
  HAL_LOG_DEBUG("Widget:: move ctor ", this);
}

Widget& Widget::operator=(const Widget& rhs) HAL_NOEXCEPT {
  HAL_LOG_DEBUG("Widget:: copy assign ", this);
  JSExportObject::operator=(rhs);
  name__   = rhs.name__;
  number__ = rhs.number__;
  return *this;
}

Widget& Widget::operator=(Widget&& rhs) HAL_NOEXCEPT {
  HAL_LOG_DEBUG("Widget:: move assign ", this);
  swap(rhs);
  return *this;
}

void Widget::swap(Widget& other) HAL_NOEXCEPT {
  HAL_LOG_DEBUG("Widget:: swap ", this);
  JSExportObject::swap(other);
  using std::swap;
  
  // By swapping the members of two classes, the two classes are
  // effectively swapped.
  swap(name__  , other.name__);
  swap(number__, other.number__);
}

std::string Widget::get_name() const HAL_NOEXCEPT {
  return name__;
}

void Widget::set_name(const std::string& name) HAL_NOEXCEPT {
    name__ = name;
}

std::int32_t Widget::get_number() const HAL_NOEXCEPT {
  return number__;
}

void Widget::set_number(const std::int32_t number) HAL_NOEXCEPT {
  number__ = number;
}

double Widget::get_pi() HAL_NOEXCEPT {
  return pi__;
}

std::string Widget::sayHello() {
  std::ostringstream os;
  os << "Hello";
  
  if (!name__.empty()) {
    os << ", " << name__;
  }
  
  os << ". Your number is " << number__ << ".";
  
  return os.str();
}

void Widget::JSExportInitialize() {
  JSExport<Widget>::SetClassVersion(1);
  JSExport<Widget>::AddValueProperty("name"       , std::mem_fn(&Widget::js_get_name)  , std::mem_fn(&Widget::js_set_name));
  JSExport<Widget>::AddValueProperty("number"     , std::mem_fn(&Widget::js_get_number), std::mem_fn(&Widget::js_set_number));
  JSExport<Widget>::AddValueProperty("pi"         , std::mem_fn(&Widget::js_get_pi));
  JSExport<Widget>::AddFunctionProperty("sayHello", std::mem_fn(&Widget::js_sayHello));
}

JSValue Widget::js_get_name() const HAL_NOEXCEPT {
  return get_context().CreateString(get_name());
}

bool Widget::js_set_name(const JSValue& value) HAL_NOEXCEPT {
  bool result = false;
  if (value.IsString()) {
    set_name(static_cast<std::string>(value));
    result = true;
  }
  return result;
}

JSValue Widget::js_get_number() const HAL_NOEXCEPT {
	return get_context().CreateNumber(get_number());
}

bool Widget::js_set_number(const JSValue& value) HAL_NOEXCEPT {
  bool result = false;
  if (value.IsNumber()) {
    number__ = static_cast<int32_t>(value);
    result = true;
  }
  return result;
}

JSValue Widget::js_get_pi() const {
	return get_context().CreateNumber(get_pi());
}

JSValue Widget::js_sayHello(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return this_object.get_context().CreateString(sayHello());
}
