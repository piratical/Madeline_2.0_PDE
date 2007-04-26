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
// Proband.h
//
#ifndef Proband_INCLUDED
#define Proband_INCLUDED

#include "Boolean.h"

class ProbandMapLoader;
//! Proband is a data type derived from Boolean.
/*! Proband keeps track of the Proband status of an individual.
*/
class Proband : public Boolean
{
public:
	
	enum PROBAND{ NOT_PROBAND, IS_PROBAND, MISSING }; /*!< Enumeration of proband values */
private:
	
	friend class ProbandMapLoader;
	static std::map<std::string,bool> _lookupTable;
public:
	
	//
	// Constructors:
	//
	// Note: Boolean value true indicates a Proband
	// Boolean value false indicates not a Proband
	// 
	Proband() : Boolean() { }
	Proband(const char *value){ set(value); }
	Proband(const std::string &value){ set( value.c_str() ); }
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
	void set(PROBAND proband);
	const PROBAND getEnum( void ) const { if(_isMissing) return Proband::MISSING; if(_value) return Proband::IS_PROBAND; else return NOT_PROBAND; }
	
};

//
// Proband Friend class that initializes the lookup table:
//
class ProbandMapLoader
{
public:
	static ProbandMapLoader probandMapLoader;
	ProbandMapLoader(){
		Proband::_lookupTable["N"] = false;
		Proband::_lookupTable["Y"] = true;
		Proband::_lookupTable["y"] = true;
		Proband::_lookupTable["n"] = false;
		Proband::_lookupTable["P"] = true;
		Proband::_lookupTable["p"] = true;
	}
};

#endif
