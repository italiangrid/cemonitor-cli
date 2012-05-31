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
        Version info: $Id: CESubscriptionMgr.cpp,v 1.14.8.2.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/CESubscriptionMgr.h"
//#include "glite/ce/monitor-client-api-c/Topic.h"
//#include "glite/ce/monitor-client-api-c/Dialect.h"
//#include "glite/ce/monitor-client-api-c/Policy.h"
//#include "glite/ce/monitor-client-api-c/CEMON_CLIENT.nsmap"
#include <boost/scoped_ptr.hpp>

using namespace std;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;
namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;

/******************************************************************
 *
 * Class Constructor with authentication
 *
 */
cemon_api::CESubscriptionMgr::CESubscriptionMgr(const string& certfile,
						const string& keyfile) 
  throw(cemon_ex::AuthenticationInitException&)
  : subids()
{
  this->authenticate( certfile.c_str(), keyfile.c_str() );
}

//______________________________________________________________________________
void cemon_api::CESubscriptionMgr::list(const string& endpoint, vector<Subscription>& target) 
  throw(cemon_ex::GenericException&,
	cemon_ex::SubscriptionException&,
	cemon_ex::AuthenticationException&,
	cemon_ex::AuthorizationException&,
	cemon_ex::ServiceNotFoundException&,
	cemon_ex::GeneralException&)
{
  //monitortypes__SubscriptionRefList *ptr = new monitortypes__SubscriptionRefList();
  boost::scoped_ptr< monitortypes__SubscriptionRefList > ptr( new monitortypes__SubscriptionRefList() );
  monitorws__GetSubscriptionResponse resp;
  monitortypes__SubscriptionList* list;

  setNamespace();
  if(SOAP_OK==soap_call_monitorws__GetSubscription(getSoap(),
						   endpoint.c_str(),
						   "http://glite.org/ce/monitorapij/ws/GetSubscriptionInfo",
						   ptr.get(),
						   resp))
    {
      list = resp.SubscriptionList;
      if(list)
	{
	  for(vector<monitortypes__Subscription*>::const_iterator sit = list->subscription.begin();
	      sit != list->subscription.end();
	      sit++) //unsigned j=0; j < list->subscription.size(); j++)
	    {
	      if((*sit)->id &&
		 (*sit)->ExpirationTime &&
		 (*sit)->Topic)
		target.push_back(Subscription(*((*sit)->id),
					      *((*sit)->ExpirationTime),
					      (*sit)->Policy->Rate,
					      (*sit)->MonitorConsumerURL,
					      (*sit)->Topic->Name,
					      endpoint));
	      else
		throw cemon_ex::GeneralException("id or ExpirationTime or Topic are NULL pointer");
	    }
	} else throw cemon_ex::GeneralException("Received NULL SubscriptionList pointer");
    } else {
    this->setError();
    if(getFaultType()>0)
      throwCEException(getFaultType());
    else
      throwAbsException();
  }
  
}

//______________________________________________________________________________
string cemon_api::CESubscriptionMgr::update(const string& endpoint,
					    const string& subid,
					    const string& newConsumerURL,
					    cemon_api::Topic& topic,
					    cemon_api::Policy& policy,
					    const time_t& t
					    ) 
  throw(cemon_ex::GenericException&,
	cemon_ex::TopicNotSupportedException&, 
	cemon_ex::DialectNotSupportedException&,
	cemon_ex::SubscriptionException&,
	cemon_ex::AuthenticationException&,
	cemon_ex::AuthorizationException&,
	cemon_ex::SubscriptionNotFoundException&,
	cemon_ex::ServiceNotFoundException&,
	cemon_ex::GeneralException&)
{
  monitorws__UpdateResponse resp;
  monitortypes__Subscription sub;
  sub.id = new string(subid);

  boost::scoped_ptr< string > ptrString;
  ptrString.reset( sub.id );

  sub.ExpirationTime = new time_t();

  boost::scoped_ptr< time_t > ptrTime;
  ptrTime.reset( sub.ExpirationTime );

  *(sub.ExpirationTime) = t;
  sub.MonitorConsumerURL = newConsumerURL;
  sub.name = NULL;
  sub.type = NULL;
  sub.jarPath = NULL;

  sub.Topic = &topic;

  sub.Policy = &policy;  
  setNamespace();
  if(SOAP_OK==soap_call_monitorws__Update(getSoap(),
					  endpoint.c_str(), 
					  "http://glite.org/ce/monitorapij/ws/Update", 
					  &sub, 
					  resp))
    {
      return resp.SubscriptionRef->SubscriptionID;
    } else {
    this->setError();
    if(getFaultType()>0)
      throwCEException(getFaultType());
    else
      throwAbsException();
  }
  return "";  // just to silent a warning; a meaningfull string is returned above.
}

