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
        Dialect: a class to handle dialects

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: Dialect.cpp,v 1.4.10.1 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/Dialect.h"
#include <string>
#include <vector>

using namespace std;

namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;

cemon_api::DialectW::DialectW(const string& _name) {
   Name = _name;
}

cemon_api::DialectW::DialectW(const DialectW& d) {

  //cout << "DialectW COPYCTOR: d.Name=["<<d.Name<<"]"<<endl;

   this->Name = d.Name;
   
   //cout << "DialectW COPYCTOR: this->Name=["<<this->Name<<"]"<<endl;
   
   // QueryLanguage is a vector<string>, then
   // it is safe to copy it
   this->QueryLanguage = d.QueryLanguage;
}

cemon_api::DialectW::~DialectW(void) throw() 
{
}

// DialectW& DialectW::operator=(const DialectW& aDialect) {
//   if(this != &aDialect) {
//     this->Name = aDialect.Name;
//     this->QueryLanguage = aDialect.QueryLanguage;
// /*    if(QueryLanguage.size()) { 
//       QueryLanguage.clear(); 
//       //delete(QueryLanguage); queryLanguage=NULL; 
//     }
//     //QueryLanguage = new vector<string>;
//     if(aDialect.QueryLanguage.size()) {
//       for(vector<string>::const_iterator qit = aDialect.QueryLanguage.begin();
// 	  qit != aDialect.QueryLanguage.end();
// 	  qit++) { //unsigned int j=0; j<aDialect.QueryLanguage.size(); j++) {
// 	     QueryLanguage.push_back( *qit );
//       }
//     }*/
//   }
//   return *this;
// }

int cemon_api::DialectW::getNumberOfQueryLanguages(void) const {
  return QueryLanguage.size();
}

string cemon_api::DialectW::getQueryLanguageAt(const int& i) const {
  return QueryLanguage.at(i);
}

string cemon_api::DialectW::getDialectName(void) const {
  return Name;
}

void cemon_api::DialectW::setDialectName(const string& newName) {
  this->Name = newName;
}

void cemon_api::DialectW::setQueryLanguageAt(const int& i, const string& newQl) {
  QueryLanguage.at(i) = newQl;
}

int cemon_api::DialectW::addQueryLanguage(const string newQl) {
  QueryLanguage.push_back(newQl);
  return QueryLanguage.size();
}

void cemon_api::DialectW::print(void) {
  cout << "Dialect ["<<Name<<"]"<<endl;
  int j = 0;
  if(QueryLanguage.size())
    for(vector<string>::const_iterator qit = QueryLanguage.begin();
	qit != QueryLanguage.end();
	qit++) //unsigned int j=0; j<QueryLanguage.size(); j++)
      cout << "  queryLanguage["<<j++<<"] = ["<< *qit <<"]"<<endl;
}
