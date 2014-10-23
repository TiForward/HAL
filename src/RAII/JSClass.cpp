// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include "JavaScriptCoreCPP/RAII/JSClassBuilder.hpp"

namespace JavaScriptCoreCPP { namespace RAII {

JSClass::JSClass(const JSClassBuilder& builder)
		: js_class_definition_(builder.get_js_class_definition())
		, js_class_ref_(JSClassCreate(js_class_definition_)) {
}

}} // namespace JavaScriptCoreCPP { namespace RAII {
