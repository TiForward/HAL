/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/detail/JSExportPropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

namespace JavaScriptCoreCPP { namespace detail {

JSExportPropertyCallback::JSExportPropertyCallback(const JSString& property_name, const std::unordered_set<JSPropertyAttribute>& attributes)
    : property_name__(property_name)
    , attributes__(attributes) {
  
  if (property_name__.empty()) {
    ThrowInvalidArgument("JSExportPropertyCallback", "property_name is missing");
  }
}

JSExportPropertyCallback::JSExportPropertyCallback(const JSExportPropertyCallback& rhs)
		: property_name__(rhs.property_name__)
		, attributes__(rhs.attributes__) {
}

JSExportPropertyCallback::JSExportPropertyCallback(JSExportPropertyCallback&& rhs)
		: property_name__(std::move(rhs.property_name__))
		, attributes__(std::move(rhs.attributes__)) {
}

JSExportPropertyCallback& JSExportPropertyCallback::operator=(const JSExportPropertyCallback& rhs) {
	JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_LOCK_GUARD;
	property_name__ = rhs.property_name__;
	attributes__    = rhs.attributes__;
	return *this;
}

JSExportPropertyCallback& JSExportPropertyCallback::operator=(JSExportPropertyCallback&& rhs) {
	JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_LOCK_GUARD;
	property_name__ = std::move(rhs.property_name__);
	attributes__    = std::move(rhs.attributes__);
	return *this;
}

void JSExportPropertyCallback::swap(JSExportPropertyCallback& other) noexcept {
	JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_LOCK_GUARD;
  using std::swap;
  
  // By swapping the members of two classes, the two classes are
  // effectively swapped.
  swap(property_name__, other.property_name__);
  swap(attributes__   , other.attributes__);
}

}} // namespace JavaScriptCoreCPP { namespace detail {
