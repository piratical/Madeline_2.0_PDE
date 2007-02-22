/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager and Ritu Khanna
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
// 2005.03.07.ET
//

//
// Boolean.cpp
//

#include "Boolean.h"

///////////////////////////////////
//
// STATIC AND CONST STATIC MEMBERS:
//
///////////////////////////////////

std::map<std::string,bool> Boolean::_lookupTable;
BooleanMapLoader BooleanMapLoader::booleanMapLoader;

///////////////////////////////////
//
// STATIC METHODS:
//
///////////////////////////////////
void Boolean::addLookupTableEntry(std::string label, bool value ){
	
	_lookupTable.insert(std::pair<std::string,bool>(label,value));
	
}

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// set():
//
void Boolean::set(const char *value){
	
	_isMissing = true;
	std::map<std::string,bool>::const_iterator iter = _lookupTable.find(value);
	if(iter != _lookupTable.end()){
		_value = iter->second;
		_isMissing = false;
	}
	
}

//
// set():
//
void Boolean::set(const std::string &value){
	
	set( value.c_str() );
	
}

//
// operator<<
//
std::ostream& operator<<(std::ostream& s, const Boolean& b){
	
	return s << b.get();
	
}

//
// operator==
//
bool Boolean::operator==(const Data& b) const{
	
	Boolean* pb = dynamic_cast<Boolean*>(const_cast<Data* const>(&b));
	if(pb){
		if(_isMissing && pb->_isMissing) return true;
		if(_isMissing || pb->_isMissing) return false;
		return _value==pb->_value;
	}
	return false;
}

///
/// operator<
///
/// NOTA BENE: This is used for SORTING ONLY
///
bool Boolean::operator<(const Data& b) const{
	
	//
	// If of differing data types, sort by the data type itself:
	//
	if(getDataType() != b.getDataType()) return getDataType() < b.getDataType();
	
	Boolean* pb = static_cast<Boolean*>(const_cast<Data* const>(&b));
	
	//
	// Sort MISSING values as LARGER than everything else:
	//
	if( _isMissing ) return false;
	if( pb->_isMissing ) return true;
	
	return _value < pb->_value;
	
}

//
// clone():
//
Boolean* Boolean::clone() const{
	
	return new Boolean(*this);
	
} 

