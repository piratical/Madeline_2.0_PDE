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
// 2009.05.19.ET
//

//
// Sterility.cpp
//
#include "Sterility.h"

std::map<std::string,std::string> Sterility::_lookupTable;
std::map<std::string,Sterility::TYPE> Sterility::_lookupTypeTable;

SterilityMapLoader SterilityMapLoader::sterilityMapLoader;

//
// Sterility::set() base method:
//
void Sterility::set(const char *value){
	
	// Set the string:
	String::set(value);
	
	// trim():
	Data::trim(_value);
	
	//
	// If the value is not either a global
	// or local missing value indicator for the
	// String class, then check whether it maps
	// to one of the Sterility states:
	//
	
	if( !_isMissing ){
		
		//
		// Any non-missing value indicates sterility:
		//
		_booleanValue = IS_STERILE;
		
		std::map<std::string,Sterility::TYPE>::const_iterator iter = _lookupTypeTable.find(_value);
		if(iter != _lookupTypeTable.end()){
			_type = iter->second;
		}
		
	}else{
		
		_type=UNKNOWN_STERILITY_TYPE;
		
	}
}

//
// Sterility::getBoolean
//
const bool Sterility::getBoolean( void ) const{
	
	return _booleanValue.getBoolean();
	
}

//
// Sterility::getBooleanAsChar
//
char Sterility::getBooleanAsChar( void ) const{
	
	return _booleanValue.getAsChar();
	
}

//
// getType
//
const Sterility::TYPE Sterility::getType(void) const{
	
	return _type;
	
}

//
// set( std::string )
//
void Sterility::set(const std::string value){
	
	set( value.c_str() );
	
}

//
// Free function: ostream operator <<
//
std::ostream& operator<<(std::ostream& s, const Sterility& n){
	
	return s << n.get();
	
}

//
// addSterilityBooleanMapping
//
void Sterility::addSterilityBooleanMapping(std::string affectedValue,std::string booleanMapping){ 
	
	Sterility::_lookupTable[ affectedValue ] = booleanMapping;
	
}

//
// isa:
//
//bool Sterility::isa(std::string inString){
//	Sterility t;
//	t.set(inString);
//	return !t.isMissing();
//}

