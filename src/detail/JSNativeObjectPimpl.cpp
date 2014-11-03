/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/detail/JSNativeObjectPimpl.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

namespace JavaScriptCoreCPP { namespace detail

bool JSNativeObjectPimpl::HasInstance(const JSValue& possible_instance) const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::HasInstance:" };
	
	try {
		static_cast<void>(dynamic_cast<const T&>(possible_instance));
		return true;
	} catch (const std::bad_cast& exception) {
#ifdef JAVASCRIPTCORECPP_JSNATIVEOBJECT_DEBUG
		std::clog << log_prefix
		          << "[DEBUG] possible_instance "
		          << possible_instance
		          << " generated std::bad_cast exception: "
		          << exception.what()
		          << "."
		          << std::endl;
#endif
	} catch (const std::exception& exception) {
#ifdef JAVASCRIPTCORECPP_JSNATIVEOBJECT_DEBUG
		std::clog << log_prefix
		          << "[DEBUG] possible_instance "
		          << possible_instance
		          << " generated std::exception: "
		          << exception.what()
		          << "."
		          << std::endl;
#endif
	} catch (...) {
#ifdef JAVASCRIPTCORECPP_JSNATIVEOBJECT_DEBUG
		std::clog << log_prefix
		          << "[DEBUG] possible_instance "
		          << possible_instance
		          << " generated an unknown exception."
		          << std::endl;
#endif
	}
	return false;
}

}: // namespace JavaScriptCoreCPP { namespace detail
