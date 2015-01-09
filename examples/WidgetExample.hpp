/**
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _HAL_EXAMPLES_WIDGETEXAMPLES_HPP_
#define _HAL_EXAMPLES_WIDGETEXAMPLES_HPP_

#include "HAL/HAL.hpp"
#include "Widget.hpp"

using namespace HAL;

class WidgetExample final : public Widget, public JSExport<WidgetExample> {
  
public:
  WidgetExample(const JSContext& js_context, const std::vector<JSValue>& arguments = {}) HAL_NOEXCEPT;
  
  virtual ~WidgetExample()                       = default;
  WidgetExample(const WidgetExample&)            = default;
  WidgetExample(WidgetExample&&)                 = default;
  WidgetExample& operator=(const WidgetExample&) = default;
  WidgetExample& operator=(WidgetExample&&)      = default;
  
  static void JSExportInitialize();
  
  JSValue js_get_data() const              HAL_NOEXCEPT;
  bool    js_set_data(const JSValue& name) HAL_NOEXCEPT;
  
private:
  JSValue data__;
};

#endif // _HAL_EXAMPLES_WIDGETEXAMPLES_HPP_
