/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/detail/JSLoggerPimpl.hpp"

#include <sstream>
#include <iomanip>
#include <ctime>
#include <limits>

namespace JavaScriptCoreCPP { namespace detail {

inline
std::string ToString(const std::chrono::system_clock::time_point& tp) {
	// Convert to system time.
  std::time_t t = std::chrono::system_clock::to_time_t(tp);
    
  // Convert to calendar time.
  //std::string ts = std::ctime(&t);
  std::string ts = std::asctime(std::localtime(&t));
  //std::string ts = std::asctime(std::gmtime(&t));
    
  // Strip trailing newline.
  ts.resize(ts.size() - 1);
    
  return ts;
}

std::string JSLoggerPimpl::GetLoglineHeader(uint32_t log_line_number) {
	std::ostringstream os;

	os.str("");
	os << std::setfill('0')
	   << setw(std::numeric_limits<uint32_t>::digits10)
	   << log_line_number
	   << " <"
	   << ToString(std::chrono::system_clock::now())
	   <<" - "
	   << setfill('0')
	   << setw(std::numeric_limits<std::clock_t>::digits10)
	   << std::clock()
	   <<" ";
	
	return os.str();
}

}} // namespace JavaScriptCoreCPP { namespace detail {
