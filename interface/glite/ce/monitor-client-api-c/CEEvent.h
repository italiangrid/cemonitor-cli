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
        CEEvent: header file for a C++ class wrapping the Ping call

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CEEvent.h,v 1.38.8.3.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_CEEVENT_H__
#define __GLITE_CE_MONITOR_CLIENT_CEEVENT_H__

#include "glite/ce/monitor-client-api-c/AbsRequest.h"
#include "glite/ce/monitor-client-api-c/Topic.h"
#include <string>
#include <list>
#include <vector>

//#define GETEVENTRESP monitorws__GetEventResponse
//#define GETEVENTCALL soap_call_monitorws__GetEvent
//#define EVENT monitortypes__Event
//#define EVENT_FIELD Event



namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;

namespace glite {
namespace ce {
namespace monitor_client_api {
namespace soap_proxy {

/// Wrapper class for getEvent request
/**
   This class inherits from AbsRequest and wraps the getEvent remote call, hiding the user from any communication and SOAP detail.\n
   It can be used for multiple getEvent calls (remember to use the ::cleanup() method between 2 subsequent calls)
*/
class CEEvent : public AbsRequest {
  
 private:
  TOPIC                           *m_topic;

  monitorws__GetEventResponse      m_evtResp;
  monitorws__GetTopicEventResponse m_topicEvtResp;
  
  monitortypes__Event             *m_savedEvent;
  int                              m_sizeMessage, m_messageCounter;
  bool                             m_retrievedInfo;

 protected:

  CEEvent(const CEEvent&) {}
  CEEvent& operator=(const CEEvent&) { return *this; }

 public:



  /// Creates a CEEvent object
  /**
	Creates a CEEvent object and initializes the SOAP runtime for SSL authentication.
        \param certfile  The filename of the user's proxy certificate.
      	\param certpath  The name of the directory containing the certification authority's files for reverse AuthN. If these certificates are not present the simple "/" path can be used instead.

	\throw AuthenticationInitException  If the certfile and/or certpath are missing on disk or not readable/accessible, or if the user's proxy certificate is corrupted or not valid for some reason. cemon_ex::AuthenticationInitException::what() gives the user the human readable cause of the exception.
  */
   CEEvent(const std::string& certfile, const std::string& keyfile) throw(cemon_ex::AuthenticationInitException&);   


  /// Destroys a CEEvent object
  /**
	Simply calls AbsRequest::~AbsRequest that cleanly destroys the SOAP runtime
  */
  virtual ~CEEvent(void) throw();



  /// Sets the requested event's parameter
  /**
	The CEMonitor getEvent remote call returns an event generated by a certain sensor. The specification of the sensor the user wants to query is made by creating a Topic object and passing it as argument of this method. This method must be called before the first call of CEEvent::getEvent(), and can be called again before subsequent calls of CEEvent::getEvent().
	\sa Topic
	\param topic  The topic (sensor) generating the events the user need to query
  */
  void setRequestParam(Topic *topic) throw();



  /// Invokes the remote call getEvent
  /**
	Invokes the CEMonitor remote call getEvent and put the received event into an internal array of strings that the user can browse by using CEEvent::getNextEventMessage().

	\sa CEEvent::getNextEventMessage()
	\throw GenericException&  When the CEMonitor service sends back to the client a generic fault;
	\throw TopicNotSupportedException&  When the user specified a not existing topic name (see CETopics to query all supported Topics by the CEMonitor installation);
	\throw DialectNotSupportedException&  When the user asked for an event with an unsupported dialect formatting language;
	\throw ServiceNotFoundException&  When the user specified an un-existing service's address (see AbsRequest::setServiceURL() for the correct address format). This exception can be raised if the host is unreachable/non-existent, or if the connection is refused (e.g. wrong tcp port), or if the root-context of the remote container (TomCAT) is wrong;
	\throw AuthenticationException When the authentication handshake failed in the remote service;
	\throw AuthorizationException When the user is not authorized to invoke the getEvent call;
	\throw GeneralException&  If some other kind of error occurred.

	All raised exceptions contain human readable information the user can get using the method ::what().
  */
//   void getEvent(void) throw(cemon_ex::GenericException&,
// 			    cemon_ex::TopicNotSupportedException&,
// 			    cemon_ex::DialectNotSupportedException&,
// 			    cemon_ex::ServiceNotFoundException&,
// 			    cemon_ex::GeneralException&,
// 			    cemon_ex::AuthenticationException&,
// 			    cemon_ex::AuthorizationException&) ;


