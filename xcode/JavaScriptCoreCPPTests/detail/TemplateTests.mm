/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#import <XCTest/XCTest.h>
#include <iostream>
#include <tuple>

struct A {void prnt(){std::cout<<"A\n";} };
struct B {void prnt(){std::cout<<"B\n";} };
struct C {void prnt(){std::cout<<"C\n";} };

/*
template<typename...Arg>
struct Prc
{
  Prc() : NumElems(sizeof...(Arg)), mems(std::make_tuple(Arg()...)){}
  
  int NumElems;
  std::tuple<Arg...> mems;
  
  void process()
  {
    for(int i=0; i<NumElems; ++i)
      std::get<i>(mems).prnt();//It's forbidden: "i" must be a constant
  }
};
*/

namespace detail
{
  template <int... Is>
  struct index { };
  
  template <int N, int... Is>
  struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };
  
  template <int... Is>
  struct gen_seq<0, Is...> : index<Is...> { };
}

template <typename...Args>
struct Prc
{
  std::tuple<Args...> mems;
  
  template <int... Is>
  void process(detail::index<Is...>) {
    auto l = { (std::get<Is>(mems).prnt(), 0) ... };
  }
  
  void process() {
    process(detail::gen_seq<sizeof...(Args)>());
  }
};

@interface TemplateTests : XCTestCase
@end

@implementation TemplateTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testExample {
  // This is an example of a functional test case.
  Prc<A,B,C> obj;
  obj.process();
}

// As of 2014.09.20 Travis CI only supports Xcode 5.1 which lacks support for
// measureBlock.
#ifndef TRAVIS
- (void)testPerformanceExample {
  // This is an example of a performance test case.
  [self measureBlock:^{
    // Put the code you want to measure the time of here.
  }];
}
#endif

@end
