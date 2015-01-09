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
// 2005.02.15.ET
//

//
// ColumnClassifier.cpp
//

#include <string.h>

#include "ColumnClassifier.h"

#include "DigitConverter.h"

// For debugging:
#include <iostream>

void ColumnClassifier::scan( const char *p, int stt, int end){
	
	const char *s;
	std::string ss;
	DigitConverter converter("0");
	// 
	// Handle non-ASCII (that is, UTF-8):
	//
	if(!_isASCII(p,stt,end)){
		//
		// Conversion to ASCII normalization required:
		//
		converter.set(p,stt,end);
		ss=converter.get();
		s=ss.c_str();
		stt=0;
		end=strlen(s);
	}else{
		// No conversion necessary:
		s=p;
	}
	if     (_isEmptyOrDot(s,stt,end)){ _missingOrDotCounter++; }
	else if(_isDate      (s,stt,end)){ _dateCounter++;         }
	else if(_isGenotype  (s,stt,end)){ _genotypeCounter++;     }
	else if(_isNumeric   (s,stt,end)){ _numericCounter++;      }
	else if(_isGender    (s,stt,end)){ _genderCounter++;       }
	//else                             {                         }
	// Add to totalCounter as well:
	_totalCounter++;
	
}


void ColumnClassifier::scan( const char *s){
	
	scan(s,0,strlen(s));
	
}

void ColumnClassifier::scan(const std::string &s){
	
	scan(s.c_str(),0,s.size());
	
}

//
// _isEmptyOrDot
//
// "stt" is the start position
// "end" is one past the end
//
bool ColumnClassifier::_isEmptyOrDot( const char *p, int stt, int end ){
	
	// Assign s_stt and s_end:
	const char *s = p + stt;
	const char *e = p + end-1;
	
	// skip white space on left side:
	for(;s<e && *s ==' ';s++);
	
	// Return true if empty:
	if(s>e) return 1;
	
	// skip white space on right edge:
	for(;e>s && *e==' ';e--);
	
	//
	// If e==s and *s=='.' or *s==' ', then the field
	// contains nothing but a dot or is empty:
	//
	return(e==s && (*s=='.' || *s==' '));
	
}

//
// _isNumeric()
//
// "stt" is the start position
// "end" is one past the end
//
// A number must be of the form "[+-]*[0-9]+\.[0-9]*"
// A consequence of this is that Numbers with decimal point must be
// preceded with at least a zero, i.e., "+.4" is not a number.
//
// Note: This does not really handle every possible pathological case,
//       but it is certainly good enough for non-pathological data sets.
//
bool ColumnClassifier::_isNumeric( const char *p, int stt, int end ){
	
	// Assign s_stt and s_end:
	const char *s = p + stt;
	const char *e = p + end-1;
	
	// skip white space on left side:
	for(;s<e && *s ==' ';s++);
	
	// Return false empty:
	if(s>e) return 0;
	
	// skip white space on right edge:
	for(;e>s && *e==' ';e--);
	
	// Number may start with '+' or '-':
	if(*s=='+' || *s=='-') s++;
	if(s>e) return 0;
	if(!isdigit(*s)) return 0;
	
	// skip numbers and dots, and commas, counting decimal points:
	int decimals; // decimal point counter
	int digits;   // digits before decimal counter
	int separs;   // thousands separator counter
	for(decimals=digits=separs=0;s<=e && (isdigit(*s) || *s==_THOUSANDS_SEPARATOR || *s==_DECIMAL_CHARACTER);s++){
		if(*s==_DECIMAL_CHARACTER) decimals++;
		if(*s==_THOUSANDS_SEPARATOR) separs++;
		if(!decimals && isdigit(*s)) digits++;
	}
	if(s<e) return 0;
	// Case of too many decimal places:
	if(decimals>1) return 0;
	// Case of too many thousands separators:
	// (Note that this does not detect the pathological case of the thousands
	//  separators being in the wrong place)
	if(separs>(digits/3)) return 0; 
	return 1;
	
}

//
// _isGenotype()
//
// "stt" is the start position
// "end" is one past the end
//
bool ColumnClassifier::_isGenotype( const char *p, int stt, int end ){
	
	// Assign s_stt and s_end:
	const char *s = p + stt;
	const char *e = p + end-1;
	
	// skip white space on left side:
	for(;s<e && *s ==' ';s++);
	
	// Return false empty:
	if(s==e) return 0;
	
	// skip white space on right edge:
	for(;e>s && *e==' ';e--);
	
	// traverse first numeric label (which may include a decimal point or a comma):
	for(;s<=e && (isdigit(*s) || *s=='.' || *s==',');s++);
	if(s>e) return 0;
	// skip middle sections of white space and slashes
	// but check if more than one slash character:
	int sc;
	for(sc=0;s<=e && (*s==' ' || *s=='/' || *s=='|');s++) if(*s=='/' || *s=='|') sc++;
	if(sc!=1) return 0;
	if(s>e)   return 0;
	// second numeric label now expected (which may include a decimal point or a comma):
	for(;s<=e && (isdigit(*s) || *s=='.' || *s==',');s++);
	// should be one past e if OK:
	return (s==e+1);
	
}

