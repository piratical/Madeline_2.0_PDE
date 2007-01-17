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
// LivingDead.h
//
#ifndef LivingDead_INCLUDED
#define LivingDead_INCLUDED

#include "Boolean.h"

class LivingDeadMapLoader;
//! LivingDead is a data type derived from Boolean
/*! It keeps track of the deceased status of individuals
*/
class LivingDead : public Boolean
{
public:
	
	enum LIVINGDEAD{ ALIVE, DEAD,  MISSING };/*< Enumeration of living status values */
private:
	
	friend class LivingDeadMapLoader;
	static std::map<std::string,bool> _lookupTable;
public:
	
	//
	// Constructors:
	//
	// Note: Boolean value true indicates Dead living status
	// Boolean value false indicates Alive living status
	// 
	LivingDead() : Boolean() { }
	LivingDead(const char *value){ set(value); }
	LivingDead(const std::string &value){ set( value.c_str() ); }
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
	void set(LIVINGDEAD livingdead);
	const LIVINGDEAD getEnum( void ) const { if(_isMissing) return LivingDead::MISSING; if(_value) return LivingDead::DEAD; else return LivingDead::ALIVE; }
	
};

//
// LivingDead Friend class that initializes the lookup table:
//
class LivingDeadMapLoader
{
public:
	static LivingDeadMapLoader livingdeadMapLoader;
	LivingDeadMapLoader(){
		LivingDead::_lookupTable["N"] = false;
		LivingDead::_lookupTable["Y"] = true;
		LivingDead::_lookupTable["y"] = true;
		LivingDead::_lookupTable["n"] = false;
	}
};

#endif
