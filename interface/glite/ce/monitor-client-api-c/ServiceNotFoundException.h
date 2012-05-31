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
        ServiceNotFoundException.h: a C++ Exception related to un-availability of the serviceURL set by user
        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: ServiceNotFoundException.h,v 1.9.10.1 2010/03/22 12:11:11 pandreet Exp $
*/


#ifndef __GLITE_CE_MONITOR_CLIENT_SERVICEEXCEPTION_H__
#define __GLITE_CE_MONITOR_CLIENT_SERVICEEXCEPTION_H__

#include "glite/ce/monitor-client-api-c/AbsException.h"
#include <string>

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace cemon_exceptions {
	/// Exception for connection-related faults
	/**
	   An object of this class is usually raised when the service's address is wrong, or the host is unreachable (e.g. for network problem), or the host refused the connection (e.g. wrong or filtered tcp port) etc. Also the case of wrong context root of the service is handled by raising this exception.
	*/
	
	class ServiceNotFoundException : public AbsException
	  {
	  public:
	    /// Creates a ServiceNotFoundException with an error message
	    ServiceNotFoundException(const std::string& cause) throw() : AbsException(cause) {}
	    virtual ~ServiceNotFoundException() throw() {}
	  };
      }
    }
  }
}

#endif
