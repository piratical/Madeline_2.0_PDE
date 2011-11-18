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
// Boolean.h
//
#ifndef Boolean_INCLUDED
#define Boolean_INCLUDED

#include <string>
#include <iostream>
#include <map>
#include "Data.h"

class BooleanMapLoader;
//! Boolean is a data type that is derived from Data.
/*! Boolean variables can take the values true, false or missing
*/
class Boolean : public Data
{
private:
	
	friend class BooleanMapLoader;
	static std::map<std::string, bool> _lookupTable;
	
protected:
	
	bool _value;
	bool _isMissing;
	
public:
	
	//
	// Constructors:
	//
	Boolean(){ _isMissing=true; }
	Boolean(const char *value){ set(value); }
	Boolean(const std::string &value){ set( value.c_str() ); }
	
	//
	//  Methods required by Variable virtual base class:
	//
	virtual bool isMissing( void ) const { return _isMissing; }
	virtual void setMissing( void ){ _isMissing=true;}
	virtual void set( const char *value );
	virtual void set( const std::string &value);
	virtual const std::string get( void ) const { if(isMissing()) return "."; if(_value) return "T"; else return "F";}
	char getAsChar(void) const {if(isMissing()) return '.'; if(_value) return 'T'; else return 'F';}
	
	static void addLookupTableEntry(std::string label, bool value);
	//
	// Additional Setters/Getters not present in virtual base class:
	//
	virtual void set(const bool value) { _value=value; _isMissing = false; }
	virtual const bool getBoolean( void ) const { if(_isMissing) return false; return _value; }
	
	bool operator==(const Data& b) const;
	bool operator<(const Data& b) const;
	virtual Boolean* clone() const;
	const DATATYPE getDataType( void ) const { return BOOLEAN; }
	
};

std::ostream& operator<<(std::ostream& s,const Boolean& g);
//
// Boolean Friend class that initializes the lookup table:
//
class BooleanMapLoader
{
public:
	static BooleanMapLoader booleanMapLoader;
	BooleanMapLoader(){
		Boolean::_lookupTable["f"] = false;
		Boolean::_lookupTable["t"] = true;
		Boolean::_lookupTable["F"] = false;
		Boolean::_lookupTable["T"] = true;
		Boolean::_lookupTable["false"] = false;
		Boolean::_lookupTable["true" ] = true;
		Boolean::_lookupTable["False"] = false;
		Boolean::_lookupTable["True" ] = true;
		Boolean::_lookupTable["FALSE"] = false;
		Boolean::_lookupTable["TRUE" ] = true;
		Boolean::_lookupTable["假"] = false;
		Boolean::_lookupTable["真"] = true;
	}
};

#endif
