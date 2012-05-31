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
        CESubscription: a C++ class wrapping the subscription (and related) call

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CESubscription.cpp,v 1.15.4.1.4.3.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/CESubscription.h"
#include "glite/ce/monitor-client-api-c/Topic.h"
#include "glite/ce/monitor-client-api-c/Policy.h"
#include "glite/ce/monitor-client-api-c/GeneralException.h"
#include <string>
#include <ctime>
#include <iostream>
#include <boost/scoped_ptr.hpp>

#define SUB_TOPIC_FIELD Topic
#define SUB_POLICY_FIELD Policy

using namespace std;
namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;

/******************************************************************
 *
 * Class Constructor with authentication
 *
 */
cemon_api::CESubscription::CESubscription(const string& certfile,
					  const string& keyfile) 
  throw(cemon_ex::AuthenticationInitException&)
  : subscriptionID(""),
    topic(NULL),
    policy(NULL),
    consumerURL(""), 
    duration(0)
{
    this->authenticate( certfile.c_str(), keyfile.c_str());
}

/******************************************************************
 *
 * Class Constructor
 *
 */
cemon_api::CESubscription::CESubscription( ) throw()
  : subscriptionID(""),
    topic(NULL),
    policy(NULL),
    consumerURL(""),
    duration(0)
{
}

/******************************************************************
 *
 * Class Destructor
 *
 */
cemon_api::CESubscription::~CESubscription() throw() {
  //cout << "CESubscription::~CESubscription - freeing topic..." << endl;
  if(topic) delete topic;
  if(policy) delete policy;
}

/******************************************************************
 *
 * Sends a subscribe request for consumer set by setRequestParam(...) method
 *
 */
void cemon_api::CESubscription::subscribe() 
  throw(cemon_ex::GenericException&,
	cemon_ex::TopicNotSupportedException&, 
	cemon_ex::DialectNotSupportedException&,
	cemon_ex::SubscriptionException&,
	cemon_ex::AuthenticationException&,
	cemon_ex::AuthorizationException&,
	cemon_ex::ServiceNotFoundException&,
	cemon_ex::GeneralException&)
{
  if(consumerURL == "")
    throw cemon_ex::GeneralException("Consumer URL not set");

  SUBSCRIPTION sub;
  sub.MonitorConsumerURL = (char *)consumerURL.c_str();
  sub.ExpirationTime = (time_t*)malloc(sizeof(time_t));

  boost::scoped_ptr< time_t > ptrTimeExp;
  ptrTimeExp.reset( sub.ExpirationTime );

  *(sub.ExpirationTime) = (time_t)time(NULL) + (time_t)this->duration;

  sub.SUB_TOPIC_FIELD = topic;
  sub.SUB_POLICY_FIELD= policy;
  sub.id = NULL;
  sub.name = NULL;
  sub.type = NULL;
  sub.jarPath = NULL;
  

  SUBSCRIBERESPONSE subResp;
  subResp.SUBSCRIPTIONREF_FIELD = new SUBSCRIPTIONREF();
  boost::scoped_ptr< monitortypes__SubscriptionRef > ptrSubRef;//( new SUBSCRIPTIONREF() );
  ptrSubRef.reset( subResp.SUBSCRIPTIONREF_FIELD );

  subResp.SUBSCRIPTIONREF_FIELD->ExpirationTime = (time_t*)malloc(sizeof(time_t));

  boost::scoped_ptr< time_t > ptrTime;//( new SUBSCRIPTIONREF() );
  ptrTime.reset( subResp.SUBSCRIPTIONREF_FIELD->ExpirationTime );

  setNamespace();//soap_set_namespaces(getSoap(), CEMON_CLIENT_namespaces);

  int ret = SUBSCRIBE(getSoap(), getServiceURL().c_str(), "http://glite.org/ce/monitorapij/ws/Subscribe",
               &sub, subResp);

  if ( ret == SOAP_OK) {
    if(subResp.SUBSCRIPTIONREF_FIELD)
      subscriptionID = subResp.SUBSCRIPTIONREF_FIELD->SubscriptionID;
    else
      throw cemon_ex::GeneralException(string("Serialization Error: subscription reference field of SOAP structure is NULL"));
      
  }
  else {
    this->setError();
    subscriptionID = "";
    if(getFaultType()>0)
      throwCEException(getFaultType());
    else
      throwAbsException();
  }
}

/******************************************************************
 *
 * Sends a un-subscribe request for a particular 
 * SubscriptionID specified as an argument
 *
 */
