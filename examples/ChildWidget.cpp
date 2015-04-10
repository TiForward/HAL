/**
 * HAL
 *
 * Copyright (c) 2015 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "ChildWidget.hpp"

#include <functional>
#include <sstream>
#include <vector>

ChildWidget::ChildWidget(const JSContext& js_context) HAL_NOEXCEPT
: Widget(js_context) {
  HAL_LOG_DEBUG("ChildWidget:: ctor ", this);
}

ChildWidget::~ChildWidget() HAL_NOEXCEPT {
  HAL_LOG_DEBUG("ChildWidget:: dtor ", this);
}

void ChildWidget::postInitialize(JSObject& js_object) {
  HAL_LOG_DEBUG("ChildWidget:: postInitialize ", this);
}

void ChildWidget::postCallAsConstructor(const JSContext& js_context, const std::vector<JSValue>& arguments) {
  HAL_LOG_DEBUG("ChildWidget:: postCallAsConstructor ", this);
}

void ChildWidget::JSExportInitialize() {
  JSExport<ChildWidget>::SetClassVersion(1);
  JSExport<ChildWidget>::SetParent(JSExport<Widget>::Class());
  JSExport<ChildWidget>::AddValueProperty("name", std::mem_fn(&ChildWidget::js_get_name));
  JSExport<ChildWidget>::AddValueProperty("my_name", std::mem_fn(&ChildWidget::js_get_my_name));
}

JSValue ChildWidget::js_get_name() const HAL_NOEXCEPT {
  // delegate to parent
  return get_context().CreateNativeNull();
}

JSValue ChildWidget::js_get_my_name() const HAL_NOEXCEPT {
  return get_context().CreateString("child widget");
}
