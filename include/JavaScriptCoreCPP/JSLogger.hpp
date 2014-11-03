/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSLOGGER_HPP_
#define _JAVASCRIPTCORECPP_JSLOGGER_HPP_

#include "JavaScriptCoreCPP/detail/JSLoggerPimpl.hpp"
#include "JavaScriptCoreCPP/JSLoggerPolicyConsole.hpp"
#include "JavaScriptCoreCPP/JSLoggerPolicyFile.hpp"
#include <sstream>
#include <cstdint>
#include <mutex>

namespace JavaScriptCoreCPP {

// The JSLogger infrastructure was inspired by this Dr. Dobbs article:
//
// A Lightweight Logger for C++
// By Filip Janiszewski, January 31, 2013
// http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505

/*!
  @class

  @abstract The logging infrastructure used by the JavaScriptCoreCPP
  library.

  @discussion Here is an example of how to use the JavaScriptCoreCPP
  logging facility:
  
  JAVASCRIPTCORECPP_LOG_DEBUG("Before loop.");
  for(short i = 0 ; i < 3 ; ++i)
  {
    JAVASCRIPTCORECPP_LOG_DEBUG("The value of 'i' is ", i , ". " , 3 - i - 1 , " more iterations left ");
  }
  JAVASCRIPTCORECPP_LOG_WARN("After loop.");
  JAVASCRIPTCORECPP_LOG_ERROR("All good things come to an end.");
 */
template<typename JSLogPolicy>
class JSLogger final {
  
 public:
  
  enum class SeverityType {
    DEBUG,
	  INFO,
    WARN,
    ERROR
  };

  JSLogger(const std::string& name) 
		  : js_log_policy__(name) {
  }
  
  JSLogger() = delete;
  ~JSLogger() = default;
  
  JSLogger(const JSLogger& rhs) = default;
  JSLogger(JSLogger&& rhs) = default;
  
  JSLogger& operator=(const JSLogger&) = default;
  JSLogger& operator=(JSLogger&&) = default;
  
 private:
  
  template<SeverityType severity, typename...Args>
  void Print(Args...args);
  
  // Core printing functionality.
  void PrintImpl();
  
  template<typename First, typename...Rest>
  void PrintImpl(First first_parameter, Rest...rest);

  JSLogPolicy        js_log_policy__;
  uint32_t           log_line_number__ { 0 };
  std::ostringstream log_stream__;
  std::mutex         js_logger_mutex__;
};

template<typename JSLogPolicy>
template<JSLogger::SeverityType severity, typename...Args>
void JSLogger<JSLogPolicy>::Print(Args...args)  {
  std::lock_guard<std::mutex>(js_logger_mutex__);

  // The Debug and Error severity strings (i.e. "DEBUG" and "ERROR")
  // are the longest of the three severity strings, and each is 5
  // characters long. Since we want all of the severity types to have
  // the same width on output, we set it to 5.
  log_stream__ << std::setw(5) << std::left;
  
  switch(severity) {
    case SeverityType::DEBUG:
      log_stream__ << "DEBUG";
      break;
    case SeverityType::INFO:
      log_stream__ << "INFO";
      break;
    case SeverityType::ERROR:
      log_stream__ << "ERROR";
      break;
    case SeverityType::WARN:
      log_stream__ << "WARN";
      break;
  };
  
  PrintImpl(args...);
}

template<typename JSLogPolicy>
void JSLogger<JSLogPolicy>::PrintImpl() {
  js_log_policy__.Write(JSLoggerPimpl::GetLoglineHeader(log_line_number__++) + ":" + log_stream__.str());
  log_stream__.str("");
}

template<typename JSLogPolicy>
template<typename First, typename...Rest >
void JSLogger<JSLogPolicy>::PrintImpl(First first_parameter, Rest...rest) {
  log_stream__ << first_parameter;
  PrintImpl(rest...); 
}

#ifdef JAVASCRIPTCORECPP_LOGGING

// TODO: Add a more flexible way to specify the logging policy.

//static JSLogger<JSLoggerPolicyFile>    js_logger_instance("JavaScriptCoreCPP.log");
static JSLogger<JSLoggerPolicyConsole> js_logger_instance("JavaScriptCoreCPP.log");

#define JAVASCRIPTCORECPP_LOG_DEBUG js_logger_instance.Print<JSLogger::Severitytype::DEBUG>
#define JAVASCRIPTCORECPP_LOG_INFO  js_logger_instance.Print<JSLogger::Severitytype::INFO>
#define JAVASCRIPTCORECPP_LOG_WARN  js_logger_instance.Print<JSLogger::Severitytype::WARN>
#define JAVASCRIPTCORECPP_LOG_ERROR js_logger_instance.Print<JSLogger::Severitytype::ERROR>
#else
#define JAVASCRIPTCORECPP_LOG_DEBUG(...) 
#define JAVASCRIPTCORECPP_LOG_INFO(...)
#define JAVASCRIPTCORECPP_LOG_WARN(...)
#define JAVASCRIPTCORECPP_LOG_ERROR(...)
#endif

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSLOGGER_HPP_