void cemon_api::CESubscription::unsubscribe(const string& subID) 
  throw(cemon_ex::GenericException&,
	cemon_ex::SubscriptionException&,
	cemon_ex::AuthenticationException&,
	cemon_ex::AuthorizationException&,
	cemon_ex::SubscriptionNotFoundException&,
	cemon_ex::ServiceNotFoundException&,
	cemon_ex::GeneralException&)
{
  if(this->getServiceURL() == "")
    throw cemon_ex::GeneralException("Service URL not set");
  
  SUBSCRIPTIONREF     subRef;
  subRef.ExpirationTime = (time_t*)malloc(sizeof(time_t));
  boost::scoped_ptr< time_t > ptrTime;
  ptrTime.reset( subRef.ExpirationTime );
  

  *(subRef.ExpirationTime) = 0;
  UNSUBSCRIBERESPONSE unsubResp;
  subRef.SubscriptionID = subID;

  setNamespace();//soap_set_namespaces(getSoap(), CEMON_CLIENT_namespaces);

  if(UNSUBSCRIBE(getSoap(), getServiceURL().c_str(), "http://glite.org/ce/monitorapij/ws/Unubscribe", 
		 &subRef, unsubResp) != SOAP_OK) 
    {
      this->setError();
      if(getFaultType()>0)
	throwCEException(getFaultType());
      else
	throwAbsException();
    }
  //free(subRef.ExpirationTime);
}

/******************************************************************
 *
 * Sends a pause request for a particular
 * SubscriptionID specified as an argument
 *
 */
void cemon_api::CESubscription::pause(const string& subID) 
  throw(cemon_ex::GenericException&,
	cemon_ex::SubscriptionException&,
	cemon_ex::AuthenticationException&,
	cemon_ex::AuthorizationException&,
	cemon_ex::SubscriptionNotFoundException&,
	cemon_ex::ServiceNotFoundException&,
	cemon_ex::GeneralException&)
{
  if(getServiceURL().compare("") == 0)
    throw cemon_ex::GeneralException("Service URL not set");

  SUBSCRIPTIONREF     subRef;
  subRef.SubscriptionID = subID;
  subRef.ExpirationTime = (time_t*)malloc(sizeof(time_t));

  boost::scoped_ptr< time_t > ptrTime;
  ptrTime.reset( subRef.ExpirationTime );

  *(subRef.ExpirationTime) = 0;
  PAUSESUBRESPONSE pauseResp;

  setNamespace();//soap_set_namespaces(getSoap(), CEMON_CLIENT_namespaces);

  if(PAUSESUB(getSoap(), getServiceURL().c_str(), "http://glite.org/ce/monitorapij/ws/PauseSubscription",
	      &subRef, pauseResp) != SOAP_OK)
    {
      this->setError();
      if(getFaultType()>0)
	throwCEException(getFaultType());
      else
	throwAbsException();
    }
  //free(subRef.ExpirationTime);
}

/******************************************************************
 *
 * Sends a resume request for a particular 
 * SubscriptionID specified as an argument
 *
 */
void cemon_api::CESubscription::resume(const string& subID) 
  throw(cemon_ex::GenericException&,
	cemon_ex::SubscriptionException&,
	cemon_ex::AuthenticationException&,
	cemon_ex::AuthorizationException&,
	cemon_ex::SubscriptionNotFoundException&,
	cemon_ex::ServiceNotFoundException&,
	cemon_ex::GeneralException&)
{
  if(getServiceURL().compare("") == 0)
    throw cemon_ex::GeneralException("Service URL not set");

  SUBSCRIPTIONREF     subRef;
  subRef.SubscriptionID = subID;
  subRef.ExpirationTime = (time_t*)malloc(sizeof(time_t));

  boost::scoped_ptr< time_t > ptrTime;
  ptrTime.reset( subRef.ExpirationTime );

  *(subRef.ExpirationTime) = 0;
  RESUMESUBRESPONSE resumeResp;

  setNamespace();//soap_set_namespaces(getSoap(), CEMON_CLIENT_namespaces);

  if(RESUMESUB(getSoap(), getServiceURL().c_str(), "http://glite.org/ce/monitorapij/ws/ResumeSubscription", 
	       &subRef, resumeResp) != SOAP_OK) 
    {
      this->setError();
      if(getFaultType()>0)
	throwCEException(getFaultType());
      else
	throwAbsException();
    }
  //free(subRef.ExpirationTime);
}

/******************************************************************
 *
 * Returns the subscription ID after a succesfull subscribe() call
 *
 */
string cemon_api::CESubscription::getSubscriptionID() const throw()
{
  return subscriptionID;
}

/******************************************************************
 *
 * 
 *
 */
void cemon_api::CESubscription::setSubscribeParam(const string& consumer,
				       const Topic& _topic,
				       const Policy& _policy,
				       const long duration
				       ) throw()
{

  if(this->topic) {delete(this->topic); this->topic=NULL; }

  if(this->policy) {delete(this->policy); this->policy=NULL; }

  consumerURL = consumer;
  this->topic  = new Topic(_topic);
  this->policy = new Policy(_policy);
  this->duration = duration;

}
