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
        CEInfo: a C++ class wrapping the GetInfo call

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CEInfo.cpp,v 1.8.8.2.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/CEInfo.h"
//#include "glite/ce/monitor-client-api-c/CEMON_CLIENT.nsmap"
#include <string>
#include <vector>

using namespace std;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;
namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;

/******************************************************************
 *
 * 
 *
 */
cemon_api::CEInfo::CEInfo( const string& certfile, const string& keyfile ) throw(cemon_ex::AuthenticationInitException&)
{
  this->authenticate( certfile.c_str(), keyfile.c_str() );
  this->init();
}

/******************************************************************
 *
 * 
 *
 */
void cemon_api::CEInfo::init() throw()
{
  resp.INFO_FIELD = &info;
}

/******************************************************************
 *
 * 
 *
 */
cemon_api::CEInfo::~CEInfo() throw()
{
}

/******************************************************************
 *
 * 
 *
 */
void cemon_api::CEInfo::getInfo() throw(cemon_ex::GenericException&, 
					cemon_ex::TopicNotSupportedException&, 
					cemon_ex::DialectNotSupportedException&,
					cemon_ex::ServiceNotFoundException&,
					cemon_ex::GeneralException&,
					cemon_ex::AuthenticationException&,
					cemon_ex::AuthorizationException&)
{  
  setNamespace();

  if(GETINFO(getSoap(), getServiceURL().c_str(), NULL, resp) != SOAP_OK) {
    this->setError();
    if(getFaultType()>0)
      throwCEException(getFaultType());
    else
      throwAbsException();
  }
}

/******************************************************************
 *
 * 
 *
 */
string cemon_api::CEInfo::getVersion() const throw()
{
  if(resp.INFO_FIELD)
    return resp.INFO_FIELD->serviceVersion.c_str();
  else
    return "";
}

/******************************************************************
 *
 * 
 *
 */
string cemon_api::CEInfo::getDescription() const throw()
{
  if(resp.INFO_FIELD && resp.INFO_FIELD->description)
    return resp.INFO_FIELD->description->c_str();
  return "";
}

/******************************************************************
 *
 * 
 *
 */
void cemon_api::CEInfo::getTopics(vector<Topic*> *tvec) const throw() {
  if(resp.INFO_FIELD) {
    for(vector<monitortypes__Topic*>::const_iterator tit = 
	  resp.INFO_FIELD->Topic.begin();
        tit != resp.INFO_FIELD->Topic.end();
	tit++) //unsigned int j=0; j<resp.INFO_FIELD->Topic.size(); j++)
      tvec->push_back((Topic*)new Topic((const Topic&)*(*tit)));
  }
}
/******************************************************************
 *
 *
 *
 */
void cemon_api::CEInfo::getActions(vector<ActionW*> *avec) const throw() {
  if(resp.INFO_FIELD) {
    //if(resp.INFO_FIELD->Action.size())
      for(vector<monitortypes__Action*>::const_iterator ait = resp.INFO_FIELD->Action.begin();
         ait != resp.INFO_FIELD->Action.end();
	 ait++) { //unsigned int j=0; j<resp.INFO_FIELD->Action.size(); j++) {
	ActionW *A = new ActionW((const ActionW&)*(*ait));
	avec->push_back(A);
      }
  }
}
/******************************************************************
 *
 *
 *
 */
string cemon_api::CEInfo::getDN( void ) throw() {
  if(!resp.INFO_FIELD) return "";
  for(vector<monitortypes__Property*>::const_iterator pit = resp.INFO_FIELD->property.begin();
      pit != resp.INFO_FIELD->property.end();
      ++pit)
  {
    if(*pit) {
      //cout << "property name=["<<(*pit)->name<<"]"<<endl;
      if((*pit)->name == "hostDN") return (*pit)->value;// this is hugly, I know. But was not me tha write the wsdl
                                                        // interface!
    }
  }
  return "";
}
