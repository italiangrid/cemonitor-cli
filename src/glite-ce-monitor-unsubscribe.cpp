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

#include "glite/ce/monitor-client-api-c/CESubscription.h"
#include "glite/ce/monitor-client-api-c/AbsException.h"
#include "glite/ce/monitor-client-api-c/GenericException.h"
#include "glite/ce/monitor-client-api-c/TopicNotSupportedException.h"
#include "glite/ce/monitor-client-api-c/DialectNotSupportedException.h"
#include "glite/ce/monitor-client-api-c/ServiceNotFoundException.h"
#include "glite/ce/monitor-client-api-c/AuthenticationException.h"
#include "glite/ce/monitor-client-api-c/GeneralException.h"
#include "glite/ce/monitor-client-api-c/CEMON_CLIENT.nsmap"
#include <iostream>
#include <string>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <getopt.h>

#include "cliUtils.h"

#include <boost/scoped_ptr.hpp>

using namespace std;
using namespace glite::ce::monitor_client_api::soap_proxy;
using namespace glite::ce::monitor_client_api::cemon_exceptions;

Namespace namespaces[] = {};

int main(int argc, char *argv[])
{
  string serviceURL = "";
  char c;
  int option_index = 0;
  
  string certfile = cliUtils::getCertificateFile();
  
  string key      = cliUtils::getKeyFile( );
  
  string subscriptionID;

  while( 1 ) {
    static struct option long_options[] = {
      {"cert", 1, 0, 'C'},
      {"key", 1, 0, 'k'},
      {"id", 1, 0, 'i'},
      {0, 0, 0, 0}
    };
    
    c = getopt_long(argc, argv, "C:k:i:", long_options, &option_index);
    if ( c == -1 )
      break;
    
    switch(c) {
    case 'k':
      key = string(optarg);
      break;
    case 'C':
      certfile = string(optarg);
      break;
    case 'i':
      subscriptionID = string(optarg);
      break;
    default:
      cerr << "The subscription ID is mandatory... " << endl;
      cerr << "Usage: glite-ce-monitor-unsubscribe [--cert <CERTIFICATE_FILE>] [--key <KEY_FILE>] --id <SUBSCRIPTION_ID_TO_REMOVE> <CEMon_URL>"<< endl;
      exit(1);
    } // switch
  }  // while
  
  if(argv[optind]==NULL) {
    cerr << "The CEMon's URL is mandatory... " << endl;
    cerr << "The subscription ID is mandatory... " << endl;
    cerr << "Usage: glite-ce-monitor-unsubscribe [--cert <CERTIFICATE_FILE>] [--key <KEY_FILE>] --id <SUBSCRIPTION_ID_TO_REMOVE> <CEMon_URL>"<< endl;
    exit(1);
  } else serviceURL = argv[optind];
  
  if( subscriptionID.empty() ) {
    cerr << "The subscription ID is mandatory... " << endl;
    cerr << "Usage: glite-ce-monitor-unsubscribe [--cert <CERTIFICATE_FILE>] [--key <KEY_FILE>] --id <SUBSCRIPTION_ID_TO_REMOVE> <CEMon_URL>"<< endl;
    exit(1);
  }

  CESubscription *ceS;
  
  try{

    ceS = new CESubscription( certfile, key );
    
  } catch(AuthenticationInitException& authNEx) {
    
    cerr << "Fault cause=["<<authNEx.what() <<"]"<< endl;
    exit(1);
    
  } catch(GeneralException& ex) {

    cerr << "Error: ["<<ex.what() <<"]"<< endl;
    exit(1);

  }

  serviceURL = cliUtils::checkCEMonURL( serviceURL );

  ceS->setServiceURL( serviceURL );
  
  try {

    ceS->unsubscribe( subscriptionID );

  } catch (CEException& ex) {
    cliUtils::showCEError(ex);
    delete(ceS);
    exit(1);
  } catch(AbsException& abs) {
    cliUtils::showError(abs);
    delete(ceS);
    exit(1);
  } 
}
