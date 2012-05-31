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
        Version info: $Id: CEConsumerP.h,v 1.1.12.1 2010/03/22 12:11:11 pandreet Exp $
*/
#ifndef __GLITE_CE_MONITOR_CLIENT_CECONSUMERP__
#define __GLITE_CE_MONITOR_CLIENT_CECONSUMERP__

#define SOAP_RECV_TIMEOUT 60
#define SOAP_SEND_TIMEOUT 60

#include "glite/ce/monitor-client-api-c/monitor_consumer_CEMON_CONSUMERStub.h"
#include "glite/ce/monitor-client-api-c/Topic.h"
#include <string>
#include <vector>

/* extern "C" {
#undef IOV_MAX // this is very ugly, but the only way to silent a warning concerning
               // the redefinition of IOV_MAX
#include "glite/security/glite_gsplugin.h"	
}
*/

/*class Event {
public:
  std::string ID;
  std::string Producer;
  time_t Timestamp;
  std::vector<std::string> Messages;
  Event(SOAP_CMAC monitortypes__Event*);
};*/

class CEConsumerP {

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
  
protected:
  CEConsumerP(const CEConsumerP&) {}
  CEConsumerP operator=(const CEConsumerP&) { return *this; }

private:
  void setResponse(int, int, time_t, std::string, std::vector<std::string>*);
  void setError(void);
  //glite_gsplugin_Context	ctx;

public:
  CEConsumerP(const int& port, const char* cert=NULL, const char* key=NULL);
  virtual ~CEConsumerP();

  bool bind(void);
  bool accept(void);
  void reset(void);
  std::string getClientIP() const { return clientIP; }
  std::string getClientName() const { return clientName; }
  int getClientSocket() const { return clientSocket; }
  int getLocalPort() const { return localPort; }
  int getLocalSocket() const { return localSocket; }
  bool serve(void);
  const char *getNextEventMessage();
  Topic* getEventTopic() const { return topic; }
  int getEventID() const { return EventID; }
  int getMessageSize() const { return messageSize; }
  std::string getEventProducer() const { return EventProducer; }
  std::string getErrorMessage() const { return errorMessage; }
  std::string getErrorCode() const { return errorCode; }
  void setRecvTimeout(const int& t) { SOAP->recv_timeout = t; }
  void setSendTimeout(const int& t) { SOAP->send_timeout = t; }
  const std::vector<monitortypes__Event>& getEvents( void ) const { return Events; }
};

#endif
