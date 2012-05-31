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
        Dialect: a wrapper class to handle dialects

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: Dialect.h,v 1.14.10.1 2010/03/22 12:11:11 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_DIALECT_H__
#define __GLITE_CE_MONITOR_CLIENT_DIALECT_H__

#include "glite/ce/monitor-client-api-c/monitor_client_CEMON_CLIENTStub.h"
#include <string>

namespace glite {
namespace ce {
namespace monitor_client_api {
namespace soap_proxy {

class DialectW : public monitortypes__Dialect {

  DialectW& operator=(const DialectW&) {return *this; }
  DialectW& operator=(const monitortypes__Dialect&) {return *this; }
  DialectW( const monitortypes__Dialect&) {}

 protected:
  
 public:
  DialectW(const std::string& name);
  DialectW(const DialectW&);

  virtual ~DialectW(void) throw();

  int         getNumberOfQueryLanguages(void) const;
  std::string getQueryLanguageAt(const int&) const;
  std::string getDialectName(void) const;
  void        setDialectName(const std::string&);
  void        setQueryLanguageAt(const int&, const std::string&);
  int         addQueryLanguage(const std::string);
  void        print(void);
};
}
}
}
}
#endif
