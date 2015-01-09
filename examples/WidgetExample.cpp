/**
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "WidgetExample.hpp"

WidgetExample::WidgetExample(const JSContext& js_context, const std::vector<JSValue>& arguments) HAL_NOEXCEPT
: Widget(js_context, arguments)
, data__(js_context.CreateString("initial string")) {
  HAL_LOG_DEBUG("WidgetExample:: ctor ", this);
}

void WidgetExample::JSExportInitialize() {
  JSExport<WidgetExample>::SetClassVersion(1);
  JSExport<WidgetExample>::AddValueProperty("data", std::mem_fn(&WidgetExample::js_get_data), std::mem_fn(&WidgetExample::js_set_data));
}

JSValue WidgetExample::js_get_data() const HAL_NOEXCEPT {
  return data__;
}

bool WidgetExample::js_set_data(const JSValue& data) HAL_NOEXCEPT {
  data__ = data;
  return true;
}
