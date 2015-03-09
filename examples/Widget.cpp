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
#include <vector>

double Widget::pi__ = 3.141592653589793;
uint32_t Widget::constructor_count__ = 0;

Widget::Widget(const JSContext& js_context) HAL_NOEXCEPT
: JSExportObject(js_context)
, name__("world")
, jsvalue__(js_context.CreateObject())
, jsnull__(js_context.CreateNull())
, jsundefined__(js_context.CreateUndefined())
, jsnumber__(js_context.CreateNumber(123))
, jsstring__(js_context.CreateString("js string"))
, jsboolean__(js_context.CreateBoolean(true))
, jsobject__(js_context.CreateObject())
, jsarray__(js_context.CreateArray())
, jsfunction__(js_context.CreateFunction("return 'it works fine';"))
, hello_callback__(js_context.CreateFunction("return 'Hello, World';"))
, jsdate__(js_context.CreateDate())
, jserror__(js_context.CreateError())
, jsregexp__(js_context.CreateRegExp())
, number__(42) {
  HAL_LOG_DEBUG("Widget:: ctor ", this);

  jsobject__.SetProperty("test", js_context.CreateString("ok"));
  jsarray__.SetProperty(0, js_context.CreateString("works"));

  constructor_count__++;
}

Widget::~Widget() HAL_NOEXCEPT {
  HAL_LOG_DEBUG("Widget:: dtor ", this);
}

Widget::Widget(const Widget& rhs) HAL_NOEXCEPT
: JSExportObject(rhs.get_context())
, name__(rhs.name__)
, jsvalue__(rhs.jsvalue__)
, jsobject__(rhs.jsobject__)
, jsarray__(rhs.jsarray__)
, jsfunction__(rhs.jsfunction__)
, hello_callback__(rhs.hello_callback__)
, jsnull__(rhs.jsnull__)
, jsundefined__(rhs.jsundefined__)
, jsboolean__(rhs.jsboolean__)
, jsnumber__(rhs.jsnumber__)
, jsstring__(rhs.jsstring__)
, jsdate__(rhs.jsdate__)
, jserror__(rhs.jserror__)
, jsregexp__(rhs.jsregexp__)
, number__(rhs.number__) {
  HAL_LOG_DEBUG("Widget:: copy ctor ", this);
}

Widget::Widget(Widget&& rhs) HAL_NOEXCEPT
: JSExportObject(rhs.get_context())
, name__(rhs.name__)
, jsvalue__(rhs.jsvalue__)
, jsobject__(rhs.jsobject__)
, jsarray__(rhs.jsarray__)
, jsfunction__(rhs.jsfunction__)
, hello_callback__(rhs.hello_callback__)
, jsnull__(rhs.jsnull__)
, jsundefined__(rhs.jsundefined__)
, jsboolean__(rhs.jsboolean__)
, jsnumber__(rhs.jsnumber__)
, jsstring__(rhs.jsstring__)
, jsdate__(rhs.jsdate__)
, jserror__(rhs.jserror__)
, jsregexp__(rhs.jsregexp__)
, number__(rhs.number__) {
  HAL_LOG_DEBUG("Widget:: move ctor ", this);
}

Widget& Widget::operator=(const Widget& rhs) HAL_NOEXCEPT {
  HAL_LOG_DEBUG("Widget:: copy assign ", this);
  JSExportObject::operator=(rhs);
  name__   = rhs.name__;
  number__ = rhs.number__;
  jsvalue__ = rhs.jsvalue__;
  jsobject__ = rhs.jsobject__;
  jsarray__ = rhs.jsarray__;
  jsfunction__ = rhs.jsfunction__;
  hello_callback__ = rhs.hello_callback__;
  jsnull__ = rhs.jsnull__;
  jsundefined__ = rhs.jsundefined__;
  jsboolean__ = rhs.jsboolean__;
  jsnumber__ = rhs.jsnumber__;
  jsstring__ = rhs.jsstring__;
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
  swap(jsvalue__, other.jsvalue__);
  swap(jsobject__, other.jsobject__);
  swap(jsarray__, other.jsarray__);
  swap(jsfunction__, other.jsfunction__);
  swap(hello_callback__, other.hello_callback__);
  swap(jsnull__, other.jsnull__);
  swap(jsundefined__, other.jsundefined__);
  swap(jsboolean__, other.jsboolean__);
  swap(jsnumber__, other.jsnumber__);
  swap(jsstring__, other.jsstring__);
}

void Widget::postInitialize(JSObject& js_object) {
  js_object.SetProperty("test_postInitialize_called", get_context().CreateBoolean(true));
}

