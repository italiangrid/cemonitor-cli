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
        CETopics: a C++ class wrapping the GetTopics call

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CETopics.cpp,v 1.6.8.3.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/CETopics.h"
#include <string>
#include <vector>
//#include "glite/ce/monitor-client-api-c/CEMON_CLIENT.nsmap"
using namespace std;

namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;

/******************************************************************
 *
 * 
 *
 */
cemon_api::CETopics::CETopics( const string& certfile, const string& keyfile ) 
  throw(cemon_ex::AuthenticationInitException&) 
  : retrievedTopics(false),sizeTopic(0),topicCounter(0)
{
  this->authenticate( certfile.c_str(), keyfile.c_str() );
}

/******************************************************************
 *
 * 
 *
 */
cemon_api::CETopics::~CETopics() throw()
{
}

/******************************************************************
 *
 * get next topic from the list of retrieved ones
 *
 */
void cemon_api::CETopics::getTopicArray(vector<Topic*> *tvec) const throw()
{
  // this method fills up the user passes vector of Topic*
  // The user will delete each element of his vector
  if(topicResp.TOPICARRAY_FIELD != NULL) {
//    if(topicResp.TOPICARRAY_FIELD->Topic.size()) {
    for(vector<monitortypes__Topic*>::const_iterator tit = topicResp.TOPICARRAY_FIELD->Topic.begin();
        tit != topicResp.TOPICARRAY_FIELD->Topic.end();
	tit++) { //unsigned int j=0; j<topicResp.TOPICARRAY_FIELD->Topic.size(); j++) {
      cemon_api::Topic *t = new cemon_api::Topic((const cemon_api::Topic&)*(*tit));
      tvec->push_back(t);
    }
//    }
  }
}

/******************************************************************
 *
 * get the number of retrieved topics
 *
 */
int cemon_api::CETopics::getNumberOfTopics() const throw()
{
  return sizeTopic;
}

/******************************************************************
 *
 * send the request to retrieve topics
 *
 */
void cemon_api::CETopics::getTopics() 
  throw(cemon_ex::GenericException&, 
	cemon_ex::ServiceNotFoundException&,
	cemon_ex::GeneralException&,
	cemon_ex::AuthenticationException&,
	cemon_ex::AuthorizationException&) 
{
  retrievedTopics = false;
  sizeTopic = 0;  
  setNamespace();
  if(GETTOPIC(getSoap(), getServiceURL().c_str(), NULL, topicResp) == SOAP_OK) {
    if(topicResp.TOPICARRAY_FIELD) {
      sizeTopic = topicResp.TOPICARRAY_FIELD->Topic.size();
      retrievedTopics = true;
    } else
      throw cemon_ex::GeneralException("GetTopicResponse->topicArray is NULL! Probable de-serialization error"); 
  } else {
    this->setError();
    if(getFaultType()>0)
      throwCEException(getFaultType());
    else
      throwAbsException();
  }
}
