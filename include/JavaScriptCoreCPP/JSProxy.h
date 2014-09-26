// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#ifndef _Titanium_Mobile_JSProxy_h_
#define _Titanium_Mobile_JSProxy_h_

#include <JavaScriptCore/JavaScript.h>

class JSProxy {
    
  JSProxy() {
  }

  // Create a copy of another JSProxy.
  JSProxy( const JSProxy& rhs ) {
    object_ = rhs.object_;
  }
    
  // Non-virtual destructor.
  ~JSProxy() {
  }
    
  // Create a copy of another JSProxy by assignment.
  JSProxy& operator=( const JSProxy& rhs ) {
    if ( this == &rhs ) {
      return *this;
    }

    object_ = rhs.object_;
        
    return *this;
  }

 private:
    
  // Our JavaScript counterpart.
  ::JSObjectRef object_ { nullptr };
};

#endif // _Titanium_Mobile_JSProxy_h_
