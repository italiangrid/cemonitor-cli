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
        CESubscriptionMgr: a class to handle subscriptions

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CESubscriptionMgr.h,v 1.12.8.3.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_CESUBSCRIPTIONMGR_H__
#define __GLITE_CE_MONITOR_CLIENT_CESUBSCRIPTIONMGR_H__

#include "glite/ce/monitor-client-api-c/AbsRequest.h"
#include "glite/ce/monitor-client-api-c/Topic.h"
#include "glite/ce/monitor-client-api-c/Policy.h"
#include <string>
#include <vector>
#include <ctime>
#include <map>

namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace soap_proxy {

/// A class wrapping all subscription-related information
class Subscription {
  
  std::string SubscriptionID;
  time_t ExpirationTime;
  time_t Rate;
  std::string ConsumerURL;
  std::string TopicName;
  std::string endpoint;

 public:

  /// Creates a Subscription object
   /**
      This constructor creates an object collecting all information related to a subscription;
      \param subID a string IDentifier,
      \param expTime the expiration time (UTC seconds),
      \param rate the notification rate (seconds between two subsequent notifications), 
      \param curl the notification consumer URL, 
      \param T the name of the Topic that must send the notifications to the consumer,
      \param _endpoint the address of the CEMon service where the user wants to create the subscription
   */
  Subscription(const std::string& subID, 
	       const time_t& expTime, 
	       const time_t& rate,
	       const std::string& curl,
	       const std::string& T,
	       const std::string& _endpoint) throw() 
    : SubscriptionID(subID), ExpirationTime(expTime), Rate(rate), ConsumerURL(curl), TopicName(T), endpoint(_endpoint) {}

    /// Returns the subscription ID
  std::string getSubscriptionID( void ) const throw() { return SubscriptionID; }

  /// Sets the subscription ID
  void        setSubscriptionID(const std::string& newid) throw() { SubscriptionID = newid; }
  
  /// Returns the Consumer's URL
  std::string getConsumerURL( void ) const throw() { return ConsumerURL; }

  /// Returns the expiration time (as UTC seconds)
    
  time_t      getExpirationTime( void ) const throw() { return ExpirationTime; }

  /// Sets the expiration time
  void        setExpirationTime( const time_t& newT ) throw() { ExpirationTime = newT; }

  /// Returns the Topic name the subscription relies to
  std::string getTopicName( void ) const throw() { return TopicName; }

  /// Returns the endpoint to send the subscription to
  std::string getEndpoint( void ) const throw() { return endpoint; } 

  /// Returns the seconds delay between two subsequents notifications
  time_t      getRate( void ) const throw() { return Rate; }
};



/// Wrapper class to list all subscription existing on a remote CEMonitor service
/**
   This class inherits from AbsRequest and allow the user to list all the subscription's IDs existing on a remote CEMonitor service, hiding him/her from any communication and SOAP detail.\n
*/
class CESubscriptionMgr : public AbsRequest {

private:

  CESubscriptionMgr(const CESubscriptionMgr&) {}
  CESubscriptionMgr& operator=(const CESubscriptionMgr&) { return *this; }
  std::string consumerURL;
  time_t duration;
  std::vector<Subscription> subids;

public:

  /// Creates a CESubscriptionMgr object
  /**
	Creates a CESubscriptionMgr object without initializing it (must call later authenticate() ).
  */
  CESubscriptionMgr() throw() : subids() {}

    

    /// Creates a CESubscriptionMgr object
    /**
       Creates a CESubscriptionMgr object and initializes the SOAP runtime for SSL authentication.
       \param certfile  The filename of the user's proxy certificate (voms or non-voms).
       \param certpath  The name of the directory containing the certification authority's files for reverse AuthN (i.e. service authenticates itself in the client). For now can be any existing path (the reverse AuthN is not yet implemented).
       \throw AuthenticationInitException  If the certfile and/or certpath are missing on disk or not readable/accessible, or if the user's proxy certificate is corrupted or for some reason not accepted by gSOAP's internal SSL routines. AuthenticationInitException::what() gives the user the human readable reason of the exception.
    */
    CESubscriptionMgr( const std::string& certfile, const std::string& keyfile ) 
    throw(cemon_ex::AuthenticationInitException&);



    /// Gets all the subscription's IDs
    /**
       This method gets all the subscription's IDs existing on a remote CEMonitor service; after invocation of this method, a vector of Subscription objects is filled up. By navigating this vector the user can retrieve information about each subscription stored in it.
       \sa Subscription
       \param endpoint A string containing the service's URL having the format: http[s]://<hostname>:<tcp_port>/ce-monitor/services/CEMonitor
       \param list A vector of Subscription objects that will be filled up by this method
       \throw GenericException&  When the CEMonitor service sends back to the client a generic fault;
       \throw ServiceNotFoundException&  When the user specified an un-existing service's address (see AbsRequest::setServiceURL() for the correct address format). This exception can be raised if the host is unreachable/non-existent, or if the connection is refused (e.g. wrong tcp port), or if the root-context of the remote container (TomCAT) is wrong;
       \throw SubscriptionException&  If the subscription failed for some other reason;
       \throw AuthenticationException When the authentication handshake failed in the remote service;
       \throw AuthorizationException When the user is not authorized to invoke the getEvent call;
       \throw GeneralException&  If some other kind of error occurred.
       
       All raised exceptions contain human readable information the user can get using the what() method.
    */
    
  void list(const std::string& endpoint, 
	    std::vector<Subscription>& list) throw(cemon_ex::GenericException&,
					      cemon_ex::SubscriptionException&,
					      cemon_ex::AuthenticationException&,
					      cemon_ex::AuthorizationException&,
					      cemon_ex::ServiceNotFoundException&,
					      cemon_ex::GeneralException&);

  /// Updates a subscription
  /**
     This method invokes the remote call Update on the remote CEMonitor. On success the same ID of the subscription that the user asked to update is returned, otherwise an exception is raised.
     \param endpoint The URL of the CEMon service keeping the subscription to update
     \param subscriptionID The string IDentifier of the subscription to update
     \param consumerURL The URL of the new consumer that will receive the notifications
     \param topic An object of the Topic class representing the new Topic that must send notifications (see Topic doc about how to build it)
     \param policy An object of the Policy class representing the new Policy that myst govern the notifications (see Policy doc about how to build it)
     \param expirationTime The timestamp (UTC seconds) of the expiration of this new subscription
     \return The subscription ID of the subscription that has been updated
     \sa Topic
     \sa Policy
     \throw GenericException&  When the CEMonitor service sends back to the client a generic fault;
     \throw TopicNotSupportedException&  When the user tried to update a subscription specifying an un-existing topic;
     \throw DialectNotSupportedException&  When the user tried to update a subscription using a topic that  specifies an unsupported dialect language;
     \throw ServiceNotFoundException&  When the user specified an un-existing service's address. This exception can be raised if the host is unreachable/non-existent, or if the connection is refused (e.g. wrong tcp port), or if the root-context of the remote container (TomCAT) is wrong;
     \throw SubscriptionException&  If the subscription update failed for some other reason;
     \throw AuthenticationException When the authentication handshake failed in the remote service;
     \throw AuthorizationException When the user is not authorized to invoke the getEvent call;
     \throw GeneralException&  If some other kind of error occurred.
	All raised exceptions contain human readable information the user can get using the what() method.
  */
    std::string update(const std::string& endpoint,
		       const std::string& subscriptionID,
		       const std::string& consumerURL,
		       Topic& topic,
		       Policy& policy,
		       const time_t& expirationTime
		       ) throw(cemon_ex::GenericException&,
			       cemon_ex::TopicNotSupportedException&, 
			       cemon_ex::DialectNotSupportedException&,
			       cemon_ex::SubscriptionException&,
			       cemon_ex::AuthenticationException&,
			       cemon_ex::AuthorizationException&,
			       cemon_ex::SubscriptionNotFoundException&,
			       cemon_ex::ServiceNotFoundException&,
			       cemon_ex::GeneralException&);
    


  /// Destroy the CESubscriptionMgr object
  /**
	Simply calls AbsRequest::DTOR that cleanly destroys the SOAP runtime
  */
  virtual ~CESubscriptionMgr() throw() {}
};
      }
    }
  }
}
#endif
