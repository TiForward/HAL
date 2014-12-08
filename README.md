# Hyperloop Abstraction Layer (HAL) [![Build Status](https://magnum.travis-ci.com/appcelerator/HAL.svg?token=SxTZxbWRYYpcfE9jALXb)](https://magnum.travis-ci.com/appcelerator/HAL)

The Hyperloop Abstraction Layer (HAL) library is a C++11 wrapper around the JavaScriptCore C API.

## Prerequisites

### OS X

Step 1. Install Xcode 6

Step 2. Install cmake version 3.0 (or higher) using either `sudo port install cmake` or `brew unlink cmake; brew install cmake`.

Step 3. Install boost using either `sudo port install boost` or `brew install boost`.

Step 4. Download our pre-compiled version of Google Test [gtest-1.7.0-osx.zip (3 MB)](http://timobile.appcelerator.com.s3.amazonaws.com/gtest-1.7.0-osx.zip), unzip anywhere (the location doesn't matter) and set the environment variable GTEST_ROOT to where you unzipped it.

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

Step 6. Install Boost

Microsoft officially supports and encourages the use of the Boost C++ open source library for Windows Store and Windows Phone apps, and they have contributed a great deal of open source code to the Boost project. Please read the Microsoft article [Using Boost Libraries in Windows Store and Phone Applications](http://blogs.msdn.com/b/vcblog/archive/2014/07/18/using-boost-libraries-in-windows-store-and-phone-applications.aspx) and follow their instructions.

Microsoft instructs us to create the folders named WPSDK\WP81 under the VC Visual Studio installation directory, `C:\Program Files
(x86)\Microsoft Visual Studio 12.0\VC\`, and copy their [phone setup script files](http://blogs.msdn.com/cfs-file.ashx/__key/communityserver-components-postattachments/00-10-54-33-32/BoostSample-.zip) to it. This has been verified to work well with our CMake build infrastructure.

After following Microsoft's instructions, define the environment variable `BOOST_ROOT` to point to where you cloned the boost git repository and add it to your `PATH`. If you are using PowerShell something like the following works:
```
setx.exe BOOST_ROOT $env:HOME\Documents\GitHub\boost
$env:Path += ";$env:BOOST_ROOT";
cd $env:BOOST_ROOT\libs\thread\build
b2 toolset=msvc-12.0 link=static windows-api=store
b2 toolset=msvc-12.0 link=static windows-api=phone
```

## Quick Start

To run our 255 (and counting) unit tests on both both OS X and Windows:

```bash
build_and_test.sh
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
Widget is an object
Widget.name; // outputs 'world'
Widget.number; // outputs '42'
Widget.sayHello(); // outputs 'Hello, world. Your number is 42.'
Widget.name = 'foo'; Widget.name; // outputs 'foo'
Widget.number = 3*7; Widget.number; // outputs '21'
Widget.sayHello(); // outputs 'Hello, foo. Your number is 21.'
```

## Description

This is HAL, a cross-platform standards-compliant C++11 library that wraps the JavaScriptCore C API. This will be the foundation for building Titanium on Windows and perhaps other Appcelerator products.

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
   
A JSContext is the only way to create a [JSValue](include/JavaScriptCoreCPP/JSValue.hpp) and a [JSObject](include/JavaScriptCoreCPP/JSObject.hpp).

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

[JSExport](include/JavaScriptCoreCPP/JSExport.hpp) is a CRTP (i.e. Curiously Recurring Template Pattern) base class that allows any C++ class derived from it to be seamlessly integrated into JavaScriptCore. This is what we used to create our Widget example in [Widget.hpp](examples/Widget.hpp) and [Widget.cpp](examples/Widget.cpp). JSExport is fully documented and the Widget example is what you want to study to learn how to seamlessly integrate your own C++ classes into the JavaScriptCore runtime execution environment.


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
10. The library uses only C++11. What this means practically is that it uses nothing platform specific. This allows the HAL library to be used on the largest number of devices possible (e.g. iOS, Android, Windows Phone, etc.).

## Lessons Learned and Future Direction

Here are some valuable lessons we have learned during the development of the HAL library and possible areas of future exploration and R&D:

1. The HAL library expose 100% of the JavaScript AST object model when used in 'strict' mode. What this means practically is that in principle a JavaScript compiler can compile JavaScript source code directly to C++ without the need of any "JavaScript Engine" for interpretation. We suggest that this is a useful avenue of Appcelerator R&D.
2. The library offers the capability of a pure C++ implementation of Ti.Next, a pure JavaScript implementation of Ti.next, or a hybrid of the two. We suggest that this is a useful avenue of Appcelerator R&D to determine the correct mix.
3. The HAL library is a practical backend target for the Hyperloop compiler. Again, we suggest that this is a useful avenue of Appcelerator R&D.

## One Last Note

Please do not concern yourself (yet) with the coding convention you see in the source code. It will be the decision of the business owners of this library to direct that effort once the design and API are locked, which is now the case. For example, if the business owners decide that this library should be upstreamed to the WebKit open source project (so that Appcelerator does not have to maintain it), then the library must follow Apple's coding conventions. If instead the business owners decide that this library is an independent Appcelerator product (open sourced or not) then it may be refactored to follow a different coding convention.


## Contributors

The original source code and design for this project was developed by [Matt Langston](https://github.com/matt-langston).
