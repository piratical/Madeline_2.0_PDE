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
// Infertility.cpp
//
#include "Infertility.h"

std::map<std::string,std::string> Infertility::_lookupTable;
std::map<std::string,Infertility::TYPE> Infertility::_lookupTypeTable;

InfertilityMapLoader InfertilityMapLoader::infertilityMapLoader;

//
// Infertility::set() base method:
//
void Infertility::set(const char *value){
	
	// Set the string:
	String::set(value);
	
	// trim():
	Data::trim(_value);
	
	//
	// If the value is not either a global
	// or local missing value indicator for the
	// String class, then check whether it maps
	// to one of the Infertility states:
	//
	
	if( !_isMissing ){
		
		//
		// Any non-missing value indicates infertility:
		//
		_booleanValue = IS_INFERTILE;
		
		std::map<std::string,Infertility::TYPE>::const_iterator iter = _lookupTypeTable.find(_value);
		if(iter != _lookupTypeTable.end()){
			_type = iter->second;
		}
		
	}else{
		
		_type=UNKNOWN_INFERTILITY_TYPE;
		
	}
}

//
// Infertility::getBoolean
//
const bool Infertility::getBoolean( void ) const{
	
	return _booleanValue.getBoolean();
	
}

//
// Infertility::getBooleanAsChar
//
char Infertility::getBooleanAsChar( void ) const{
	
	return _booleanValue.getAsChar();
	
}

//
// getType
//
const Infertility::TYPE Infertility::getType(void) const{
	
	return _type;
	
}

//
// set( std::string )
//
void Infertility::set(const std::string value){
	
	set( value.c_str() );
	
}

//
// Free function: ostream operator <<
//
std::ostream& operator<<(std::ostream& s, const Infertility& n){
	
	return s << n.get();
	
}

//
// addInfertilityBooleanMapping
//
void Infertility::addInfertilityBooleanMapping(std::string affectedValue,std::string booleanMapping){ 
	
	Infertility::_lookupTable[ affectedValue ] = booleanMapping;
	
}

//
// isa:
//
//bool Infertility::isa(std::string inString){
//	Infertility t;
//	t.set(inString);
//	return !t.isMissing();
//}

