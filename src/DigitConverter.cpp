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


#include "DigitConverter.h"

// For DEBUG:
#include <iostream>

// The non-ASCII digits.  Row 17 is a bit of
// a hack, as it contains the simplified versions of
// the "financial" Han characters when there is a
// simplified version, otherwise the places are filled
// with alternates or the so-called "Hang Zhou" characters
// (but there isn't a complete set of "Hang Zhou" here and
// that is why it is a hack ... ).  Everything from Unicode
// version 3.0 is here.  Limbu digits, from Unicode 4.0, are
// currently not here:
const char *DigitConverter::_digits[]={
	"٠", "١", "٢", "٣", "٤", "٥", "٦", "٧", "٨", "٩",
	"०","१","२","३","४","५","६","७","८","९",
	"๐","๑","๒","๓","๔","๕","๖","๗","๘","๙",
	"໐","໑","໒","໓","໔","໕","໖","໗","໘","໙",
	"༠","༡","༢","༣","༤","༥","༦","༧","༨","༩",
	"〇","一","二","三","四","五","六","七","八","九",
	"０","１","２","３","４","５","６","７","８","９",
	"০","১","২","৩","৪","৫","৬","৭","৮","৯",
	"੦","੧","੨","੩","੪","੫","੬","੭","੮","੯",
	"૦","૧","૨","૩","૪","૫","૬","૭","૮","૯",
	"୦","୧","୨","୩","୪","୫","୬","୭","୮","୯",
	"*","௧","௨","௩","௪","௫","௬","௭","௮","௯",
	"౦","౧","౨","౩","౪","౫","౬","౭","౮","౯",
	"೦","೧","೨","೩","೪","೫","೬","೭","೮","೯",
	"൦","൧","൨","൩","൪","൫","൬","൭","൮","൯",
	"零","壹","貳","叄","肆","伍","陸","柒","捌","玖",
	"*","么","贰","參","〤","〥","陆","〧","〨","〩",
	"۰","۱","۲","۳","۴","۵","۶","۷","۸","۹",
	"၀","၁","၂","၃","၄","၅","၆","၇","၈","၉",
	"*","፩","፪","፫","፬","፭","፮","፯","፰","፱",
	"០","១","២","៣","៤","៥","៦","៧","៨","៩",
	"᠐","᠑","᠒","᠓","᠔","᠕","᠖","᠗","᠘","᠙"
};

const char *DigitConverter::_ArabicIndicDigits[]={
	"0","1","2","3","4","5","6","7","8","9"
};

//
//! This constructor is tailored for use by the MadelineTable class
//
DigitConverter::DigitConverter(const char *p,int stt,int end){
	
	const char *s = p + stt;
	const char *e = p + end;
	
	for(;s<e;_input+=*s++);
	_normalize();
	
}

//
//! set method tailored for use by the MadelineTable class
//
void DigitConverter::set(const char *p,int stt,int end){
	
	_input.clear();
	const char *s = p + stt;
	const char *e = p + end;
	for(;s<e;_input+=*s++);
	_normalize();
	
}

//
// DigitConverter
//
//! Constructor taking const char *
//
DigitConverter::DigitConverter(const char *p){
	
	_input=p;
	_normalize();
}

//
//! set method taking const char *
//
void DigitConverter::set(const char *p){
	
	_input.clear();
	_input=p;
	_normalize();
	
}

//
// DigitConverter
//
//! Constructor taking const reference to a standard string
//
DigitConverter::DigitConverter(const std::string &input){
	
	_input=input;
	_normalize();
	
}

//
//! set method taking const reference to a standard string
//
void DigitConverter::set(const std::string &input){
	
	_input.clear();
	_input=input;
	_normalize();
	
}

//
// Tests the high bits in a UTF-8 string to see if there are
// characters outside the ASCII range or not.
//
bool DigitConverter::_isASCII( void ){
	
	// Test high bit for non-ASCII:
	for(unsigned i=0;i<_input.size();i++) if( _input[i] & 0x80 ) return false;
	// Get here if it is ASCII:
	return true;
	
}

//
// _normalize(): Private member function which
// converts non-ASCII digits to ASCII equivalents.
// The same is done to non-ASCII THOUSANDS and DECIMAL
// SEPARATORS which are converted to "," and "."
// respectively (using the U.S./British convention as
// the internal standard).
// 
void DigitConverter::_normalize( void ){
	
	if( !_isASCII() ){
		// Normalize:
		
		// DEBUG:
		//std::cerr<<"NORMALIZING NON - ASCII:"<< _input << " ==> ";
		
		std::string::size_type pos;
		for(int i=0;i<TABLE_SIZE;i++){
			while((pos= _input.find( _digits[i] ))!=std::string::npos){
				_input.replace(
					pos, 
					strlen( _digits[i] ),
					_ArabicIndicDigits[ i%10 ]
				);
			}
		}
		
		// Convert THOUSANDS SEPARATORS:
		while((pos= _input.find("٬"))!=std::string::npos) _input.replace(pos,strlen("٬"),",");
		// Convert DECIMAL SEPARATORS:
		while((pos= _input.find("٫"))!=std::string::npos) _input.replace(pos,strlen("٫"),".");
		// Convert PERCENT SIGN:
		while((pos= _input.find("٪"))!=std::string::npos) _input.replace(pos,strlen("٪"),"%");
		
	}
	
	//DEBUG:
	//std::cerr<< "RESULT: " << _input << std::endl;
}


//
//! get(): Returns the normalized version of the string
//
const std::string DigitConverter::get( void ) const {
	
	return (const std::string) _input;
	
}


//
//! Free function: ostream operator <<
//
std::ostream& operator<<(std::ostream& s, const DigitConverter& dc){
	
	return s << dc.get();
	
}

