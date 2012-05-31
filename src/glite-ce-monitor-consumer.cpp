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

#include "glite/ce/monitor-client-api-c/CEConsumer.h"
#include "glite/ce/monitor-client-api-c/Topic.h"

#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <cstdio>

#include "glite/ce/monitor-client-api-c/CEMON_CONSUMER.nsmap"

using namespace std;
using namespace glite::ce::monitor_client_api::soap_proxy;

Namespace namespaces[] = {};

int main(int argc, char *argv[])
{
  int port;

  CEConsumer *consumer;

  bool useout=false;

  string outpath = "";

  string keyfile="", certfile="";

  char c;
  int option_index = 0;
  
  bool append = false;

  while( 1 ) {
    static struct option long_options[] = {
      {"cert", 1, 0, 'C'},
      {"key", 1, 0, 'K'},
      {"outpath", 1, 0, 'P'},
      {"append-output", 0, 0, 'A'},
      {0, 0, 0, 0}
    };
    c = getopt_long(argc, argv, "C:K:P:A", long_options, &option_index);
    if ( c == -1 )
      break;

    switch(c) {
    case 'A':
      append = true;
      break;
    case 'P':
      outpath = string(optarg);
      useout = true;
      break;
    case 'K':
      keyfile = string(optarg);
      break;
    case 'C':
      certfile = string(optarg);
      break;
    default:
      cerr << "Usage: glite-ce-monitor-consumer [--key keyfile --cert certfile] [--outpath path] [--append-output] <tcpport>";
      exit(1);
    } // switch
  } // while

  if(argv[optind]==NULL) {
    cerr << "Must specify at least the tcpport number... Stop." << endl;
    exit(1);
  } else port = atoi(argv[optind]);

  if( keyfile.empty() != certfile.empty() ) {
    cerr << "If you specify the key you also must specify the cert and viceversa. Stop." << endl;
    exit(1);
  }

  if( !keyfile.empty() ) {
    consumer = new CEConsumer(port, certfile.c_str(), keyfile.c_str());
  } else consumer = new CEConsumer( port );

  while(!consumer->bind())
    {
      cout << "error message=" << consumer->getErrorMessage() << endl;
      cout << "error code   =" << consumer->getErrorCode() << endl;
      cout << "Retrying in 5 seconds..." <<endl;
      sleep(5);
    }
  
  {
    cout << "Socket connection successful on port " << consumer->getLocalPort()
	 << " master socket "<< consumer->getLocalSocket()<<endl;
    for (int i = 1;  ; i++)
      {
	cout << "Accepting connection ... " << endl;
	if(!consumer->accept())
	  {
	    cout << "error message=" << consumer->getErrorMessage() << endl;
	    cout << "error code   =" << consumer->getErrorCode() << endl;
	    continue;
	  }
	time_t T = time(NULL);
	cout << ctime(&T) << " - " << i << ": accepted connection from IP=" << consumer->getClientIP() <<
	  " ("<< consumer->getClientName()<<")"<<endl;
	
	string clientName = consumer->getClientName();

	bool ret = consumer->serve();
	
	
	if(ret == false) {
	  cout << "ErrorCode=[" << consumer->getErrorCode() << "]"<<endl;
	  cout << "ErrorMess=[" << consumer->getErrorMessage() << "]"<<endl;
	} else {
 	  cout << "Event Producer="<<consumer->getEventProducer()<<endl;
 	  cout << "Event ID      ="<<consumer->getEventID()<<endl;
	  
   	  consumer->getEventTopic()->print();
	  
	  FILE* outstream;

	  if(!useout) {
	    outstream = stdout;
	  } else {
	    string fileout = outpath + "/" + clientName;
	    if(append)
	      outstream = fopen(fileout.c_str(), "a");
	    else
	      outstream = fopen(fileout.c_str(), "w");
	    
	    if(!outstream) {
	      cerr << "Error opening outfile [" << fileout << "]. Stop." << endl;
	      perror(0);
	      exit(1);
	    }
	    cout << "Notification redirected to ["<<fileout<<"]" << endl;
	  }
	  
 	  const char* c;
	  
	  const vector< monitortypes__Event >& events( consumer->getEvents() );
	  vector< monitortypes__Event >::const_iterator it;
	  int j = 0;

	  for ( it = events.begin(); events.end() != it; ++it ) {
	    fprintf(stdout, ">>> Event #%d - Dumping messages: \n\n", j);
	    
	    vector< string >::const_iterator msg_it;
	    int m = 0;
	    for ( msg_it = it->Message.begin();
		  msg_it != it->Message.end(); msg_it++ ) {
	      
	      fprintf(stdout, "Event #%d - Message #%d\n", j, m);

	      fprintf(outstream, "%s\n\n", (*msg_it).c_str());

	      m++;
	      
	    }
	    
	    j++;
          }
	  if(useout) fclose(outstream);
	}
      }
  }
  delete(consumer);
}
