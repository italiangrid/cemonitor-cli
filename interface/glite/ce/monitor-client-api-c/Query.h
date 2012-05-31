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
        Query: a class to handle Query

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: Query.h,v 1.11.10.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_QUERY_H__
#define __GLITE_CE_MONITOR_CLIENT_QUERY_H__

#include "glite/ce/monitor-client-api-c/monitor_client_CEMON_CLIENTStub.h"
#include <string>

#define QUERY monitortypes__Query

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace soap_proxy {

class Policy;

class QueryW : public QUERY {

  //friend class Policy;

 protected:
  QueryW(const QUERY&) {}
  QueryW& operator=(const QueryW&) { return *this; }
  QueryW& operator=(const QUERY&) { return *this; }

public:
   QueryW();
   QueryW(const std::string&, const std::string&);
   QueryW(const QueryW&);

   virtual ~QueryW() {}

   std::string getExpression(void) const;
   std::string getQueryLanguage(void) const;
   void        setExpression(const std::string&);
   void        setQueryLanguage(const std::string&);
};
      }
    }
  }
}

#endif
