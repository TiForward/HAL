/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSLOGGERPOLICYINTERFACE_HPP_
#define _JAVASCRIPTCORECPP_JSLOGGERPOLICYINTERFACE_HPP_

namespace JavaScriptCoreCPP {

class JSLoggerPolicyInterface {
 public:
  
	JSLoggerPolicyInterface() = default;
	virtual ~JSLoggerPolicyInterface() = default;
	
	JSLoggerPolicyInterface(const JSLoggerPolicyInterface& rhs) = default;
	JSLoggerPolicyInterface(JSLoggerPolicyInterface&& rhs) = default;
	
	JSLoggerPolicyInterface& operator=(const JSLoggerPolicyInterface&) = default;
	JSLoggerPolicyInterface& operator=(JSLoggerPolicyInterface&&) = default;
	
	virtual void Write(const std::string& log_message) = 0;
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSLOGGERPOLICYINTERFACE_HPP_