  /// Invokes the remote call getTopicEvent
  /**
	Invokes the CEMonitor remote call getTopicEvent for the selected Topic and put the received information into an internal array of strings that the user can browse by using CEEvent::getNextEventMessage().

	\sa CEEvent::getNextEventMessage()
	\throw GenericException&  When the CEMonitor service sends back to the client a generic fault;
	\throw TopicNotSupportedException&  When the user specified a not existing topic name (see CETopics to query all supported Topics by the CEMonitor installation);
	\throw DialectNotSupportedException&  When the user asked for an event with an unsupported dialect formatting language;
	\throw ServiceNotFoundException&  When the user specified an un-existing service's address (see AbsRequest::setServiceURL() for the correct address format). This exception can be raised if the host is unreachable/non-existent, or if the connection is refused (e.g. wrong tcp port), or if the root-context of the remote container (TomCAT) is wrong;
	\throw AuthenticationException When the authentication handshake failed in the remote service;
	\throw AuthorizationException When the user is not authorized to invoke the getEvent call;
	\throw GeneralException&  If some other kind of error occurred.

	All raised exceptions contain human readable information the user can get using the method ::what().
  */
  void getTopicEvents(std::list< std::vector<std::string> >& ) throw(cemon_ex::GenericException&,
								     cemon_ex::TopicNotSupportedException&,
								     cemon_ex::DialectNotSupportedException&,
								     cemon_ex::ServiceNotFoundException&,
								     cemon_ex::GeneralException&,
								     cemon_ex::AuthenticationException&,
								     cemon_ex::AuthorizationException&) ;



  /// Gets the ID of last event
  /**
	Returns the ID (integer) of last retrieved event.
	\return The integer ID of last event
  */
  int getEventID() const throw();


  
  /// Gets the timestamp of last event
  /**
	Returns the timestamp (expressed as an integer indicating the UTC time) of last retrieved event.
	\return The last event's timestamp in UTC
  */
  time_t getEventTimestamp() const throw();



  /// Gets the timestamp of last event
  /**
	Returns the human readable timestamp of last retrieved event.
	\return A string containig the last event's timestamp in human readable format
  */
  std::string getEventDateTime() const throw();



  /// Gets the event's producer name
  /**
	Returns the name of the sensor that produced last retrieved event 
	\return The name of the last event producer
  */
  std::string getEventProducer() const throw();



  /// Returns the next event message
  /**
	This method returns the next event message stored in the array that has been
	filled up by last invocation of CEEvent::getTopicEvents(); the size of the array
	is given by CEEvent::getNumberOfMessages(). When the last message is reached, a subsequent call
	of this method returns NULL.
	\return A pointer to a char array containing the next message in the internal array
  */
  const char* getNextEventMessage() throw();



  /// Returns the last event message
  /**
	Returns the last event message stored in the internal array filled up by last
	invocation of the CEEvent::getTopicEvents() method.
	\return A string containing the last event in the internal array.
  */
  std::string getLastEventMessage() const throw() ;



  /// Returns a particular message
  /**
	Returns the message at the position 'index' in the internal array.
	\param index identifying the position of the message the user need to retrieve
	\return A string containing the message at 'index' position.
  */
  std::string getEventMessageAt(int index) const throw();



  /// Returns the number of event messages
  /**
	Returns the number of messages stored in the internal array filled up by last 
	invocation of the CEEvent::getTopicEvents() method.
	\return The number of messages contained in the internal array.
  */
  int         getNumberOfMessages() const throw();
};
}
}
}
}
#endif
