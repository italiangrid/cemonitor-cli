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
        AbsRequest: a C++ parent class for all CEMonitor requests

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: AbsRequest.cpp,v 1.17.8.3.2.4 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/AbsRequest.h"
#include "glite/ce/monitor-client-api-c/CEMON_CLIENT.nsmap"
//#include "glite/ce/monitor-client-api-c/
#include <string>
#include <unistd.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <openssl/ssl.h>
#include <cstdlib>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>

using namespace std;

/*void sigpipe_handle(int x) { 
  std::cerr << "AbsRequezst - PIPE handled; int x = [" << x <<"]" << std::endl;
}*/

namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;

const long getProxyTimeLeft( const string& ) throw ( cemon_ex::AuthenticationInitException& );
time_t ASN1_UTCTIME_get(const ASN1_UTCTIME*);

namespace {

class helper_BIOnew {
    BIO *in;
  public:
    helper_BIOnew(BIO* infile) : in(infile) {}
    ~helper_BIOnew() { 
      if(in) {
        BIO_set_close(in, BIO_CLOSE);    
      //BIO_close(in);
        BIO_free(in);
      } 
    }
  };

  class helper_X509new {
    X509* x;
  public:
    helper_X509new(X509* xin) : x(xin) {}
    ~helper_X509new() { if(x) X509_free(x); }
  };


}

/******************************************************************
 *
 * 
 *
 */
cemon_api::AbsRequest::AbsRequest() throw(cemon_ex::GeneralException&) 
  : m_serviceURL(""),
    m_ctx(NULL),
    m_BaseFault_integer( m_BaseFault.soap_type() ),
    m_SubscriptionFault_integer( m_SubscriptionFault.soap_type() ),
    m_TopicNotSupportedFault_integer( m_TopicNotSupportedFault.soap_type() ),
    m_DialectNotSupportedFault_integer( m_DialectNotSupportedFault.soap_type() ),
    m_GenericFault_integer( m_GenericFault.soap_type() ),
    m_AuthenticationFault_integer( m_AuthenticationFault.soap_type() ),
    m_AuthorizationFault_integer( m_AuthorizationFault.soap_type() ),
    m_SubscriptionNotFoundFault_integer( m_SubscriptionNotFoundFault.soap_type() ),
    m_errorMessage( "" ),
    m_errorCode( "" )
{
  m_SOAP = 0;
  m_SOAP = soap_new();
  if(!m_SOAP)
    throw cemon_ex::GeneralException("SOAP struct initialization failed!");
  
  this->setNamespace();

  //signal(SIGPIPE, sigpipe_handle);

}

/******************************************************************
 *
 * 
 *
 */
void cemon_api::AbsRequest::authenticate(const char* certificate, const char* key) throw(cemon_ex::AuthenticationInitException&)
{
 
  /**
   * This method raises only AuthenticationInitException that derives from AbsException that derives std::exception
   */

  if( !boost::filesystem::exists( boost::filesystem::path(certificate, boost::filesystem::native) ) ) {
    string err = "Certificate file [" + string(certificate) + "] missing on disk";
    throw cemon_ex::AuthenticationInitException(err);
  }
  if( !boost::filesystem::exists( boost::filesystem::path(key, boost::filesystem::native) ) ) {
    string err = "Key file [" + string(key) + "] missing on disk";
    throw cemon_ex::AuthenticationInitException(err);
  }

  ifstream iskey(key, ios::in);
  ifstream iscert(certificate, ios::in);
  if(!iskey) {
    throw cemon_ex::AuthenticationInitException(string("keyfile [")
				      +key
				      +"] is there but cannot open it for reading");
  }
  
  if(!iscert) {
    throw cemon_ex::AuthenticationInitException(string("certfile [")
				      +certificate
				      +"] is there but cannot open it for reading");
  }

  time_t leftcert = getProxyTimeLeft(certificate);// this can raises an AuthenticationInitException deriving from AbsException deriving from std::exception
  leftcert = (leftcert<0) ? 0 : leftcert;
  if(!leftcert)
    throw cemon_ex::AuthenticationInitException("Proxy certificate has expired. Please Check it.");

  if ( glite_gsplugin_init_context(&m_ctx) ) {
    throw cemon_ex::AuthenticationInitException("gsplugin initialization failed");
  }
  
  if (glite_gsplugin_set_credential(m_ctx, strdup(certificate), strdup(key))) {
    throw cemon_ex::AuthenticationInitException("Cannot set credentials in the gsoap-plugin context");
  }

  /**
   * Set of the SOAP TIMEOUT
   */
  struct timeval T;
  T.tv_sec = (time_t)60; // FIXME: SOAP Timeout must be customizable
  T.tv_usec = 0;
  glite_gsplugin_set_timeout( m_ctx, &T );
  /*********/

  if(!getenv("CEMON_NO_AUTH")) {
    if ( soap_register_plugin_arg(m_SOAP, glite_gsplugin, m_ctx) )
      throw cemon_ex::AuthenticationInitException("soap_register_plugin_arg FAILED");
  }

}

