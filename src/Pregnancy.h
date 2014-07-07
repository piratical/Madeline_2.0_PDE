/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna and Adrian Marrs
// Copyright (c) 2014 by the
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
// 2014.07.01.ET
//

//
// Pregnancy.h
//
#ifndef Pregnancy_INCLUDED
#define Pregnancy_INCLUDED

#include "Boolean.h"

class PregnancyMapLoader;
//! Pregnancy is a data type derived from Boolean.
/*! Pregnancy keeps track of pregnancies where the individual is not yet born 
*/
class Pregnancy : public Boolean
{
public:
	
	enum PREGNANCY{ NOT_PREGNANCY, IS_PREGNANCY, MISSING }; /*!< Enumeration of pregnancy state values */
private:
	
	friend class PregnancyMapLoader;
	static std::map<std::string,bool> _lookupTable;
public:
	
	//
	// Constructors:
	//
	// Note: Boolean value true indicates a Consultand
	// Boolean value false indicates not a Consultand
	// 
	Pregnancy() : Boolean() { }
	Pregnancy(const char *value){ set(value); }
	Pregnancy(const std::string &value){ set( value.c_str() ); }
	//
	// Static Methods:
	//
	static void addLookupTableEntry(std::string label, bool value);
	
	//
	//  Methods required by Variable virtual base class:
	//
	void set( const char *value );
	void set( const std::string &value);
	const std::string get( void ) const { if(isMissing()) return "."; if(_value) return "Y"; else return "N";}
	void set( const bool value ) { Boolean::set(value); }
	
	//
	// Additional Setters/Getters not present in virtual base class:
	//
	void set(PREGNANCY pregnancy);
	const PREGNANCY getEnum( void ) const { if(_isMissing) return Pregnancy::MISSING; if(_value) return Pregnancy::IS_PREGNANCY; else return NOT_PREGNANCY; }
	
};

//
// Pregnancy Friend class that initializes the lookup table:
//
class PregnancyMapLoader
{
public:
	static PregnancyMapLoader pregnancyMapLoader;
	PregnancyMapLoader(){
		Pregnancy::_lookupTable["Y"] = true;
		Pregnancy::_lookupTable["y"] = true;
		Pregnancy::_lookupTable["N"] = false;
		Pregnancy::_lookupTable["n"] = false;
		Pregnancy::_lookupTable["yes"] = true;
		Pregnancy::_lookupTable["no" ] = false;
		Pregnancy::_lookupTable["Yes"] = true;
		Pregnancy::_lookupTable["No" ] = false;
		Pregnancy::_lookupTable["YES"] = true;
		Pregnancy::_lookupTable["NO" ] = false;
		Pregnancy::_lookupTable["P"] = true;
		Pregnancy::_lookupTable["p"] = true;
		Pregnancy::_lookupTable["pregnancy"] = true;
		Pregnancy::_lookupTable["Pregnancy"] = true;
		Pregnancy::_lookupTable["PREGNANCY"] = true;
	}
};

#endif
