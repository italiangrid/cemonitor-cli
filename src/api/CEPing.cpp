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
        CEPing: a C++ class wrapping the GetInfo call

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CEPing.cpp,v 1.5.8.1.2.1 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/CEPing.h"
#include <string>

using namespace std;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;
namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;

/******************************************************************
 *
 * 
 *
 */
cemon_api::CEPing::CEPing(const string& certfile, const string& keyfile) throw(cemon_ex::AuthenticationInitException&)
{
    this->authenticate( certfile.c_str(), keyfile.c_str());
}

/******************************************************************
 *
 * 
 *
 */
bool cemon_api::CEPing::Ping() throw()
{  
  setNamespace();
  if(PING(getSoap(), getServiceURL().c_str(), NULL, pingResp) != SOAP_OK)
    return false;
  else
    return true;
}
