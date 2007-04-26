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

//
// Consultand.h
//
#ifndef Consultand_INCLUDED
#define Consultand_INCLUDED

#include "Boolean.h"

class ConsultandMapLoader;
//! Consultand is a data type derived from Boolean.
/*! Consultand keeps track of the Consultand status of an individual.
*/
class Consultand : public Boolean
{
public:
	
	enum CONSULTAND{ NOT_CONSULTAND, IS_CONSULTAND, MISSING }; /*!< Enumeration of consultand values */
private:
	
	friend class ConsultandMapLoader;
	static std::map<std::string,bool> _lookupTable;
public:
	
	//
	// Constructors:
	//
	// Note: Boolean value true indicates a Consultand
	// Boolean value false indicates not a Consultand
	// 
	Consultand() : Boolean() { }
	Consultand(const char *value){ set(value); }
	Consultand(const std::string &value){ set( value.c_str() ); }
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
	void set(CONSULTAND consultand);
	const CONSULTAND getEnum( void ) const { if(_isMissing) return Consultand::MISSING; if(_value) return Consultand::IS_CONSULTAND; else return NOT_CONSULTAND; }
	
};

//
// Consultand Friend class that initializes the lookup table:
//
class ConsultandMapLoader
{
public:
	static ConsultandMapLoader consultandMapLoader;
	ConsultandMapLoader(){
		Consultand::_lookupTable["N"] = false;
		Consultand::_lookupTable["Y"] = true;
		Consultand::_lookupTable["y"] = true;
		Consultand::_lookupTable["n"] = false;
		Consultand::_lookupTable["C"] = true;
		Consultand::_lookupTable["c"] = true;
	}
};

#endif
