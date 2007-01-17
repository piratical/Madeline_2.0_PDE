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
#include "Scalar.h"

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// Assignment Operator:
//
Scalar& Scalar::operator= (const Scalar& s){
	
	if (this != &s) {              // check for self-assignment
		if(s._valueType == ANY){
			// RHS is of type ANY. If the LHS is not of type ANY retain its type
			// but set the value to missing
			if(_valueType != ANY){
				_val->set("");
			}
		}else if(_valueType == ANY){
			// If the LHS is of type ANY change its type to 
			// that of RHS
			_val = s._val->clone();
			_valueType = s._valueType;
		}else{
			// assign value of RHS to LHS:
			_val->set(s._val->get());
		}
	}
	return *this;
	
}

//
// get():
//!   Returns a std::string representation of the scalar value
//
const std::string Scalar::get( void ) const{
	
	std::ostringstream oss;
	oss << getName();
	if(_valueType == ANY) oss << " = ."; 
	else oss <<  " = " << _val->get();
	return oss.str();
	
}

//
// set():
//!   Assigns a value to a scalar variable
//
void Scalar::set(const short valueType,const std::string value){
	
	switch(valueType){
		case BOOLEAN: _val = new Boolean(value);
			break;
		case DATE: _val = new Date(value);
			break;
		case GENOTYPE: _val = new Genotype(value);
			break;
		case NUMBER: _val = new Number(value);
			break;
		case STRING: _val = new String(value);
			break;
	}
	// If the initial value type was ANY set it to the specific type:
	if(_valueType == ANY){ _valueType = valueType; }
	
}

//
// getValueType(): 
//!   Returns a string version of the value type
//
const std::string Scalar::getValueType() const{
	
	switch(_valueType){
		case ANY:       return "a";
		case BOOLEAN:   return "b";
		case GENOTYPE:  return "g";
		case NUMBER:    return "n";
		case STRING:    return "s";
	}
	return "a";
}


//
// Free functions:
//
std::ostream& operator<< (std::ostream& out, const Scalar& var){
	
	return out << var.get();
	
}

//
// Friend functions:
//

bool operator==(const Scalar& a, const Scalar& b){
	
	if(a._valueType != b._valueType) return false;
	if(a._valueType == ANY) return true;
	return *(a._val) == *(b._val); 
	
}

bool operator<(const Scalar& a,const Scalar& b){
	
	//
	// If the two types of variable are different 
	// sort based on the string representation of the variable type
	if(a._valueType != b._valueType) return a._valueType < b._valueType;
	if(a._valueType == ANY) return false;
	return *(a._val) < *(b._val);
	
}
