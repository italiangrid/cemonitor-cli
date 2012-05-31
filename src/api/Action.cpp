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
        Action: a wrapper class to handle actions

        Authors: Alvise Dorigo <alvise.dorigo@pd.infn.it>
        Version info: $Id: Action.cpp,v 1.6.10.1 2010/03/22 12:11:11 pandreet Exp $
*/

#include "glite/ce/monitor-client-api-c/Action.h" // this also includes Parameter.h and Property.h
#include <string>
#include <vector>

#define PARAMETER_FIELD parameter
#define PROPERTY_FIELD property

using namespace std;
namespace cemon_api = glite::ce::monitor_client_api::soap_proxy;
//namespace cemon_ex = glite::ce::monitor_client_api::cemon_exceptions;

//______________________________________________________________________________
cemon_api::ActionW::ActionW(const string& name, const string& type, 
			    const bool doActionWhenQueryIs) 
{
  this->name = new string(name);
  this->doActionWhenQueryIs = doActionWhenQueryIs;
  this->type = new string(type);
  id = new string("");
  jarPath = new string("");
}

//______________________________________________________________________________
// ActionW::ActionW(const ACTION& _act) 
// {
//   cout << "CALLED ActionW::ActionW(const ACTION& _act) "<<endl;
//   this->init( _act );
  
// }

//______________________________________________________________________________
cemon_api::ActionW::ActionW(const ActionW& _act) 
{
  //cout << "CALLED ActionW::ActionW(const ActionW& _act) "<<endl;
  this->init( _act );
  
}


//______________________________________________________________________________
cemon_api::ActionW::~ActionW() {
  if(this->PARAMETER_FIELD.size()) {
    for(unsigned int i = 0; i < this->PARAMETER_FIELD.size(); ++i) {
      if( this->PARAMETER_FIELD.at(i) )
	delete((ParameterW*)this->PARAMETER_FIELD.at(i));
    }
//     for(vector<monitortypes__Parameter*>::const_iterator pit = this->PARAMETER_FIELD.begin();
//         pit != this->PARAMETER_FIELD.end();
// 	pit++) { //unsigned int j=0; j<this->PARAMETER_FIELD.size(); j++) {
//       if(*pit) delete((ParameterW*)*pit);
//     }
  }
  if(this->PROPERTY_FIELD.size()) {
//     for(vector<monitortypes__Property*>::const_iterator pit = this->PROPERTY_FIELD.begin();
//         pit != this->PROPERTY_FIELD.end();
// 	pit++) { //unsigned int j=0; j<this->PROPERTY_FIELD.size(); j++) {
//       if(*pit) delete((PropertyW*)*pit);
//     }
    for(unsigned int i = 0; i < this->PROPERTY_FIELD.size(); ++i) {
      if( this->PROPERTY_FIELD.at(i) )
	delete((PropertyW*)this->PROPERTY_FIELD.at(i));
    }
  }
  delete id;
  delete jarPath;
  delete name;
  delete type;
}

//______________________________________________________________________________
// ActionW& ActionW::operator=(const ACTION& anAction) {

//   return *this;
// }

//______________________________________________________________________________
string cemon_api::ActionW::getActionName(void) const {
  return *name;
}

//______________________________________________________________________________
string cemon_api::ActionW::getActionType(void) const {
  return *type;
}

//______________________________________________________________________________
bool cemon_api::ActionW::getPerformAction(void) const {
  return doActionWhenQueryIs;
}

//______________________________________________________________________________
int cemon_api::ActionW::addParameter(ParameterW* p) {
  ParameterW* P = new ParameterW(p->name, p->value);
  this->PARAMETER_FIELD.push_back(P);
  return this->PARAMETER_FIELD.size();
}

//______________________________________________________________________________
int cemon_api::ActionW::getNumberOfParameters(void) const {
  return this->PARAMETER_FIELD.size();
}

//______________________________________________________________________________
int cemon_api::ActionW::getNumberOfProperties(void) const {
  return this->PROPERTY_FIELD.size();
}

//______________________________________________________________________________
cemon_api::ParameterW* cemon_api::ActionW::getParameterAt(const int& i) const {
  return (ParameterW*)this->PARAMETER_FIELD.at(i);
}

//______________________________________________________________________________
cemon_api::PropertyW* cemon_api::ActionW::getPropertyAt(const int& i) const {
  return (PropertyW*)this->PROPERTY_FIELD.at(i);
}

//______________________________________________________________________________
string cemon_api::ActionW::getActionID(void) const {
  return *(id);
}

