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
        Topic: a class to handle topic

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: Topic.cpp,v 1.10.10.1 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/Topic.h"
#include "glite/ce/monitor-client-api-c/Dialect.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;

//______________________________________________________________________________
cemon_api::Topic::Topic(const string& name) {
  this->Name = name;
  visibility = NULL;
}


//______________________________________________________________________________
cemon_api::Topic::Topic(const Topic& t) {
  Name = t.Name;
  visibility=NULL;
  if(t.Dialect.size()) {
    for(vector<monitortypes__Dialect*>::const_iterator dit = t.Dialect.begin();
	dit != t.Dialect.end();
	dit++) 
      {
	cemon_api::DialectW *dial = 0;
        if ( *dit ) {
	  dial = new cemon_api::DialectW( (const cemon_api::DialectW&)*(*dit) );
	  //cout << "COPYCTOR di Topic..."<<endl;
	  //((DialectW*) dial)->print();
        }
	Dialect.push_back(dial);
      }
  }
}

//______________________________________________________________________________
cemon_api::Topic::~Topic() {
  if(Dialect.size()) {
    for(vector<monitortypes__Dialect*>::const_iterator dit = Dialect.begin();
	dit != Dialect.end();
	dit++) {
	cemon_api::DialectW* d = dynamic_cast< DialectW* >( *dit );
	if ( 0 != d ) {
	  delete d;
	}
    }
  }
}

// Topic& Topic::operator=(const Topic& aTopic) {
//   if(this!=&aTopic) {
//     Name = aTopic.Name;
//     Dialect.clear();
//     visibility = aTopic.visibility;
//     if(aTopic.Dialect.size()) {
//       for(vector<monitortypes__Dialect*>::const_iterator dit = aTopic.Dialect.begin();
// 	  dit != aTopic.Dialect.end();
// 	  dit++) {
//           DialectW *dial = 0;
//           if ( *dit ) {
//               dial = new DialectW( *(*dit) );
//           }
// 	Dialect.push_back(dial);
//       }
//     }
    
//   }
//   return *this;
// }

//______________________________________________________________________________
int cemon_api::Topic::addDialect(DialectW* d) 
{
  cemon_api::DialectW *newDial = 0;
  if ( d ) {
      newDial = new DialectW( *d ); // COPY CTOR WELL DEFINED;
  }
  Dialect.push_back( newDial );

  return Dialect.size();
}

//______________________________________________________________________________
void cemon_api::Topic::print(void) const {
  cout << "Topic [" << Name<<"]"<<endl;
  if(Dialect.size()) {
    for(vector<monitortypes__Dialect*>::const_iterator dit = Dialect.begin();
	dit != Dialect.end();
	dit++) {
      cout << "   Dialect ["<< (*dit)->Name<<"]"<<endl;
      vector<string> ql = (*dit)->QueryLanguage;
      if(ql.size()) {
	for(vector<string>::const_iterator qit = ql.begin();
	    qit != ql.end();
	    qit++)
	  cout << "      queryLanguage ["<< *qit <<"]"<<endl;
      }
    }
  }
}

//______________________________________________________________________________
int cemon_api::Topic::getNumOfDialects(void) const
{
  return Dialect.size();
}

//______________________________________________________________________________
cemon_api::DialectW* cemon_api::Topic::getDialectAt(const int& idx) {
  if( Dialect.begin() == Dialect.end() ) return NULL;
  return (cemon_api::DialectW*)Dialect.at(idx);
}
