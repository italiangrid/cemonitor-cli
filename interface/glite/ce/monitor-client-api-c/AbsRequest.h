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

        AbsRequest: parent class for all CEMonitor requests

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: AbsRequest.h,v 1.38.8.2.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_ABSREQUEST__
#define __GLITE_CE_MONITOR_CLIENT_ABSREQUEST__

#include "glite/ce/monitor-client-api-c/monitor_client_CEMON_CLIENTStub.h"
#include "glite/ce/monitor-client-api-c/GenericException.h"
#include "glite/ce/monitor-client-api-c/TopicNotSupportedException.h"
#include "glite/ce/monitor-client-api-c/DialectNotSupportedException.h"
#include "glite/ce/monitor-client-api-c/SubscriptionException.h"
#include "glite/ce/monitor-client-api-c/ServiceNotFoundException.h"
#include "glite/ce/monitor-client-api-c/GeneralException.h"
#include "glite/ce/monitor-client-api-c/AuthenticationException.h"
#include "glite/ce/monitor-client-api-c/AuthenticationInitException.h"
#include "glite/ce/monitor-client-api-c/AuthorizationException.h"
#include "glite/ce/monitor-client-api-c/SubscriptionNotFoundException.h"
//#include "glite/ce/monitor-client-api-c/CEMON_CLIENT.nsmap"

#include <string>
#include <vector>

extern "C" {
#undef IOV_MAX
#include "glite/security/glite_gsplugin.h"
};

namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace soap_proxy {

/// Base class for all CEMonitor requests.
/**
	AbsRequest class is the base class of all remote-call
	wrappers which are the client side of the CEMonitor.\n
	A remote-call wrapper
	completely hides the user from the details of the SOAP communication and 
	can perform one or more remote calls
	to the Web Service (CEMonitor) using the SOAP transport protocol.
	The AbsRequest class provides some interfaces and implements some common utilities like
	setting and retrieve error messages (caused by internal failures or by communication errors), 
	hiding the user from the details of obtaining them from the SOAP data structure, setting the
	service URL the client need to connect to, cleanin up the heap-allocated SOAP data structures and
	setting up the authentication credentials.\n 

	The creation of an object of this class is possible but useless, because it just
	initializes the SOAP runtine and allows setting of some parameters (as said above) 
	but doesn't actually perform any communication with the CEMonitor Web Service. The user has to
	instantiate the right class for the request she/he wants to send to the CEMonitor. See below for
	the list of the client classes:

	\sa CEEvent
	\sa CEInfo
	\sa CEPing
	\sa CETopics
	\sa CESubscription
	\sa CESubscriptionMgr
*/
class AbsRequest {

 private:
  //struct soap *SOAP;

  struct soap                              *m_SOAP;
  std::string                               m_serviceURL;
  glite_gsplugin_Context                    m_ctx;
  cefaults__BaseFaultType                   m_BaseFault;
  _monitorfaults__SubscriptionFault         m_SubscriptionFault;
  _monitorfaults__TopicNotSupportedFault    m_TopicNotSupportedFault;
  _monitorfaults__DialectNotSupportedFault  m_DialectNotSupportedFault;
  _cefaults__GenericFault                   m_GenericFault;
  _cefaults__AuthenticationFault            m_AuthenticationFault;
  _cefaults__AuthorizationFault             m_AuthorizationFault;
  _monitorfaults__SubscriptionNotFoundFault m_SubscriptionNotFoundFault;

  int m_BaseFault_integer, m_SubscriptionFault_integer, m_TopicNotSupportedFault_integer, m_DialectNotSupportedFault_integer, m_GenericFault_integer, m_AuthenticationFault_integer, m_AuthorizationFault_integer, m_SubscriptionNotFoundFault_integer;

protected:
  struct soap* getSoap() const throw() { return m_SOAP; }

  /**
   * No one can copy an AbsRequest object
   */
  AbsRequest(const AbsRequest&) {} 
  AbsRequest& operator=(const AbsRequest&) { return *this; }
  void         setError(void) throw();

  std::string  m_errorMessage;
  std::string  m_errorCode;

  void throwCEException(int) throw(cemon_ex::GenericException&, 
				   cemon_ex::TopicNotSupportedException&, 
				   cemon_ex::DialectNotSupportedException&, 
				   cemon_ex::SubscriptionException&,
				   cemon_ex::AuthenticationException&,
				   cemon_ex::AuthorizationException&,
				   cemon_ex::SubscriptionNotFoundException&);

  void throwAbsException() throw(cemon_ex::ServiceNotFoundException&,
			  	 cemon_ex::GeneralException&);

  void setNamespace() throw();
  void* getFault() throw();
  int getFaultType() throw();

public:



  /// Creates an AbsRequest object
  /**
	The default class constructor takes care of the SOAP runtime structures initialization.
	\throw cemon_ex::GeneralException If the SOAP runtime initialization has failed. cemon_ex::GeneralException::what() returns the cause in a human readable form
  */
  AbsRequest() throw(cemon_ex::GeneralException&);



  /// Deletes an AbsRequest object
  /**
	The destructor clears the heap-allocated SOAP runtime data structures.
  */
  virtual ~AbsRequest(void) throw();



  /// Returns the error message
  /**
	A fault raised from the CEMonitor service and sent back to the client via SOAP (or even an internal SOAP failure) is usually composed by an error message and an error code. This method provides the composition of them in a human readable form.
	\return A string containing the error message.
  */
  std::string getErrorMessage(void) const throw() { return m_errorMessage; }



  /// Returns the error code
  /**
	A fault raised from the CEMonitor service and sent to the client via SOAP (or even an internal SOAP failure) is usually composed by an error message and an error code. This method provides read access to the code.
	\return A string containing the error code.
  */
  std::string getErrorCode(void) const throw() { return m_errorCode; }



  /// Sets the service endpoint address
  /**
	Sets the address of the remote CEMonitor service. The format must be: http[s]://<hostname>:<tcp_port>/ce-monitor/services/CEMonitor. For example example: https://grid005.pd.infn.it:8443/ce-monitor/services/CEMonitor .
	\param A string containing the service's URL.
  */
  void setServiceURL(const std::string& url) throw() { m_serviceURL = url; }



  /// Cleans up SOAP heap
  /**
	Cleans up all deserialized C++ data structures allocated on the heap at run-time, preventing memory leaks; another remote call can be safely invoked just after calling this method.
	
  */
  virtual void cleanup(void) throw();



  /// Returns the address of the service.
  /** 
      Returns the address of the remote CEMonitor service previously set.
      \return A string containing the service's URL.
  */
  std::string getServiceURL() const throw() { return m_serviceURL; }



  /// Sets up authentication
  /**
     This method sets up the authentication credentials.
     \param cert is a string containing the complete path of the certificate file of the user
     \param cert_path is a string containing the complete path where the certificates CA are installed. If no path is available, just use "/".
     \throw cemon_ex::AuthenticationInitException If authentication fails. The cause of failure is reported by the method cemon_ex::AuthenticationInitException::what().
  */
  void authenticate(const char* certfile, const char* keyfile) throw(cemon_ex::AuthenticationInitException&);
};
      }
    }
  }
}

#endif
