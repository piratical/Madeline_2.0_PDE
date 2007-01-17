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

//! The DigitConverter class converts non-ASCII UTF-8 Unicode digits to their ASCII equivalents

///
/// The DigitConverter class converts non-ASCII UTF-8 Unicode digits in
/// C strings or std::strings to their ASCII equivalents.  Non-digit characters
/// remain unchanged.  A DigitConverter object can be instantiated with a
/// C string or std::string. There is also a constructor tailored for use
/// with the MadelineTable class which takes a pointer to a char string and
/// start and end offsets into the string.  The get() method returns a std::string
/// in which all of the non-ASCII digits have been converted to ASCII 
/// equivalents.
/// 
#include <string>
#include <ostream>

#ifndef DigitConverter_INCLUDED
#define DigitConverter_INCLUDED

class DigitConverter{
	
private:
	
	std::string _input;
	
	bool _isASCII( void );
	void _normalize( void );
	
	static const char *_digits[];
	static const char *_ArabicIndicDigits[];
	// 22 rows of ten digits each
	// Defining all the non-ASCII decimal
	// digits in Unicode version 3:
	static const int TABLE_SIZE=220;
	
public:
	
	// Constructors:
	DigitConverter(const char *p,int stt, int end);
	DigitConverter(const char *p);
	DigitConverter(const std::string &input);
	
	
	// Set functions:
	void set(const char *p, int stt, int end);
	void set(const char *p);
	void set(const std::string &input);
	
	// Get function:
	const std::string get( void ) const;
	
};

std::ostream& operator<<(std::ostream& s, const DigitConverter& dc);

#endif

