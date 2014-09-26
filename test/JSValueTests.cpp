#include "JavaScriptCoreCPP/JSValue.h"

#include <string>
#include <iostream>

#include <JavaScriptCore/JavaScript.h>
#include "gtest/gtest.h"

namespace UnitTestConstants {
static const double pi { 3.141592653589793 };
}

class JSValueTests : public testing::Test {
 protected:
  virtual void SetUp() {
  }
	
  virtual void TearDown() {
    std::cout << "JSSValue::ctorCounter = " << JSValue::ctorCounter() << std::endl;
    std::cout << "JSSValue::dtorCounter = " << JSValue::dtorCounter() << std::endl;
  }
	
  JSContext_ptr_t context_ptr { JSContext::create() };
};


TEST_F(JSValueTests, testUndefined) {
  auto undefinedValue = JSValue::valueWithUndefinedInContext(context_ptr);
  ASSERT_TRUE(undefinedValue->isUndefined());
  ASSERT_FALSE(undefinedValue->isNull());
  ASSERT_FALSE(undefinedValue->isBoolean());
  ASSERT_FALSE(undefinedValue->isNumber());
  ASSERT_FALSE(undefinedValue->isString());
}

TEST_F(JSValueTests, testBool) {
  auto falseValue = JSValue::valueWithBoolInContext(false, context_ptr);
  ASSERT_FALSE(bool(*falseValue));

  ASSERT_FALSE(falseValue->isUndefined());
  ASSERT_FALSE(falseValue->isNull());
  ASSERT_TRUE(falseValue->isBoolean());
  ASSERT_FALSE(falseValue->isNumber());
  ASSERT_FALSE(falseValue->isString());
    
  auto trueValue = JSValue::valueWithBoolInContext(true, context_ptr);
  ASSERT_TRUE(bool(*trueValue));

  ASSERT_FALSE(trueValue->isUndefined());
  ASSERT_FALSE(trueValue->isNull());
  ASSERT_TRUE(trueValue->isBoolean());
  ASSERT_FALSE(trueValue->isNumber());
  ASSERT_FALSE(trueValue->isString());
}

TEST_F(JSValueTests, testDouble) {
  auto doubleValue = JSValue::valueWithDoubleInContext(UnitTestConstants::pi, context_ptr);
  ASSERT_DOUBLE_EQ(UnitTestConstants::pi, double(*doubleValue));
    
  auto piValue_ptr  = context_ptr -> evaluateScript("Math.PI");
  ASSERT_DOUBLE_EQ(UnitTestConstants::pi, double(*piValue_ptr));

  ASSERT_FALSE(doubleValue->isUndefined());
  ASSERT_FALSE(doubleValue->isNull());
  ASSERT_FALSE(doubleValue->isBoolean());
  ASSERT_TRUE(doubleValue->isNumber());
  ASSERT_FALSE(doubleValue->isString());
}

TEST_F(JSValueTests, testInt32) {
  auto intValue = JSValue::valueWithInt32InContext(42, context_ptr);
  ASSERT_EQ(42, int32_t(*intValue));

  auto result_ptr  = context_ptr -> evaluateScript("21 / 7");
  ASSERT_EQ(3, int32_t(*result_ptr));

  ASSERT_FALSE(intValue->isUndefined());
  ASSERT_FALSE(intValue->isNull());
  ASSERT_FALSE(intValue->isBoolean());
  ASSERT_TRUE(intValue->isNumber());
  ASSERT_FALSE(intValue->isString());
}

TEST_F(JSValueTests, testUInt32) {
  auto uintValue = JSValue::valueWithUInt32InContext(42, context_ptr);
  ASSERT_EQ(42, uint32_t(*uintValue));
    
  auto result_ptr  = context_ptr -> evaluateScript("21 / 7");
  ASSERT_EQ(3, uint32_t(*result_ptr));
    
  ASSERT_FALSE(uintValue->isUndefined());
  ASSERT_FALSE(uintValue->isNull());
  ASSERT_FALSE(uintValue->isBoolean());
  ASSERT_TRUE(uintValue->isNumber());
  ASSERT_FALSE(uintValue->isString());
}
