/* LICENSE:
Copyright (c) Members of the EGEE Collaboration. 2010.
See http://www.eu-egee.org/partners/ for details on the copyright
holders.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied.
See the License for the specific language governing permissions and
limitations under the License.

END LICENSE */

#include "glite/ce/monitor-client-api-c/CEConsumerP.h"
#include "glite/ce/monitor-client-api-c/Dialect.h"
#include "glite/ce/monitor-client-api-c/CEMON_CONSUMER.nsmap"
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sstream>
#include <string>
#include <signal.h>

extern int h_errno;

using namespace std;

monitortypes__Notification *notification;
bool pointer_nullness = false;
string resolveName(const string& address);

/******************************************************************
 *
 * 
 *
 */
CEConsumerP::CEConsumerP(const int& tcpport, const char* cert, const char* key)
  : localPort(tcpport),
    localSocket(-1),
    clientSocket(-1), 
    errorMessage(""), 
    errorCode(""), 
    SOAP(NULL),
    clientIP(""), 
    clientName(""),
    EventTimestamp(0),
    topic(NULL),
    messageSize(0),
    EventID(0),
    EventProducer(""),
    messageCounter(0),
    Events(),
    //ctx(NULL),
    authn(false)
{

  //printf("cert=%s key=%s\n", cert, key);

/*   if ( cert || key ) {
    if ( glite_gsplugin_init_context(&ctx) ) {
      perror("init context"); 
      exit(1); 
    }
    ::unsetenv("X509_USER_CERT");
    ::unsetenv("X509_USER_KEY");
    ::setenv("X509_USER_CERT", cert, 0);
    ::setenv("X509_USER_KEY", key, 0);
    if (glite_gsplugin_set_credential(ctx, cert, key)) {
      fprintf (stderr, "Failed to set credentials\n");
      exit(1);
    }
  } */

  SOAP = soap_new();
  if(!SOAP) {
    fprintf(stderr, "Couldn't create SOAP structure. STOP!");
    exit(1);
  }
  SOAP->send_timeout = SOAP_RECV_TIMEOUT;
  SOAP->recv_timeout = SOAP_SEND_TIMEOUT;

/*   if(cert && key ) {
    if ( soap_register_plugin_arg(SOAP, glite_gsplugin, ctx? : NULL) ) {
      fprintf(stderr, "Can't register plugin\n");
      exit(1);
    }
    authn = true;
  } */
  soap_set_namespaces(SOAP, CEMON_CONSUMER_namespaces);
  Events.reserve(1000);
}


/******************************************************************
 *
 *
 *
 */
CEConsumerP::~CEConsumerP()
{
  soap_destroy(SOAP);
  soap_end(SOAP);
  soap_done(SOAP); // close master socket and detach environment
  //if( ctx ) glite_gsplugin_free_context( ctx );
  free(SOAP);
}

/******************************************************************
 *
 *
 *
 */
bool CEConsumerP::bind() {
  localSocket = soap_bind(SOAP, NULL, localPort, 100);
  if (localSocket < 0) {
    this->setError();
    return false;
  }
  return true;
}

/******************************************************************
 *
 *
 *
 */
bool CEConsumerP::accept()
{
  this->reset();  // clean up all event information members of CEConsumer class
  clientSocket = soap_accept(SOAP);
  if (clientSocket < 0)
    {
      this->setError();
//       if(authn)
//         fprintf(stderr, "plugin err: %s", glite_gsplugin_errdesc(SOAP));
      return false;
    }

  ostringstream S("");
  S << ((SOAP->ip >> 24)&0xFF) << "." <<
    ((SOAP->ip >> 16)&0xFF) << "." << ((SOAP->ip >> 8)&0xFF) << "." <<
    (SOAP->ip&0xFF);
  clientIP = S.str();

//   struct hostent *H = gethostbyname((const char*)clientIP.c_str());
//   if(H)
//     clientName = string(H->h_name);
//   else {
//     cerr << "Error resolving: "<<strerror(h_errno)<<endl;
//     clientName = clientIP.c_str();
//   }
//  clientName = resolveName(clientIP);
  return true;
}

/******************************************************************
 *
 *
 *
 */
