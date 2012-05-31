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
        Policy: a class to handle policy

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: Policy.cpp,v 1.5.10.1 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/Policy.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;

// class SOAP_CMAC monitortypes__Policy
// {
// public:
//         monitortypes__Query *Query;     /* optional element of type monitortypes:Query */
//         std::vector<monitortypes__Action * >Action;     /* optional element of type monitortypes:Action */
//         int Rate;       /* required element of type xsd:int */
//         struct soap *soap;      /* transient */
// public:
//         virtual int soap_type() const { return 26; } /* = unique id SOAP_TYPE_monitortypes__Policy */
//         virtual void soap_default(struct soap*);
//         virtual void soap_serialize(struct soap*) const;
//         virtual int soap_put(struct soap*, const char*, const char*) const;
//         virtual int soap_out(struct soap*, const char*, int, const char*) const;
//         virtual void *soap_get(struct soap*, const char*, const char*);
//         virtual void *soap_in(struct soap*, const char*, const char*);
//                  monitortypes__Policy() { }
//         virtual ~monitortypes__Policy() { }
// };

//______________________________________________________________________________
cemon_api::Policy::Policy(const int& rate) {
  this->Rate = rate;
  this->Query = NULL;
}

//______________________________________________________________________________
cemon_api::Policy::~Policy() {
  if(Query) delete Query;
  if(this->Action.size()) {
    for(unsigned int j=0; j<this->Action.size(); j++) {
      if(this->Action.at(j)) delete((cemon_api::ActionW*)this->Action.at(j));
    }
  }
}

//______________________________________________________________________________
cemon_api::Policy::Policy(const cemon_api::Policy& p) {
    Query = 0;
    if ( p.Query ) {
      Query = new cemon_api::QueryW((const cemon_api::QueryW&)(*p.Query));
    }
  Rate  = p.Rate;
  if(p.Action.size()) {
    for(vector<monitortypes__Action*>::const_iterator ait = p.Action.begin();
	ait != p.Action.end();
	ait++) { //unsigned int j=0; j<p.Action.size(); j++) {
      cemon_api::ActionW *a = new cemon_api::ActionW( (const cemon_api::ActionW&)*(*ait) );
      this->Action.push_back(a);
    }
  }
}

//______________________________________________________________________________
// Policy& Policy::operator=(const Policy& p) {
//   if(this!=&p) {
//     if(Query) delete(Query);
//     Query = 0;
//     if ( p.Query ) {
//         Query = new QueryW(*p.Query);
//     }
//     Rate  = p.Rate;
    
//     if(this->Action.size()) {
//       for(vector<monitortypes__Action*>::const_iterator ait = this->Action.begin();
// 	  ait != this->Action.end();
// 	  ait++) { //unsigned int j=0; j<this->Action.size(); j++) {
// 	if( *ait ) delete( (ActionW*)*ait );
//       }
//       this->Action.clear();
//     }
    
//     if(p.Action.size()) {
//       for(vector<monitortypes__Action*>::const_iterator ait = p.Action.begin();
// 	  ait != p.Action.end();
// 	  ait++) { //unsigned int j=0; j<p.Action.size(); j++) {
// 	ActionW *tmpA = new ActionW( (const ActionW&)*(*ait) );
// 	this->Action.push_back(tmpA);
//       }
//     }
//   }
//   return *this;
// }

//______________________________________________________________________________
int cemon_api::Policy::addAction_Query(cemon_api::ActionW* a, QueryW* q) {
  cemon_api::ActionW *tmpA = new cemon_api::ActionW(*a);
  this->Action.push_back(tmpA);
  if(Query) delete(Query);
  Query = 0;
  if ( q ) {
      Query = new cemon_api::QueryW(*q);
  }
  return this->Action.size();
}

//______________________________________________________________________________
int cemon_api::Policy::addAction( cemon_api::ActionW* a ) {
  cemon_api::ActionW *tmpA = new cemon_api::ActionW(*a);
  this->Action.push_back(tmpA);
  return this->Action.size();
}

//______________________________________________________________________________
void cemon_api::Policy::setQuery( cemon_api::QueryW* q ) {
  if(Query) delete(Query);
  Query = 0;
  if ( q ) {
      Query = new cemon_api::QueryW(*q);
  }
}

//______________________________________________________________________________
void cemon_api::Policy::print(void) const {
  if(Query)
    cout << "Policy ["<< Query->Expression<<", "<<
      Query->QueryLanguage<<"]"<<endl;
  else
    cout << "Policy [UnNamed]"<<endl;
  if(this->Action.size()) {
    for(vector<monitortypes__Action*>::const_iterator ait = this->Action.begin();
	ait != this->Action.end();
	ait++) { //unsigned int j=0; j<this->Action.size(); j++) {
      cout << "  Action ["<< (*ait)->name <<
	"] do=" << (*ait)->doActionWhenQueryIs << endl;
      for(vector<monitortypes__Parameter*>::const_iterator pit = (*ait)->parameter.begin();
	  pit != (*ait)->parameter.end();
	  pit++) //igned int k=0; k < this->Action.at(j)->parameter.size(); k++)
	cout << "    Parameter "<</*this->action.at(j)->parameter<<*/": ["<<
	  (*pit)->name<<"]=["<<
	  (*pit)->value<<"]"<<endl;
    }
  }
}

//______________________________________________________________________________
cemon_api::ActionW* cemon_api::Policy::getActionAt(const int& idx) {
    return (cemon_api::ActionW*)this->Action.at(idx);
}
