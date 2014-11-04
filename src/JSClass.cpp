/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSClassPimpl.hpp"

#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

JSClass::JSClass(std::shared<detail::JSClassPimpl> js_class_pimpl_ptr)
		: js_class_pimpl_ptr__(js_class_pimpl_ptr) {
}

std::string JSClass::get_name() {
	return js_class_pimpl_ptr__ -> get_name();
}
  
std::uint32_t get_version() {
	return js_class_pimpl_ptr__ -> get_version();
}

JSClass::operator JSClassRef() const {
	return js_class_pimpl_ptr__ -> operator JSClassRef();
}

} // namespace JavaScriptCoreCPP {





// static JSClass EmptyJSClass() {
// 	static std::once_flag of;
// 	std::call_once(of, [] {
// 			JSClassDefinition js_class_definition = kJSClassDefinitionEmpty;
// 			empty_js_class_ = JSClass(&js_class_definition);
// 		});
// }

// void JSClass::ThrowRuntimeErrorIfJSClassAlreadyExists(const JSString& js_class_name) {
// 	JAVASCRIPTCORECPP_JSCLASS_STATIC_LOCK_GUARD;
	
// 	const auto position = js_class_map_.find(js_class_name);
// 	const bool found    = position != js_class_map_.end();
	
// 	if (found) {
// 		const std::string message = "A JSClass with class name " + static_cast<std::string>(js_class_name) + " already exists.";
// 		JAVASCRIPTCORECPP_LOG_ERROR(message);
// 		ThrowRuntimeError("JSClass", message);
// 	}
// }

} // namespace JavaScriptCoreCPP {


	// // Create the JSNativeClassPimpl, which holds and initializes the
	// // JavaScriptCore C API JSClassDefinition.

	// // These are for the JavaScriptCore C API JSStaticValue.
	// JSNativeObjectStaticPropertyMap_t native_value_property_map;
  // if (!native_value_property_callback_map__.empty()) {
  //   for (const auto& entry : native_value_property_callback_map__) {
  //     const auto& property_name           = entry.first;
  //     const auto& value_property_callback = entry.second;
  //     native_value_property_map(property_name, value_property_callback.get_attributes());
  //   }
  // }
  
	// // These are for the JavaScriptCore C API JSStaticFunction.
  // JSNativeObjectStaticPropertyMap_t native_function_property_map;
  // if (!native_function_property_callback_map__.empty()) {
  //   for (const auto& entry : native_function_property_callback_map__) {
  //     const auto& function_name              = entry.first;
  //     const auto& function_property_callback = entry.second;
  //     native_function_property_map(property_name, function_property_callback.get_attributes());
  //   }
  // }

  // JSNativeClassPimpl js_native_class_pimpl(js_class_name__,
  //                                          js_class_version__,
  //                                          js_class_attribute__,
  //                                          js_class_parent__,
  //                                          native_value_property_map,
  //                                          native_function_property_map);

  // // We must provide the following 5 callbacks to JSNativeClassPimpl
	// // because they refer to members of JSNativeClass<T>.
	// js_native_class_pimpl.js_class_definition_.initialize        = initialize_callback__;
	// js_native_class_pimpl.js_class_definition_.finalize          = finalize_callback__;
	// js_native_class_pimpl.js_class_definition_.callAsFunction    = call_as_function_callback__;
	// js_native_class_pimpl.js_class_definition_.callAsConstructor = call_as_constructor_callback__;
	// js_native_class_pimpl.js_class_definition_.hasInstance       = has_instance_callback__;
	// js_native_class_pimpl.js_class_definition_.convertToType     = convert_to_type_callback__;

