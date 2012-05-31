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

#include "glite/ce/monitor-client-api-c/CEInfo.h"

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
using namespace glite::ce::monitor_client_api::soap_proxy;
using namespace glite::ce::monitor_client_api::cemon_exceptions;

Namespace namespaces[] = {};

int main(int argc, char *argv[])
{

  string serviceURL = "";
  char c;
  int option_index = 0;
  
  string certfile = cliUtils::getCertificateFile( );

  string key      = cliUtils::getKeyFile( );

  while( 1 ) {
    static struct option long_options[] = {
      {"cert", 1, 0, 'C'},
      {"key", 1, 0, 'k'},
      {0, 0, 0, 0}
    };

    c = getopt_long(argc, argv, "C:k:", long_options, &option_index);
    if ( c == -1 )
      break;
    
    switch(c) {
    case 'k':
      key = string(optarg);
      break;
    case 'C':
      certfile = string(optarg);
      break;
    default:
      cerr << "Usage: glite-ce-monitor-getinfo [--cert <CERTIFICATE_FILE>] [--key <KEY_FILE> <CEMon_URL>"<<endl;
      exit(1);
    } // switch
  }  // while
  
  if(argv[optind]==NULL) {
    cerr << "The CEMon's URL is mandatory... " << endl;
    cerr << "Usage: glite-ce-monitor-getinfo [--cert <CERTIFICATE_FILE] [--key <KEY_FILE>] <CEMon_URL>" << endl;
    exit(1);
  } else serviceURL = argv[optind];
  
  CEInfo *ceI;

  try {
    ceI = new CEInfo(certfile, key);
    
  } catch(AuthenticationInitException& authNEx) {
    cerr << "AuthenticationException Error: ["<<authNEx.what() <<"]"<< endl;
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
  
  ceI->setServiceURL( serviceURL );
  
  try {
    ceI->getInfo();
  } catch (CEException& ex) {
    cliUtils::showCEError(ex);
    exit(1);
  } catch(AbsException& abs) {
    cliUtils::showError(abs);
    exit(1);
  }
  
  cout << "Description=[" << ceI->getDescription()<<"]"<<endl;
  cout << "Version    =[" << ceI->getVersion()<<"]"<<endl;
  cout << "HostDN     =[" << ceI->getDN()<<"]"<<endl;
  vector<Topic*> *T = new vector<Topic*>;
  vector<ActionW*> *A = new vector<ActionW*>;
  ceI->getTopics(T);
  ceI->getActions(A);
  
  for(int j=0; j<T->size(); j++) {
    T->at(j)->print();
  }
  
  for(int j=0; j<A->size(); j++) {
    A->at(j)->print();
  }
  for(int j=0; j<T->size(); j++)
    delete T->at(j);
  
  delete T;
  delete A;
  delete(ceI);
  
}

