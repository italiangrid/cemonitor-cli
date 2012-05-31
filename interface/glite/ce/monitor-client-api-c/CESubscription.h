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
        CESubscription: a class allowing the user to subscribe an information consumer

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CESubscription.h,v 1.38.8.3.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_CESUBSCRIPTION_H__
#define __GLITE_CE_MONITOR_CLIENT_CESUBSCRIPTION_H__

#include "glite/ce/monitor-client-api-c/AbsRequest.h"
#include "glite/ce/monitor-client-api-c/Topic.h"
#include "glite/ce/monitor-client-api-c/Policy.h"
#include <string>

namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;

/**
 *
 * Defines macros for function calls and
 * data structures depending on the gSOAP version in use
 *
 */
#define SUBSCRIBE soap_call_monitorws__Subscribe
#define UNSUBSCRIBE soap_call_monitorws__Unsubscribe
#define SUBSCRIPTION monitortypes__Subscription
#define SUBSCRIPTIONREF monitortypes__SubscriptionRef
#define SUBSCRIBERESPONSE monitorws__SubscribeResponse
#define UNSUBSCRIBERESPONSE monitorws__UnsubscribeResponse
#define SUBSCRIPTIONREF_FIELD SubscriptionRef
#define PAUSESUB soap_call_monitorws__PauseSubscription
#define PAUSESUBRESPONSE monitorws__PauseSubscriptionResponse
#define RESUMESUBRESPONSE monitorws__ResumeSubscriptionResponse
#define RESUMESUB soap_call_monitorws__ResumeSubscription
namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace soap_proxy {

/// Wrapper class for subscribe/unsubscribe/suspend/resume requests
/**
   This class inherits from AbsRequest and wraps the remote calls subscribe/unsubscribe/pause/resume, hiding the user from any communication and SOAP detail.\n
*/
class CESubscription : public AbsRequest {

private:

  std::string         subscriptionID;
  TOPIC               *topic;
  POLICY              *policy;
  CESubscription(const CESubscription&) { }
  CESubscription& operator=(const CESubscription&) { return *this; }
  std::string consumerURL;
  time_t duration;

public:



  /// Creates a CESubscription object
  /**
	Creates a CESubscription object without initializing it (must call later authenticate() ).
  */
  CESubscription() throw();



  /// Creates a CESubscription object
  /**
	Creates a CESubscription object and initializes the SOAP runtime for SSL authentication.
        \param certfile  The filename of the user's proxy certificate (voms or non-voms).
      	\param certpath  The name of the directory containing the certification authority's files for reverse AuthN (i.e. service authenticates itself in the client). For now can be any existing path (the reverse AuthN is not yet implemented).
	\throw AuthenticationInitException  If the certfile and/or certpath are missing on disk or not readable/accessible, or if the user's proxy certificate is corrupted or for some reason not accepted by gSOAP's internal SSL routines. AuthenticationInitException::what() gives the user the human readable reason of the exception.
  */
  CESubscription(const std::string& certfile, const std::string& keyfile) throw(cemon_ex::AuthenticationInitException&);



  /// Destroy the CESubscription object
  /**
	Simply calls AbsRequest::DTOR that cleanly destroys the SOAP runtime
  */
    virtual ~CESubscription() throw();



  /// Subscribes a consumer
  /**
	This method invokes the remote call Subscribe on the remote CEMonitor and must be called AFTER calling AbsRequest::setServiceURL() and CESubscription::setSubscribeParam(). After a successfull call of subscribe() (i.e. no exception has been raised) the user can get the CEMonitor's response (the subscription ID) by invoking CESubscription::getSubscriptionID().
	\throw GenericException&  When the CEMonitor service sends back to the client a generic fault;
	\throw TopicNotSupportedException&  When the user tried to subscribe a consumer to an un-existing topic;
	\throw DialectNotSupportedException&  When the user tried to subscribe a consumer to a topic specifying an unsupported dialect language;
	\throw ServiceNotFoundException&  When the user specified an un-existing service's address (see AbsRequest::setServiceURL() for the correct address format). This exception can be raised if the host is unreachable/non-existent, or if the connection is refused (e.g. wrong tcp port), or if the root-context of the remote container (TomCAT) is wrong;
	\throw SubscriptionException&  If the subscription failed for some other reason;
	\throw AuthenticationException When the authentication handshake failed in the remote service;
	\throw AuthorizationException When the user is not authorized to invoke the getEvent call;
	\throw GeneralException&  If some other kind of error occurred.
	All raised exceptions contain human readable information the user can get using the what() method.
  */
    void        subscribe() throw(cemon_ex::GenericException&,
				  cemon_ex::TopicNotSupportedException&, 
				  cemon_ex::DialectNotSupportedException&,
				  cemon_ex::SubscriptionException&,
				  cemon_ex::AuthenticationException&,
				  cemon_ex::AuthorizationException&,
				  cemon_ex::ServiceNotFoundException&,
				  cemon_ex::GeneralException&);



  /// Removes a subscription
  /**
	This method invokes the remote call Unsubscribe on the remote CEMonitor and must be called AFTER calling AbsRequest::setServiceURL(). A successfull call of unsubscribe() produces no response.
	\param SubscriptionID  The identifier of the subscription to remove (as returned by the Subscribe remote call)
	\throw GenericException&  When the CEMonitor service sends back to the client a generic fault;
	\throw TopicNotSupportedException&  When the user tried to subscribe a consumer to an un-existing topic;
	\throw DialectNotSupportedException&  When the user tried to subscribe a consumer to a topic specifying an unsupported dialect language;
	\throw ServiceNotFoundException&  When the user specified an un-existing service's address (see AbsRequest::setServiceURL() for the correct address format). This exception can be raised if the host is unreachable/non-existent, or if the connection is refused (e.g. wrong tcp port), or if the root-context of the remote container (TomCAT) is wrong;
	\throw SubscriptionException&  If the subscription removal failed for some other reason;
	\throw SubscriptionNotFoundException& If the user is trying to remove a not existing subscription ID;
	\throw AuthenticationException When the authentication handshake failed in the remote service;
       \throw AuthorizationException When the user is not authorized to invoke the getEvent call;
       \throw GeneralException&  If some other kind of error occurred;
	\sa  CESubscription::subscribe()
	All raised exceptions contain human readable information the user can get using the what() methods
  */
    void        unsubscribe(const std::string& SubscriptionID) throw(cemon_ex::GenericException&,
								     cemon_ex::SubscriptionException&,
								     cemon_ex::AuthenticationException&,
								     cemon_ex::AuthorizationException&,
								     cemon_ex::SubscriptionNotFoundException&,
								     cemon_ex::ServiceNotFoundException&,
								     cemon_ex::GeneralException&);



  /// Pauses a subscription
  /**
	This method invokes the remote call Pause on the remote CEMonitor and must be called AFTER calling   AbsRequest::setServiceURL(). A successfull call of pause() produces no response.
	\param SubscriptionID  The identifier of the subscription to pause (as returned by the Subscribe remote call)
	\throw GenericException&  When the CEMonitor service sends back to the client a generic fault;
	\throw TopicNotSupportedException&  When the user tried to subscribe a consumer to an un-existing topic;
	\throw DialectNotSupportedException&  When the user tried to subscribe a consumer to a topic specifying an unsupported dialect language;
	\throw ServiceNotFoundException&  When the user specified an un-existing service's address (see AbsRequest::setServiceURL() for the correct address format). This exception can be raised if the host is unreachable/non-existent, or if the connection is refused (e.g. wrong tcp port), or if the root-context of the remote container (TomCAT) is wrong;
	\throw SubscriptionException&  If the subscription pause failed for some other reason;
	\throw SubscriptionNotFoundException& If the user is trying to remove a not existing subscription ID;
	\throw AuthenticationException When the authentication handshake failed in the remote service;
	\throw AuthorizationException When the user is not authorized to invoke the getEvent call;
	\throw GeneralException&  If some other kind of error occurred.
	\sa  CESubscription::subscribe()
	All raised exceptions contain human readable information the user can get using the what() methods
  */
      void  pause(const std::string& SubscriptionID) throw(cemon_ex::GenericException&,
				  cemon_ex::SubscriptionException&,
				  cemon_ex::AuthenticationException&,
				  cemon_ex::AuthorizationException&,
				  cemon_ex::SubscriptionNotFoundException&,
				  cemon_ex::ServiceNotFoundException&,
				  cemon_ex::GeneralException&);



  /// Resumes a previously paused subscription
  /**
	This method invokes the remote call Resume on the remote CEMonitor and must be called AFTER calling AbsRequest::setServiceURL(). A successfull call of resume() produces no response.
	\param SubscriptionID  The identifier of the subscription to resume (as returned by the Subscribe remote call)
	\throw GenericException&  When the CEMonitor service sends back to the client a generic fault;
	\throw TopicNotSupportedException&  When the user tried to subscribe a consumer to an un-existing topic;
	\throw DialectNotSupportedException&  When the user tried to subscribe a consumer to a topic specifying an unsupported dialect language;
	\throw ServiceNotFoundException&  When the user specified an un-existing service's address (see AbsRequest::setServiceURL() for the correct address format). This exception can be raised if the host is unreachable/non-existent, or if the connection is refused (e.g. wrong tcp port), or if the root-context of the remote container (TomCAT) is wrong;
	\throw SubscriptionException&  If the subscription failed for some other reason;
	\throw SubscriptionNotFoundException& If the user is trying to remove a not existing subscription ID;
	\throw AuthenticationException When the authentication handshake failed in the remote service;
	\throw AuthorizationException When the user is not authorized to invoke the getEvent call;
	\throw GeneralException&  If some other kind of error occurred.
	\sa  CESubscription::subscribe()
	All raised exceptions contain human readable information the user can get using the what() methods
  */
  void        resume(const std::string& SubscriptionID) throw(cemon_ex::GenericException&,
				  cemon_ex::SubscriptionException&,
				  cemon_ex::AuthenticationException&,
				  cemon_ex::AuthorizationException&,
				  cemon_ex::SubscriptionNotFoundException&,
				  cemon_ex::ServiceNotFoundException&,
				  cemon_ex::GeneralException&);



  /// Returns the subscription identifier.
  /**
	This method has to be called after a successfull call of CESubscription::subscribe()
	\return A string representing a unique ID for the subscription done by CESubscription::subscribe()
  */
    std::string getSubscriptionID() const throw();



  /// Set the subscription parameters
  /**
	Sets subscription parameters.
	\param consumerURL the address of the notification consumer (must have the format http://<host>:<port>);
	\param topic the Topic that will send events to consumer (\see Topic);
	\param policy the Policy that rules the action the CEMonitor takes when an event happens (\see Policy);
	\param duration the subscription duration (in seconds).
  */
    void        setSubscribeParam(const std::string& consumerURL,
				  const Topic& topic,
				  const Policy& policy,
				  const long duration
				  ) throw();
};
      }
    }
  }
}
#endif
