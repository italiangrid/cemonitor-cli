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
        CETopics.hh: header file for a C++ class wrapping the GetTopics call

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CETopics.h,v 1.29.8.3.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_CETOPICS__
#define __GLITE_CE_MONITOR_CLIENT_CETOPICS__

#include "glite/ce/monitor-client-api-c/AbsRequest.h"
#include "glite/ce/monitor-client-api-c/Topic.h"
#include <string>
#include <vector>

using namespace std;

namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;

#define GETTOPICSRESP monitorws__GetTopicsResponse
#define TOPICARRAY monitortypes__TopicArray
#define TOPICARRAY_FIELD TopicArray
#define GETTOPIC soap_call_monitorws__GetTopics

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace soap_proxy {



/// Wrapper class for getTopics request
/**
   This class inherits from AbsRequest and wraps the getTopics remote call, hiding the user from any communication and SOAP detail.\n
   The information the getToppics can retrieve from the service is composed by an array of objects of the Topic class. After the invocation of the getTopics, the user can browse the information returned by the service by navigating into the array of Topic objects and getting information from each of them (see the Topic class for its methods).
*/
class CETopics : public AbsRequest {
  
 private:
  bool retrievedTopics;
  GETTOPICSRESP topicResp;
  int sizeTopic, topicCounter;
  
 protected:
  CETopics(const CETopics&) {}
  CETopics& operator=(const CETopics&) { return *this; }

public:



  /// Creates a CETopics object
   /**
      Creates a CETopics object and initializes the SOAP runtime for SSL authentication.
      \param certfile  The filename of the user's proxy certificate.
      \param certpath  The name of the directory containing the certification authority's files for reverse AuthN. If these certificates are not present the simple "/" path can be used instead.
      
      \throw AuthenticationInitException  If the certfile and/or certpath are missing on disk or not readable/accessible, or if the user's proxy certificate is corrupted or not valid for some reason. cemon_ex::AuthenticationInitException::what() gives the user the human readable cause of the exception.
   */
  CETopics(const std::string& certfile, const std::string& keyfile) throw(cemon_ex::AuthenticationInitException&);

  /// Destroy the CETopics object
  /**
	Simply calls AbsRequest::DTOR that cleanly destroys the SOAP runtime
  */
  virtual ~CETopics() throw();

  /// Gets the array of topic returned by the service
    /**
       After invocation of the getTopics method, an internal array of Topic object is filled up.
       The user can obtain all those topics by passing a (pre-allocated) pointer of pointers to Topic as argument of this method.
       \param A pointer to a vector of pointers to Topic, that the method will fill up.
    */
  void getTopicArray(vector<Topic*> *) const throw();

  /// Gets the number of Topics returned by the service
  /**
  \return An integer representing the number of installed topics on the remote service.
    */
  int getNumberOfTopics(void) const throw();

    /// Invokes the remote call getTopics
    /**
       Invokes the CEMonitor remote call getTopics and stores the received informations.\n
       The user can get these info using the methods CETopics::getTopicArray().\n
       
       \sa CETopics::getTopicArray()

       \throw GenericException&  When the CEMonitor service sends back to the client a generic fault;
       \throw ServiceNotFoundException&  When the user specified an un-existing service's address (see AbsRequest::setServiceURL() for the correct address format). This exception can be raised if the host is unreachable/non-existent, or if the connection is refused (e.g. wrong tcp port), or if the root-context of the remote container (TomCAT) is wrong;
       \throw AuthenticationException When the authentication handshake failed in the remote service;
       \throw AuthorizationException When the user is not authorized to invoke the getEvent call;
       \throw GeneralException&  If some other kind of error occurred.
       All raised exceptions contain human readable information the user can get using the method what().
       
    */
  void getTopics(void) throw(cemon_ex::GenericException&, 
			     cemon_ex::ServiceNotFoundException&,
			     cemon_ex::GeneralException&,
			     cemon_ex::AuthenticationException&,
			     cemon_ex::AuthorizationException&);
	};
      }
    }
  }
}
#endif
