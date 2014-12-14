/**
 * HAL
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "Widget.hpp"

#include <iostream>

int main () {
  using namespace HAL;
  JSContextGroup js_context_group;

  JSContext js_context = js_context_group.CreateContext();
  auto global_object   = js_context.get_global_object();

  auto widget = js_context.CreateObject(JSExport<Widget>::Class());
  js_context.get_global_object().SetProperty("Widget", widget);

  auto result = js_context.JSEvaluateScript("typeof Widget;");
  std::cout << "Widget is an " << static_cast<std::string>(result) << std::endl;
  
  result = js_context.JSEvaluateScript("Widget.name;");
  std::cout << "Widget.name; // outputs '" << static_cast<std::string>(result) << "'" << std::endl;

  result = js_context.JSEvaluateScript("Widget.number;");
  std::cout << "Widget.number; // outputs '" << static_cast<std::uint32_t>(result) << "'" << std::endl;

  result = js_context.JSEvaluateScript("Widget.sayHello();");
  std::cout << "Widget.sayHello(); // outputs '" << static_cast<std::string>(result) << "'" << std::endl;

  result = js_context.JSEvaluateScript("Widget.name = 'foo'; Widget.name;");
  std::cout << "Widget.name = 'foo'; Widget.name; // outputs '" << static_cast<std::string>(result) << "'" << std::endl;
  
  result = js_context.JSEvaluateScript("Widget.number = 3*7; Widget.number;");
  std::cout << "Widget.number = 3*7; Widget.number; // outputs '" << static_cast<std::uint32_t>(result) << "'" << std::endl;
  
  result = js_context.JSEvaluateScript("Widget.sayHello();");
  std::cout << "Widget.sayHello(); // outputs '" << static_cast<std::string>(result) << "'" << std::endl;
}
