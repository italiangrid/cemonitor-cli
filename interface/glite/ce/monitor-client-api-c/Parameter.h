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
        Parameter: a class to handle parameters for actions

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: Parameter.h,v 1.12.10.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_PARAMETER_H__
#define __GLITE_CE_MONITOR_CLIENT_PARAMETER_H__

#include "glite/ce/monitor-client-api-c/monitor_client_CEMON_CLIENTStub.h"
#include <string>
#include <iostream>

#define PARAMETER monitortypes__Parameter

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace soap_proxy {

class CEEvent;
class CESubscription;
class ActionW;

class ParameterW : public PARAMETER {

/*    friend class CEEvent; */
/*    friend class CESubscription; */
/*    friend class ActionW; */
   
/* private: */
/*    PARAMETER param; */
   
protected:
   ParameterW(const PARAMETER&);
   ParameterW& operator=(const ParameterW&);

public:
   ParameterW(const std::string& p, const std::string& pval);
   virtual ~ParameterW() {/*std::cerr<<"~ParameterW..."<<std::endl;*/}
   std::string getParameterName(void) const;
   std::string getParameterValue(void) const;
   void        setParameterName(const std::string& name);
   void        setParameterValue(const std::string& val);
};

      }
    }
  }
}
#endif
