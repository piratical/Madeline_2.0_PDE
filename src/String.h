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
// 2005.03.31.ET.RK
//

//
// String.h
//
#ifndef STRING_INCLUDED
#define STRING_INCLUDED

#include "Data.h"
#include <string>
#include <iostream>
//! String is a data type derived from Data

class String : public Data{
private:
	
	static std::set<std::string> _stringMissingValue;
	
protected:
	
	//
	// These are protected so that the derived Affected class
	// can have direct access:
	// 
	std::string _value;
	bool _isMissing;
	
public:
	//
	// Constructors:
	//
	String() { _isMissing=true;_value="."; }
	String( const std::string value ){ set(value); }
	String( const char* value ){ set(value); }
	
	//
	// Static Methods:
	//
	static void addStringMissingValue(std::string value){ _stringMissingValue.insert(value); }
	
	bool isMissingValue(const std::string &value) const;
	
	//
	// Methods required by Variable virtual base class:
	//
	bool isMissing( void ) const { return _isMissing; }
	void setMissing( void ) { _isMissing=true; _value="."; }
	void set( const std::string &value);
	void set( const char* value);
	const std::string get( void ) const { if(_isMissing) return "."; return _value; }
	//
	// Operators:
	// 
	String operator+( const String& b) const;
	String& operator+=(const String& b);
	bool operator==(const Data& b) const;
	bool operator<(const Data& b) const;
	bool operator!=(const String& b) const;
	bool operator<=(const String& b) const;
	bool operator>=(const String& b) const;
	bool operator>(const String& b) const;

	// Added on 2005-07-17	
	const DATATYPE getDataType( void ) const { return STRING; }
	virtual String* clone() const;
	
	char operator[](unsigned int n);
	
	
};
	std::ostream& operator<<(std::ostream& s,const String& n);
#endif