/******************************************************************
 *
 * 
 *
 */
cemon_api::AbsRequest::~AbsRequest( void ) throw()
{
  
  cleanup();
  soap_done(m_SOAP); // detaches SOAP runtime
  if( m_ctx ) glite_gsplugin_free_context( m_ctx );
  if(m_SOAP) free(m_SOAP);
  //cout << "AbsRequest has been destroyed"<<endl;
}

/******************************************************************
 *
 *
 *
 */
void cemon_api::AbsRequest::setError() throw() {

  if( m_SOAP->error != SOAP_FAULT  && m_SOAP->error != SOAP_CLI_FAULT && m_SOAP->error != SOAP_SVR_FAULT)
    {
      string err;
    
      if( m_SOAP->endpoint && (string(m_SOAP->endpoint)!="") )
        err = string("Connection to service [")+m_SOAP->endpoint+"] failed: ";
      else
        err = string("Connection to service [http(s)://")+m_SOAP->host+m_SOAP->path+"] failed: ";

      char *faultstring =  (char*)*soap_faultstring(m_SOAP);
      char *faultcode = (char*)*soap_faultcode(m_SOAP);
      char *faultsubcode = (char*)*soap_faultsubcode(m_SOAP);
      char *faultdetail = (char*)*soap_faultdetail(m_SOAP);

      if (faultstring)
        err += string("FaultString=[")+faultstring+"]";

      if (faultcode)
        err += string(" - FaultCode=[")+faultcode+"]";

      if (faultsubcode)
        err += string(" - FaultSubCode=[")+faultsubcode+"]";

      if (faultdetail)
        err += string(" - FaultDetail=[")+faultdetail+"]";

      m_errorMessage = err;
      
      return;
    }

  if (m_SOAP->error) {
    if (!*soap_faultcode(m_SOAP))
      soap_set_fault(m_SOAP);
    m_errorMessage = string(*soap_faultstring(m_SOAP));
    m_errorCode    = string(*soap_faultcode(m_SOAP));
  }
}

/******************************************************************
 *
 * 
 *
 */
void* cemon_api::AbsRequest::getFault()  throw() {
  if (m_SOAP->version == 2) {
    if(m_SOAP->fault && m_SOAP->fault->SOAP_ENV__Detail)
      return m_SOAP->fault->SOAP_ENV__Detail->fault;
    else
      return NULL;
  }
  else {
    if(m_SOAP->fault) {
      if(m_SOAP->fault->detail) {
	return m_SOAP->fault->detail->fault;
      } else {
	return NULL;
      }
    } else {
      return NULL;
    }
  }
}

/******************************************************************
 *
 *
 *
 */
int cemon_api::AbsRequest::getFaultType()  throw() {
  if (m_SOAP->version == 2) {
    if(m_SOAP->fault) {
      if(m_SOAP->fault->SOAP_ENV__Detail) {
	//	cerr << "SOAP V2"<<endl;
	return m_SOAP->fault->SOAP_ENV__Detail->__type;
      }
      else
	return -1;
    }
    else
      return -1;
  }
  else {
    if(m_SOAP->fault) {
      
      if(m_SOAP->fault->detail) {
	return m_SOAP->fault->detail->__type;
      }
      else {
	return -1;
      }
    }
    else {
      return -1;
    }
  }
}

/******************************************************************
 *
 *
 *
 */
void cemon_api::AbsRequest::cleanup() throw() {
  soap_destroy(m_SOAP); // deletes deserialized data object
  soap_end(m_SOAP);     // removes deserialized data
}

/******************************************************************
 *
 *
 *
 */
