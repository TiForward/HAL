# Hyperloop Abstraction Layer (HAL) [![Build Status](https://magnum.travis-ci.com/appcelerator/HAL.svg?token=SxTZxbWRYYpcfE9jALXb)](https://magnum.travis-ci.com/appcelerator/HAL)

The Hyperloop Abstraction Layer (HAL) library is a cross-platform standards-compliant C++11 library wrapper around the JavaScriptCore C API.

## Prerequisites

### OS X

Step 1. Install Xcode 6

Step 2. Install cmake version 3.0 (or higher) using either `sudo port install cmake` or `brew unlink cmake; brew install cmake`.

Step 3. Download our pre-compiled version of Google Test [gtest-1.7.0-osx.zip (3 MB)](http://timobile.appcelerator.com.s3.amazonaws.com/gtest-1.7.0-osx.zip), unzip anywhere (the location doesn't matter) and set the environment variable GTEST_ROOT to where you unzipped it.

Just run the following commands from your bash prompt to setup your development environment for Google Test before proceeding:

```bash
$ curl -O http://timobile.appcelerator.com.s3.amazonaws.com/gtest-1.7.0-osx.zip
$ unzip gtest-1.7.0-osx.zip
```

### Windows

Step 1. Install Visual Studio 2013

Step 2. Install this version of [CMake](https://cmakems.codeplex.com/releases/view/126914) in order to generate VS 2013 project files.

Step 3. Install [64-bit Cygwin](http://cygwin.com/setup-x86_64.exe).

* Choose the mirror http://mirrors.kernel.org which we have measured is at least 100x faster than the default selected mirror.
* Select both ```curl``` and ```unzip``` in addition to the defaults.

Step 4. Install JavascriptCore

Download our pre-compiled version of JavascriptCore [JavaScriptCore-Windows-1411436814.zip (276 MB)](http://timobile.appcelerator.com.s3.amazonaws.com/jscore/JavaScriptCore-Windows-1411436814.zip), unzip anywhere (the location doesn't matter) and set the environment variable JavaScriptCore_HOME to where you unzipped it.

Just run the following commands from your Cygwin bash prompt to setup your development environment for JavaScriptCore_HOME before
proceeding:

```bash
$ curl -O http://timobile.appcelerator.com.s3.amazonaws.com/jscore/JavaScriptCore-Windows-1411436814.zip
$ unzip JavaScriptCore-Windows-1411436814.zip
```

step 5. Install Google Test

Download our pre-compiled version of Google Test [gtest-1.7.0-windows.zip (3 MB)](http://timobile.appcelerator.com.s3.amazonaws.com/gtest-1.7.0-windows.zip), unzip anywhere (the location doesn't matter) and set the environment variable GTEST_ROOT to where you unzipped it.

Just run the following commands from your Cygwin bash prompt to setup your development environment for Google Test before proceeding:

```bash
$ curl -O http://timobile.appcelerator.com.s3.amazonaws.com/gtest-1.7.0-windows.zip
$ unzip gtest-1.7.0-windows.zip
```

## Quick Start

To run our unit tests on both both OS X and Windows:

```bash
build_and_test.sh
```

Here is [EvaluateScript.cpp](examples/EvaluateScript.cpp), a simple main program that evaluates the JavaScript expression `21 / 7` and prints `3` to the terminal. To run it on Windows type `./build.debug/examples/EvaluateScript.exe` and to run it on OS X type `./build.debug/examples/EvaluateScript`.
```
#include "HAL/HAL.hpp"
#include <iostream>

int main () {
	using namespace HAL;
	JSContextGroup js_context_group;
	JSContext js_context = js_context_group.CreateContext();
	auto js_result       = js_context.JSEvaluateScript("21 / 7");
	std::cout << static_cast<int32_t>(js_result) << std::endl;
}
```

To add a C++ class to the JavaScriptCore runtime take a look at our Widget example in [Widget.hpp](examples/Widget.hpp) and [Widget.cpp](examples/Widget.cpp). This will show you how to expose a C++ class to JavaScriptCore.

Running it on Windows (type `./build.debug/examples/WidgetMain.exe`) or OS X (type `./build.debug/examples/WidgetMain`) produces this output.
```
Widget is an object
Widget.name; // outputs 'world'
Widget.number; // outputs '42'
Widget.sayHello(); // outputs 'Hello, world. Your number is 42.'
Widget.name = 'foo'; Widget.name; // outputs 'foo'
Widget.number = 3*7; Widget.number; // outputs '21'
Widget.sayHello(); // outputs 'Hello, foo. Your number is 21.'
```

## Description

This is HAL, a cross-platform standards-compliant C++11 library that wraps the JavaScriptCore C API.

This library was initially created using both iOS 8 (i.e. Apple LLVM version 6.0, aka clang-600.0.54 based on LLVM 3.5svn) and Windows Phone 8.1 (i.e Microsoft's Visual C++ 18.0.30723.0 from Visual Studio 2013 Update 3 RTM) as reference platforms. However, it was designed to work on any platform that has a standards compliant C++11 compiler and library.

To use the library you will always want to include the main header:

`#include "HAL/HAL.hpp"`

These are the main C++ classes in the library and their inheritance hierarchy:

### JSContextGroup and JSContext 

A [JSContext](include/HAL/JSContext.hpp) is an RAII wrapper around a [JSContextRef](https://github.com/WebKit/webkit/blob/master/Source/JavaScriptCore/API/JSContext.h), the JavaScriptCore C API representation of a JavaScript execution context that holds the global object and other execution state.

JSContexts are created by the [JSContextGroup](include/HAL/JSContextGroup.hpp) `CreateContext` member function. JSContextGroups may be created with either the default or custom global objects. See the individual JSContextGroup constructors for more details.
   
JavaScript scripts may execute concurrently with scripts executing in other JSContexts, and JSContexts within the same JSContextGroup may share and exchange their JSValues anad JSObjects with one another.
   
A JSContext is the only way to create a [JSValue](include/HAL/JSValue.hpp) and a [JSObject](include/HAL/JSObject.hpp).

### JSValue

A [JSValue](include/HAL/JSValue.hpp) is an RAII wrapper around a [JSValueRef](https://github.com/WebKit/webkit/blob/master/Source/JavaScriptCore/API/JSValueRef.h), the JavaScriptCore C API representation of a JavaScript value. This is the base class for all JavaScript values in the library. These are the direct descendants of JSValue:

1. [JSUndefined](include/HAL/JSUndefined.hpp)
2. [JSNull](include/HAL/JSNull.hpp)
3. [JSBoolean](include/HAL/JSBoolean.hpp)
4. [JSNumber](include/HAL/JSNumber.hpp)

Although not a JSValue, the [JSString](include/HAL/JSString.hpp) class is used throughout the library, and seamlessly bridges JavaScriptCore's strings and C++ strings. A JSString is an RAII wrapper around a [JSStringRef](https://github.com/WebKit/webkit/blob/master/Source/JavaScriptCore/API/JSStringRef.h), the JavaScriptCore C API representation of a JavaScript string.
   
A JSString satisfies satisfies all the requirements for use in all STL containers. For example, a JSString can be used as a key in a [std::unordered_map](http://en.cppreference.com/w/cpp/container/unordered_map). Specifically, a JSString is comparable with an equivalence relation, provides a strict weak ordering, and provides a custom hash function.

### JSObject

A [JSObject](include/HAL/JSObject.hpp) is an RAII wrapper around a [JSObjectRef](https://github.com/WebKit/webkit/blob/master/Source/JavaScriptCore/API/JSObjectRef.h), the JavaScriptCore C API representation of a JavaScript object. This is the base class for all JavaScript objects in the library. These are JSObject's direct descendants:
   
1. [JSArray](include/HAL/JSArray.hpp)
2. [JSDate](include/HAL/JSDate.hpp)
3. [JSError](include/HAL/JSError.hpp)
4. [JSRegExp](include/HAL/JSRegExp.hpp)
5. [JSFunction](include/HAL/JSFunction.hpp)

## JSExport

[JSExport](include/HAL/JSExport.hpp) is a CRTP (i.e. Curiously Recurring Template Pattern) base class that allows any C++ class derived from it to be seamlessly integrated into JavaScriptCore. This is what we used to create our Widget example in [Widget.hpp](examples/Widget.hpp) and [Widget.cpp](examples/Widget.cpp). JSExport is fully documented and the Widget example is what you want to study to learn how to seamlessly integrate your own C++ classes into the JavaScriptCore runtime execution environment.


## Project Goals

1. The library is RAII through-and-through.
2. The interaction between the JavaScript and C++ languages is as seamless as possible.
3. The library is as pleasant to use for the C++ developer as Apple's JavaScriptCore Objective-C API is to the Objective-C/Swift developer.
4. The library is high-performance and leaves the majority of the CPU and heap memory for use by the user's application. Although each library component has a large functional API, internally every library component  takes up the memory space of only one or two integers.
5. The library is thread-safe.
6. The library uses only Apple's public JavaScriptCore C API, making it "App Store Compliant".
7. Performance counters are built-in for testability.
8. The library's public API is thoroughly documented.
9. The library comes with a suite of unit tests.
10. The library uses only C++11. What this means practically is that it uses nothing platform specific. This allows the HAL library to be used on the largest number of devices possible (e.g. iOS, Android, Windows Phone, etc.).

## Contributors

The original source code and design for this project was developed by [Matt Langston](https://github.com/matt-langston).

## Legal

Appcelerator is a registered trademark of Appcelerator, Inc. Titanium is a registered trademark of Appcelerator, Inc.  Please see the LICENSE file included with this distribution for information about using our trademarks, privacy policy, terms of usage and other legal information at [http://www.appcelerator.com/legal](http://www.appcelerator.com/legal).
