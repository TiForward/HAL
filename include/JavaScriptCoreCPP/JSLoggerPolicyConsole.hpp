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
#include <iostream>

namespace JavaScriptCoreCPP {

class JSLoggerPolicyConsole final : public JSLogPolicyInterface {
 public:
  
	JSLoggerPolicyConsole(const std::string& name) {
	}
	
	JSLoggerPolicyConsole() = delete;
	~JSLoggerPolicyConsole() = default;
	
	JSLoggerPolicyConsole(const JSLoggerPolicyConsole& rhs) = default;
	JSLoggerPolicyConsole(JSLoggerPolicyConsole&& rhs) = default;
	
	JSLoggerPolicyConsole& operator=(const JSLoggerPolicyConsole&) = default;
	JSLoggerPolicyConsole& operator=(JSLoggerPolicyConsole&&) = default;
	
	virtual void Write(const std::string& log_message) override final {
		std::clog << log_message << std::endl;
	}
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSLOGGERPOLICYFILE_HPP_
