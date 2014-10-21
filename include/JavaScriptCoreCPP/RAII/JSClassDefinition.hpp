/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITION_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITION_HPP_

#include <functional>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

enum class ClassAttributes {
	None                 = kJSClassAttributeNone,
	NoAutomaticPrototype = kJSClassAttributeNoAutomaticPrototype
};

class JSClassDefinition final	{
	
 public:

	JSClassDefinition(const JSClassDefinition* js_class_definition)
			: js_class_definition_(*js_class_definition)
			, class_name_(js_class_definition -> className) {

    // int                                 version;
    // JSClassAttributes                   attributes;

    // const char*                         className;
    // JSClassRef                          parentClass;
        
    // const JSStaticValue*                staticValues;
    // const JSStaticFunction*             staticFunctions;
    
    // JSObjectInitializeCallback          initialize;
    // JSObjectFinalizeCallback            finalize;
    // JSObjectHasPropertyCallback         hasProperty;
    // JSObjectGetPropertyCallback         getProperty;
    // JSObjectSetPropertyCallback         setProperty;
    // JSObjectDeletePropertyCallback      deleteProperty;
    // JSObjectGetPropertyNamesCallback    getPropertyNames;
    // JSObjectCallAsFunctionCallback      callAsFunction;
    // JSObjectCallAsConstructorCallback   callAsConstructor;
    // JSObjectHasInstanceCallback         hasInstance;
    // JSObjectConvertToTypeCallback       convertToType;
	}
	
	~JSClassDefinition() {
	}
	
	// Copy constructor.
	JSClassDefinition(const JSClassDefinition& rhs) {
	}
	
	// Create a copy of another JSClassDefinition by assignment.
	JSClassDefinition& operator=(const JSClassDefinition& rhs) {
		if (this == &rhs) {
			return *this;
		}
		
		return *this;
	}
	
	operator JSClassDefinition() const {
		return js_object_;
	}
	
 private:

	// Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects

	JSClassDefinition             js_class_definition_;
	std::string                   class_name_;
	std::vector<JSStaticValue>    static_values_;
	std::vector<JSStaticFunction> static_functions_;
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASSDEFINITION_HPP_
