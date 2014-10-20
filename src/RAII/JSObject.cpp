// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include "JavaScriptCoreCPP/RAII/JSPropertyNameArray.hpp"
#include <cassert>

namespace JavaScriptCoreCPP {

std::vector<JSString> JSObject::GetPropertyNames() const {
	return JSPropertyNameArray(*this);
}

} // namespace JavaScriptCoreCPP
