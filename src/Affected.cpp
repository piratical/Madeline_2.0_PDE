/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna, and Adrian Marrs
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
// 2005.08.31.ET
//

//
// Affected.cpp
//
#include "Affected.h"

std::map<std::string,std::string> Affected::_lookupTable;
AffectedMapLoader AffectedMapLoader::affectedMapLoader;

//
// Affected::set() base method:
//
void Affected::set(const char *value){
	
	// Set the string:
	String::set(value);
	
	// trim():
	Data::trim(_value);
	
	//
	// If the value is not either a global
	// or local missing value indicator for the
	// String class, then check whether it maps
	// to a boolean "affected" or "unaffected" state:
	//
	
	if( !_isMissing ){
		
		std::map<std::string,std::string>::const_iterator iter = _lookupTable.find(_value);
		if(iter != _lookupTable.end()){
			_booleanValue = iter->second;
		}
		
	}
}

//
// Affected::getBoolean
//
const bool Affected::getBoolean( void ) const{
	
	return _booleanValue.getBoolean();
	
}

//
// Affected::getBooleanAsChar
//
char Affected::getBooleanAsChar( void ) const{
	
	return _booleanValue.getAsChar();
	
}

//
// set( std::string )
//
void Affected::set(const std::string &value){
	
	set( value.c_str() );
	
}

//
// Free function: ostream operator <<
//
std::ostream& operator<<(std::ostream& s, const Affected& n){
	
	return s << n.get();
	
}

//
// addAffectedBooleanMapping
//
void Affected::addAffectedBooleanMapping(std::string affectedValue,std::string booleanMapping){ 
	
	Affected::_lookupTable[ affectedValue ] = booleanMapping;
	
}

//
// isa:
//
//bool Affected::isa(std::string inString){
//	Affected t;
//	t.set(inString);
//	return !t.isMissing();
//}