void cemon_api::AbsRequest::throwCEException(int ex) throw(cemon_ex::GenericException&,
							   cemon_ex::TopicNotSupportedException&, 
							   cemon_ex::DialectNotSupportedException&,
							   cemon_ex::SubscriptionException&,
							   cemon_ex::AuthenticationException&,
							   cemon_ex::AuthorizationException&,
							   cemon_ex::SubscriptionNotFoundException&
							   )
{
  string _ErrorCode(""), _FaultCause(""), _Description("");
  cefaults__BaseFaultType *fault = (cefaults__BaseFaultType *)getFault();

  string _MethodName("");
  time_t tstamp = 0;

  if(fault) {
    _MethodName = fault->MethodName;
    tstamp = fault->Timestamp;
    if(fault->ErrorCode)
      _ErrorCode = string(*fault->ErrorCode);
    if(fault->Description)
      _Description = string(*fault->Description);
    if(fault->FaultCause)
      _FaultCause = string(*fault->FaultCause);
  }
    
  if(ex == m_GenericFault_integer )
	throw  cemon_ex::GenericException(_MethodName, tstamp, _ErrorCode, _Description, _FaultCause);
  if( ex == m_TopicNotSupportedFault_integer )
	throw  cemon_ex::TopicNotSupportedException(_MethodName, tstamp, _ErrorCode, _Description, _FaultCause);
  
  if( ex == m_TopicNotSupportedFault_integer )
    throw  cemon_ex::TopicNotSupportedException(_MethodName, tstamp, _ErrorCode, _Description, _FaultCause);

  if( ex == m_DialectNotSupportedFault_integer )
    throw  cemon_ex::DialectNotSupportedException(_MethodName, tstamp, _ErrorCode, _Description, _FaultCause);

  if( ex == m_SubscriptionFault_integer )
    throw  cemon_ex::SubscriptionException(_MethodName, tstamp, _ErrorCode, _Description, _FaultCause);

  if( ex == m_AuthorizationFault_integer )
    throw  cemon_ex::AuthorizationException(_MethodName, tstamp, _ErrorCode, _Description, _FaultCause);

  if( ex == m_AuthenticationFault_integer )
    throw  cemon_ex::AuthenticationException(_MethodName, tstamp, _ErrorCode, _Description, _FaultCause);

  if( ex == m_SubscriptionNotFoundFault_integer )
    throw cemon_ex::SubscriptionNotFoundException(_MethodName, tstamp, _ErrorCode, _Description, _FaultCause);

  

  throw cemon_ex::GenericException("Client internal error", time(NULL), "", "Fault type not recognized", "");
}

/******************************************************************
 *
 *
 *
 */
void cemon_api::AbsRequest::throwAbsException() throw(cemon_ex::ServiceNotFoundException&,
		 	  			      cemon_ex::GeneralException&)
{

  string mex = getErrorMessage();
  string code= getErrorCode();

  if(mex == "No route to host" ||
     mex == "Host not found" ||
     code.find("Server.NoService", 0) != string::npos ||
     mex == "Connection refused" ||
     mex == "HTTP error")
    {
      throw cemon_ex::ServiceNotFoundException(m_errorMessage);
    }
  throw cemon_ex::GeneralException(mex);
}

/******************************************************************
 *
 *
 *
 */
void cemon_api::AbsRequest::setNamespace() throw() {
  soap_set_namespaces(m_SOAP, CEMON_CLIENT_namespaces);
}

/*************************************************************************
	private method:	converts ASN1_UTCTIME to time_t
*************************************************************************/
time_t ASN1_UTCTIME_get(const ASN1_UTCTIME *s)
{
  struct tm tm;
  int offset;
  memset(&tm,'\0',sizeof tm);
#define g2(p) (((p)[0]-'0')*10+(p)[1]-'0')
  tm.tm_year=g2(s->data);
  if(tm.tm_year < 50)
    tm.tm_year+=100;
  tm.tm_mon=g2(s->data+2)-1;
  tm.tm_mday=g2(s->data+4);
  tm.tm_hour=g2(s->data+6);
  tm.tm_min=g2(s->data+8);
  tm.tm_sec=g2(s->data+10);
  if(s->data[12] == 'Z')
    offset=0;
  else
    {
      offset=g2(s->data+13)*60+g2(s->data+15);
      if(s->data[12] == '-')
	offset= -offset;
    }
#undef g2
  
  return timegm(&tm)-offset*60;
}

/*************************************************************************
* gets the proxy time-left
*************************************************************************/
const long getProxyTimeLeft( const string& pxfile ) throw (cemon_ex::AuthenticationInitException&) 
{
  time_t timeleft = 0;
  BIO *in = NULL;
  X509 *x = NULL;
  in = BIO_new(BIO_s_file());
  helper_BIOnew h( in );

  if (in) {
    BIO_set_close(in, BIO_CLOSE);
    if (BIO_read_filename( in, pxfile.c_str() ) > 0) {
      x = PEM_read_bio_X509(in, NULL, 0, NULL);
      helper_X509new X(x);

      if (x) {
	
	timeleft = (ASN1_UTCTIME_get(X509_get_notAfter(x)) - time(NULL) ) / 60 ;

      } else{
	throw  cemon_ex::AuthenticationInitException(string("unable to read X509 proxy file: ")+ pxfile);
      }
    } else {
      
      throw  cemon_ex::AuthenticationInitException(string("unable to open X509 proxy file: ")+ pxfile);
    }
    //BIO_free(in);
    //free(x);
  } else {
    throw  cemon_ex::AuthenticationInitException(string("unable to allocate memory for the proxy file: ")+ pxfile);
  }
  return timeleft;
}
