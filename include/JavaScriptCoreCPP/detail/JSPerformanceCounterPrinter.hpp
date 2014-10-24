/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_DETAIL_JSPERFORMANCECOUNTERPRINTER_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_DETAIL_JSPERFORMANCECOUNTERPRINTER_HPP_

#ifdef JAVASCRIPTCORECPP_RAII_PERFORMANCE_COUNTER

#include "JavaScriptCoreCPP/RAII/RAII.hpp"
#include <iostream>

namespace JavaScriptCoreCPP { namespace detail {

class JSPerformanceCounterPrinter {
 public:

	~JSPerformanceCounterPrinter() {
		using namespace JavaScriptCoreCPP::RAII;
		
		static const std::string log_prefix { "MDL: JSPerformanceCounterPrinter: " };
		std::clog << log_prefix << std::endl;

		std::clog << std::endl;
		std::clog << "JSContextGroup:            objects_alive            = " << JSContextGroup::get_objects_alive()            << std::endl;
		std::clog << "JSContextGroup:            objects_created          = " << JSContextGroup::get_objects_created()          << std::endl;
		std::clog << "JSContextGroup:            objects_destroyed        = " << JSContextGroup::get_objects_destroyed()        << std::endl;
		std::clog << "JSContextGroup:            objects_copy_constructed = " << JSContextGroup::get_objects_copy_constructed() << std::endl;
		std::clog << "JSContextGroup:            objects_move_constructed = " << JSContextGroup::get_objects_move_constructed() << std::endl;
		std::clog << "JSContextGroup:            objects_copy_assigned    = " << JSContextGroup::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSContextGroup:            objects_move_assigned    = " << JSContextGroup::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSContext:                 objects_alive            = " << JSContext::get_objects_alive()            << std::endl;
		std::clog << "JSContext:                 objects_created          = " << JSContext::get_objects_created()          << std::endl;
		std::clog << "JSContext:                 objects_destroyed        = " << JSContext::get_objects_destroyed()        << std::endl;
		std::clog << "JSContext:                 objects_copy_constructed = " << JSContext::get_objects_copy_constructed() << std::endl;
		std::clog << "JSContext:                 objects_move_constructed = " << JSContext::get_objects_move_constructed() << std::endl;
		std::clog << "JSContext:                 objects_copy_assigned    = " << JSContext::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSContext:                 objects_move_assigned    = " << JSContext::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSString:                  objects_alive            = " << JSString::get_objects_alive()            << std::endl;
		std::clog << "JSString:                  objects_created          = " << JSString::get_objects_created()          << std::endl;
		std::clog << "JSString:                  objects_destroyed        = " << JSString::get_objects_destroyed()        << std::endl;
		std::clog << "JSString:                  objects_copy_constructed = " << JSString::get_objects_copy_constructed() << std::endl;
		std::clog << "JSString:                  objects_move_constructed = " << JSString::get_objects_move_constructed() << std::endl;
		std::clog << "JSString:                  objects_copy_assigned    = " << JSString::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSString:                  objects_move_assigned    = " << JSString::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSValue:                   objects_alive            = " << JSValue::get_objects_alive()            << std::endl;
		std::clog << "JSValue:                   objects_created          = " << JSValue::get_objects_created()          << std::endl;
		std::clog << "JSValue:                   objects_destroyed        = " << JSValue::get_objects_destroyed()        << std::endl;
		std::clog << "JSValue:                   objects_copy_constructed = " << JSValue::get_objects_copy_constructed() << std::endl;
		std::clog << "JSValue:                   objects_move_constructed = " << JSValue::get_objects_move_constructed() << std::endl;
		std::clog << "JSValue:                   objects_copy_assigned    = " << JSValue::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSValue:                   objects_move_assigned    = " << JSValue::get_objects_move_assigned()    << std::endl;

		/*
		std::clog << std::endl;
		std::clog << "JSUndefined:               objects_alive            = " << JSUndefined::get_objects_alive()            << std::endl;
		std::clog << "JSUndefined:               objects_created          = " << JSUndefined::get_objects_created()          << std::endl;
		std::clog << "JSUndefined:               objects_destroyed        = " << JSUndefined::get_objects_destroyed()        << std::endl;
		std::clog << "JSUndefined:               objects_copy_constructed = " << JSUndefined::get_objects_copy_constructed() << std::endl;
		std::clog << "JSUndefined:               objects_move_constructed = " << JSUndefined::get_objects_move_constructed() << std::endl;
		std::clog << "JSUndefined:               objects_copy_assigned    = " << JSUndefined::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSUndefined:               objects_move_assigned    = " << JSUndefined::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSNull:                    objects_alive            = " << JSNull::get_objects_alive()            << std::endl;
		std::clog << "JSNull:                    objects_created          = " << JSNull::get_objects_created()          << std::endl;
		std::clog << "JSNull:                    objects_destroyed        = " << JSNull::get_objects_destroyed()        << std::endl;
		std::clog << "JSNull:                    objects_copy_constructed = " << JSNull::get_objects_copy_constructed() << std::endl;
		std::clog << "JSNull:                    objects_move_constructed = " << JSNull::get_objects_move_constructed() << std::endl;
		std::clog << "JSNull:                    objects_copy_assigned    = " << JSNull::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSNull:                    objects_move_assigned    = " << JSNull::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSBoolean:                 objects_alive            = " << JSBoolean::get_objects_alive()            << std::endl;
		std::clog << "JSBoolean:                 objects_created          = " << JSBoolean::get_objects_created()          << std::endl;
		std::clog << "JSBoolean:                 objects_destroyed        = " << JSBoolean::get_objects_destroyed()        << std::endl;
		std::clog << "JSBoolean:                 objects_copy_constructed = " << JSBoolean::get_objects_copy_constructed() << std::endl;
		std::clog << "JSBoolean:                 objects_move_constructed = " << JSBoolean::get_objects_move_constructed() << std::endl;
		std::clog << "JSBoolean:                 objects_copy_assigned    = " << JSBoolean::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSBoolean:                 objects_move_assigned    = " << JSBoolean::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSNumber:                  objects_alive            = " << JSNumber::get_objects_alive()            << std::endl;
		std::clog << "JSNumber:                  objects_created          = " << JSNumber::get_objects_created()          << std::endl;
		std::clog << "JSNumber:                  objects_destroyed        = " << JSNumber::get_objects_destroyed()        << std::endl;
		std::clog << "JSNumber:                  objects_copy_constructed = " << JSNumber::get_objects_copy_constructed() << std::endl;
		std::clog << "JSNumber:                  objects_move_constructed = " << JSNumber::get_objects_move_constructed() << std::endl;
		std::clog << "JSNumber:                  objects_copy_assigned    = " << JSNumber::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSNumber:                  objects_move_assigned    = " << JSNumber::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSObject:                  objects_alive            = " << JSObject::get_objects_alive()            << std::endl;
		std::clog << "JSObject:                  objects_created          = " << JSObject::get_objects_created()          << std::endl;
		std::clog << "JSObject:                  objects_destroyed        = " << JSObject::get_objects_destroyed()        << std::endl;
		std::clog << "JSObject:                  objects_copy_constructed = " << JSObject::get_objects_copy_constructed() << std::endl;
		std::clog << "JSObject:                  objects_move_constructed = " << JSObject::get_objects_move_constructed() << std::endl;
		std::clog << "JSObject:                  objects_copy_assigned    = " << JSObject::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSObject:                  objects_move_assigned    = " << JSObject::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSArray:                   objects_alive            = " << JSArray::get_objects_alive()            << std::endl;
		std::clog << "JSArray:                   objects_created          = " << JSArray::get_objects_created()          << std::endl;
		std::clog << "JSArray:                   objects_destroyed        = " << JSArray::get_objects_destroyed()        << std::endl;
		std::clog << "JSArray:                   objects_copy_constructed = " << JSArray::get_objects_copy_constructed() << std::endl;
		std::clog << "JSArray:                   objects_move_constructed = " << JSArray::get_objects_move_constructed() << std::endl;
		std::clog << "JSArray:                   objects_copy_assigned    = " << JSArray::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSArray:                   objects_move_assigned    = " << JSArray::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSDate:                    objects_alive            = " << JSDate::get_objects_alive()            << std::endl;
		std::clog << "JSDate:                    objects_created          = " << JSDate::get_objects_created()          << std::endl;
		std::clog << "JSDate:                    objects_destroyed        = " << JSDate::get_objects_destroyed()        << std::endl;
		std::clog << "JSDate:                    objects_copy_constructed = " << JSDate::get_objects_copy_constructed() << std::endl;
		std::clog << "JSDate:                    objects_move_constructed = " << JSDate::get_objects_move_constructed() << std::endl;
		std::clog << "JSDate:                    objects_copy_assigned    = " << JSDate::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSDate:                    objects_move_assigned    = " << JSDate::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSError:                   objects_alive            = " << JSError::get_objects_alive()            << std::endl;
		std::clog << "JSError:                   objects_created          = " << JSError::get_objects_created()          << std::endl;
		std::clog << "JSError:                   objects_destroyed        = " << JSError::get_objects_destroyed()        << std::endl;
		std::clog << "JSError:                   objects_copy_constructed = " << JSError::get_objects_copy_constructed() << std::endl;
		std::clog << "JSError:                   objects_move_constructed = " << JSError::get_objects_move_constructed() << std::endl;
		std::clog << "JSError:                   objects_copy_assigned    = " << JSError::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSError:                   objects_move_assigned    = " << JSError::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSFunction:                objects_alive            = " << JSFunction::get_objects_alive()            << std::endl;
		std::clog << "JSFunction:                objects_created          = " << JSFunction::get_objects_created()          << std::endl;
		std::clog << "JSFunction:                objects_destroyed        = " << JSFunction::get_objects_destroyed()        << std::endl;
		std::clog << "JSFunction:                objects_copy_constructed = " << JSFunction::get_objects_copy_constructed() << std::endl;
		std::clog << "JSFunction:                objects_move_constructed = " << JSFunction::get_objects_move_constructed() << std::endl;
		std::clog << "JSFunction:                objects_copy_assigned    = " << JSFunction::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSFunction:                objects_move_assigned    = " << JSFunction::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSRegExp:                  objects_alive            = " << JSRegExp::get_objects_alive()            << std::endl;
		std::clog << "JSRegExp:                  objects_created          = " << JSRegExp::get_objects_created()          << std::endl;
		std::clog << "JSRegExp:                  objects_destroyed        = " << JSRegExp::get_objects_destroyed()        << std::endl;
		std::clog << "JSRegExp:                  objects_copy_constructed = " << JSRegExp::get_objects_copy_constructed() << std::endl;
		std::clog << "JSRegExp:                  objects_move_constructed = " << JSRegExp::get_objects_move_constructed() << std::endl;
		std::clog << "JSRegExp:                  objects_copy_assigned    = " << JSRegExp::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSRegExp:                  objects_move_assigned    = " << JSRegExp::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSClass:                   objects_alive            = " << JSClass::get_objects_alive()            << std::endl;
		std::clog << "JSClass:                   objects_created          = " << JSClass::get_objects_created()          << std::endl;
		std::clog << "JSClass:                   objects_destroyed        = " << JSClass::get_objects_destroyed()        << std::endl;
		std::clog << "JSClass:                   objects_copy_constructed = " << JSClass::get_objects_copy_constructed() << std::endl;
		std::clog << "JSClass:                   objects_move_constructed = " << JSClass::get_objects_move_constructed() << std::endl;
		std::clog << "JSClass:                   objects_copy_assigned    = " << JSClass::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSClass:                   objects_move_assigned    = " << JSClass::get_objects_move_assigned()    << std::endl;

		std::clog << std::endl;
		std::clog << "JSPropertyNameAccumulator: objects_alive            = " << JSPropertyNameAccumulator::get_objects_alive()            << std::endl;
		std::clog << "JSPropertyNameAccumulator: objects_created          = " << JSPropertyNameAccumulator::get_objects_created()          << std::endl;
		std::clog << "JSPropertyNameAccumulator: objects_destroyed        = " << JSPropertyNameAccumulator::get_objects_destroyed()        << std::endl;
		std::clog << "JSPropertyNameAccumulator: objects_copy_constructed = " << JSPropertyNameAccumulator::get_objects_copy_constructed() << std::endl;
		std::clog << "JSPropertyNameAccumulator: objects_move_constructed = " << JSPropertyNameAccumulator::get_objects_move_constructed() << std::endl;
		std::clog << "JSPropertyNameAccumulator: objects_copy_assigned    = " << JSPropertyNameAccumulator::get_objects_copy_assigned()    << std::endl;
		std::clog << "JSPropertyNameAccumulator: objects_move_assigned    = " << JSPropertyNameAccumulator::get_objects_move_assigned()    << std::endl;
		*/
	}

	JSPerformanceCounterPrinter()                                                  = default;
	JSPerformanceCounterPrinter(const JSPerformanceCounterPrinter& rhs)            = delete;
	JSPerformanceCounterPrinter(JSPerformanceCounterPrinter&& rhs)                 = delete;
	JSPerformanceCounterPrinter& operator=(const JSPerformanceCounterPrinter& rhs) = delete;
	JSPerformanceCounterPrinter& operator=(JSPerformanceCounterPrinter&& rhs)      = delete;
};

}} // namespace JavaScriptCoreCPP { namespace detail {


#endif  // JAVASCRIPTCORECPP_RAII_PERFORMANCE_COUNTER

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_DETAIL_JSPERFORMANCECOUNTERPRINTER_HPP_
