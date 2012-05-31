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
        CEEvent: a C++ class wrapping the getEvent call

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CEEvent.cpp,v 1.7.8.2.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/CEEvent.h"
#include "glite/ce/monitor-client-api-c/Topic.h"
#include "glite/ce/monitor-client-api-c/GeneralException.h"
#include "glite/ce/monitor-client-api-c/AuthenticationException.h"
//#include "glite/ce/monitor-client-api-c/CEMON_CLIENT.nsmap"
//#include "CEMON_CLIENTH.h"
#include <string>
#include <vector>
#include <list>
#include <time.h>

using namespace std;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;
namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;

/******************************************************************
 *
 * 
 *
 */
cemon_api::CEEvent::CEEvent( const string& certfile, const string& keyfile )
  throw(cemon_ex::AuthenticationInitException&) :
  m_topic(NULL),
  m_sizeMessage(0),
  m_messageCounter(0),
  m_retrievedInfo(false)
{
  this->authenticate( certfile.c_str(), keyfile.c_str() );
}

/******************************************************************
 *
 * Destroy the allocated pointer in the init() method
 *
 */
cemon_api::CEEvent::~CEEvent() throw()
{
 // cout << "Executing ~CEEvent" << endl;
  if(m_retrievedInfo)
    m_evtResp.Event = m_savedEvent;
  
 // cout << "~CEEvent executed" << endl;
}

/******************************************************************
 *
 * Set the URL of the remote CEMonitor service and the topic name 
 * of the event the user wants to retrieve.
 *
 * string _topic can be in 2 forms:
 * "TOPIC_NAME" specify simply a topic (without dialect)
 * "TOPIC_NAME:DIALECT" specify to ask a response formatted with
 * special dialect "DIALECT".
 *
 */
void cemon_api::CEEvent::setRequestParam(Topic* _topic) throw() {
  this->m_topic = _topic;
}

/******************************************************************
 *
 * Connect to the CEMonitor and get last event of the topic
 * set in the setRequestParam(...) methodpro
 *
 */
// void cemon_api::CEEvent::getEvent() throw(cemon_ex::GenericException&, 
// 					  cemon_ex::TopicNotSupportedException&, 
// 					  cemon_ex::DialectNotSupportedException&,
// 					  cemon_ex::ServiceNotFoundException&,
// 					  cemon_ex::GeneralException&,
// 					  cemon_ex::AuthenticationException&,
// 					  cemon_ex::AuthorizationException&) // these are all possibile ex raised by the 2 method throwCEException and thwoAbsException
// {
//   if( this->getServiceURL() == "" )
//     throw cemon_ex::GeneralException("Must set a remote service to connect to");

//   this->setNamespace();;//soap_set_namespaces(getSoap(), CEMON_CLIENT_namespaces);

//   if(soap_call_monitorws__GetEvent(getSoap(), getServiceURL().c_str(), NULL, m_topic, m_evtResp) != SOAP_OK)
//     {
//       this->setError();
//       int fault_type=getFaultType();
//       if(fault_type > 0)
//         throwCEException(fault_type);
//       else
//         throwAbsException();
//     }

//   if(!m_evtResp.Event) {
//     m_sizeMessage = 0;
//     throw cemon_ex::GeneralException("Null pointer Event. Probable de-serialization error");
//   }

//   m_sizeMessage = m_evtResp.Event->Message.size();
//   m_savedEvent = m_evtResp.Event;
//   m_messageCounter = 0;
//   m_retrievedInfo = true;
// }

/******************************************************************
 *
 * Connect to the CEMonitor and get last event of the topic
 * set in the setRequestParam(...) method
 *
 */
