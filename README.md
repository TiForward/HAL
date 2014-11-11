# JavaScriptCoreCPP [![Build Status](https://magnum.travis-ci.com/appcelerator/titanium_mobile_windows.svg?token=SxTZxbWRYYpcfE9jALXb&branch=master)](https://magnum.travis-ci.com/appcelerator/titanium_mobile_windows)

JavaScriptCoreCPP: C++11 wrapper around the JavaScriptCore C API.

## Quick Start

Run our 237 (and counting) unit tests.

On Windows:

```bash
pushd Source/JavaScriptCoreCPP/
build_and_test_windows.sh
```

On OSX:

```bash
pushd Source/JavaScriptCoreCPP/
build_and_test_osx.sh
```

Here is [EvaluateScript.cpp](examples/EvaluateScript.cpp), a simple main program that evaluates the JavaScript expression `21 / 7` and prints `3` to the terminal. To run it on Windows type `./build.debug/examples/EvaluateScript.exe` and to run it on OS X type `./build.debug/examples/EvaluateScript`.
```
#include "JavaScriptCoreCPP/JavaScriptCoreCPP.hpp"
#include <iostream>

int main () {
	using namespace JavaScriptCoreCPP;
	JSContextGroup js_context_group;
	JSContext js_context = js_context_group.CreateContext();
	auto js_result       = js_context.JSEvaluateScript("21 / 7");
	std::cout << static_cast<int32_t>(js_result) << std::endl;
}
```

To seamlessly add a C++ class to the JavaScriptCore runtime take a look at our Widget example in [Widget.hpp](examples/Widget.hpp) and [Widget.cpp](examples/Widget.cpp). This will show you how to expose a C++ class to JavaScriptCore.

Running it on Windows (type `./build.debug/examples/WidgetMain.exe`) or OS X (type `./build.debug/examples/WidgetMain`) produces this output.
```
JavaScriptCoreCPP (JavaScriptCore_RAII)$ ./build.debug/examples/WidgetMain
Widget is an object
Widget.name; // outputs 'world'
Widget.number; // outputs '42'
Widget.sayHello(); // outputs 'Hello, world. Your number is 42.'
Widget.name = 'foo'; Widget.name; // outputs 'foo'
Widget.number = 3*7; Widget.number; // outputs '21'
Widget.sayHello(); // outputs 'Hello, foo. Your number is 21.'
```

## Description

This is JavaScriptCoreCPP, a C++11 library that wraps the JavaScriptCore C API. This will be the foundation for building Titanium on Windows and perhaps other Appcelerator products.

The design and implementation of this library has currently gone through several experimental iterations by only one senior Appcelerator software engineer, and it is critical that it be critically peer reviewed by Appcelerator business owners, Appcelerator's senior technical staff, and the entire Appcelerator engineering staff as a whole. There are sure to be bugs and room for improvement, and all empirical critical feedback is essential and welcome for the library's success.

The API documentation is thorough and complete, but there is currently no introductory or tutorial material other than this README and one example. The introductory and tutorial material will come as prioritized by our chain of command.

