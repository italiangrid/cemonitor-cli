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

#include "glite/ce/monitor-client-api-c/CEConsumer.h"
#include "glite/ce/monitor-client-api-c/Dialect.h"
#include "glite/ce/monitor-client-api-c/CEMON_CONSUMER.nsmap"
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/algorithm/string/split.hpp>


extern int h_errno;

using namespace std;

monitortypes__Notification *notification;
bool pointer_nullness = false;
string resolveName(const string& address);
int address_len;

namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;

/******************************************************************
 *
 * 
 *
 */
cemon_api::CEConsumer::CEConsumer(const int& tcpport, const char* cert, const char* key)
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
    authn(false),
    ctx(NULL),
    alen(sizeof(peerAddress)),
    m_cert( cert ),
    m_key( key )
{
  if ( cert && key ) {
    if ( glite_gsplugin_init_context(&ctx) ) {
      perror("init context"); 
      exit(1); 
    }
    ::unsetenv("X509_USER_CERT");
    ::unsetenv("X509_USER_KEY");
    ::setenv("X509_USER_CERT", cert, 0);
    ::setenv("X509_USER_KEY", key, 0);
    if (glite_gsplugin_set_credential(ctx, cert, key)) {
      fprintf (stderr, "Failed to set credentials. Maybe you need to remove the passphrase from the key pem file.\n");
      exit(1);
    }
  }

  SOAP = soap_new();
  if(!SOAP) {
    fprintf(stderr, "Couldn't create SOAP structure. STOP!");
    exit(1);
  }
  SOAP->send_timeout = SOAP_RECV_TIMEOUT;
  SOAP->recv_timeout = SOAP_SEND_TIMEOUT;

  if(cert && key ) {
    if ( soap_register_plugin_arg(SOAP, glite_gsplugin, ctx? : NULL) ) {
      fprintf(stderr, "Can't register plugin\n");
      exit(1);
    }
    authn = true;
  }
  soap_set_namespaces(SOAP, CEMON_CONSUMER_namespaces);
  Events.reserve(1000);

}


/******************************************************************
 *
 *
 *
 */
cemon_api::CEConsumer::~CEConsumer()
{
  soap_destroy(SOAP);
  soap_end(SOAP);
  soap_done(SOAP); // close master socket and detach environment
  if( ctx ) glite_gsplugin_free_context( ctx );
  free(SOAP);
}

/******************************************************************
 *
 *
 *
 */
bool cemon_api::CEConsumer::bind() {
  localSocket = soap_bind(SOAP, NULL, localPort, 5000);
  if (localSocket < 0) {    
    this->setError();
    close(SOAP->master);
    SOAP->master = -1;
    return false;
  }
  return true;
}


/******************************************************************
 *
 *
 *
 */
bool cemon_api::CEConsumer::accept()
{
  this->reset();  // clean up all event information members of CEConsumer class
  
  if( m_cert && m_key ) {
    if (glite_gsplugin_set_credential(ctx, m_cert, m_key)) {
      fprintf (stderr, "Failed to set credentials. Maybe you need to remove the passphrase from the key pem file.\n");
      return false;//exit(1);
    }
  }
  
  clientSocket = soap_accept(SOAP);
  if (clientSocket < 0)
    {
      this->setError();
      if(authn)
        fprintf(stderr, "plugin err: %s", glite_gsplugin_errdesc(SOAP));

      soap_end( SOAP );
      return false;
    }

  if(authn) {
  // obtains the DN of the remote peer from its certificate
  gss_buffer_desc token = GSS_C_EMPTY_BUFFER;
  OM_uint32 min_stat, maj_stat;
  glite_gsplugin_Context ctx = NULL;
  gss_name_t client = GSS_C_NO_NAME;
  ctx = glite_gsplugin_get_context(SOAP);
  maj_stat = gss_inquire_context(&min_stat, (gss_ctx_id_desc_struct*) ctx->connection->context, &client, NULL, NULL, NULL, NULL, NULL, NULL);
  if (GSS_ERROR(maj_stat)) {
    cout << "CLIENT DN EMPTY" << endl;
    client_dn = "";
  }
  else {
    maj_stat = gss_display_name(&min_stat, client, &token, NULL);
    if (!GSS_ERROR(maj_stat)) {
      client_dn = (char*)token.value;
      gss_release_buffer(&min_stat, &token);
    } else
      client_dn = "";
  }
  if (client != GSS_C_NO_NAME)
    gss_release_name(&min_stat, &client);

  } else { client_dn = ""; }

  clientIP = getPeerName().c_str();
  clientName = resolveName(clientIP);

  return true;
}

/******************************************************************
 *
 *
 *
 */
