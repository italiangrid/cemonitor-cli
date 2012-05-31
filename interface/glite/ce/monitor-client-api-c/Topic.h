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
        Topic: a class to handle topic

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: Topic.h,v 1.15.10.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_TOPIC_H__
#define __GLITE_CE_MONITOR_CLIENT_TOPIC_H__

#include "glite/ce/monitor-client-api-c/monitor_client_CEMON_CLIENTStub.h"
#include "glite/ce/monitor-client-api-c/Dialect.h"
#include <string>

#define TOPIC monitortypes__Topic

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace soap_proxy {

class CEEvent;
class CESubscription;
class CETopics;

class Topic : public TOPIC {

/*   friend class CEEvent; */
/*   friend class CEInfo; */
/*   friend class CESubscription; */
/*   friend class CETopics; */
/*   friend class CEConsumer; */
/*   friend class CEConsumerP; */

  Topic& operator=(const Topic&) { return *this; }
  Topic& operator=(const TOPIC&) { return *this; }
  Topic(const TOPIC&) {}

 protected:
  

 public:
  Topic(const std::string&);
  Topic(const Topic&);

  virtual ~Topic();

  int addDialect(DialectW*);
  DialectW* getDialectAt(const int&);
  int getNumOfDialects(void) const;
  std::vector<DialectW*> getDialects(void) const;
  void print(void) const;
};
      }
    }
  }
}
#endif