void Widget::postCallAsConstructor(const JSContext& js_context, const std::vector<JSValue>& arguments) {
  HAL_LOG_DEBUG("Widget:: postCallAsConstructor ", this);
  
  get_object().SetProperty("test_postCallAsConstructor_called", get_context().CreateBoolean(true));
  
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

std::string Widget::testMemberObjectProperty() const HAL_NOEXCEPT {
  if (!jsobject__.HasProperty("test")) {
    return "failed";
  }
  return static_cast<std::string>(jsobject__.GetProperty("test"));
}

std::string Widget::testMemberArrayProperty() const HAL_NOEXCEPT {
  return static_cast<std::string>(jsarray__.GetProperty(0));
}

std::string Widget::testCallAsFunction(JSObject& this_object) HAL_NOEXCEPT {
  // Every function can be called outside of Widget (e.g. var test = widget.callAsFunction)
  // In that case "this" could be null. We need to make sure if parent is still there
  if (this == nullptr) {
    return "not on Widget";
  } else {
    return static_cast<std::string>(jsfunction__(this_object));
  }
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
  JSExport<Widget>::AddValueProperty("value"     , std::mem_fn(&Widget::js_get_value), std::mem_fn(&Widget::js_set_value));
  JSExport<Widget>::AddValueProperty("pi"         , std::mem_fn(&Widget::js_get_pi));
  JSExport<Widget>::AddFunctionProperty("helloCallback", std::mem_fn(&Widget::js_helloLambda));
  JSExport<Widget>::AddFunctionProperty("sayHello", std::mem_fn(&Widget::js_sayHello));
  JSExport<Widget>::AddFunctionProperty("sayHelloWithCallback", std::mem_fn(&Widget::js_sayHelloWithCallback));
  JSExport<Widget>::AddFunctionProperty("testMemberObjectProperty", std::mem_fn(&Widget::js_testMemberObjectProperty));
  JSExport<Widget>::AddFunctionProperty("testMemberArrayProperty", std::mem_fn(&Widget::js_testMemberArrayProperty));
  JSExport<Widget>::AddFunctionProperty("testMemberNullProperty", std::mem_fn(&Widget::js_testMemberNullProperty));
  JSExport<Widget>::AddFunctionProperty("testMemberUndefinedProperty", std::mem_fn(&Widget::js_testMemberUndefinedProperty));
  JSExport<Widget>::AddFunctionProperty("testMemberBooleanProperty", std::mem_fn(&Widget::js_testMemberBooleanProperty));
  JSExport<Widget>::AddFunctionProperty("testMemberNumberProperty", std::mem_fn(&Widget::js_testMemberNumberProperty));
  JSExport<Widget>::AddFunctionProperty("testMemberStringProperty", std::mem_fn(&Widget::js_testMemberStringProperty));
  JSExport<Widget>::AddFunctionProperty("testMemberDateProperty", std::mem_fn(&Widget::js_testMemberDateProperty));
  JSExport<Widget>::AddFunctionProperty("testMemberErrorProperty", std::mem_fn(&Widget::js_testMemberErrorProperty));
  JSExport<Widget>::AddFunctionProperty("testMemberRegExpProperty", std::mem_fn(&Widget::js_testMemberRegExpProperty));
  JSExport<Widget>::AddFunctionProperty("testCallAsFunction", std::mem_fn(&Widget::js_testCallAsFunction));
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

JSValue Widget::js_get_value() const HAL_NOEXCEPT {
  return jsvalue__;
}

bool Widget::js_set_value(const JSValue& value) HAL_NOEXCEPT {
  jsvalue__ = value;
  return true;
}

JSValue Widget::js_get_pi() const {
  return get_context().CreateNumber(get_pi());
}

JSValue Widget::js_sayHelloWithCallback(const std::vector<JSValue>& arguments, JSObject& this_object) {
  if (arguments.size() > 0) {
    if (arguments.at(0).IsObject()) {
      // sayHello(callback);
      JSObject callback = static_cast<JSObject>(arguments.at(0));
      const std::vector<JSValue> callback_args = { js_get_name(), js_get_number() };
      return callback(callback_args, this_object);
    }
  }
  return this_object.get_context().CreateString(sayHello());
}

// async callback and get_object() example
// this shows how to get JSObject associated with JSExport object
JSValue Widget::js_helloLambda(const std::vector<JSValue>& arguments, JSObject& ignore) {
  if (arguments.size() > 0) {
    if (arguments.at(0).IsObject()) {
      hello_callback__ = static_cast<JSObject>(arguments.at(0));
      // suppose you need async operation but you can not pass "this" JSObject to C++ callback
      // in this case you still have access to JSObject by calling get_object()
      auto func = [this]() {
        JSObject this_object = get_object();
        const std::vector<JSValue> callback_args = { get_context().CreateString("Callback") };
        return hello_callback__(callback_args, this_object);
      };
      return func();
    }
  }
  return get_context().CreateUndefined();
}

JSValue Widget::js_testMemberObjectProperty(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return this_object.get_context().CreateString(testMemberObjectProperty());
}

JSValue Widget::js_testMemberArrayProperty(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return this_object.get_context().CreateString(testMemberArrayProperty());
}

JSValue Widget::js_testMemberNullProperty(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return jsnull__;
}

JSValue Widget::js_testMemberUndefinedProperty(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return jsundefined__;
}

JSValue Widget::js_testMemberBooleanProperty(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return jsboolean__;
}

JSValue Widget::js_testMemberNumberProperty(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return jsnumber__;
}

JSValue Widget::js_testMemberStringProperty(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return jsstring__;
}

JSValue Widget::js_testMemberDateProperty(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return jsdate__;
}

JSValue Widget::js_testMemberErrorProperty(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return jserror__;
}

JSValue Widget::js_testMemberRegExpProperty(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return jsregexp__;
}

JSValue Widget::js_testCallAsFunction(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return this_object.get_context().CreateString(testCallAsFunction(this_object));
}

JSValue Widget::js_sayHello(const std::vector<JSValue>& arguments, JSObject& this_object) {
  return this_object.get_context().CreateString(sayHello());
}
