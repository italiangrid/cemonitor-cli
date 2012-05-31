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


#include "cliUtils.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>

#include <iostream>

using namespace std;
using namespace glite::ce::monitor_client_api::cemon_exceptions;

//______________________________________________________________________________
string cliUtils::getCertificateFile(void)
{
  const char* _cert;
  string certfile;
  if(!(_cert = getenv("X509_USER_CERT"))) {
    if(!(_cert = getenv("X509_USER_PROXY"))) {
      string tmp = "/tmp/x509up_u";
      certfile = (string&)tmp + boost::str( boost::format("%d") % ::getuid() );
    } else {
      certfile = _cert;
    }
  } else certfile = _cert;

  return certfile;
}

//______________________________________________________________________________
string cliUtils::getKeyFile(void)
{
  const char* _key;
  string key;
  if(!(_key = getenv("X509_USER_KEY"))) {
    if(!(_key = getenv("X509_USER_PROXY"))) {
      string tmp = "/tmp/x509up_u";
      key = (string&)tmp + boost::str( boost::format("%d") % ::getuid() );
    } else key = _key;
  } else key = _key;

  return key;
}
//______________________________________________________________________________
void cliUtils::showCEError(CEException& ex) {
  cerr << "CEMon raised a fault:"<<endl;
  cerr << "MethodName  =[" << ex.getMethod()<<"]"<<endl;
  cerr << "Timestamp   =[" << ex.getTimeStamp()<<"]"<<endl;
  cerr << "ErrorCode   =[" << ex.getErrorCode()<<"]"<<endl;
  cerr << "Description =[" << ex.getDescription()<<"]"<<endl;
  cerr << "FaultCause  =[" << ex.getFaultCause()<<"]"<<endl;
}

//______________________________________________________________________________
void cliUtils::showError(AbsException& ex) {
  cerr << "Client exception: "<<endl;
  cerr << "Error =[" << ex.what()<<"]"<<endl;
}

//______________________________________________________________________________
string cliUtils::checkCEMonURL( const std::string& url ) {
  
  boost::cmatch what;

  static const boost::regex url_pattern_1("^(https?)://([^/:]+)(:[0-9]+)/ce-monitor/services/CEMonitor");
  static const boost::regex url_pattern_2("^([^/:]+)(:[0-9]+)?");
  
  if ( !boost::regex_match(url.c_str(), what, url_pattern_1) ) {
    
    if( boost::regex_match(url.c_str(), what, url_pattern_2) ) {
     
      string tcpport = what[2];
      if(tcpport.empty())
	return string("https://") + what[1] + ":8443/ce-monitor/services/CEMonitor";
      else
	return string("https://") + url + "/ce-monitor/services/CEMonitor";
    }
    
    return "";
  } else return url;
  
}