bool CEConsumerP::serve()
{
  int ret = CEMON_CONSUMER_serve(SOAP); // process the RPC request
  bool ok = false;
  if(ret != SOAP_OK) {
    if(!pointer_nullness) {  // another kind of error
      this->setError();
//       if(authn) {
//         if(SOAP)
//           fprintf(stderr, "plugin err: %s", glite_gsplugin_errdesc(SOAP));
//       }
    }
    else {  // null pointer (not-)deserialized by gSOAP
      errorCode = string("");
      errorMessage = string("A null pointer as been sent by the CEMon");
    }
  }
  else {
    ok = true;
    if(notification->Topic) {
      if(topic) { delete(topic); topic=NULL; }
      topic = new Topic(*notification->Topic);
    }
    else {
      topic = NULL;
    }
    /**
     * Let's copy all relevant data structures because exiting this
     * method will trigger the clear of the SOAP runtime
     */
    if(notification->Event[0]->Message.size())
      setResponse(notification->Event[0]->ID,
		  notification->Event[0]->Message.size(),
		  notification->Event[0]->Timestamp,
		  notification->Event[0]->Producer,
		  &notification->Event[0]->Message);

    else setResponse(notification->Event[0]->ID,
		     0,
		     notification->Event[0]->Timestamp,
		     notification->Event[0]->Producer,
		     NULL);
    for(vector<monitortypes__Event * >::const_iterator eit = notification->Event.begin();
    	eit != notification->Event.end();
	eit++) //unsigned j=0; j<notification->Event.size(); j++)
      Events.push_back(*(*eit)); // Event.at(#) is of type monitortypes__Event that is safely copiable (not containing pointer for interesting data member)
  }
  soap_destroy(SOAP); // clean up class instances
  soap_end(SOAP);     // clean up everything and close socket

  return ok;
}

/******************************************************************
 *
 *
 *
 *///
void CEConsumerP::reset(void) {
  EventID = 0;
  messageSize = 0;
  messageCounter = 0;
  EventTimestamp = 0;
  EventProducer.assign("");
  clientIP.assign("");
  clientName.assign("");
  Messages.clear();
  
  //topic = NULL;
  Events.clear();
  if(topic) { delete topic; topic=NULL; }
}

/******************************************************************
 *
 * 
 *
 */
const char *CEConsumerP::getNextEventMessage() 
{
  if(messageCounter>=messageSize) {
    messageCounter=0;
    return NULL;
  }
  return Messages.at(messageCounter++).c_str();
}

/******************************************************************
 *
 * 
 *
 */
void CEConsumerP::setError() {
  if (SOAP->error) {
    if (!*soap_faultcode(SOAP))
      soap_set_fault(SOAP);
    errorMessage = string(*soap_faultstring(SOAP));
    errorCode    = string(*soap_faultcode(SOAP));
  }
}

/******************************************************************
 *
 * 
 *
 */
void CEConsumerP::setResponse(int eventid, int size,
			     time_t timestamp, string producer, 
			     vector<string>* messages)
{
  EventID        = eventid;
  messageSize    = size;
  EventTimestamp = timestamp;
  EventProducer  = producer;

  for(short j=0; j < messageSize; j++)
    Messages.push_back(messages->at(j));
}

/******************************************************************
 *
 *
 *
 */
int monitorws__Notify(struct soap *soap,
		      monitortypes__Notification *notif, 
		      monitorws__NotifyResponse &notifr)
{
  notification = notif;
  pointer_nullness = false;
  if(notif == NULL) {
    cerr << "The received notification is NULL!" <<endl;
    pointer_nullness = true;
    return SOAP_FAULT;
  }
  if(notif->Topic == NULL) {
    cerr << "The received notification->topic is NULL!" <<endl;
    pointer_nullness = true;
    return SOAP_FAULT;
  }
  return SOAP_OK;
}

/******************************************************************
 *
 *
 *
 */
// Event::Event(SOAP_CMAC monitortypes__Event* evt) {
//   ID = evt->ID;
//   Producer = evt->Producer;
//   Timestamp = evt->Timestamp;
//   Messages = evt->Message;
// }

//______________________________________________________________________________
string resolveName(const string& address) {

  //---------------------------------------------------------------------------
  struct addrinfo * result;
  struct addrinfo * res;
  int error;
  
  /* resolve the domain name into a list of addresses */
  error = getaddrinfo(address.c_str(), NULL, NULL, &result);
  
  if (0 != error)
    {   
      perror("error in getaddrinfo: ");
      return "UnresolvedHost";
    }   
  
  if (result == NULL)
    {   
      perror("getaddrinfo found no results\n");
      return "UnresolvedHost";
    }   
  
  string name = "UnresolvedHost";

  for (res = result; res != NULL; res = res->ai_next)
    {   
      char hostname[NI_MAXHOST] = "";
      
      error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0); 
      
      if (0 != error)
        {
	  //fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
	  continue;
        }
      
      if (*hostname)
        {
	  printf("hostname: %s\n", hostname);
	  name = hostname;
	  break;
        }
      
    }   
  
  freeaddrinfo(result);
  return name;
}
