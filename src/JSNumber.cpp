/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSNumber.hpp"
#include "JSUtil.hpp"
#include <sstream>
#include <cassert>

namespace JavaScriptCoreCPP { namespace RAII {

JSNumber::operator double() const {
	JSValueRef exception { nullptr };
	const double result = JSValueToNumber(get_context(), *this, &exception);
	if (exception) {
		static const std::string log_prefix { "MDL: operator double() const: " };
		std::ostringstream os;
		os << "JSNumber could not be converted to a double: " << JSValue(get_context(), exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::logic_error(message);
		// return std::numeric_limits<double>::quiet_NaN();
	}
	
		return result;
}

JSNumber::operator int32_t() const {
	return detail::to_int32_t(operator double());
}

}} // namespace JavaScriptCoreCPP { namespace RAII {
