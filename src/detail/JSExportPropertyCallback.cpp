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

JSExportPropertyCallback::JSExportPropertyCallback(const JSString& property_name, const std::unordered_set<JSObjectPropertyAttribute>& attributes)
    : property_name__(property_name)
    , attributes__(attributes) {
  
  if (property_name__.empty()) {
    ThrowInvalidArgument("JSExportPropertyCallback", "property_name is missing");
  }
}

JSExportPropertyCallback::(const JSExportPropertyCallback& rhs)
: property_name__(rhs.property_name__)
    , attributes__(rhs.attributes__) {
}

JSExportPropertyCallback::(JSExportPropertyCallback&& rhs)
: property_name__(std::move(rhs.property_name__))
    , attributes__(std::move(rhs.attributes__)) {
}

// Create a copy of another JSExportPropertyCallback by
// assignment. This is a unified assignment operator that fuses the
// copy assignment operator
//
// X& X::operator=(const X&)
//
// and the move assignment operator
//
// X& X::operator=(X&&);
JSExportPropertyCallback& JSExportPropertyCallback::operator=(JSExportPropertyCallback rhs) {
  JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPROPERTYCALLBACK_LOCK_GUARD;
  swap(rhs);
  return *this;
}

void JSExportFunctionPropertyCallback::swap(JSExportFunctionPropertyCallback& other) noexcept {
  JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPROPERTYCALLBACK_LOCK_GUARD;
  using std::swap;
  
  // By swapping the members of two classes, the two classes are
  // effectively swapped.
  swap(property_name__, other.property_name__);
  swap(attributes__   , other.attributes__);
}

}} // namespace JavaScriptCoreCPP { namespace detail {
