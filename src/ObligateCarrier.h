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
// 2015.12.03.ET
//
// Last updated 2015.12.03.ET
// 

//
// ObligateCarrier.h
//
#ifndef ObligateCarrier_INCLUDED
#define ObligateCarrier_INCLUDED

#include "Boolean.h"

class ObligateCarrierMapLoader;
//! ObligateCarrier is a data type derived from Boolean.
/*! ObligateCarrier keeps track of whether an individual is an obligate carrier of a genetic trait.
*/
class ObligateCarrier : public Boolean
{
public:
	
	enum OBLIGATE_CARRIER{ NOT_OBLIGATE_CARRIER, IS_OBLIGATE_CARRIER, MISSING }; /*!< Enumeration of sampled values */
private:
	
	friend class ObligateCarrierMapLoader;
	static std::map<std::string,bool> _lookupTable;
public:
	
	//
	// Constructors:
	//
	// Note: Boolean value true indicates a ObligateCarrier
	// Boolean value false indicates not a ObligateCarrier
	// 
	ObligateCarrier() : Boolean() { }
	ObligateCarrier(const char *value){ set(value); }
	ObligateCarrier(const std::string &value){ set( value.c_str() ); }
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
	void set(OBLIGATE_CARRIER carrier);
	const OBLIGATE_CARRIER getEnum( void ) const { if(_isMissing) return ObligateCarrier::MISSING; if(_value) return ObligateCarrier::IS_OBLIGATE_CARRIER; else return ObligateCarrier::NOT_OBLIGATE_CARRIER; }
	
};

//
// Sampled Friend class that initializes the lookup table:
//
class ObligateCarrierMapLoader
{
public:
	static ObligateCarrierMapLoader obligateCarrierMapLoader;
	ObligateCarrierMapLoader(){
		ObligateCarrier::_lookupTable["N"] = false;
		ObligateCarrier::_lookupTable["Y"] = true;
		ObligateCarrier::_lookupTable["y"] = true;
		ObligateCarrier::_lookupTable["n"] = false;
		ObligateCarrier::_lookupTable["yes"] = true;
		ObligateCarrier::_lookupTable["no" ] = false;
		ObligateCarrier::_lookupTable["Yes"] = true;
		ObligateCarrier::_lookupTable["No" ] = false;
		ObligateCarrier::_lookupTable["YES"] = true;
		ObligateCarrier::_lookupTable["NO" ] = false;
		ObligateCarrier::_lookupTable["c"] = true;
		ObligateCarrier::_lookupTable["C"] = true;
		ObligateCarrier::_lookupTable["o"] = true;
		ObligateCarrier::_lookupTable["O"] = true;
		ObligateCarrier::_lookupTable["carrier"] = true;
		ObligateCarrier::_lookupTable["Carrier"] = true;
		ObligateCarrier::_lookupTable["CARRIER"] = true;
		ObligateCarrier::_lookupTable["obligatecarrier"] = true;
		ObligateCarrier::_lookupTable["ObligateCarrier"] = true;
		ObligateCarrier::_lookupTable["OBLIGATECARRIER"] = true;
	}
};

#endif
