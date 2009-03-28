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
// 2009.03.28.ET
//

//
// Divorced.h
//
#ifndef Divorced_INCLUDED
#define Divorced_INCLUDED

#include "Boolean.h"

class DivorcedMapLoader;
//! Divorced is a data type derived from Boolean.
//! Divorced keeps track of whether an individual is divorced.
//! Be sure to marke *both* divorced partners ...
//
class Divorced : public Boolean
{
public:
	
	enum DIVORCED{ NOT_DIVORCED, IS_DIVORCED, MISSING }; /*!< Enumeration of sampled values */
private:
	
	friend class DivorcedMapLoader;
	static std::map<std::string,bool> _lookupTable;
public:
	
	//
	// Constructors:
	//
	// Note: Boolean value true indicates a Carrier
	// Boolean value false indicates not a Carrier
	// 
	Divorced() : Boolean() { }
	Divorced(const char *value){ set(value); }
	Divorced(const std::string &value){ set( value.c_str() ); }
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
	void set(DIVORCED divorced);
	const DIVORCED getEnum( void ) const { if(_isMissing) return Divorced::MISSING; if(_value) return Divorced::IS_DIVORCED; else return Divorced::NOT_DIVORCED; }
	
};

//
// Sampled Friend class that initializes the lookup table:
//
class DivorcedMapLoader
{
public:
	static DivorcedMapLoader divorcedMapLoader;
	DivorcedMapLoader(){
		Divorced::_lookupTable["N"] = false;
		Divorced::_lookupTable["Y"] = true;
		Divorced::_lookupTable["y"] = true;
		Divorced::_lookupTable["n"] = false;
		Divorced::_lookupTable["离婚"] = true;
		Divorced::_lookupTable["離婚"] = true;
	}
};

#endif
