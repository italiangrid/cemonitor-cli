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
        CEConsumer.hh: header file for a C++ providing consumer APIs

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: CEConsumer.h,v 1.37.8.3.2.2.4.2 2012/06/19 07:34:19 adorigo Exp $
*/
#ifndef __GLITE_CE_MONITOR_CLIENT_CECONSUMER__
#define __GLITE_CE_MONITOR_CLIENT_CECONSUMER__

#define SOAP_RECV_TIMEOUT 60
#define SOAP_SEND_TIMEOUT 60

#include "glite/ce/monitor-client-api-c/monitor_consumer_CEMON_CONSUMERStub.h"
#include "glite/ce/monitor-client-api-c/Topic.h"
#include <string>
#include <vector>

extern "C" {
#undef IOV_MAX // this is very ugly, but the only way to silent a warning concerning
               // the redefinition of IOV_MAX
#ifdef NEW_GSOAP_PLUGIN_API
#include <gssapi.h>
#include "glite/security/glite_gsplugin-int.h"
#endif
#include "glite/security/glite_gsplugin.h"	
}

namespace glite {
  namespace ce {
    namespace monitor_client_api {
      namespace soap_proxy {
	
	/// Class implementing a SOAP listener for notification coming from a CREAM type CE
	/**
	   An object of this class is a listener of notification (= an array of CEMonitor events); it provides methods to listen, parse and return 
	   notification events sent by CEMon (the Monitor of a CREAM CE)
	*/
	class CEConsumer {
	  
	private:
	  int localPort;
	  int localSocket;
	  int clientSocket;
	  std::string errorMessage;
	  std::string errorCode;
	  struct soap *SOAP;
	  std::string clientIP;
	  std::string clientName;
	  time_t EventTimestamp;
	  Topic* topic;
	  int messageSize;
	  int EventID;
	  std::string EventProducer;
	  std::vector<std::string> Messages;
	  int messageCounter;
	  std::vector<monitortypes__Event> Events;
	  bool authn;
	  std::string client_dn;
	  bool m_valid;
	  
	protected:
	  CEConsumer(const CEConsumer&) {}
	  CEConsumer operator=(const CEConsumer&) { return *this; }
	  std::string getPeerName( void );
	  
	private:
	  void setResponse(int, int, time_t, std::string, std::vector<std::string>*);
	  void setError(void);
	  glite_gsplugin_Context	ctx;
	  int alen;
	  struct sockaddr_in6 peerAddress;

	  const char* m_cert;
	  const char* m_key;
	  
	public:
   	  bool is_valid( void ) const { return m_valid; }
	  /**
	     Costructor. Build up a listener object.
	     \param port The TCP port where the listener will be listening for notifications
	     \param cert The certificate file to setup authentication
	     \param key  The key file to setup authentication
	  */
	  CEConsumer(const int& port, const char* cert=NULL, const char* key=NULL);
	  /// Destroy the listener object
	    /**
	       Cleanly destroys the SOAP runtime
	    */
	    virtual ~CEConsumer();
	    
	    /**
	       Invoke the TCP bind call on the underlying TCP socket. This call can return false if the port is 
	       already in use. This is tipical when a listener has been killed; the port remains used for a while.
	       Then it could be worth to wait and retry this call.
	       \return bool True if the TCP bind call succeded, False otherwise.
	    */
	    bool bind(void);
	    /**
	       Put the CEConsumer object in listening mode. This call is blocking (in fact it calls the 
	       underlying TCP accept system call that is blocking).
	       
	       \return bool True if a connection is incoming and ready to be read. False if something went wrong.
	    */
	    bool accept(void);
	    //bool accept2( std::string& );
	    /**
	       Clean up internal data and arrays. Not calling this method between different 'accept' 
	       sessions leads to memory leak.
	       After a successfull accept call and subsequent serve invocation, reset MUST be called to clean all 
	       data related to processing of notification.
	    */
	    void reset(void);
	    /**
	       After a succesfull call of accept, this method can be invoked to get the IP address of the 
	       CE that sent the notification.
	       \return string The string representing the IP of the CE (in the for XYZ.XYZ.XYZ.XYZ)
	    */
	    std::string getClientIP() const { return clientIP; }
	    /**
	       After a succesfull call of accept, this method can be invoked to 
	       get the name of the CE sending the notification.
	       \return string The string containing the name of the CE that sent the notification
	    */
	    std::string getClientName() const { return clientName; }
	    /**
	       Get the socket returned internally by the succesfull accept call
	       \return int The socket on which the listener will receive
	    */
	    int getClientSocket() const { return clientSocket; }
	    /**
	       Get the TCP port which the listener is binded to.
	       \return int The TCP port the listener is binded to
	    */
	    int getLocalPort() const { return localPort; }
	    /**
	       Get the local socket internally returned by the soap_bind invoked by the bind call.
	       \return int The bind socket
	    */
	    int getLocalSocket() const { return localSocket; }
	    /**
	       After the call accept returns true and data are ready to come from the CEMon service, the method serve
	       processes the incoming data and load them into an internal data structure.
	       \return bool if the read of incoming data is successfull
	    */
	    bool serve(void);
	    /**
	       After call of the serve method, getNextEventMessage iterates over all loaded messages of the
	       first event of the notification sent by the CE.
	       When last message is reached, a subsequent call of getNextEventMessage return 0;
	       \return A pointer to a character buffer
	    */
	    const char* getNextEventMessage();
	    /**
	       Returns the Topic related to the events sent by the CE
	       \return Topic* a pointer to the Topic related to the received event
	    */
	    Topic* getEventTopic() const { return topic; }
	    /**
	       Returns the identifier of the first event contained in the notification
	       \return int The identifier of the notification event
	    */
	    int getEventID() const { return EventID; }
	    /**
	       Returns the number of messages contained in the first event of the notification
	       \return int The number of messages
	    */
	    int getMessageSize() const { return messageSize; }
	    /**
	       Returns the name of the event producer (in practice: the name of the CEMon topic the listener has been
	       subscribed to)
	       \return string The name of the event produces
	    */
	    std::string getEventProducer() const { return EventProducer; }
	    /**
	       If the soap communication or the serve call fail, this method returns a human readable explanation
	       of the problem
	       \return string The human readable error message
	    */
	    std::string getErrorMessage() const { return errorMessage; }
	    /**
	       Like getErrorMessage, but returning a SOAP fault code
	       \return string A string containing the numeric error code
	    */
	    std::string getErrorCode() const { return errorCode; }
	    /**
	       Set the maximum time a SOAP connection can wait for an answer from the CE
	    */
	    void setRecvTimeout(const int& t) { SOAP->recv_timeout = t; }
	    /**
	       Set the maximum time a SOAP connection can wait to send the request to the CE
	    */
	    void setSendTimeout(const int& t) { SOAP->send_timeout = t; }
	    
	    /**
	       Get all events contained in the notification. To get access to the messages contained 
	       in each event the user must get access to the public member monitortypes__Event::Message, that is 
	       in turn an std::vector< std::string >.
	    */
	    const std::vector<monitortypes__Event>& getEvents( void ) const { return Events; }
	    
	    /**
	       Get the Distinguished name of the CE that sent the notification, as obtained from the 
	       certificate that the CE used to get authentication into the listener.
	    */
	    const std::string getClientDN( void ) const { return client_dn; }
	};
      }
    }
  }
}

#endif
