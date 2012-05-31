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
        Policy: a class to handle policy

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: Policy.h,v 1.11.10.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_POLICY_H__
#define __GLITE_CE_MONITOR_CLIENT_POLICY_H__

#include "glite/ce/monitor-client-api-c/monitor_client_CEMON_CLIENTStub.h"
#include "glite/ce/monitor-client-api-c/Action.h"
#include "glite/ce/monitor-client-api-c/Query.h"
#include <vector>

#define POLICY monitortypes__Policy

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace soap_proxy {

class CESubscription;

// class SOAP_CMAC monitortypes__Policy
// {
// public:
//         monitortypes__Query *Query;     /* optional element of type monitortypes:Query */
//         std::vector<monitortypes__Action * >Action;     /* optional element of type monitortypes:Action */
//         int Rate;       /* required element of type xsd:int */
//         struct soap *soap;      /* transient */
// public:
//         virtual int soap_type() const { return 26; } /* = unique id SOAP_TYPE_monitortypes__Policy */
//         virtual void soap_default(struct soap*);
//         virtual void soap_serialize(struct soap*) const;
//         virtual int soap_put(struct soap*, const char*, const char*) const;
//         virtual int soap_out(struct soap*, const char*, int, const char*) const;
//         virtual void *soap_get(struct soap*, const char*, const char*);
//         virtual void *soap_in(struct soap*, const char*, const char*);
//                  monitortypes__Policy() { }
//         virtual ~monitortypes__Policy() { }
// };

class Policy : public POLICY {
  
  //friend class CESubscription;
  Policy(const POLICY&) {}
  Policy& operator=(const Policy&) { return *this; }
  Policy& operator=(const POLICY&) { return *this; }

 protected:
  //  Policy(const POLICY&);

 public:
  Policy(const int& rate);
  Policy(const Policy&);

  virtual ~Policy();

  int addAction_Query(ActionW*, QueryW*);
  int addAction(ActionW*);
  void setQuery(QueryW*);
  ActionW* getActionAt(const int& index);
  void print(void) const;
};
      }
    }
  }
}
#endif
