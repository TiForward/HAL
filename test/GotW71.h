//
//  GotW71.h
//  TiValue
//
//  Created by Matt Langston on 9/10/14.
//  Copyright (c) 2014 Pedro Enrique. All rights reserved.
//
// http://www.gotw.ca/gotw/071.htm


#ifndef __TiValue__GotW71__
#define __TiValue__GotW71__

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

class Clonable {
};

// Example 3(b): Using IsDerivedFrom to make use of
// derivation from Clonable if available, and do
// something else otherwise.
//
template<class T, int>
class XImpl
{
    // general case: T is not derived from Clonable
};

template<class T>
class XImpl<T, 1>
{
    // T is derived from Clonable
};

// Example 3(a), continued: Using IsDerivedFrom
// to enforce derivation from Clonable
//
template<class T>
class X
{
    XImpl<T, IsDerivedFrom<T, Clonable>::Is> impl_;
    // ... delegates to impl_ ...
    
    bool ValidateRequirements() const
    {
        // typedef needed because of the ,
        typedef IsDerivedFrom<T, Clonable> Y;
        
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


class MyClonable : public Clonable { /*...*/ };
class MyNotClonable { /*...*/ };

// Example 4: Using traits instead of IsDerivedFrom
// to make use of Clonability if available, and do
// something else otherwise. Requires writing a
// specialization for each Clonable class.
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
class XTraits<MyClonable>
{
    // MyClonable is derived from Clonable, so use Clone()
    static MyClonable* Clone( const MyClonable* p )
    { return p->Clone(); }
};
*/

// ... etc. for every class derived from Clonable

#endif /* defined(__TiValue__GotW71__) */