bool cemon_api::CEConsumer::serve()
{
  int ret = CEMON_CONSUMER_serve(SOAP); // process the RPC request
  bool ok = false;
  if(ret != SOAP_OK) {
    if(!pointer_nullness) {  // another kind of error
      this->setError();
      if(authn) {
        if(SOAP)
          fprintf(stderr, "plugin err: %s", glite_gsplugin_errdesc(SOAP));
      }
    }
    else {  // null pointer (not-)deserialized by gSOAP
      errorCode = string("");
      errorMessage = string("A null pointer as been sent by the CEMon");
    }
  } else { // continue if SOAP_OK
    ok = true;
    if(notification->Topic) {
      if(topic) { delete(topic); topic=NULL; }
      topic = new Topic((const Topic&)*notification->Topic);
    }
    else {
      topic = NULL;
    }
    /**
     * Let's copy all relevant data structures because exiting this
     * method will trigger the clearing of the SOAP runtime
     */
    if( notification && notification->Event[0] ) {

	if( notification->Event[0]->Message.size()) {
	  setResponse(notification->Event[0]->ID,
		      notification->Event[0]->Message.size(),
		      notification->Event[0]->Timestamp,
		      notification->Event[0]->Producer,
		      &notification->Event[0]->Message);
	} else {
	  setResponse(notification->Event[0]->ID,
		      0,
		      notification->Event[0]->Timestamp,
		      notification->Event[0]->Producer,
		      NULL);
	}
      
	for(vector<monitortypes__Event * >::const_iterator eit = notification->Event.begin();
	    eit != notification->Event.end();
	    eit++) 
	  {
	    if( *eit )
	      Events.push_back(*(*eit)); // Event.at(#) is of type monitortypes__Event that is safely copiable (not containing pointer for interesting data member)
	  }
    }
  }
  soap_destroy(SOAP); // clean up class instances
  soap_end(SOAP);     // clean up everything and close socket
  
  return ok;
}

/******************************************************************
 *
 *
 *
 */
void cemon_api::CEConsumer::reset(void) {
  EventID = 0;
  messageSize = 0;
  messageCounter = 0;
  EventTimestamp = 0;
  EventProducer.assign("");
  clientIP.assign("");
  clientName.assign("");
  Messages.clear();
  {
    vector<string> temp;
    Messages.swap( temp );
  }

  Events.clear();
  {
    vector<monitortypes__Event> temp;
    Events.swap( temp );
  }
  if(topic) { delete topic; topic=NULL; }
  
}

/******************************************************************
 *
 * 
 *
 */
const char *cemon_api::CEConsumer::getNextEventMessage() 
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
void cemon_api::CEConsumer::setError() {
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
void cemon_api::CEConsumer::setResponse(int eventid, int size,
			     time_t timestamp, string producer, 
			     vector<string>* messages)
{
  EventID        = eventid;
  messageSize    = size;
  EventTimestamp = timestamp;
  EventProducer  = producer;

  if(messages) {
    for(short j=0; j < messageSize; j++)
      Messages.push_back(messages->at(j));
  }
}

/******************************************************************
 *
 *
 *
 */
int monitorws__Notify(struct soap *soap, monitortypes__Notification *notif, monitorws__NotifyResponse &notifr)
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

//______________________________________________________________________________
string resolveName(const string& address)
{
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
	  //printf("hostname: %s\n", hostname);
	  name = hostname;
	  break;
        }
      
    }   
  
  freeaddrinfo(result);
  return name;

}

//______________________________________________________________________________
string cemon_api::CEConsumer::getPeerName() {

  if(!authn) {
    sockaddr_storage addr_client;
    int slen = sizeof(addr_client);
    getpeername(clientSocket, (struct sockaddr *)&addr_client, (socklen_t*)&slen );
  
    char ipstr[INET6_ADDRSTRLEN];
    int port;
    
    if(addr_client.ss_family == AF_INET) {
      struct sockaddr_in *s = (struct sockaddr_in *)&addr_client;
      port = ntohs(s->sin_port);
      inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
    }
    
    if(addr_client.ss_family == AF_INET6) {
    
      struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr_client;
      port = ntohs(s->sin6_port);
      inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
    
    }
    
    string peer = ipstr;
      string::size_type pos = peer.find_last_of(':', 0);
      if(pos != string::npos ) {
        vector<string> SplitVec; // #2: Search for tokens
        boost::split( SplitVec, peer, boost::is_any_of(":"), boost::token_compress_on );
        //peer = peer.substr(pos, substr.length()-pos-1 ); 
        peer = SplitVec.at( SplitVec.size()-1 );
      }
    
    return peer;//tmp;
  } else { 
    struct sockaddr_in6 peerAddress;
    int alen = sizeof(peerAddress);
    
    glite_gsplugin_Context glite_ctx;
    glite_ctx = glite_gsplugin_get_context(SOAP);
    memset( (void*)&peerAddress, 0, sizeof(peerAddress));
    if (glite_ctx != NULL && glite_ctx->connection != NULL) { 
      ::getpeername(glite_ctx->connection->sock, (struct sockaddr *)&peerAddress, (socklen_t*)&alen);
      unsigned char *tmp = (unsigned char *) &peerAddress.sin6_addr;//.s_addr;
      char dest[1024];
      memset((void*)dest, 0, 1024);
      
      cout << "tmp=" << tmp << endl;
      
      inet_ntop(AF_INET6, tmp, dest, 1024 );
      
      cout << "dest=" << dest << endl;
      
      string peer = dest;
      string::size_type pos = peer.find_last_of(':', 0);
      if(pos != string::npos ) {
        vector<string> SplitVec; // #2: Search for tokens
        boost::split( SplitVec, peer, boost::is_any_of(":"), boost::token_compress_on );
        //peer = peer.substr(pos, substr.length()-pos-1 ); 
        peer = SplitVec.at( SplitVec.size()-1 );
      }
      return peer;
    } else {return "";}
  }
  
}
