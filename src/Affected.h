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
// Last updated 2011.11.18.ET
// 

//
// Affected.h
//
#ifndef Affected_INCLUDED
#define Affected_INCLUDED

#include "String.h"
#include "Boolean.h"
#include <map>

#define BOOLEAN_AFFECTED "T"
#define BOOLEAN_UNAFFECTED "F"

class Affected : public String {
	
private:
	
	friend class AffectedMapLoader;
	static std::map<std::string,std::string> _lookupTable;
	
	// Here is the "Boolean" attribute that the regular "String" 
	// class lacks: this defaults to missing:
	Boolean _booleanValue;
	
public:
	
	//
	// Constructors:
	//
	Affected() { _isMissing=true; }
	Affected( const std::string value ){ set(value); }
	Affected( const char* value ){ set(value); }
	
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
	
	static void addAffectedBooleanMapping(std::string affectedValue,std::string booleanMapping);
	static bool isa(std::string inString);
	
};

//
// Affected Friend class that initializes the lookup table:
//
class AffectedMapLoader
{
public:
	static AffectedMapLoader affectedMapLoader;
	
	AffectedMapLoader(){
		Affected::_lookupTable["a"]  = BOOLEAN_AFFECTED;
		Affected::_lookupTable["u"]  = BOOLEAN_UNAFFECTED;
		Affected::_lookupTable["A"]  = BOOLEAN_AFFECTED;
		Affected::_lookupTable["U"]  = BOOLEAN_UNAFFECTED;
		Affected::_lookupTable["affected"  ]  = BOOLEAN_AFFECTED;
		Affected::_lookupTable["unaffected"]  = BOOLEAN_UNAFFECTED;
		Affected::_lookupTable["Affected"  ]  = BOOLEAN_AFFECTED;
		Affected::_lookupTable["Unaffected"]  = BOOLEAN_UNAFFECTED;
		Affected::_lookupTable["AFFECTED"  ]  = BOOLEAN_AFFECTED;
		Affected::_lookupTable["UNAFFECTED"]  = BOOLEAN_UNAFFECTED;
	}
};

std::ostream& operator<<(std::ostream& s,const Affected& n);

#endif
