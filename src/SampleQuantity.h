/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager and Ritu Khanna
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
// 2015.12.07.ET
//

//
// SampleQuantity.h
//

#ifndef SampleQuantity_INCLUDED
#define SampleQuantity_INCLUDED

#include "String.h"
#include <map>

#define HAS_SAMPLE true
#define NO_SAMPLE false

class SampleQuantity : public String
{
private:
	
	bool   _booleanValue;
	double _doubleValue;
	
	void   _set(void);
	
	friend class SampleQuantityMapLoader;
	static std::map<std::string,bool> _lookupTable;
	
public:
	//
	// Constructors:
	//
	SampleQuantity(){ _value="."; _isMissing = true; _booleanValue=false; _doubleValue=0; }
	SampleQuantity( const char *sampleQuantityMarker) { set(sampleQuantityMarker); }
	SampleQuantity( const std::string& sampleQuantityMarker) { set(sampleQuantityMarker); }
		
	void set(const char *sampleQuantityMarker){ 
		_value=sampleQuantityMarker;
		_set();
	}
	
	void set(const std::string& sampleQuantityMarker){ 
		_value=sampleQuantityMarker;
		_set();
	}
	
	const std::string get(void)        const { return _value;        }
	const bool        getBoolean(void) const { return _booleanValue; }
	const double      getValue(void)   const { return _doubleValue;  }
	
	static bool isa(std::string inString);
	
};

//
// SampleQuantity Friend class that initializes the lookup table:$
//
class SampleQuantityMapLoader
{
public:
	static SampleQuantityMapLoader sampleQuantityMapLoader;
	
	SampleQuantityMapLoader(){
		SampleQuantity::_lookupTable["y"]      = HAS_SAMPLE;
		SampleQuantity::_lookupTable["n"]      = NO_SAMPLE;
		SampleQuantity::_lookupTable["Y"]      = HAS_SAMPLE;
		SampleQuantity::_lookupTable["N"]      = NO_SAMPLE;
		SampleQuantity::_lookupTable["yes"]    = HAS_SAMPLE;
		SampleQuantity::_lookupTable["no"]     = NO_SAMPLE;
		SampleQuantity::_lookupTable["YES"]    = HAS_SAMPLE;
		SampleQuantity::_lookupTable["NO"]     = NO_SAMPLE;
		SampleQuantity::_lookupTable["Yes"]    = HAS_SAMPLE;
		SampleQuantity::_lookupTable["No"]     = NO_SAMPLE;
		SampleQuantity::_lookupTable["s"]      = HAS_SAMPLE;
		SampleQuantity::_lookupTable["S"]      = HAS_SAMPLE;
		SampleQuantity::_lookupTable["sample"] = HAS_SAMPLE;
		SampleQuantity::_lookupTable["Sample"] = HAS_SAMPLE;
		SampleQuantity::_lookupTable["SAMPLE"] = HAS_SAMPLE;
	}
};

std::ostream& operator<<(std::ostream& s,const SampleQuantity& n);

#endif
