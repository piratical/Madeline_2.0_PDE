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
// utf8String.h
//
// (c) 2006 by Edward H. Trager
// released under the GNU General Public License
// 
// This file was originally written for inclusion
// in "Font Playground" .
//
// 2006.04.30.et.
// LAST UPDATE: 2007.01.08
// 

#ifndef UTF8STRING_INCLUDED
#define UTF8STRING_INCLUDED

#include "ScriptCodes.h"
#include <string>

typedef unsigned long  UTF32; // at least 32 bits
typedef unsigned short UTF16; // at least 16 bits
typedef unsigned char  UTF8;

#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF

//
// The following are needed for UTF-16 conversion:
// 
#define UNI_SUR_HIGH_START  (UTF32)0xD800
#define UNI_SUR_HIGH_END    (UTF32)0xDBFF
#define UNI_SUR_LOW_START   (UTF32)0xDC00
#define UNI_SUR_LOW_END     (UTF32)0xDFFF

class UTF8String : public std::string {
	
	
private:
	
	const char *_UTF32ValueToUTF8( UTF32 UTF32Value );
	
public:
	
	// Default constructor just calls base class std::String():
	UTF8String();
	// Copy Constructors:
	UTF8String(const std::string &s);
	UTF8String(const UTF8String &s);
	// How many Unicode values are stored in the string?:
	unsigned int unicodeValueCount() const;
	// Get the Unicode substring starting at the "stt" unicode value --
	// Note that stt=1 (*not* zero) returns the entire string:
	UTF8String unicodeSubString(unsigned int stt,unsigned int howManyCharacters=0) const;
	// Read-only bracket operator retrieves the nth unicode character --
	// Note that pos=1 (*not* zero) specifies the first character:
	UTF8String operator[](unsigned int pos) const;
	// Return the Unicode code value of the nth Unicode character:
	UTF32 unicodeValueAtPosition(unsigned int pos=0) const;
	
	// Returns boolean TRUE if the string begins with a character
	// from a right-to-left script:
	bool isRTL(void) const;
	// Returns a boolean TRUE if the string begins with a character
	// from an Indic or Indic-derived script.  Such scripts have
	// special complex text layout requirements:
	bool isIndic(void) const;
	
	// Returns a boolean TRUE if the string begins with a character
	// from the Arabic script.  This script has
	// special complex text layout requirements:
	bool isArabic(void) const;
	
	//
	// Returns a script code based on the Unicode range of the first
	// character in the string: Currently only handles the Arabic and
	// Indic cases relevant for complex text layout
	// 
	SCRIPTCODE getScriptCode(void);
	
	// Returns a UTF32 String:
	std::basic_string<UTF32> UTF32String() const;
	
	//
	// Append and Derived Overloaded Assignment operators:
	//
	UTF8String& append( const std::basic_string<UTF32> &UTF32String );
	UTF8String& append( const std::basic_string<UTF16> &UTF16String );
	
	UTF8String& operator+=( const std::basic_string<UTF32> &UTF32String );
	UTF8String& operator+=( const std::basic_string<UTF16> &UTF16String );
	
	UTF8String& operator=( const std::basic_string<UTF32> &UTF32String );
	UTF8String& operator=( const std::basic_string<UTF16> &UTF16String );
	
	// 
	// Specialized constructors:
	// 
	// Construct a UTF8String from a UTF32 or UTF16 string:
	// 
	// These also ultimately use the append() methods from above:
	// 
	UTF8String( const std::basic_string<UTF32> &UTF32String );
	UTF8String( const std::basic_string<UTF16> &UTF16String );
	
};

#endif

