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
// 2009.05.11.ET
//

//
// RelationshipEnded.h
//
#ifndef RelationshipEnded_INCLUDED
#define RelationshipEnded_INCLUDED

#include "String.h"
#include "Boolean.h"
#include <map>

#define BOOLEAN_RELATIONSHIP_ENDED     "T"
#define BOOLEAN_RELATIONSHIP_NOT_ENDED "F"

class RelationshipEnded : public String {
	
public:
	// What type of relationship end?
	enum TYPE{ DIVORCE, SEPARATION, RELATIONSHIP_ENDED_OTHER_TYPE, RELATIONSHIP_ENDED_MISSING_TYPE };
	
private:
	
	friend class RelationshipEndedMapLoader;
	//
	static std::map<std::string,std::string> _lookupTable;
	
	// See TYPE enum below:
	static std::map<std::string,TYPE> _lookupTypeTable;
	
	// Here is the "Boolean" attribute that the regular "String" 
	// class lacks: this defaults to missing:
	Boolean _booleanValue;
	TYPE _type;
	
public:
	
	//
	// Constructors:
	//
	RelationshipEnded() { _isMissing=true; }
	RelationshipEnded( const std::string value ){ set(value); }
	RelationshipEnded( const char* value ){ set(value); }
	
	//
	// Set methods:
	//
	void set(const char *value);
	void set(const std::string value);
	
	//
	// Get methods not present in base "String" class:
	//
	const bool getBoolean( void ) const;
	char getBooleanAsChar(void) const;
	const TYPE getType(void) const;
	
	static void addRelationshipEndedBooleanMapping(std::string relationshipValue,std::string booleanMapping);
	static bool isa(std::string inString);
	
};

//
// RelationshipEnded Friend class that initializes the lookup table:
//
class RelationshipEndedMapLoader
{
public:
	static RelationshipEndedMapLoader relationshipEndedMapLoader;
	
	RelationshipEndedMapLoader(){
		RelationshipEnded::_lookupTable["d"        ]  = BOOLEAN_RELATIONSHIP_ENDED; // Divorced
		RelationshipEnded::_lookupTable["s"        ]  = BOOLEAN_RELATIONSHIP_ENDED; // Separated
		RelationshipEnded::_lookupTable["e"        ]  = BOOLEAN_RELATIONSHIP_ENDED; // Ended; reason unspecified
		RelationshipEnded::_lookupTable["D"        ]  = BOOLEAN_RELATIONSHIP_ENDED; // Divorced
		RelationshipEnded::_lookupTable["S"        ]  = BOOLEAN_RELATIONSHIP_ENDED; // Separated
		RelationshipEnded::_lookupTable["E"        ]  = BOOLEAN_RELATIONSHIP_ENDED; // Ended; reason unspecified
		RelationshipEnded::_lookupTable["y"        ]  = BOOLEAN_RELATIONSHIP_ENDED; // Ended; reason unspecified
		RelationshipEnded::_lookupTable["Y"        ]  = BOOLEAN_RELATIONSHIP_ENDED; // Ended; reason unspecified
		RelationshipEnded::_lookupTable["yes"      ]  = BOOLEAN_RELATIONSHIP_ENDED; // Ended; reason unspecified
		RelationshipEnded::_lookupTable["divorced" ]  = BOOLEAN_RELATIONSHIP_ENDED;
		RelationshipEnded::_lookupTable["separated"]  = BOOLEAN_RELATIONSHIP_ENDED;
		RelationshipEnded::_lookupTable["ended"    ]  = BOOLEAN_RELATIONSHIP_ENDED;     // Ended; reason unspecified
		
		RelationshipEnded::_lookupTypeTable["d"        ]  = RelationshipEnded::DIVORCE; // Divorced
		RelationshipEnded::_lookupTypeTable["s"        ]  = RelationshipEnded::SEPARATION; // Separated
		RelationshipEnded::_lookupTypeTable["e"        ]  = RelationshipEnded::RELATIONSHIP_ENDED_OTHER_TYPE; // Ended; reason unspecified
		RelationshipEnded::_lookupTypeTable["D"        ]  = RelationshipEnded::DIVORCE; // Divorced
		RelationshipEnded::_lookupTypeTable["S"        ]  = RelationshipEnded::SEPARATION; // Separated
		RelationshipEnded::_lookupTypeTable["E"        ]  = RelationshipEnded::RELATIONSHIP_ENDED_OTHER_TYPE; // Ended; reason unspecified
		RelationshipEnded::_lookupTypeTable["y"        ]  = RelationshipEnded::RELATIONSHIP_ENDED_OTHER_TYPE; // Ended; reason unspecified
		RelationshipEnded::_lookupTypeTable["Y"        ]  = RelationshipEnded::RELATIONSHIP_ENDED_OTHER_TYPE; // Ended; reason unspecified
		RelationshipEnded::_lookupTypeTable["yes"      ]  = RelationshipEnded::RELATIONSHIP_ENDED_OTHER_TYPE; // Ended; reason unspecified
		RelationshipEnded::_lookupTypeTable["divorced" ]  = RelationshipEnded::DIVORCE;
		RelationshipEnded::_lookupTypeTable["separated"]  = RelationshipEnded::SEPARATION;
		RelationshipEnded::_lookupTypeTable["ended"    ]  = RelationshipEnded::RELATIONSHIP_ENDED_OTHER_TYPE;     // Ended; reason unspecified
	}
};

std::ostream& operator<<(std::ostream& s,const RelationshipEnded& n);

#endif
