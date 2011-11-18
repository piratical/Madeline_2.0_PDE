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
// 2007.04.26.ET
//
// Last updated 2011.11.18.ET
// 

//
// Carrier.h
//
#ifndef Carrier_INCLUDED
#define Carrier_INCLUDED

#include "Boolean.h"

class CarrierMapLoader;
//! Carrier is a data type derived from Boolean.
/*! Carrier keeps track of whether an individual is a carrier of a genetic trait.
*/
class Carrier : public Boolean
{
public:
	
	enum CARRIER{ NOT_CARRIER, IS_CARRIER, MISSING }; /*!< Enumeration of sampled values */
private:
	
	friend class CarrierMapLoader;
	static std::map<std::string,bool> _lookupTable;
public:
	
	//
	// Constructors:
	//
	// Note: Boolean value true indicates a Carrier
	// Boolean value false indicates not a Carrier
	// 
	Carrier() : Boolean() { }
	Carrier(const char *value){ set(value); }
	Carrier(const std::string &value){ set( value.c_str() ); }
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
	void set(CARRIER carrier);
	const CARRIER getEnum( void ) const { if(_isMissing) return Carrier::MISSING; if(_value) return Carrier::IS_CARRIER; else return Carrier::NOT_CARRIER; }
	
};

//
// Sampled Friend class that initializes the lookup table:
//
class CarrierMapLoader
{
public:
	static CarrierMapLoader carrierMapLoader;
	CarrierMapLoader(){
		Carrier::_lookupTable["N"] = false;
		Carrier::_lookupTable["Y"] = true;
		Carrier::_lookupTable["y"] = true;
		Carrier::_lookupTable["n"] = false;
		Carrier::_lookupTable["yes"] = true;
		Carrier::_lookupTable["no" ] = false;
		Carrier::_lookupTable["Yes"] = true;
		Carrier::_lookupTable["No" ] = false;
		Carrier::_lookupTable["YES"] = true;
		Carrier::_lookupTable["NO" ] = false;
		Carrier::_lookupTable["c"] = true;
		Carrier::_lookupTable["C"] = true;
		Carrier::_lookupTable["carrier"] = true;
		Carrier::_lookupTable["Carrier"] = true;
		Carrier::_lookupTable["CARRIER"] = true;
	}
};

#endif
