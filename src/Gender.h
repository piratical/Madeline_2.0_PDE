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
// 2005.03.07.ET.RK
//
// Last updated 2011.11.18.ET
//

//
// Gender.h
//
#ifndef Gender_INCLUDED
#define Gender_INCLUDED
#define BOOLEAN_MALE false
#define BOOLEAN_FEMALE true


#include "Boolean.h"

class GenderMapLoader;
//! Gender is data type derived from Boolean
class Gender : public Boolean
{
public:
	
	enum GENDER{ MALE, FEMALE, MISSING };
private:
	
	friend class GenderMapLoader;
	static std::map<std::string,bool> _lookupTable;
public:
	
	//
	// Constructors:
	//
	// Note: Boolean value true indicates Female
	// Boolean value false indicates Male
	// 
	Gender() : Boolean() { }
	Gender(const char *value){ set(value); }
	Gender(const std::string &value){ set( value.c_str() ); }
	//
	// Static Methods:
	//
	static void addLookupTableEntry(std::string label, bool value);
	
	//
	//  Methods required by Variable virtual base class:
	//
	void set( const char *value );
	void set( const std::string &value);
	const std::string get( void ) const { if(isMissing()) return "."; if(_value) return "F"; else return "M";}
	void set( const bool value ) { Boolean::set(value); }
	
	//
	// Additional Setters/Getters not present in virtual base class:
	//
	void set(GENDER gender);
	const GENDER getEnum( void ) const { if(_isMissing) return Gender::MISSING; if(_value) return Gender::FEMALE; else return MALE; }

	static bool isA(std::string inString);
};

//
// Gender Friend class that initializes the lookup table:
//
class GenderMapLoader
{
public:
	static GenderMapLoader genderMapLoader;
	GenderMapLoader(){
		Gender::_lookupTable["f"]  = BOOLEAN_FEMALE;
		Gender::_lookupTable["m"]  = BOOLEAN_MALE;
		Gender::_lookupTable["F"]  = BOOLEAN_FEMALE;
		Gender::_lookupTable["M"]  = BOOLEAN_MALE;
		Gender::_lookupTable["male"  ] = BOOLEAN_MALE;
		Gender::_lookupTable["female"] = BOOLEAN_FEMALE;
		Gender::_lookupTable["Male"  ] = BOOLEAN_MALE;
		Gender::_lookupTable["Female"] = BOOLEAN_FEMALE;
		Gender::_lookupTable["MALE"  ] = BOOLEAN_MALE;
		Gender::_lookupTable["FEMALE"] = BOOLEAN_FEMALE;
		Gender::_lookupTable["♀"]  = BOOLEAN_FEMALE; 
		Gender::_lookupTable["♂"]  = BOOLEAN_MALE;
		Gender::_lookupTable["女"] = BOOLEAN_FEMALE;
		Gender::_lookupTable["男"] = BOOLEAN_MALE;
		Gender::_lookupTable["雌"] = BOOLEAN_FEMALE;
		Gender::_lookupTable["雄"] = BOOLEAN_MALE;
	}
};

#endif
