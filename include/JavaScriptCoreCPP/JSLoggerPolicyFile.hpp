/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSLOGGERPOLICYFILE_HPP_
#define _JAVASCRIPTCORECPP_JSLOGGERPOLICYFILE_HPP_

#include "JavaScriptCoreCPP/JSLoggerPolicyInterface.hpp"
#include <fstream>

namespace JavaScriptCoreCPP {
  
  class JSLoggerPolicyFile final : public JSLoggerPolicyInterface {
  public:
    
    JSLoggerPolicyFile(const std::string& name) {
      ofstream__.open(name, std::ios_base::binary | std::ios_base::out);
      if(!ofstream__.is_open() ) {
        throw(std::runtime_error("JSLoggerPolicyFile: Unable to open an output stream"));
      }
    }
    
    ~JSLoggerPolicyFile() {
      ofstream__.close();
    }
    
    JSLoggerPolicyFile()                                     = delete;
    JSLoggerPolicyFile(const JSLoggerPolicyFile&)            = default;
    JSLoggerPolicyFile(JSLoggerPolicyFile&&)                 = default;
    
#ifdef JAVASCRIPTCORECPP_MOVE_CTOR_AND_ASSIGN_DEFAULT_ENABLE
    JSLoggerPolicyFile& operator=(const JSLoggerPolicyFile&) = default;
    JSLoggerPolicyFile& operator=(JSLoggerPolicyFile&&)      = default;
#endif
    
    virtual void Write(const std::string& log_message) override final {
      ofstream__ << log_message << std::endl;
      }
      
    private:
      
      std::ofstream ofstream__;
      };
      
      } // namespace JavaScriptCoreCPP {
      
#endif // _JAVASCRIPTCORECPP_JSLOGGERPOLICYFILE_HPP_
