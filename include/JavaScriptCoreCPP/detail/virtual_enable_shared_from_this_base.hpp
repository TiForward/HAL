/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_VIRTUAL_ENABLE_SHARED_FROM_THIS_BASE_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_VIRTUAL_ENABLE_SHARED_FROM_THIS_BASE_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include <memory>

namespace JavaScriptCoreCPP { namespace detail {

// This header file allows std::enable_shared_from_this to work when
// both base and derived classes both need to inherit from
// std::enable_shared_from_this. Use it like this:
//
// struct A: virtual_enable_shared_from_this<A> {};
//
// struct B: virtual_enable_shared_from_this<B> {};
//
// struct C: A, B, virtual_enable_shared_from_this<C> {
//   using enable_shared_from_this<C>::shared_from_this;
// }

struct JAVASCRIPTCORECPP_EXPORT virtual_enable_shared_from_this_base : public std::enable_shared_from_this<virtual_enable_shared_from_this_base> {
  virtual ~virtual_enable_shared_from_this_base() {
  }
};

template<typename T>
struct JAVASCRIPTCORECPP_EXPORT virtual_enable_shared_from_this : virtual public virtual_enable_shared_from_this_base {
  std::shared_ptr<T> shared_from_this() {
    return std::dynamic_pointer_cast<T>(virtual_enable_shared_from_this_base::shared_from_this());
  }
};

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_VIRTUAL_ENABLE_SHARED_FROM_THIS_BASE_HPP_