void 
cemon_api::CEEvent::getTopicEvents( list< vector<string> >& event_Mex ) throw(cemon_ex::GenericException&, 
									      cemon_ex::TopicNotSupportedException&, 
									      cemon_ex::DialectNotSupportedException&,
									      cemon_ex::ServiceNotFoundException&,
									      cemon_ex::GeneralException&,
									      cemon_ex::AuthenticationException&,
									      cemon_ex::AuthorizationException&) // these are all possibile ex raised by the 2 method throwCEException and thwoAbsException
{
  if( this->getServiceURL() == "" )
    throw cemon_ex::GeneralException("Must set a remote service to connect to");

  this->setNamespace();;//soap_set_namespaces(getSoap(), CEMON_CLIENT_namespaces);

  if(soap_call_monitorws__GetTopicEvent(getSoap(), getServiceURL().c_str(), NULL, m_topic, m_topicEvtResp) != SOAP_OK)
    {
      this->setError();
      int fault_type=getFaultType();
      if(fault_type > 0)
        throwCEException(fault_type);
      else
        throwAbsException();
    }

  if(!m_topicEvtResp.EventArray) {
    //m_sizeMessage = 0;
    throw cemon_ex::GeneralException("Null pointer Event. Probable de-serialization error");
  }

  for(vector<monitortypes__Event*>::iterator it = m_topicEvtResp.EventArray->Event.begin();
      it != m_topicEvtResp.EventArray->Event.end();
      ++it)
  {
    // *it is a pointer to a monitortypes__Event
    if( *it )
    {
      event_Mex.push_back( (*it)->Message );
    }
  }

  //m_sizeMessage = m_evtResp.Event->Message.size();
  //m_savedEvent = m_evtResp.Event;
  //m_messageCounter = 0;
  m_retrievedInfo = true;
}

/******************************************************************
 *
 * Connect to the CEMonitor and get last event of the topic
 * set in the setRequestParam(...) method
 *
 */
// bool CEEvent::nextEvent() 
// {
//  
//   if(!m_evtResp.Event) {
//     m_sizeMessage = 0;
//     return false;
//   }
// 
//   m_evtResp.Event++;
//   if(!m_evtResp.Event) {
//     m_evtResp.Event = m_savedEvent;
//     return false;
//   }
//   m_sizeMessage = m_evtResp.Event->Message.size();
//   m_messageCounter = 0;
//   m_retrievedInfo = true;
//   return true;
// }
/******************************************************************
 *
 * 
 *
 */
int cemon_api::CEEvent::getEventID() const throw()
{
  if(!m_retrievedInfo)
    return -1;
  if(m_evtResp.Event)
    return m_evtResp.Event->ID;
  else
    return -1;
}

/******************************************************************
 *
 * 
 *
 */
time_t cemon_api::CEEvent::getEventTimestamp() const throw()
{
  if(!m_retrievedInfo)
    return 0;
  if(m_evtResp.Event)
    return m_evtResp.Event->Timestamp;
  else
    return 0;
}

/******************************************************************
 *
 * 
 *
 */
string cemon_api::CEEvent::getEventDateTime() const throw()
{
  if(!m_retrievedInfo)
    return "";
  time_t _t = this->getEventTimestamp();
  return asctime( localtime( &_t ) );
}

/******************************************************************
 *
 * 
 *
 */
string cemon_api::CEEvent::getEventProducer() const throw()
{
  if(!m_retrievedInfo)
    return "";
  if(m_evtResp.Event)
    return (const char*)m_evtResp.Event->Producer.c_str();
  else
    return "";
}

/******************************************************************
 *
 * 
 *
 */
const char* cemon_api::CEEvent::getNextEventMessage() throw() {
  if(!m_retrievedInfo)
    return NULL;
  if(m_messageCounter > m_sizeMessage-1) {
    m_messageCounter = 0;
    return NULL;
  }
  if(m_evtResp.Event) {
    if(m_evtResp.Event->Message.size()) {
      cout << "m_messageCounter = " << m_messageCounter << endl;
      return (const char*)(m_evtResp.Event->Message.at(m_messageCounter++).c_str());
    }
    else
      return NULL;
  } else return NULL;
    
}

/******************************************************************
 *
 * 
 *
 */
string cemon_api::CEEvent::getLastEventMessage() const  throw() {
  if(!m_retrievedInfo)
    return NULL;

  if(m_evtResp.Event) {
    if(m_evtResp.Event->Message.size())
      return (const char*)(m_evtResp.Event->Message.at(m_evtResp.Event->Message.size()-1).c_str());
    else
      return NULL;
  } else return NULL;
}


/******************************************************************
 *
 * 
 *
 */
string cemon_api::CEEvent::getEventMessageAt(int idx) const  throw() {
  if(!m_retrievedInfo)
    return NULL;

  if( (idx < 0) || (idx > m_evtResp.Event->Message.size()-1) )
    {
      cerr << "Index out of boundary" << endl;
      return NULL;
    }

  if(m_evtResp.Event) {
    if(m_evtResp.Event->Message.size())
      return (const char*)(m_evtResp.Event->Message.at(idx).c_str());
    else
      return NULL;
  } else return NULL;
}

/******************************************************************
 *
 *
 *
 */
int cemon_api::CEEvent::getNumberOfMessages() const  throw()
{
  if(!m_retrievedInfo)
    return 0;
  return m_sizeMessage;
}
