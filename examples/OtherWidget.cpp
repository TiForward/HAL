/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "OtherWidget.hpp"

OtherWidget::OtherWidget(const JSContext& js_context) HAL_NOEXCEPT
: JSExportObject(js_context) {
  HAL_LOG_DEBUG("OtherWidget:: ctor ", this);
}

OtherWidget::~OtherWidget() HAL_NOEXCEPT {
  HAL_LOG_DEBUG("OtherWidget:: dtor ", this);
}

OtherWidget::OtherWidget(const OtherWidget& rhs) HAL_NOEXCEPT
: JSExportObject(rhs.get_context()) {
  HAL_LOG_DEBUG("OtherWidget:: copy ctor ", this);
}

OtherWidget::OtherWidget(OtherWidget&& rhs) HAL_NOEXCEPT
: JSExportObject(rhs.get_context()) {
  HAL_LOG_DEBUG("OtherWidget:: move ctor ", this);
}

OtherWidget& OtherWidget::operator=(const OtherWidget& rhs) HAL_NOEXCEPT {
  HAL_LOG_DEBUG("OtherWidget:: copy assign ", this);
  JSExportObject::operator=(rhs);
  return *this;
}

OtherWidget& OtherWidget::operator=(OtherWidget&& rhs) HAL_NOEXCEPT {
  HAL_LOG_DEBUG("OtherWidget:: move assign ", this);
  swap(rhs);
  return *this;
}

void OtherWidget::swap(OtherWidget& other) HAL_NOEXCEPT {
  HAL_LOG_DEBUG("OtherWidget:: swap ", this);
  JSExportObject::swap(other);
  using std::swap;
}

void OtherWidget::postInitialize(JSObject& js_object) {
  HAL_LOG_DEBUG("OtherWidget:: postInitialize ", this);
}

void OtherWidget::postCallAsConstructor(const JSContext& js_context, const std::vector<JSValue>& arguments) {
  HAL_LOG_DEBUG("OtherWidget:: postCallAsConstructor ", this);
}

void OtherWidget::JSExportInitialize() {
  JSExport<OtherWidget>::SetClassVersion(1);
  JSExport<OtherWidget>::SetParent(JSExport<JSExportObject>::Class());
}
