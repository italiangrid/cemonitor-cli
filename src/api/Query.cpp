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
        Query: a wrapper class to handle Query

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: Query.cpp,v 1.3.10.1 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/Query.h"
#include <string>

using namespace std;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;

cemon_api::QueryW::QueryW(const std::string& expr, const std::string& qlang)
{
  Expression = expr;
  QueryLanguage = qlang;
}

cemon_api::QueryW::QueryW()
{
  Expression = "";
  QueryLanguage = "";
}

// QueryW::QueryW(const QUERY& aQ)
// {
//   Expression = aQ.Expression;
//   QueryLanguage = aQ.QueryLanguage;
// }

// QueryW& QueryW::operator=(const QueryW& aQ) {
//   if(this!=&aQ) {
//     Expression = aQ.Expression;
//     QueryLanguage = aQ.QueryLanguage;
//   }
//   return *this;
// }

cemon_api::QueryW::QueryW(const cemon_api::QueryW& aQ)
{
  Expression = aQ.Expression;
  QueryLanguage = aQ.QueryLanguage;
}

inline string cemon_api::QueryW::getQueryLanguage(void) const {
   return QueryLanguage;
}

inline string cemon_api::QueryW::getExpression(void) const {

   return Expression;
}

void cemon_api::QueryW::setExpression(const string& expr) {
   Expression = expr;
}

void cemon_api::QueryW::setQueryLanguage(const string& qlang) {
   QueryLanguage = qlang;
}
