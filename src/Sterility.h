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
// Sterility.h
//
#ifndef Sterility_INCLUDED
#define Sterility_INCLUDED

#include "String.h"
#include "Boolean.h"
#include <map>

#define IS_STERILE  "T"
#define NOT_STERILE "F"

class Sterility : public String {
	
public:
	// What type of Sterility?
	enum TYPE{ MALE_STERILITY,FEMALE_STERILITY,UNKNOWN_STERILITY_TYPE };
	
private:
	
	friend class SterilityMapLoader;
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
	Sterility() { _isMissing=true; }
	Sterility( const std::string value ){ set(value); }
	Sterility( const char* value ){ set(value); }
	
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
	
	static void addSterilityBooleanMapping(std::string relationshipValue,std::string booleanMapping);
	//static bool isa(std::string inString);
	
};

//
// Sterility Friend class that initializes the lookup table:
//
class SterilityMapLoader
{
public:
	static SterilityMapLoader sterilityMapLoader;
	
	SterilityMapLoader(){
		
		Sterility::_lookupTable["vasectomy"     ] = IS_STERILE;
		Sterility::_lookupTable["tubal"         ] = IS_STERILE;
		Sterility::_lookupTable["tubal ligation"] = IS_STERILE;
		
		Sterility::_lookupTypeTable["vasectomy"      ] = Sterility::MALE_STERILITY;
		Sterility::_lookupTypeTable["tubal"          ] = Sterility::FEMALE_STERILITY;
		Sterility::_lookupTypeTable["tubal ligation" ] = Sterility::FEMALE_STERILITY; 
		
	}
};

std::ostream& operator<<(std::ostream& s,const Sterility& n);

#endif
