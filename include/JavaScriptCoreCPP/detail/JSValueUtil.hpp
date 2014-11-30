/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSVALUEUTIL_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSVALUEUTIL_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include "JavaScriptCoreCPP/JSValue.hpp"
#include <string>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace detail {
  
  JAVASCRIPTCORECPP_EXPORT JSValue::Type ToJSValueType(JSType type) JAVASCRIPTCORECPP_NOEXCEPT;
  
}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSVALUEUTIL_HPP_
