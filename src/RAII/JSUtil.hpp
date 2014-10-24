/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSUTIL_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSUTIL_HPP_

#include <cstdint>

namespace JavaScriptCoreCPP { namespace detail {


// This in the ToInt32 operation as defined in section 9.5 of the
// ECMA-262 spec. Note that this operation is identical to ToUInt32
// other than to interpretation of the resulting bit-pattern (as such
// this method is also called to implement ToUInt32).
//
// The operation can be descibed as round towards zero, then select
// the 32 least bits of the resulting value in 2s-complement
// representation.
int32_t to_int32_t(double number);

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_RAII_JSUTIL_HPP_
