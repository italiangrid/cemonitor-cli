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
        ParameterW: a wrapper class to handle parameters for actions

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: Parameter.cpp,v 1.2.10.1 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/Parameter.h"
#include <string>

using namespace std;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;

cemon_api::ParameterW::ParameterW(const PARAMETER& p) {
   name  = p.name;
   value = p.value;
}

cemon_api::ParameterW::ParameterW(const string& name, const string& value) 
{
   this->name  = name;
   this->value = value;
}

cemon_api::ParameterW& cemon_api::ParameterW::operator=(const ParameterW& p)
{
  if(this!=&p) {
    name  = p.name;
    value = p.value;
  }
  return *this;
}

inline string cemon_api::ParameterW::getParameterName(void) const {
   return name;
}

inline string cemon_api::ParameterW::getParameterValue(void) const {
   return value;
}

void cemon_api::ParameterW::setParameterName(const string& newName) {
   name = newName;
}

void cemon_api::ParameterW::setParameterValue(const string& newValue) {
   value = newValue;
}
