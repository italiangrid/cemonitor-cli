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

        Action: a wrapper class to handle actions

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: Action.h,v 1.14.10.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_ACTION_H__
#define __GLITE_CE_MONITOR_CLIENT_ACTION_H__

#include "glite/ce/monitor-client-api-c/monitor_client_CEMON_CLIENTStub.h"
#include "glite/ce/monitor-client-api-c/Parameter.h"
#include "glite/ce/monitor-client-api-c/Property.h"
#include <string>

#define ACTION monitortypes__Action

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace soap_proxy {

class CEEvent;
class CESubscription;
class Policy;
class CEInfo;

class ActionW : public ACTION {

/*   friend class CEEvent; */
/*   friend class CEInfo; */
/*   friend class CESubscription; */
/*   friend class Policy; */
  
  ActionW& operator=(const ActionW&) { return *this; }
  ActionW& operator=(const ACTION&) { return *this; }
  ActionW(const ACTION&) {}
  
 protected:
 
  void init(const ActionW&);

 public:
  ActionW(const std::string& Name, const std::string& type, const bool doActionWhenQueryIs);
  ActionW(const ActionW&);

  virtual ~ActionW(void);

  std::string  getActionName(void) const;
  std::string  getActionType(void) const;
  bool         getPerformAction(void) const;
  int          addParameter(ParameterW*);
  int          getNumberOfParameters(void) const;
  int          getNumberOfProperties(void) const;
  ParameterW*  getParameterAt(const int&) const;
  PropertyW*   getPropertyAt(const int&) const;
  //std::string  getJarPath(void) const;
  std::string  getActionID(void) const;
  void         print(void) const;
};
      }
    }
  }
}

#endif
