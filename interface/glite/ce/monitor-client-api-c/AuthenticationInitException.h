/*
  Copyright (c) Members of the EGEE Collaboration. 2004. 
  See http://www.eu-egee.org/partners/ for details on the copyright
  holders.  
  
  Licensed under the Apache License, Version 2.0 (the "License"); 
  you may not use this file except in compliance with the License. 
  You may obtain a copy of the License at 
  
      http://www.apache.org/licenses/LICENSE-2.0 
  
  Unless required by applicable law or agreed to in writing, software 
  distributed under the License is distributed on an "AS IS" BASIS, 
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
  See the License for the specific language governing permissions and 
  limitations under the License.
*/

/*
        AuthenticationInitException.h: a C++ Exception related to un-availability of the serviceURL set by user
        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: AuthenticationInitException.h,v 1.3.10.1 2010/03/22 12:11:11 pandreet Exp $
*/


#ifndef __GLITE_CE_MONITOR_CLIENT_AUTHNINITEXCEPTION_H__
#define __GLITE_CE_MONITOR_CLIENT_AUTHNINITEXCEPTION_H__

#include "glite/ce/monitor-client-api-c/AbsException.h"
#include <string>

/// Exception for soap_authn init errors
namespace glite {
namespace ce {
namespace monitor_client_api {
namespace cemon_exceptions {

class AuthenticationInitException : public AbsException
{
 public:
  AuthenticationInitException(const std::string& _cause) throw() : AbsException(_cause) {}
  virtual ~AuthenticationInitException() throw() {};
};
}
}
}
}

#endif
