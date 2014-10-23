/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_RAII_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_RAII_HPP_

#include "JavaScriptCoreCPP/RAII/JSContextGroup.hpp"
#include "JavaScriptCoreCPP/RAII/JSContext.hpp"

#include "JavaScriptCoreCPP/RAII/JSString.hpp"

#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include "JavaScriptCoreCPP/RAII/JSUndefined.hpp"
#include "JavaScriptCoreCPP/RAII/JSNull.hpp"
#include "JavaScriptCoreCPP/RAII/JSBoolean.hpp"
#include "JavaScriptCoreCPP/RAII/JSNumber.hpp"

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include "JavaScriptCoreCPP/RAII/JSArray.hpp"
#include "JavaScriptCoreCPP/RAII/JSDate.hpp"
#include "JavaScriptCoreCPP/RAII/JSError.hpp"
#include "JavaScriptCoreCPP/RAII/JSFunction.hpp"
#include "JavaScriptCoreCPP/RAII/JSRegExp.hpp"

#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include "JavaScriptCoreCPP/RAII/JSClassBuilder.hpp"
#include "JavaScriptCoreCPP/RAII/JSClassDefinition.hpp"
#include "JavaScriptCoreCPP/RAII/JSPropertyNameAccumulator.hpp"

#include "JavaScriptCoreCPP/RAII/JSStaticFunction.hpp"
#include "JavaScriptCoreCPP/RAII/JSStaticValue.hpp"

namespace JavaScriptCoreCPP { namespace RAII {

/* Script Evaluation */

/*!
  @function JSEvaluateScript
  @abstract                   Evaluate a string of JavaScript using the global object as "this.".
  @param js_context           The execution context to use.
  @param script               A JSString containing the script to evaluate.
  @param source_url           An optional JSString containing a URL for the script's source file. This is used by debuggers and when reporting exceptions.
  @param starting_line_number An optional integer value specifying the script's starting line number in the file located at source_url. This is only used when reporting exceptions. The value is one-based, so the first line is line 1 and invalid values are clamped to 1.
  @result                     The JSValue that results from evaluating script.
  @throws                     std::runtime_error exception if the evaluated script threw an exception.
*/
JSValue JSEvaluateScript(const JSContext& js_context, const JSString& script, const JSString& source_url = JSString(), int starting_line_number = 1);

/*!
  @function JSEvaluateScript
  @abstract                   Evaluate a string of JavaScript.
  @param js_context           The execution context to use.
  @param script               A JSString containing the script to evaluate.
  @param this_object          The object to use as "this".
  @param source_url           An optional JSString containing a URL for the script's source file. This is used by debuggers and when reporting exceptions.
  @param starting_line_number An optional integer value specifying the script's starting line number in the file located at source_url. This is only used when reporting exceptions. The value is one-based, so the first line is line 1 and invalid values are clamped to 1.
  @result                     The JSValue that results from evaluating script.
  @throws                     std::runtime_error exception if the evaluated script threw an exception.
*/
JSValue JSEvaluateScript(const JSContext& js_context, const JSString& script, const JSObject& this_object, const JSString& source_url = JSString(), int starting_line_number = 1);


/*!
  @function JSCheckScriptSyntax
  @abstract Checks for syntax errors in a string of JavaScript.
  @param ctx The execution context to use.
  @param script A JSString containing the script to check for syntax errors.
  @param sourceURL A JSString containing a URL for the script's source file. This is only used when reporting exceptions. Pass NULL if you do not care to include source file information in exceptions.
  @param startingLineNumber An integer value specifying the script's starting line number in the file located at sourceURL. This is only used when reporting exceptions. The value is one-based, so the first line is line 1 and invalid values are clamped to 1.
  @param exception A pointer to a JSValueRef in which to store a syntax error exception, if any. Pass NULL if you do not care to store a syntax error exception.
  @result true if the script is syntactically correct, otherwise false.
*/
bool JSCheckScriptSyntax(const JSContext& js_context, const JSString& script, const JSString& source_url = JSString(), int starting_line_number = 1);

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_RAII_HPP_

