/**
 * TitaniumCommon
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSLOGGERPIMPL_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSLOGGERPIMPL_HPP_

#include <string>
#include <cstdint>

namespace JavaScriptCoreCPP {
  template<typename JSLoggerPolicy>
  class JSLogger;
}

namespace JavaScriptCoreCPP { namespace detail {
  
  class JSLoggerPimpl final {
    
  public:
    
    // private:
    
    //  // We are used exclusively JSLogger<T>.
    //  template<typename JSLoggerPolicy>
    //  friend class JSLogger;
    
    JSLoggerPimpl()                                = delete;
    ~JSLoggerPimpl()                               = delete;
    JSLoggerPimpl(const JSLoggerPimpl&)            = delete;
    JSLoggerPimpl(JSLoggerPimpl&&)                 = delete;
    JSLoggerPimpl& operator=(const JSLoggerPimpl&) = delete;
    JSLoggerPimpl& operator=(JSLoggerPimpl&&)      = delete;
    
    static std::string GetLoglineHeader(uint32_t log_line_number);
  };
  
}} // namespace JavaScriptCoreCPP { namespace detail

#endif // _JAVASCRIPTCORECPP_DETAIL_JSLOGGERPIMPL_HPP_

