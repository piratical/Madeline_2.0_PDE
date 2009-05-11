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
// RelationshipEnded.cpp
//
#include "RelationshipEnded.h"

std::map<std::string,std::string> RelationshipEnded::_lookupTable;
std::map<std::string,RelationshipEnded::TYPE> RelationshipEnded::_lookupTypeTable;

RelationshipEndedMapLoader RelationshipEndedMapLoader::relationshipEndedMapLoader;

//
// RelationshipEnded::set() base method:
//
void RelationshipEnded::set(const char *value){
	
	// Set the string:
	String::set(value);
	
	// trim():
	Data::trim(_value);
	
	//
	// If the value is not either a global
	// or local missing value indicator for the
	// String class, then check whether it maps
	// to one of the RELATIONSHIP_ENDED states:
	//
	
	if( !_isMissing ){
		
		//
		// Any non-missing value indicates the relationship ended:
		//
		_booleanValue = BOOLEAN_RELATIONSHIP_ENDED;
		
		std::map<std::string,RelationshipEnded::TYPE>::const_iterator iter = _lookupTypeTable.find(_value);
		if(iter != _lookupTypeTable.end()){
			_type = iter->second;
		}
		
	}else{
		
		_type=RELATIONSHIP_ENDED_MISSING_TYPE;
		
	}
}

//
// RelationshipEnded::getBoolean
//
const bool RelationshipEnded::getBoolean( void ) const{
	
	return _booleanValue.getBoolean();
	
}

//
// RelationshipEnded::getBooleanAsChar
//
char RelationshipEnded::getBooleanAsChar( void ) const{
	
	return _booleanValue.getAsChar();
	
}

//
// getType
//
const RelationshipEnded::TYPE RelationshipEnded::getType(void) const{
	
	return _type;
	
}

//
// set( std::string )
//
void RelationshipEnded::set(const std::string value){
	
	set( value.c_str() );
	
}

//
// Free function: ostream operator <<
//
std::ostream& operator<<(std::ostream& s, const RelationshipEnded& n){
	
	return s << n.get();
	
}

//
// addRelationshipEndedBooleanMapping
//
void RelationshipEnded::addRelationshipEndedBooleanMapping(std::string affectedValue,std::string booleanMapping){ 
	
	RelationshipEnded::_lookupTable[ affectedValue ] = booleanMapping;
	
}

//
// isa:
//
//bool RelationshipEnded::isa(std::string inString){
//	RelationshipEnded t;
//	t.set(inString);
//	return !t.isMissing();
//}

