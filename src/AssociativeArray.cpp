/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna and Adrian Marrs
// Copyright (c) 2005 by the
// Regents of the University of Michigan.
// All Rights Reserved.
// 
// The latest version of this program is available from:
// 
//   http://eyegene.ophthy.med.umich.edu/madeline/
//   
// Released under the GNU General Public License.
// A copy of the GPL is included in the distribution
// package of this software, or see:
// 
//   http://www.gnu.org/copyleft/
//   
// ... for licensing details.
// 
/////////////////////////////////////////////////////////
//
// 2005.07.17.RK.ET
//
#include "AssociativeArray.h"

///////////////////////////////////
//
// PRIVATE METHODS:
//
///////////////////////////////////

bool AssociativeArray::_keyIsValid(Data* d, const std::string key){
	
	// Used for warning messages:
	const char *methodName="AssociativeArray::_keyIsValid()";
	
	// Approximate numbers are not allowed as keys:
	if(d->getDataType() == NUMBER){
		if((static_cast<Number*>(d))->isApproximate()){
			Warning(methodName,
			        "%1$s AssociativeArray does not support approximate key %2$s",
			        _name.c_str(),
			        key.c_str() 
			);
			delete d;
			return false;
		}
	}
	// Approximate dates are not allowed as keys:
	if(d->getDataType() == DATE){
		if((static_cast<Date*>(d))->isApproximate()){
			Warning(methodName,
			        "%1$s AssociativeArray does not support approximate key %2$s",
			        _name.c_str(),
			        key.c_str()
			);
			delete d;
			return false;
		}
	}
	// Missing values are not allowed as keys:
	if(d->isMissing()){
		Warning(methodName,
		        "Missing cannot be set as a key in %1$s AssociativeArray",
		        _name.c_str()
		);
		delete d;
		return false;
	}else{
		// Check if the map already contains the key to be inserted:
		std::map<Data*,Data*,compare>::iterator iter;
		iter = _map.find(d);
		if(iter != _map.end()){
			Warning(methodName,
			        "%1$s already exists as a key in AssociativeArray %2$s",
			        key.c_str(),
			        _name.c_str()
			);
			delete d;
			return false;
		}
	}
	delete d;
	return true;
	
}


///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// Assignment Operator:
//
AssociativeArray& AssociativeArray::operator= (const AssociativeArray& s){
	
	if (this != &s) { // Check for self-assignment
		//
		// In case of arrays the keyType and valueType should match between the LHS and RHS
		// 
		if(_keyType != s._keyType || _valueType != s._valueType){
			Warning("AssociativeArray %s cannot be assigned to %s as the key type/value type do not match",s._name.c_str(),_name.c_str());
			return *this;
		}
		// Delete all the previous entries from the map
		std::map<Data*,Data*,compare>::const_iterator p;
		p=_map.begin();
		while(p != _map.end()){
			delete (p->first);
			delete (p->second);
			p++;
		}
		_map.clear();
		//std::cout << "No. of elements in the old map " << _map.size() << std::endl;
		p = s._map.begin();
		while(p != s._map.end()){
			_map.insert(std::pair<Data*,Data*>(p->first->clone(),p->second->clone()));
			p++;
		}
	}
	return *this;
	
}

//
// get(): 
//!   Returns a std::string representation of the Associative Array
//
const std::string AssociativeArray::get( void ) const{
	
	std::ostringstream oss;
	oss << Variable::getName();
	oss << "[] = " <<  std::endl; 
	std::map<Data*,Data*,compare>::const_iterator p;
	p = _map.begin();
	while(p != _map.end()){
		oss << (p->first)->get() << " : " << (p->second)->get() << std::endl;
		p++;
	}
	return oss.str();
	
}

//
// getValueType(): 
//!   Returns a string representation of the value type
//
const std::string AssociativeArray::getValueType() const{
	
	if(_valueType == ANY) return "a";
	std::string temp;
	if(_valueType & BOOLEAN){
		temp += "b";
	}
	if(_valueType & DATE){
		temp += "d";
	}
	if(_valueType & GENOTYPE){
		temp += "g";
	}
	if(_valueType & NUMBER){
		temp += "n";
	}
	if(_valueType & STRING){
		temp += "s";
	}
	return temp;
	
}

