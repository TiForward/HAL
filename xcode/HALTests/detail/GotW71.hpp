/**
 * HAL
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

// http://www.gotw.ca/gotw/071.htm


#ifndef _HAL_XCODE_HALTESTS_GOTW71_HPP_
#define _HAL_XCODE_HALTESTS_GOTW71_HPP_

// Example 2(d): Alternative way of requiring
// exactly T* T::Clone() const
//
// T must provide T* T::Clone() const
template<class T>
class C
{
  bool ValidateRequirements()
  {
    T* (T::*test)() const = &T::Clone;
    test; // suppress warnings about unused variables
    // ...
    return true;
  }
    
public:
  // in C's destructor (easier than putting it
  // in every C ctor):
  ~C()
  {
    assert( ValidateRequirements() );
  }
    
  // ...
};

class Foo {
public:
  Foo* Clone() const {
    return nullptr;
  }
};

class Bar {
};


// Example 3(a): An IsDerivedFrom helper
//
template<class D, class B>
    class IsDerivedFrom
{
private:
  class Yes { char a[1]; };
  class No { char a[10]; };
    
  static Yes Test( B* ); // undefined
  static No Test( ... ); // undefined
    
public:
  enum { Is = sizeof(Test(static_cast<D*>(0))) == sizeof(Yes) ? 1 : 0 };
};

// Example 3(a), continued
//
template<class T>
class IsDerivedFrom<T,T>
{
public:
  enum { Is = 0 };
};

class Cloneable {
};

// Example 3(b): Using IsDerivedFrom to make use of
// derivation from Cloneable if available, and do
// something else otherwise.
//
template<class T, int>
    class XImpl
{
  // general case: T is not derived from Cloneable
};

template<class T>
class XImpl<T, 1>
{
  // T is derived from Cloneable
};

// Example 3(a), continued: Using IsDerivedFrom
// to enforce derivation from Cloneable
//
template<class T>
class X
{
  XImpl<T, IsDerivedFrom<T, Cloneable>::Is> impl_;
  // ... delegates to impl_ ...
    
  bool ValidateRequirements() const
  {
    // typedef needed because of the ,
    typedef IsDerivedFrom<T, Cloneable> Y;
        
    // a runtime check, but one that can be turned
    // into a compile-time check without much work
    assert( Y::Is );
        
    return true;
  }
    
public:
  // in X's destructor (easier than putting it
  // in every X ctor):
  ~X()
  {
    assert( ValidateRequirements() );
  }
    
  // ...
};


class MyCloneable : public Cloneable { /*...*/ };
class MyNotCloneable { /*...*/ };

// Example 4: Using traits instead of IsDerivedFrom
// to make use of Clonability if available, and do
// something else otherwise. Requires writing a
// specialization for each Cloneable class.
//
template<class T>
class XTraits
{
  // general case: use copy constructor
  static T* Clone( const T* p )
  { return new T( *p ); }
};

/*
  template<>
  class XTraits<MyCloneable>
  {
  // MyCloneable is derived from Cloneable, so use Clone()
  static MyCloneable* Clone( const MyCloneable* p )
  { return p->Clone(); }
  };
*/

// ... etc. for every class derived from Cloneable

#endif  // _HAL_XCODE_HALTESTS_GOTW71_HPP_
