/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_HPP_
#define _JAVASCRIPTCORECPP_HPP_

#include "JavaScriptCoreCPP/JSContextGroup.hpp"
#include "JavaScriptCoreCPP/JSContext.hpp"

#include "JavaScriptCoreCPP/JSString.hpp"

#include "JavaScriptCoreCPP/JSValue.hpp"
#include "JavaScriptCoreCPP/JSUndefined.hpp"
#include "JavaScriptCoreCPP/JSNull.hpp"
#include "JavaScriptCoreCPP/JSBoolean.hpp"
#include "JavaScriptCoreCPP/JSNumber.hpp"

#include "JavaScriptCoreCPP/JSObject.hpp"
#include "JavaScriptCoreCPP/JSArray.hpp"
#include "JavaScriptCoreCPP/JSDate.hpp"
#include "JavaScriptCoreCPP/JSError.hpp"
#include "JavaScriptCoreCPP/JSFunction.hpp"
#include "JavaScriptCoreCPP/JSRegExp.hpp"

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSPropertyNameAccumulator.hpp"

#include "JavaScriptCoreCPP/JSNativeClassBuilder.hpp"
#include "JavaScriptCoreCPP/JSNativeClass.hpp"
#include "JavaScriptCoreCPP/JSNativeClassAttribute.hpp"
#include "JavaScriptCoreCPP/JSNativeObject.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#include "JavaScriptCoreCPP/detail/JSPerformanceCounterPrinter.hpp"
#endif

#endif // _JAVASCRIPTCORECPP_HPP_
