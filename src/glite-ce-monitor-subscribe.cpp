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
#include "glite/ce/monitor-client-api-c/Topic.h"
#include "glite/ce/monitor-client-api-c/Policy.h"
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
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace glite::ce::monitor_client_api::soap_proxy;
using namespace glite::ce::monitor_client_api::cemon_exceptions;

Namespace namespaces[] = {};

int main(int argc, char *argv[])
{
  string serviceURL = "";
  string topicName = "";
  string dialectName = "";
  char c;
  int option_index = 0;
  
  string certfile = cliUtils::getCertificateFile();
  
  string key      = cliUtils::getKeyFile( );
  
  int rate=20;
  int duration=600;

  string topic;
  string vecDialects;
  string consumerURL;

  while( 1 ) {
    static struct option long_options[] = {
      {"cert", 1, 0, 'C'},
      {"key", 1, 0, 'k'},
      {"consumer-url", 1, 0, 'U'},
      {"topic",1 ,0, 'T'},
      {"dialects", 1, 0, 'D'},
      {"rate", 1, 0, 'r'},
      {"duration", 1, 0, 'd'},
      {0, 0, 0, 0}
    };
    
    c = getopt_long(argc, argv, "C:k:U:T:D:r:d:", long_options, &option_index);
    if ( c == -1 )
      break;
    
    switch(c) {
    case 'k':
      key = string(optarg);
      break;
    case 'C':
      certfile = string(optarg);
      break;
    case 'U':
      consumerURL = string(optarg);
      break;
    case 'T':
      topic = string(optarg);
      break;
    case 'D':
      vecDialects = string(optarg);
      break;
    case 'r':
      rate = atoi(optarg);
      break;
    case 'd':
      duration = atoi(optarg);
      break;
    default:
      cerr << "Usage: glite-ce-monitor-subscribe [--cert <CERTIFICATE_FILE>] [--key <KEY_FILE>] --topic <TOPC_NAME> [--dialects <COLON-SEPARATED_DIALECT_LIST>] --consumer-url <CONSUMER_URL_TO_SUBSCRIBE> [--rate <DELAY_BETWEEN_TWO_NOTIFICATIONS>] [--duration <DURATION_OF_SUBSCRIPTION>] <CEMon_URL>"<< endl;
      exit(1);
    } // switch
  }  // while
  
  if(argv[optind]==NULL) {
    cerr << "The CEMon's URL is mandatory... " << endl;
    cerr << "Usage: glite-ce-monitor-subscribe [--cert <CERTIFICATE_FILE>] [--key <KEY_FILE>] --topic <TOPC_NAME> [--dialects <COLON-SEPARATED_DIALECT_LIST>] --consumer-url <CONSUMER_URL_TO_SUBSCRIBE> [--rate <DELAY_BETWEEN_TWO_NOTIFICATIONS>] [--duration <DURATION_OF_SUBSCRIPTION>] <CEMon_URL>"<< endl;
    exit(1);
  } else serviceURL = argv[optind];

  if( consumerURL.empty() ) {
    cerr << "The Consumer URL is mandatory... " << endl;
    cerr << "Usage: glite-ce-monitor-subscribe [--cert <CERTIFICATE_FILE>] [--key <KEY_FILE>] --topic <TOPC_NAME> --dialects <COLON-SEPARATED_DIALECT_LIST> --consumer-url <CONSUMER_URL_TO_SUBSCRIBE> [--rate <DELAY_BETWEEN_TWO_NOTIFICATIONS>] [--duration <DURATION_OF_SUBSCRIPTION>] <CEMon_URL>"<< endl;
    exit(1);
  }

  if( topic.empty() ) {
    cerr << "The Topic Name is mandatory... " << endl;
    cerr << "Usage: glite-ce-monitor-subscribe [--cert <CERTIFICATE_FILE>] [--key <KEY_FILE>] --topic <TOPC_NAME> --dialects <COLON-SEPARATED_DIALECT_LIST> --consumer-url <CONSUMER_URL_TO_SUBSCRIBE> [--rate <DELAY_BETWEEN_TWO_NOTIFICATIONS>] [--duration <DURATION_OF_SUBSCRIPTION>] <CEMon_URL>"<< endl;
    exit(1);
  }

  CESubscription *ceS;
  
  try{
    ceS = new CESubscription(certfile, key );
    //ceS->authenticate(certfile.c_str(), capath.c_str());
  } catch(AuthenticationInitException& authNEx) {
    cerr << "Authentication Error: ["<<authNEx.what() <<"]"<< endl;
    exit(1);
  } catch(GeneralException& ex) {
    cerr << "Error: ["<<ex.what() <<"]"<< endl;
    exit(1);
  }
  
  serviceURL = cliUtils::checkCEMonURL( serviceURL );

  ceS->setServiceURL( serviceURL );
  
  vector<string> dialects;
  boost::split( dialects, vecDialects, boost::is_any_of(",") );
  Topic T = Topic( topic );
  
  
  for(vector<string>::const_iterator it=dialects.begin();
      it!=dialects.end();
      it++) 
    {
      if( !(*it).empty() ) {
	DialectW *D = new DialectW( *it );
	T.addDialect(D);
	delete D;
      }
    }
  
  Policy P( rate );
  ActionW A1("SendNotification", "", true);
  ActionW A2("DoNotSendNotification", "", false);
  P.addAction( &A1 );
  P.addAction( &A2 );
  
  ceS->setSubscribeParam( consumerURL, T, P, duration );
  
  T.addDialect(NULL);
  
  try {
    ceS->subscribe();
    cout << "SubscriptionID=[" << ceS->getSubscriptionID() <<"]"<< endl;
  } catch (CEException& ex) {
    cliUtils::showCEError(ex);
    delete(ceS);
    exit(1);
  } catch(AbsException& abs) {
    cliUtils::showError(abs);
    delete(ceS);
    exit(1);
  }
  delete(ceS);
}
