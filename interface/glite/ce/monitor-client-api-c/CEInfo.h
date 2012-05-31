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
        CEInfo: header file for a C++ class wrapping the GetInfo  call

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CEInfo.h,v 1.30.8.3.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_CEINFO__
#define __GLITE_CE_MONITOR_CLIENT_CEINFO__

#include "glite/ce/monitor-client-api-c/AbsRequest.h"
#include "glite/ce/monitor-client-api-c/Topic.h"
#include "glite/ce/monitor-client-api-c/Action.h"
#include <string>
#include <vector>

#define INFO monitortypes__Info
#define GETINFORESP monitorws__GetInfoResponse
#define INFO_FIELD Info
#define GETINFO soap_call_monitorws__GetInfo

namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace soap_proxy {

/// Wrapper class for getInfo request
/**
	This class inherits from AbsRequest and wraps the getInfo remote call, hiding the user from any communication and SOAP detail.\n
	The information the getEvent can retrieve from the service is composed by its "Version" and "Description", a list of supported topics (sensors of the CEMonitor) and a list of supported actions. See CEMonitor Documentation for more details about topics and actions.
*/
class CEInfo : public AbsRequest {

 private:
  INFO info;
  GETINFORESP resp;
  void init() throw();

 protected:
  CEInfo(const CEInfo&) {}
  CEInfo& operator=(const CEInfo&) { return *this; }

 public:



   /// Creates a CEInfo object
   /**
      Creates a CEInfo object and initializes the SOAP runtime for SSL authentication.
      \param certfile  The filename of the user's proxy certificate.
      \param certpath  The name of the directory containing the certification authority's files for reverse AuthN. If these certificates are not present the simple "/" path can be used instead.
      
      \throw AuthenticationInitException  If the certfile and/or certpath are missing on disk or not readable/accessible, or if the user's proxy certificate is corrupted or not valid for some reason. cemon_ex::AuthenticationInitException::what() gives the user the human readable cause of the exception.
   */
   CEInfo(const std::string& certfile, const std::string& keyfile) throw(cemon_ex::AuthenticationInitException&);



  /// Destroy the CEInfo object
  /**
	Simply calls AbsRequest::DTOR that cleanly destroys the SOAP runtime
  */
  virtual ~CEInfo(void) throw();


  
  /// Invokes the remote call getInfo
    /**
       Invokes the CEMonitor remote call getInfo and stores the received informations.\n
       The user can get these info using the methods CEInfo::getVersion(), CEInfo::getDescription(), CEInfo::getTopics(), CEInfo::getActions().\n
       
       \sa CEInfo::getVersion()
       \sa CEInfo::getDescription()
       \sa CEInfo::getTopics()
       \sa CEInfo::getActions()

       \throw GenericException&  When the CEMonitor service sends back to the client a generic fault;
       \throw TopicNotSupportedException&  When the user specified a not existing topic name (see CETopics to query all supported Topics by the CEMonitor installation);
       \throw DialectNotSupportedException&  When the user asked for an event with an unsupported dialect formatting language;
       \throw ServiceNotFoundException&  When the user specified an un-existing service's address (see AbsRequest::setServiceURL() for the correct address format). This exception can be raised if the host is unreachable/non-existent, or if the connection is refused (e.g. wrong tcp port), or if the root-context of the remote container (TomCAT) is wrong;
       \throw AuthenticationException When the authentication handshake failed in the remote service;
       \throw AuthorizationException When the user is not authorized to invoke the getEvent call;
       \throw GeneralException&  If some other kind of error occurred.
       All raised exceptions contain human readable information the user can get using the method ::what().
       
    */
    void        getInfo() throw(cemon_ex::GenericException&, 
				cemon_ex::TopicNotSupportedException&, 
				cemon_ex::DialectNotSupportedException&,
				cemon_ex::ServiceNotFoundException&,
				cemon_ex::GeneralException&,
				cemon_ex::AuthenticationException&,
				cemon_ex::AuthorizationException&);
			      


    /// Returns the service's version
    /**
       \return A string containing the service's version
    */
    std::string getVersion() const throw();



  /// Returns the service's description
    /**
       \return A string containing the service's description
    */
  std::string getDescription() const throw();



  /// Gets the service's topics
  /**
	This method provides all the topics (sensors) supported by the CEMonitor.\n
	They are put in the user provided (and allocated) pointer to a vector of pointers to Topic objects.\n
	For each topic object the user can browse it invoking the methods of Topic class.
	\sa Topic
  */
  void getTopics(std::vector<Topic*> *) const throw();



  /// Gets the service's actions
  /**
	This method provides all the actions supported by the CEMonitor.\n
	They are put in the user provided (and allocated) pointer to vector of pointers to Action objects.\n
	For each action object the user can browse it by invoking the methods of ActionW class.
	\sa ActionW
  */
  void getActions(std::vector<ActionW*> *) const throw();


  
  /// Gets the service certificate's distinguished name
  /**
     \return A string containing the distinguished name of the CEMonitor service
  */
  std::string getDN( void ) throw();
};
      }
    }
  }
}


#endif
