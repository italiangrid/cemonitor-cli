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
        CEException: a C++ Exception related to any CE-generated fault
        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CEException.h,v 1.16.10.1.4.1 2011/12/07 10:12:00 pandreet Exp $
*/

#ifndef __GLITE_CE_MONITOR_CLIENT_CEEXCEPTION_H__
#define __GLITE_CE_MONITOR_CLIENT_CEEXCEPTION_H__

#include <string>
#include <time.h>
#include <sstream>
#include <exception>
#include <fstream>

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace cemon_exceptions {
	
	class CEException : public std::exception
	  {
	    
	  private:
	    std::string MethodName;
	    std::string ErrorCode;
	    std::string Description;
	    std::string FaultCause;
	    time_t Timestamp;
	    
	  public:
	    
	    /*   enum { */
	    /*     Subscription = 23, */
	    /*     TopicNotSupported = 24, */
	    /*     DialectNotSupported = 25, */
	    /*     Base = 26, */
	    /*     Generic = 27, */
	    /*     Authentication = 28, */
	    /*     Authorization = 29 */
	    /*   }; */
	    
	    CEException(const std::string& methodname, 
			const time_t& tstamp, 
			const std::string& error_code, 
			const std::string& description, 
			const std::string& fault_cause) : 
	      MethodName(methodname), 
	      ErrorCode(error_code), 
	      Description(description), 
	      FaultCause(fault_cause), 
	      Timestamp(tstamp) { }
	    
	    CEException() {}
	    virtual ~CEException() throw() {}
	    
	    const char* what() const throw() {
	      std::ostringstream os("");
	      os << "MethodName=["<<MethodName<<"] Timestamp=["<<
		Timestamp<<"] Description=["<<Description<<"] FaultCause=["<<
		FaultCause<<"] ErrorCode=["<<ErrorCode<<"]"<<EOF;
	      return os.str().c_str();
	    }
	    inline std::string getMethod() { return MethodName; }
	    inline std::string getErrorCode() { return ErrorCode; }
	    inline std::string getDescription() { return Description; }
	    inline std::string getFaultCause() { return FaultCause; }
	    inline time_t      getTimeStamp() { return Timestamp; }
	  };
      }
    }
  }
}

#endif
