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

#include "glite/ce/monitor-client-api-c/CEEvent.h"
#include "glite/ce/monitor-client-api-c/Dialect.h"
#include "glite/ce/monitor-client-api-c/Topic.h"
#include "glite/ce/monitor-client-api-c/GenericException.h"
#include "glite/ce/monitor-client-api-c/TopicNotSupportedException.h"
#include "glite/ce/monitor-client-api-c/DialectNotSupportedException.h"
#include "glite/ce/monitor-client-api-c/ServiceNotFoundException.h"
#include "glite/ce/monitor-client-api-c/AuthenticationException.h"
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

using namespace std;
using namespace glite::ce::monitor_client_api::cemon_exceptions;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;

Namespace namespaces[] = {};

int main(int argc, char *argv[])
{

  string serviceURL = "";
  string topicName = "";
  string dialectName = "";
  char c;
  int option_index = 0;
  
  string certfile = cliUtils::getCertificateFile( );

  string key      = cliUtils::getKeyFile( );

  while( 1 ) {
    static struct option long_options[] = {
      {"cert", 1, 0, 'C'},
      {"key", 1, 0, 'k'},
      {"topic", 1, 0, 'T'},
      {"dialect", 1, 0, 'D'},

      {0, 0, 0, 0}
    };
    
    c = getopt_long(argc, argv, "C:k:T:D:", long_options, &option_index);
    if ( c == -1 )
      break;
    
    switch(c) {
    case 'k':
      key = string(optarg);
      break;
    case 'C':
      certfile = string(optarg);
      break;
    case 'T':
      topicName = string(optarg);
      break;
    case 'D':
      dialectName = string(optarg);
      break;
    default:
      cerr << "Usage: glite-ce-monitor-getevent --topic <TOPIC_NAME> [--dialect <DIALECT_NAME>] [--cert <CERTIFICATE_FILE>] [--key <KEY_FILE>] <CEMon_URL>"<< endl;
      exit(1);
    } // switch
  }  // while
  
  cout << "cert="<<certfile << " - key=" << key << endl;
  
  if(argv[optind]==NULL) {
    cerr << "The CEMon's URL is mandatory... " << endl;
    cerr << "Usage: glite-ce-monitor-getevent --topic <TOPIC_NAME> [--dialect <DIALECT_NAME>] [--cert <CERTIFICATE_FILE>] [--key <KEY_FILE>] <CEMon_URL>" << endl;
    exit(1);
  } else serviceURL = argv[optind];

  cemon_api::CEEvent *ceE;

  if( topicName.empty() )
    {
      cerr << "The Topic name is mandatory... " << endl;
      cerr << "Usage: glite-ce-monitor-getevent --topic <TOPIC_NAME> [--dialect <DIALECT_NAME>] [--cert <CERTIFICATE_FILE>] [--key <KEY_FILE>] <CEMon_URL>"<<endl;
      exit(1);
    }

  cemon_api::Topic topic = cemon_api::Topic( topicName );

  if( !dialectName.empty() ) {
    cemon_api::DialectW* dial = new cemon_api::DialectW( dialectName );
    topic.addDialect(dial);
  }

  try{
    
    ceE = new cemon_api::CEEvent( certfile, key );
    
  } catch(AuthenticationInitException& authNEx) {
    
    cerr << "Authentication init Error: ["<<authNEx.what() <<"]"<< endl;
    exit(1);
    
  } catch(GeneralException& ex) {
    
    cerr << "Error: ["<<ex.what() <<"]"<< endl;
    exit(1);

  }

  serviceURL = cliUtils::checkCEMonURL( serviceURL );

  if( serviceURL.empty() )
    {
      cerr << "Wrong CEMon_URL format specified. Must be:"<<endl;
      cerr << " - http(s)://<hostname>:<tcpport>/ce-monitor/services/CEMonitor" << endl;
      cerr << " or "<<endl;
      cerr << " - <hostname>[:tcpport]" << endl;
      exit(1);
	
    }

  ceE->setServiceURL( serviceURL );
  ceE->setRequestParam(&topic);
  int j=0;
    
  char *t;
  
  list< vector<string> > event_Messages;
  
  try {
  
      ceE->getTopicEvents( event_Messages );
      
  } catch (CEException& ex) {
    cliUtils::showCEError(ex);
    exit(1);
  } catch(AbsException& abs) {
    cliUtils::showError(abs);
    exit(1);
  }
    
  int i = 0;
  for(list<vector<string> >::const_iterator it = event_Messages.begin();
      it != event_Messages.end();
      ++it)
  {
    cout << "Event[" << i++ << "]:"<< endl;
    int j = 0;
    for(vector<string>::const_iterator mit = it->begin();
        mit != it->end();
	++mit)
	{
	  cout << "\tMessage["<<j++<<"]=["<<*mit<<"]"<<endl;
	 
	}
  }
   
  ceE->cleanup();
  delete(ceE); // call CEEvent's DTOR (doing nothing) and then the AbsRequest's DTOR (that calls soap_end(...))
}