//
// insert(): 
//!   Inserts a key-value pair into the Associative Array
//
void AssociativeArray::insert(const short keyType,const std::string key, const short valueType,const std::string value){
	
	// Used for warning messages:
	const char *methodName="AssociativeArray::insert()";
	
	Data* d;
	// Check if the keyType is valid
	if((keyType & _keyType) || (_keyType==ANY)){ 
		switch(keyType){
			case DATE: if((valueType & _valueType) || (_valueType==ANY)){ // Check if the valueType is valid 
				d = new Date(key);
				if(_keyIsValid(d,key))
				switch(valueType){
					case BOOLEAN: _map.insert(std::pair<Data*,Data*>(new Date(key),new Boolean(value)));
						break;
					case DATE: _map.insert(std::pair<Data*,Data*>(new Date(key),new Date(value)));
						break;
					case GENOTYPE: _map.insert(std::pair<Data*,Data*>(new Date(key),new Genotype(value)));
						break;
					case NUMBER: _map.insert(std::pair<Data*,Data*>(new Date(key),new Number(value)));
						break;
					case STRING: _map.insert(std::pair<Data*,Data*>(new Date(key),new String(value)));
						break;
					case ANY: 
						Warning(methodName,
						        "In AssociativeArray %1$s with DATE key type, ANY as value type is invalid",
						        _name.c_str()
						);
						break;
					}
				}else{
					
					Warning(methodName,
					        "In AssociativeArray %1$s with DATE key type, value type of %2$s is invalid",
					        _name.c_str(),
					        value.c_str()
					);
					
				}
				break;
			case GENOTYPE: if((valueType & _valueType) || (_valueType==ANY)){ // Check if the valueType is valid 
				d = new Genotype(key);
				if(_keyIsValid(d,key))
				switch(valueType){
					case BOOLEAN: _map.insert(std::pair<Data*,Data*>(new Genotype(key),new Boolean(value)));
						break;
					case DATE: _map.insert(std::pair<Data*,Data*>(new Genotype(key),new Date(value)));
						break;
					case GENOTYPE: _map.insert(std::pair<Data*,Data*>(new Genotype(key),new Genotype(value)));
						break;
					case NUMBER: _map.insert(std::pair<Data*,Data*>(new Genotype(key),new Number(value)));
						break;
					case STRING: _map.insert(std::pair<Data*,Data*>(new Genotype(key),new String(value)));
						break;
					case ANY: 
						Warning(methodName,
						        "In AssociativeArray %1$s with GENOTYPE key type, ANY as value type is invalid",
						        _name.c_str()
						);
						break;
					}
				}else{
					
					Warning(methodName,
					        "In AssociativeArray %1$s with GENOTYPE key type, value type of %2$s is invalid",
					        _name.c_str(),
					        value.c_str()
					);
				}
				break;
				
			case NUMBER: if((valueType & _valueType) || (_valueType==ANY)){ // Check if the valueType is valid 
				d = new Number(key);
				if(_keyIsValid(d,key))
				switch(valueType){
					case BOOLEAN: _map.insert(std::pair<Data*,Data*>(new Number(key),new Boolean(value)));
						break;
					case DATE: _map.insert(std::pair<Data*,Data*>(new Number(key),new Date(value)));
						break;
					case GENOTYPE: _map.insert(std::pair<Data*,Data*>(new Number(key),new Genotype(value)));
						break;
					case NUMBER: _map.insert(std::pair<Data*,Data*>(new Number(key),new Number(value)));
						break;
					case STRING: _map.insert(std::pair<Data*,Data*>(new Number(key),new String(value)));
						break;
					case ANY: 
						Warning(methodName,
						        "In AssociativeArray %1$s with NUMBER key type, ANY as value type is invalid",
						        _name.c_str()
						);
						break;
					}
				}else{
					 
					Warning(methodName,
					        "In AssociativeArray %1$s with NUMBER key type, value type of %2$s is invalid",
					        _name.c_str(),
					        value.c_str()
					);
				}
				break;
			case STRING: if((valueType & _valueType) || (_valueType==ANY)){ // Check if the valueType is valid 
				d = new String(key);
				if(_keyIsValid(d,key))
				switch(valueType){
					case BOOLEAN: _map.insert(std::pair<Data*,Data*>(new String(key),new Boolean(value)));
						break;
					case DATE: _map.insert(std::pair<Data*,Data*>(new String(key),new Date(value)));
						break;
					case GENOTYPE: _map.insert(std::pair<Data*,Data*>(new String(key),new Genotype(value)));
						break;
					case NUMBER: _map.insert(std::pair<Data*,Data*>(new String(key),new Number(value)));
						break;
					case STRING: _map.insert(std::pair<Data*,Data*>(new String(key),new String(value)));
						break;
					case ANY: 
						Warning(methodName,
						        "In AssociativeArray %1$s with STRING key type, ANY as value type is invalid",
						        _name.c_str()
						);
						break;
					}
				}else{
					Warning(methodName,
					        "In AssociativeArray %1$s with STRING key type, value type of %2$s is invalid",
					        _name.c_str(),
					        value.c_str()
					);
				}
				break;
			case BOOLEAN: if((valueType & _valueType) || (_valueType == ANY)){ // Check if the valueType is valid
				d = new Boolean(key);
				if(_keyIsValid(d,key))
				switch(valueType){
					case BOOLEAN: _map.insert(std::pair<Data*,Data*>(new Boolean(key),new Boolean(value)));
						break;
					case DATE:   _map.insert(std::pair<Data*,Data*>(new Boolean(key),new Date(value)));
						break;
					case GENOTYPE: _map.insert(std::pair<Data*,Data*>(new Boolean(key),new Genotype(value)));
						break;
					case NUMBER: _map.insert(std::pair<Data*,Data*>(new Boolean(key),new Number(value)));
						break;
					case STRING: _map.insert(std::pair<Data*,Data*>(new Boolean(key),new String(value)));
						break;
					case ANY:
						Warning(methodName,
						        "In AssociativeArray %1$s with BOOLEAN key type, ANY as value type is invalid",
						        _name.c_str()
						);
						break;
					}
				}else{
					
					Warning(methodName,
					        "In AssociativeArray %1$s with BOOLEAN key type, value type of %2$s is invalid",
					        _name.c_str(),
					        value.c_str()
					);
				}
				break;
			case ANY: // ANY not allowed as a specific key
				Warning(methodName,
				        "In AssociativeArray %1$s, ANY not allowed as a specific key",
				        _name.c_str()
				);
				break;
		}
	}else{
		
		Warning(methodName,
		        "In AssociativeArray %1$s, key type of %2$s is invalid",
		        _name.c_str(),
		        key.c_str()
		);
	}
}

