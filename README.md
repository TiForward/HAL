# Hyperloop Abstraction Layer (HAL)

> The Hyperloop Abstraction Layer (HAL) library is a cross-platform standards-compliant C++11 library wrapper around the JavaScriptCore C API.

[![Build Status](https://travis-ci.org/appcelerator/HAL.svg)](https://travis-ci.org/appcelerator/HAL)


## Description

This is HAL, a cross-platform standards-compliant C++11 library that wraps the JavaScriptCore C API.

##### THE GOAL IF THIS PROJECT IS TO BE THE CORE OF Ti.Next

This library was initially created using both iOS 8 (i.e. Apple LLVM version 6.0, aka clang-600.0.54 based on LLVM 3.5svn) and Windows Phone 8.1 (i.e Microsoft's Visual C++ 18.0.30723.0 from Visual Studio 2013 Update 3 RTM) as reference platforms. However, it was designed to work on any platform that has a standards compliant C++11 compiler and library such as Android etc...

## Features of HAL

  - Provide a single "core" for Titanium rather then rebuild for each platform
  - Allow for a fast, JS bridge to native
  - Provide memory managment for native objects exposed to JS
  - Support for the latest JSCore
  - Allow for the exposing 100% of all native APIs on iOS, Android, and Windows Phone

### Which platforms are supported

  - Android 4.0+ (WIP)
  - iOS 8+ (WIP)
  - Windows Phone
  - Anything else that can support running C++11

### Roadmap

See [Roadmap](doc/ROADMAP.md) document.

### To compile / test:

See [How to Compile](doc/HOW_TO_COMPILE.md) document.

## How to contribute:

We are working fast to create a to-do list. For now check out the github issues here, and discuss issues on the issues project.

### Contributors

The original source code and design for this project was developed by [Matt Langston](https://github.com/matt-langston).

## Legal

Appcelerator is a registered trademark of Appcelerator, Inc. Titanium is a registered trademark of Appcelerator, Inc.  Please see the LICENSE file included with this distribution for information about using our trademarks, privacy policy, terms of usage and other legal information at [http://www.appcelerator.com/legal](http://www.appcelerator.com/legal).