//______________________________________________________________________________
void cemon_api::ActionW::print(void) const {
  cout << "Action [" << *name<<":"<<*type;
  if(id) cout << ":"<<*id;
  cout << "]"<<endl;
  if(this->PARAMETER_FIELD.size()) {
    for(vector<monitortypes__Parameter*>::const_iterator pit = this->PARAMETER_FIELD.begin();
        pit != this->PARAMETER_FIELD.end();
	pit++) //unsigned int k=0; k<this->PARAMETER_FIELD.size(); k++)
      cout << "   Parameter ["<<(*pit)->name<<
	"]=["<<(*pit)->value<<"]"<<endl;
  }
  if(this->PROPERTY_FIELD.size()) {
    for(vector<monitortypes__Property*>::const_iterator pit = this->PROPERTY_FIELD.begin();
        pit != this->PROPERTY_FIELD.end();
	pit++) //unsigned int k=0; k<this->PROPERTY_FIELD.size(); k++)
      cout << "   Property ["<<(*pit)->name<<
	"]=["<<(*pit)->value<<"]"<<endl;
  }
}

//______________________________________________________________________________
void cemon_api::ActionW::init( const ActionW& _act ) 
{
  
  if(_act.name)
    name = new string(*_act.name);
  else
    name = new string("");

  if(_act.type)
    type = new string(*_act.type);
  else
    type = new string("");

  if(_act.id)
     id = new string(*_act.id);
  else
     id = new string("");

  doActionWhenQueryIs = _act.doActionWhenQueryIs;
  if(_act.PARAMETER_FIELD.size()) {
    for(vector<monitortypes__Parameter*>::const_iterator pit = _act.PARAMETER_FIELD.begin();
        pit != _act.PARAMETER_FIELD.end();
	++pit) { 
      ParameterW *tmpP = new ParameterW( (const ParameterW&)*(*pit) );
      this->PARAMETER_FIELD.push_back(tmpP);
    }
  }
  if(_act.PROPERTY_FIELD.size()) {
    for(vector<monitortypes__Property*>::const_iterator pit = _act.PROPERTY_FIELD.begin();
        pit != _act.PROPERTY_FIELD.end();
	++pit) { 
      PropertyW *tmpP = new PropertyW( (const PropertyW&)*(*pit) );
      this->PROPERTY_FIELD.push_back(tmpP);
    }
  }
  jarPath = new string("");

}

// //______________________________________________________________________________
// void ActionW::setEqual( const ActionW& anAction )
// {
//   if(this!=&anAction) {
//     if(this->PARAMETER_FIELD.size()) {
//       for(vector<monitortypes__Parameter*>::const_iterator pit = this->PARAMETER_FIELD.begin();
//           pit != this->PARAMETER_FIELD.end();
// 	  pit++) //unsigned int j=0; j<this->PARAMETER_FIELD.size(); j++)
//         if(*pit) delete((ParameterW*)*pit);
//       this->PARAMETER_FIELD.clear();
//     }
//     if(this->PROPERTY_FIELD.size()) {
//       for(vector<monitortypes__Property*>::const_iterator pit = this->PROPERTY_FIELD.begin();
//           pit != this->PROPERTY_FIELD.end();
// 	  pit++) //unsigned int j=0; j<this->PROPERTY_FIELD.size(); j++)
//         if(*pit) delete((PropertyW*)*pit);
//       this->PROPERTY_FIELD.clear();
//     }
//     if(id) { delete id; id=NULL; }
//     if(name) { delete name; name = NULL; }
//     if(type) { delete type; type = NULL; }

//     if(anAction.id)
//       id = new string(*anAction.id);
//     else
//       id = new string("");

//     if(anAction.name)
//       name = new string(*anAction.name);
//     else
//       name = new string("");

//     if(anAction.type)
//       type = new string(*anAction.type);
//     else
//       type = new string("");

//     //*id = *(anAction.id);

//     doActionWhenQueryIs = anAction.doActionWhenQueryIs;

//     if(anAction.PARAMETER_FIELD.size()) {
//       for(vector<monitortypes__Parameter*>::const_iterator pit = anAction.PARAMETER_FIELD.begin();
//           pit != anAction.PARAMETER_FIELD.end();
// 	  pit++) { //unsigned int j=0; j<anAction.PARAMETER_FIELD.size(); j++) {
// 	ParameterW *tmpP = new ParameterW(*(*pit));
// 	this->PARAMETER_FIELD.push_back(tmpP);
//       }
//     }

//     if(anAction.PROPERTY_FIELD.size()) {
//       for(vector<monitortypes__Property*>::const_iterator pit = anAction.PROPERTY_FIELD.begin();
//           pit != anAction.PROPERTY_FIELD.end();
// 	  pit++) { //unsigned int j=0; j<anAction.PROPERTY_FIELD.size(); j++) {
// 	PropertyW *tmpP = new PropertyW(*(*pit));
// 	this->PROPERTY_FIELD.push_back(tmpP);
//       }
//     }
//   }  
// }
