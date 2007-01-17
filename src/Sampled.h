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

//
// Sampled.h
//
#ifndef Sampled_INCLUDED
#define Sampled_INCLUDED

#include "Boolean.h"

class SampledMapLoader;
//! Sampled is a data type derived from Boolean.
/*! Sampled keeps track of the Sampled status of an individual.
*/
class Sampled : public Boolean
{
public:
	
	enum SAMPLED{ NOT_SAMPLED, IS_SAMPLED, MISSING }; /*!< Enumeration of sampled values */
private:
	
	friend class SampledMapLoader;
	static std::map<std::string,bool> _lookupTable;
public:
	
	//
	// Constructors:
	//
	// Note: Boolean value true indicates a Sampled
	// Boolean value false indicates not a Sampled
	// 
	Sampled() : Boolean() { }
	Sampled(const char *value){ set(value); }
	Sampled(const std::string &value){ set( value.c_str() ); }
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
	void set(SAMPLED sampled);
	const SAMPLED getEnum( void ) const { if(_isMissing) return Sampled::MISSING; if(_value) return Sampled::IS_SAMPLED; else return Sampled::NOT_SAMPLED; }
	
};

//
// Sampled Friend class that initializes the lookup table:
//
class SampledMapLoader
{
public:
	static SampledMapLoader sampledMapLoader;
	SampledMapLoader(){
		Sampled::_lookupTable["N"] = false;
		Sampled::_lookupTable["Y"] = true;
		Sampled::_lookupTable["y"] = true;
		Sampled::_lookupTable["n"] = false;
	}
};

#endif
