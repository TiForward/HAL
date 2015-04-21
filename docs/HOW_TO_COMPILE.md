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

Here is [EvaluateScript.cpp](../examples/EvaluateScript.cpp), a simple main program that evaluates the JavaScript expression `21 / 7` and prints `3` to the terminal. To run it on Windows type `./build.debug/examples/EvaluateScript.exe` and to run it on OS X type `./build.debug/examples/EvaluateScript`.

```c++
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

To add a C++ class to the JavaScriptCore runtime take a look at our Widget example in [Widget.hpp](../examples/Widget.hpp) and [Widget.cpp](../examples/Widget.cpp). This will show you how to expose a C++ class to JavaScriptCore.

Running it on Windows (type `./build.debug/examples/WidgetMain.exe`) or OS X (type `./build.debug/examples/WidgetMain`) produces this output.

```c++
Widget is an object
Widget.name; // outputs 'world'
Widget.number; // outputs '42'
Widget.sayHello(); // outputs 'Hello, world. Your number is 42.'
Widget.name = 'foo'; Widget.name; // outputs 'foo'
Widget.number = 3*7; Widget.number; // outputs '21'
Widget.sayHello(); // outputs 'Hello, foo. Your number is 21.'
```