//
// getKeyType(): 
//!   Returns a string representation of the key type
//
const std::string AssociativeArray::getKeyType() const{
	
	if(_keyType == ANY) return "a";
	std::string temp;
	if(_keyType & BOOLEAN){
		temp += "b";
	}
	if(_keyType & DATE){
		temp += "d";
	}
	if(_keyType & GENOTYPE){
		temp += "g";
	}
	if(_keyType & NUMBER){
		temp += "n";
	}
	if(_keyType & STRING){
		temp += "s";
	}
	return temp;
	
}
//
// Free function: ostream operator <<
//
std::ostream & operator<< (std::ostream & out, const AssociativeArray & var){
	
	return out << var.get();
	
}

bool operator==(const AssociativeArray& a, const AssociativeArray& b){
	
	if(a._keyType != b._keyType || a._valueType != b._valueType) return false;
	if(a._map.size() != b._map.size()) return false;
	std::map<Data*,Data*,compare>::const_iterator p1;
	std::map<Data*,Data*,compare>::const_iterator p2;
	p1 = a._map.begin();
	p2 = b._map.begin();
	while(p1 != a._map.end()){
		if((p1->first->getDataType() != p2->first->getDataType()) || (p1->second->getDataType() != p2->second->getDataType())) return false;
		if(*(p1->first) == *(p2->first) && *(p1->second) == *(p2->second)) ; else return false;
		p1++; p2++;
	}
	return true;
	
}
