/**
 * HAL
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#import <XCTest/XCTest.h>
#include <memory>
#include <iostream>
#include <sstream>

/*
#include "Util/virtual_enable_shared_from_this_base.hpp"
using Appcelerator::Util::virtual_enable_shared_from_this;
*/

struct virtual_enable_shared_from_this_base : std::enable_shared_from_this<virtual_enable_shared_from_this_base> {
  virtual ~virtual_enable_shared_from_this_base() {
  }
};

template<typename T>
struct virtual_enable_shared_from_this : virtual virtual_enable_shared_from_this_base {
  std::shared_ptr<T> shared_from_this() {
    return std::dynamic_pointer_cast<T>(virtual_enable_shared_from_this_base::shared_from_this());
  }
};

struct A: virtual_enable_shared_from_this<A> {
};

struct B: virtual_enable_shared_from_this<B> {
};

using helloCallback_t = std::function<std::string()>;

struct C: A, B, virtual_enable_shared_from_this<C> {
  static constexpr int arraySize { 10 };
  int* intArray { new int[arraySize] };
  
  std::string hello() {
    std::ostringstream os;
    os << "MDL: intArray = [";
    for (int i = 0; i < arraySize; ++i) {
      if (i > 0) {
        os << ", ";
      }
      os << intArray[i];
    }
    os << "]";
    return os.str();
  }
  
  C() {
    for (int i = 0; i < arraySize; ++i) {
      intArray[i] = i;
    }
  }
  
  ~C() {
    delete[] intArray;
  }
  
  using virtual_enable_shared_from_this<C>::shared_from_this;

  helloCallback_t helloCallbackCorrect() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++1y-extensions"
    auto lambda = [weakThis = std::weak_ptr<C>(shared_from_this())]() -> std::string {
      auto strong_ptr = weakThis.lock();
      if (strong_ptr) {
        return strong_ptr  -> hello();
      } else {
        return "weak pointer is invalid";
      }
    };
#pragma clang diagnostic pop

    return lambda;
  }

  helloCallback_t helloCallbackIncorrect() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++1y-extensions"
    auto lambda = [rawThis = this]() -> std::string {
      // This will crash is this is a dangling pointer.
     return rawThis -> hello();
    };
#pragma clang diagnostic pop
    
    return lambda;
  }
};

@interface LambdaTests : XCTestCase
@end

static const std::string expectedHelloResult("MDL: intArray = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");

@implementation LambdaTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testhelloCallbackCorrect {
  // Create a scope so that the destructors run.
  {
    auto c_ptr = std::make_shared<C>();
    auto helloCallback = c_ptr -> helloCallbackCorrect();
    std::cout << "helloCallback() = " << helloCallback() << std::endl;
    XCTAssertEqual(expectedHelloResult, helloCallback());
  }
  
  // This is initialized in the following scope in order to test that the weak
  // pointer works correctly.
  helloCallback_t helloCallback { nullptr };
  
  // Create a scope so that the destructors run.
  {
    auto c_ptr = std::make_shared<C>();
    helloCallback = c_ptr -> helloCallbackCorrect();
  }
  
  // The weak pointer logic in helloCallbackCorrect will detect that the weak
  // pointer is no longer valid.
  std::cout << "helloCallback() = " << helloCallback() << std::endl;
  XCTAssertEqual("weak pointer is invalid", helloCallback());
}

- (void)testhelloCallbackInorrect {
  // Create a scope so that the destructors run.
  {
    auto c_ptr = std::make_shared<C>();
    auto helloCallback = c_ptr -> helloCallbackIncorrect();
    XCTAssertEqual(expectedHelloResult, helloCallback());
  }
  
  // This is initialized in the following scope in order to test that the weak
  // pointer works correctly.
  helloCallback_t helloCallback { nullptr };
  
  // Create a scope so that the destructors run.
  {
    auto c_ptr = std::make_shared<C>();
    helloCallback = c_ptr -> helloCallbackIncorrect();
  }

  // The "this" pointer in the lambda is no longer valid, so using this lambda
  // will result undefined behaviour, probably crashing the app.
  // This causes EXC_BAD_ACCESS
  //std::cout << "helloCallback() = " << helloCallback() << std::endl;
  //XCTAssertEqual(expectedHelloResult, helloCallback());
}

@end