This library was initially created using both iOS 8 (i.e. Apple LLVM version 6.0, aka clang-600.0.54 based on LLVM 3.5svn) and Windows Phone 8.1 (i.e Microsoft's Visual C++ 18.0.30723.0 from Visual Studio 2013 Update 3 RTM) as reference platforms. However, as stated in goal number 10 of the library (see below), it was designed to work on any platform that has a standards compliant C++11 compiler and library.

To use the library you will always want to include the main header:

`#include "JavaScriptCoreCPP/JavaScriptCoreCPP.hpp"`

These are the main C++ classes in the library and their inheritance hierarchy:

### JSContextGroup and JSContext 

A [JSContext](include/JavaScriptCoreCPP/JSContext.hpp) is an RAII wrapper around a [JSContextRef](https://github.com/WebKit/webkit/blob/master/Source/JavaScriptCore/API/JSContext.h), the JavaScriptCore C API representation of a JavaScript execution context that holds the global object and other execution state.

JSContexts are created by the [JSContextGroup](include/JavaScriptCoreCPP/JSContextGroup.hpp) `CreateContext` member function. JSContextGroups may be created with either the default or custom global objects. See the individual JSContextGroup constructors for more details.
   
JavaScript scripts may execute concurrently with scripts executing in other JSContexts, and JSContexts within the same JSContextGroup may share and exchange their JSValues anad JSObjects with one another.
   
A JSContext is the only way to create a [JSValue](include/JavaScriptCoreCPP/JSValue.hpp)s and [JSObject](include/JavaScriptCoreCPP/JSObject.hpp)s.

### JSValue

A [JSValue](include/JavaScriptCoreCPP/JSValue.hpp) is an RAII wrapper around a [JSValueRef](https://github.com/WebKit/webkit/blob/master/Source/JavaScriptCore/API/JSValueRef.h), the JavaScriptCore C API representation of a JavaScript value. This is the base class for all JavaScript values in the library. These are the direct descendants of JSValue:
1. [JSUndefined](include/JavaScriptCoreCPP/JSUndefined.hpp)
2. [JSNull](include/JavaScriptCoreCPP/JSNull.hpp)
3. [JSBoolean](include/JavaScriptCoreCPP/JSBoolean.hpp)
4. [JSNumber](include/JavaScriptCoreCPP/JSNumber.hpp)

Although not a JSValue, the [JSString](include/JavaScriptCoreCPP/JSString.hpp) class is used throughout the library, and seamlessly bridges JavaScriptCore's strings and C++ strings. A JSString is an RAII wrapper around a [JSStringRef](https://github.com/WebKit/webkit/blob/master/Source/JavaScriptCore/API/JSStringRef.h), the JavaScriptCore C API representation of a JavaScript string.
   
A JSString satisfies satisfies all the requirements for use in all STL containers. For example, a JSString can be used as a key in a [std::unordered_map](http://en.cppreference.com/w/cpp/container/unordered_map). Specifically, a JSString is comparable with an equivalence relation, provides a strict weak ordering, and provides a custom hash function.

### JSObject

A [JSObject](include/JavaScriptCoreCPP/JSObject.hpp) is an RAII wrapper around a [JSObjectRef](https://github.com/WebKit/webkit/blob/master/Source/JavaScriptCore/API/JSObjectRef.h), the JavaScriptCore C API representation of a JavaScript object. This is the base class for all JavaScript objects in the library. These are JSObject's direct descendants:
   
1. [JSArray](include/JavaScriptCoreCPP/JSArray.hpp)
2. [JSDate](include/JavaScriptCoreCPP/JSDate.hpp)
3. [JSError](include/JavaScriptCoreCPP/JSError.hpp)
4. [JSRegExp](include/JavaScriptCoreCPP/JSRegExp.hpp)
5. [JSFunction](include/JavaScriptCoreCPP/JSFunction.hpp)

## JSExport

[JSExport](include/JavaScriptCoreCPP/JSExport.hpp) is a CRTP (i.e. Curiously Recurring Template Pattern) base class that allows any C++ class derived from it to be seamlessly integrated into JavaScriptCore. This is what we used to create out Widget example in [Widget.hpp](examples/Widget.hpp) and [Widget.cpp](examples/Widget.cpp). JSExport is fully documented and the Widget example is what you want to study to learn how to seamlessly integrate your own C++ classes into the JavaScriptCore runtime execution environment.


## Project Goals

1. The library is RAII through-and-through.
2. The interaction between the JavaScript and C++ languages is as seamless as possible.
3. The library is difficult (if not impossible) to use incorrectly. It exposes none of JavaScriptCore C API.
4. The library is high-performance and leaves the majority of the CPU and heap memory for use by the user's application. Although each library component has a large functional API, internally every library component  takes up the memory space of only one or two integers. This allows the library to live entirely on the stack, which leaves heap memory (i.e. the vast majority of the mobile device's memory) dedicated to running the user's application. The cost of the JavaScriptCore bridge has been measured to be less than 1% (i.e. despite tall tales of Appcelerator lore to the contrary that "the JavaScriptCore C API bridge is slow" - empirically it is not) which also leaves the majority of the CPU to run the user's application.
5. The library is 100% thread-safe. What this means practically is that we have successfully run hundreds of JavaScriptCore contexts simultaneously in multiple "context groups" among multiple threads of execution without any issues. We have shown empirically that JavaScript values and objects may be safely shared and exchanged between multiple JavaScriptCore contexts that share the same "context group". The executive summary is that the javaScriptCoreCPP library offers a truly thread-safe asynchronous computation engine for the JavaScript language.
6. The library uses only Apple's public JavaScriptCore C API. It does not depend on anything that is not in Apple's public API (i.e. no internal implementation details are used or we're referenced in the design).
7. Performance counters are built-in for testability.
8. The library's public API is 100% thoroughly documented.
9. The library comes with a suite of unit tests that covers a minimum of 80% of the API.
10. The library uses only C++11. What this means practically is that it uses nothing platform specific. This allows the JavaScriptCoreCPP library to be used on the largest number of devices possible (e.g. iOS, Android, Windows Phone, etc.).

## Lessons Learned and Future Direction

Here are some valuable lessons we have learned during the development of the JavaScriptCoreCPP library and possible areas of future exploration and R&D:

1. The JavaScriptCoreCPP library expose 100% of the JavaScript AST object model when used in 'strict' mode. What this means practically is that in principle a JavaScript compiler can compile JavaScript source code directly to C++ without the need of any "JavaScript Engine" for interpretation. We suggest that this is a useful avenue of Appcelerator R&D.
2. The library offers the capability of a pure C++ implementation of Ti.Next, a pure JavaScript implementation to TI.next, or a hybrid of the two. We suggest that this is a useful avenue of Appcelerator R&D to determine the correct mix.
3. The JavaScriptCoreCPP library is a practical backend target for the Hyperloop compiler. Again, we suggest that this is a useful avenue of Appcelerator R&D.

## One Last Note

Please do not concern yourself (yet) with the coding convention you see in the source code. It will be the decision of the business owners of this library to direct that effort once the design and API are locked, which is now the case. For example, if the business owners decide that this library should be upstreamed to the WebKit open source project (so that Appcelerator does not have to maintain it), then the library must follow Apple's coding conventions. If instead the business owners decide that this library is an independent Appcelerator product (open sourced or not) then it may be refactored to follow a different coding convention.


## Contributors

The original source code and design for this project was developed by [Matt Langston](https://github.com/matt-langston).