//
// _isDate()
//
// -> Only allowed format is ISO "YYYY.MM.DD"
// -> But allow a "-" negative prefix for years before Common Era.
// -> Also, allow any of "/",".", or "-" as delimiter
//
// "stt" is the start position
// "end" is one past the end
//
bool ColumnClassifier::_isDate( const char *p, int stt, int end ){
	
	// Assign s_stt and s_end:
	const char *s = p + stt;
	const char *e = p + end-1;
	
	// skip white space on left side:
	for(;s<e && *s ==' ';s++);
	
	// Return false empty:
	if(s==e) return 0;
	
	// skip white space on right edge:
	for(;e>s && *e==' ';e--);
	
	// Allow "-" negative prefix for years before CE:
	if(*s=='-') s++;
	if(s>e) return 0;
	
	// YYYY:
	// Must have four digits for year:
	int yc;
	for(yc=0;s<=e && isdigit(*s);s++) yc++;
	if(yc!=4) return 0;
	// Must have delimiter:
	if(!(*s=='.' || *s=='-' || *s=='/')) return 0;
	s++;
	if(s>e) return 0;
	
	// MM:
	// Must have two digits for month:
	// One space plus one digit is not allowed:
	int mc;
	for(mc=0;s<=e && isdigit(*s);s++) mc++;
	if(mc!=2) return 0;
	
	// Must have the second delimiter:
	if(!(*s=='.' || *s=='-' || *s=='/')) return 0;
	s++;
	if(s>e) return 0;
	
	// DD:
	// Must have two digits for the day
	// One space plus one digit is not allowed:
	int dc;
	for(dc=0;s<=e && isdigit(*s);s++) dc++;
	if(dc!=2) return 0;
	
	// should be one past e if OK:
	return (s==e+1);
	
}


//
// _isGender()
//
// "stt" is the start position
// "end" is one past the end
//
//  Any combination of "m","f","M","F", or "♀" or "♂":
//  Now "男", "女", "雌", and "雄" are also supported:
//
bool ColumnClassifier::_isGender( const char *p, int stt, int end ){
	
	// Assign s_stt and s_end:
	const char *s = p + stt;
	const char *e = p + end-1;
	
	// skip white space on left side:
	for(;s<e && *s ==' ';s++);
	
	// Return false empty:
	if(s>e) return 0;
	
	// skip white space on right edge:
	for(;e>s && *e==' ';e--);
	
	// Check for m/f/M/F cases first:
	if(*s=='m' || *s=='M'){
		if(s==e) return 1;
		return (*(s+1)=='a' || *(s+1)=='A');
	}
	if(*s=='f' || *s=='F'){
		if(s==e) return 1;
		return (*(s+1)=='e' || *(s+1)=='E');
	}
	// Now check for "♀" or "♂":
	if(*s=='\xe2'){
		// These symbols require 3 bytes in UTF-8
		// Recall that the data must be encoded in UTF-8:
		if(s!=e-2) return 0;
		if(*(s+1)!='\x99') return 0;
		return (*(s+2)=='\x80' || *(s+2)=='\x82');
	}
	// Check for "男" or "女":
	if(*s=='\xe7' && s<=e-2 && *(s+1)=='\x94' && *(s+2)=='\xb7') return 1;
	if(*s=='\xe5' && s<=e-2 && *(s+1)=='\xa5' && *(s+2)=='\xb3') return 1;
	// Check for "雌" or "雄":
	if(*s=='\xe9' && s<=e-2 && *(s+1)=='\x9b' && *(s+2)=='\x8c') return 1;
	if(*s=='\xe9' && s<=e-2 && *(s+1)=='\x9b' && *(s+2)=='\x84') return 1;
		
	return 0;
	
}


bool ColumnClassifier::_isASCII(const char *p,int stt, int end){
	
	// Assign s_stt and s_end:
	const char *s = p + stt;
	const char *e = p + end-1;
	
	// Return if a byte is outside the ASCII range:
	for(;s<e;s++) if( *s & 0x80 ) return 0;
	// Get here if it is ASCII:
	return 1;
	
}


DATATYPE ColumnClassifier::classify( void ){
	
	// Return UNCLASSIFIED if the data have not yet been scanned:
	if( !_totalCounter )                       return UNCLASSIFIED;
	// Return MISSING if all examined data points were missing:
	if( _missingOrDotCounter == _totalCounter) return MISSING;
	// Check for "pure" columns which still may have some missing,
	// but clearly are not of a mixed types:

	if( _dateCounter     && ! ( _numericCounter || _genotypeCounter || _genderCounter )) return DATE;
	//
	// Single numerals could represent homozygote genotypes, so don't check the _numericCounter
	// in a potential genotype column:
	//
	//if( _genotypeCounter && ! ( _numericCounter || _dateCounter     || _genderCounter )) return GENOTYPE;
	//
	if( _genotypeCounter && ! ( _dateCounter    || _genderCounter                     )) return GENOTYPE;
	if( _numericCounter  && ! ( _dateCounter    || _genotypeCounter || _genderCounter )) return NUMBER;
	if( _genderCounter   && ! ( _numericCounter || _genotypeCounter || _dateCounter   )) return GENDER;
	// Get here if it is of mixed types, or none of the above, which can be
	// classified as generic CHARACTER string:
	return STRING;
	
}
