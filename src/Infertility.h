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
// Last updated 2011.11.18.ET
//

//
// Infertility.h
//
#ifndef Infertility_INCLUDED
#define Infertility_INCLUDED

#include "String.h"
#include "Boolean.h"
#include <map>

#define IS_INFERTILE  "T"
#define NOT_INFERTILE "F"

class Infertility : public String {
	
public:
	// What type of Infertility?
	enum TYPE{ MALE_INFERTILITY,FEMALE_INFERTILITY,UNKNOWN_INFERTILITY_TYPE };
	
private:
	
	friend class InfertilityMapLoader;
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
	Infertility() { _isMissing=true; }
	Infertility( const std::string value ){ set(value); }
	Infertility( const char* value ){ set(value); }
	
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
	
	static void addInfertilityBooleanMapping(std::string relationshipValue,std::string booleanMapping);
	//static bool isa(std::string inString);
	
};

//
// Infertility Friend class that initializes the lookup table:
//
class InfertilityMapLoader
{
public:
	static InfertilityMapLoader infertilityMapLoader;
	
	InfertilityMapLoader(){
		
		Infertility::_lookupTable["azoospermia"   ] = IS_INFERTILE;
		Infertility::_lookupTable["endometriosis" ] = IS_INFERTILE;
		Infertility::_lookupTable["Azoospermia"   ] = IS_INFERTILE;
		Infertility::_lookupTable["Endometriosis" ] = IS_INFERTILE;
		Infertility::_lookupTable["AZOOSPERMIA"   ] = IS_INFERTILE;
		Infertility::_lookupTable["ENDOMETRIOSIS" ] = IS_INFERTILE;
		
		Infertility::_lookupTypeTable["azoospermia"   ] = Infertility::MALE_INFERTILITY;
		Infertility::_lookupTypeTable["endometriosis" ] = Infertility::FEMALE_INFERTILITY;
		Infertility::_lookupTypeTable["Azoospermia"   ] = Infertility::MALE_INFERTILITY;
		Infertility::_lookupTypeTable["Endometriosis" ] = Infertility::FEMALE_INFERTILITY;
		Infertility::_lookupTypeTable["AZOOSPERMIA"   ] = Infertility::MALE_INFERTILITY;
		Infertility::_lookupTypeTable["ENDOMETRIOSIS" ] = Infertility::FEMALE_INFERTILITY;
		
	}
};

std::ostream& operator<<(std::ostream& s,const Infertility& n);

#endif
