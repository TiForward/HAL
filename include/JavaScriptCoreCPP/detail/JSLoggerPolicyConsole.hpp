/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSLOGGERPOLICYCONSOLE_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSLOGGERPOLICYCONSOLE_HPP_

#include "JavaScriptCoreCPP/detail/JSLoggerPolicyInterface.hpp"
#include <iostream>

namespace JavaScriptCoreCPP { namespace detail {
  
  class JAVASCRIPTCORECPP_EXPORT JSLoggerPolicyConsole final : public JSLoggerPolicyInterface {
  public:
    
    JSLoggerPolicyConsole(const std::string& name) {
    }
    
    JSLoggerPolicyConsole()                                        = delete;
    ~JSLoggerPolicyConsole()                                       = default;
    JSLoggerPolicyConsole(const JSLoggerPolicyConsole&)            = default;
    JSLoggerPolicyConsole& operator=(const JSLoggerPolicyConsole&) = default;
    
#ifdef JAVASCRIPTCORECPP_MOVE_CTOR_AND_ASSIGN_DEFAULT_ENABLE
    JSLoggerPolicyConsole(JSLoggerPolicyConsole&&)                 = default;
    JSLoggerPolicyConsole& operator=(JSLoggerPolicyConsole&&)      = default;
#endif
    
    virtual void Write(const std::string& log_message) override final {
      std::clog << log_message << std::endl;
      }
  };
      
}} // namespace JavaScriptCoreCPP { namespace detail {
      
#endif // _JAVASCRIPTCORECPP_DETAIL_JSLOGGERPOLICYCONSOLE_HPP_
