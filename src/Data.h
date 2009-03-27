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
// 2005.03.10.RK.ET
//

#ifndef DATA_INCLUDED
#define DATA_INCLUDED

#include <string>
#include <set>
#include <iostream>
#include "DataTypes.h"

class DataMissingValueLoader;
class Data {
private:
	friend class DataMissingValueLoader; // initializes global missing values
	//static _locale;
	static bool _displayNumberSeparators;
	static bool _displayNativeDigitsInDates;
	static bool _displayNativeDigitsInNumbers;

	
	
protected:
	static std::set<std::string> _missingValue; // container that holds the global missing values
	
	// Used to suppress warnings when the "isA()" methods in derived classes call "set()" methods
	// to test whether strings conform to patterns expected for that class:
	//static bool _suppressWarnings;


	
	
public:


	Data() {}
	//
	// This must be declared virtual so that 
	// the destructors of derived classes are called only,
	// --This base class destructor must *never* be called
	// 
	virtual ~Data(){};
	// Static methods
	static void addGlobalMissingValue(std::string value);
	static bool isGlobalMissingValue(std::string value);
	
	//
	// NOTE BENE: this "trim" function operates on a reference
	// and modifies the original string. The trimmed string is
	// returned:
	//
	// Note also that the default drop string includes Unicode characters
	// for ZWSP,NBSP, EN SPACE, EM SPACE etc.:
	// 
	static std::string & trim(std::string& s,const std::string &drop = "	            ​⁠　﻿");
	// Virtual methods:
	
	//
	// We'll pretend that C++ allows virtual static methods:
	//
	//virtual static bool isA(const std::string&);
	//
	
	virtual bool isMissing( void ) const =0;
	virtual void setMissing( void ) =0;
	
	// Minimal Setters common to all derived classes:
	virtual void set(const std::string &)=0;
	virtual void set(const char *)=0;
	
	// Get a display (string) version 
	// of the data value:
	virtual const std::string get( void ) const =0;
	// Get the type of data value:
	virtual const DATATYPE getDataType( void ) const=0;
	// Virtual copy constructor
	virtual Data* clone() const = 0;
	// Virtual operators
	virtual bool operator==(const Data&) const =0;
	virtual bool operator<(const Data&) const =0;
	
};

//
// Friend class: initializes global missing values
//
class DataMissingValueLoader
{
	
public:
	static DataMissingValueLoader dataMissingValueLoader;
	DataMissingValueLoader(){
		Data::_missingValue.insert(".");
	}
	
};

#endif


