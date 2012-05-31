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
        CEPing: header file for a C++ class wrapping the Ping call

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CEPing.h,v 1.22.8.2.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_CEPING__
#define __GLITE_CE_MONITOR_CLIENT_CEPING__

#include "glite/ce/monitor-client-api-c/AbsRequest.h"
#include <string>

#define PINGRESPONSE monitorws__PingResponse
#define PING soap_call_monitorws__Ping

namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace soap_proxy {

/// Wrapper class for Ping remote call
/**
   This class inherits from AbsRequest and wraps the Ping remote call, hiding the user from any communication and SOAP detail.\n
   The remote service container (TomCAT) will return a fault if the service CEMonitor doesn't respond; alternatively if even the container is off the CEPing::Ping() will raise a ServiceNotFoundException
*/
class CEPing : public AbsRequest {
  
 private:
  PINGRESPONSE pingResp;
  
 protected:
  CEPing(const CEPing&) {}
  CEPing& operator=(const CEPing&) { return *this; }
  
 public:



  /// Creates a CEPing object
  /**
	Creates a CEPing object and initializes the SOAP runtime for SSL authentication.
        \param certfile  The filename of the user's proxy certificate (voms or non-voms).
      	\param certpath  The name of the directory containing the certification authority's files for reverse AuthN (i.e. service authenticates itself in the client). For now can be any existing path (the reverse AuthN is not yet implemented).
	\throw AuthenticationException  If the certfile and/or certpath are missing on disk or not readable/accessible, or if the user's proxy certificate is corrupted or for some reason not accepted by gSOAP's internal SSL routines. AuthenticationInitException::what() gives the user the human readable reason of the exception.
  */
   CEPing(const std::string& certfile, const std::string& keyfile) throw(cemon_ex::AuthenticationInitException&);



  /// Destroy the CEPing object
  /**
	Simply calls AbsRequest::DTOR that cleanly destroys the SOAP runtime
  */
     virtual ~CEPing() throw() {}

  /// Pings the remote service
  /**
	Invokes the remote call Ping in order to check that the remote service is up & running.
	
	\return True if the remote CEMonitor responds, False otherwise.
  */
  bool Ping() throw();

};

      }
    }
  }
}
#endif
