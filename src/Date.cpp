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
// 2005.06.06.ET: Complete revision of set method to support
//                date ranges more flexibly.
// 2005.07.01.ET: Started new implementations of addition and
//                subtraction operators.
//

//
// Date.cpp
//
// Date class implementation
//
// Original implementation (c) 2003 Ed Trager
//
// Last revised: 2005.07.01.ET
//

#include "Warning.h"
#include "DigitConverter.h"
#include "Date.h"
#include <sstream>
#include <iomanip>

// DEBUG:
//#include <iostream>

///////////////////////////////////
//
// STATIC AND CONST STATIC MEMBERS:
//
///////////////////////////////////

const double Date::_DAYSINYEAR = 365.2425;      // Current (in year 2005) average no. of days per year

// Static days in month array:
const int Date::_daysInMonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};

// Static _calendarDisplayType:
Date::CALENDAR Date::_calendarDisplayType = Date::GREGORIAN;

// MISSING Date:
const Date Date::MISSING;

std::set<std::string> Date::_dateMissingValue;

bool Date::_displayRangeMeans=false;
bool Date::_displayDelimiters=false;

//////////////////////////
//
// PRIVATE METHODS
//
//////////////////////////

///
/// _yearMonthDayToJulian(): Converts Gregorian civil year yyyy, month mm, day dd 
/// to Julian day number:
///
long Date::_yearMonthDayToJulian(int yyyy,int mm, int dd){
	
	int ja,jy,jm;
	int julianday;
	
	if(yyyy<0) yyyy++;
	if(mm>2){
		jy=yyyy;
		jm=mm+1;
	}else{
		jy=yyyy-1;
		jm=mm+13;
	}
	julianday=(long)(floor(365.25*jy)+floor(30.6001*jm)+dd+1720995);
	if(dd+31L*(mm+12L*yyyy)>=_IGREG){
		/*                                                             */
		/* Original code from Press et. al. does not show cast here,   */
		/* but assignment to 'int' from 'double' will generate warning */
		/* without cast:                                               */
		/*                                                             */
		ja=(int)(0.01*jy);
		julianday+= 2-ja+(int)(0.25*ja);
	}
	
	return julianday;
	
}

///
/// _isLeapYear(): Determines if a Gregorian calendar year is a leap year.
///
bool Date::_isLeapYear(int year){
	
	
	return ( (year%4==0) && (year%100!=0) ) || (year%400==0);
	
}

///
/// _getLastDayOfMonth(): Returns the last day of the month of the given year.
///
int Date::_getLastDayOfMonth(int year, int month ){
	
	//
	// Handle February:
	//
	// if(month==2 && _isLeapYear(year)) return _daysInMonth[month-1]+1;
	//
	if(month==2 && _isLeapYear(year)) return 29;
	// 
	// Else use table lookup:
	//
	
	// DEBUG:
	// std::cout << "getLastDayOfMonth (2):" << (_daysInMonth[ month-1 ]) << std::endl;
	return _daysInMonth[month-1];
	
}

///
/// _getYearMonthDay(): Obtain the year, month, and day integers from a julian day number.
///
void Date::_getYearMonthDay(const long julian, int *yyyy, int *mm, int *dd) const {
	
	long ja,jalpha,jb,jc,jd,je;
	
	if(julian >= _IGREGJULIAN){
		/*                                                                */
		/* Original code from Press et. al. does not show cast to         */
		/* long here, but missing that will generate warning on ANSI C++  */
		/* compilers:                                                     */
		/*                                                                */
		jalpha=(long)(((double)(julian-1867216)-0.25)/36524.25);
		ja=julian+1+jalpha-(long)(0.25*jalpha);
	}else{
		ja=julian;
	}
	/*                                                              */
	/* Assignment to 'long int' from 'double' will generate warning */
	/* on ANSI C++ compilers:                                       */
	/*                                                              */
	jb=ja+1524;
	jc=(long)(6680.0+((double)(jb-2439870)-122.1)/365.25);
	jd=(long)(365*jc+(0.25*jc));
	je=(long)((jb-jd)/30.6001);
	*dd=jb-jd-(int)(30.6001*je);
	*mm=je-1;
	if(*mm>12) *mm-=12;
	*yyyy=jc-4715;
	if(*mm>2) --(*yyyy);
	if(*yyyy<=0) --(*yyyy);
	
	//
	// Correct year if using Buddhist calendar:
	// According to Thai Buddhism, Siddhartha Gotama 
	// reached nirvana 543 years before Jesus Christ
	// was born.  Thais now just use the Gregorian calendar, but with
	// years counted according to the Buddhist Era:
	//
	if( _calendarDisplayType == BUDDHIST_GREGORIAN ) *yyyy+=543;
	
}

///
/// _islamicYearMonthDayToJulian(): Obtain the Julian day number from an Islamic Hijri date:
///
long Date::_islamicYearMonthDayToJulian( int yyyy, int mm, int dd) const {
	
	return ( 
	           dd                     // days so far this month
	           + 29 * (mm - 1)        // days so far...
	           + mm/2                 // ...this year
	           + 354 * (yyyy - 1)     // non-leap days in prior years
	           + (3 + (11 * yyyy))/30 // leap days in prior years
	           + _IHIJRIJULIAN         // days before start of calendar
	       );
	       
}

/// 
/// _isIslamicLeapYear(): Returns true if it is an Islamic Hijri leap year:
///
bool Date::_isIslamicLeapYear(int year) const {
	
	return ((((11 * year) + 14) % 30) < 11);
	
}

///
/// _lastDayOfIslamicMonth(): Returns the number of days in the Islamic month.
///
int Date::_lastDayOfIslamicMonth(int year, int month) const {
	
	if(((month % 2) == 1) || ((month == 12) && _isIslamicLeapYear(year))){
		return 30;
	}else{
		return 29;
	}
}

///
/// _getIslamicYearMonthDay():
/// --> Only use if date is > _IHIJRIJULIAN (start of Islamic calendar)
///
void Date::_getIslamicYearMonthDay(const long julian, int *yyyy, int *mm, int *dd) const {
	
	int year, month, day;
	//
	// Search forward year by year from approximate year:
	//
	year = ( julian - _IHIJRIJULIAN )/355;
	while( julian >= _islamicYearMonthDayToJulian(year+1,1,1) ){
		year++;
	}
	//
	// Search forward month by month from Muharram:
	//
	month = 1;
	while( julian > _islamicYearMonthDayToJulian(year,month,_lastDayOfIslamicMonth(year,month) ) ){
		month++;
	}
	day = julian - _islamicYearMonthDayToJulian(year,month,1) + 1;
	// Finally, assign to return values:
	*yyyy=year;
	*mm  =month;
	*dd  =day;
	
}

//
// _intersectionIsNotNull(): 
//
bool Date::_intersectionIsNotNull(const Date& b) const{
	
	if( _isMissing || b._isMissing ) return false;
	if( _isRange && b._isRange ) {
		if(b._highEndJulian < _julian || b._julian > _highEndJulian ||
		   _highEndJulian < b._julian || _julian > b._highEndJulian)
			return false;
		return true;
	}
	
	if( _isRange || b._isRange ){
		const Date* r;
		const Date* n;
		if(_isRange){
			r = this;
			n = &b;
		}else{
			r = &b;
			n = this;
		}
		if( n->_julian >= r->_julian && n->_julian <= r->_highEndJulian ){
			return true;
		}
		return false;
	}
	// Get here if both dates are not ranges
	return _julian == b._julian;
	
}

//////////////////////////////////////////////////////
//
// Level 0 Functions for parsing a date or date range:
//
//////////////////////////////////////////////////////

///
/// _hasASCIIDateDelimiter()
///
///    If the string pointed to by "*position" starts with
///    ".", "/", or "-", then the
///    string pointer is incremented to point to the next character
///    and true is returned.  Otherwise, the pointer is not
///    incremented and false is returned.
///
bool Date::_hasASCIIDateDelimiter(const char **position){
	
	//register const char *s=*position;
	
	const char *s=*position;
	
	if( *s && (*s=='.' || *s=='/' || *s=='-')){
		
		*position+=1;
		return true;
		
	}else{
		
		return false;
		
	}
	
}

///
/// _hasApproximationIndicator()
///
///    If the string pointed to by "*position" starts with
///   "~", then the string pointer is incremented 
///    to point to the next character
///   and true is returned.  Otherwise, the pointer is not
///    incremented and false is returned.
///
bool Date::_hasApproximationIndicator(const char **position){
	
	//register const char *s=*position;
	
	const char *s=*position;
	
	if( *s && *s=='~'){
		
		*position+=1;
		return true;
		
	}else{
		
		return false;
		
	}
	
}

///
/// _hasCJKCharacterForYear(const char **position):
///
///   If the string pointed to by "*position" starts with
///    the CJKCharacter for year (年 in UTF8 format), then the
///    string pointer is incremented to point to the next character
///    and true is returned.  Otherwise, the pointer is not
///    incremented and false is returned.
///    
///    : e5 b9 b4
///
bool Date::_hasCJKCharacterForYear(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xe5' && *t=='\xb9' &&  *u=='\xb4' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

///
/// _hasCJKCharacterForMonth(const char **position):
///
///    If the string pointed to by "*position" starts with
///    the CJKCharacter for month (月 in UTF8 format), then the
///    string pointer is incremented to point to the next character
///    and true is returned.  Otherwise, the pointer is not
///   incremented and false is returned.
///    
///    : e5 b9 b4
/// 
bool Date::_hasCJKCharacterForMonth(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xe6' && *t=='\x9c' &&  *u=='\x88' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

///
/// _hasCJKCharacterForDay(const char **position):
///
///    If the string pointed to by "*position" starts with
///    the CJKCharacter for day (日 in UTF8 format), then the
///    string pointer is incremented to point to the next character
///    and true is returned.  Otherwise, the pointer is not
///    incremented and false is returned.
///    
///    : e6 97 a5
/// 
bool Date::_hasCJKCharacterForDay(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xe6' && *t=='\x97' &&  *u=='\xa5' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

//
// _hasCJKCharacterForArrive(const char **position):
//
//    If the string pointed to by "*position" starts with
//    the CJKCharacter for arrive (到 in UTF8 format), then the
//    string pointer is incremented to point to the next character
//    and true is returned.  Otherwise, the pointer is not
//    incremented and false is returned.
//    
//    : e5 88 b0
// 
bool Date::_hasCJKCharacterForArrive(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xe5' && *t=='\x88' &&  *u=='\xb0' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

//
// _hasCJKCharacterForFullStop(const char **position):
//
//    If the string pointed to by "*position" starts with
//    the CJKCharacter for full stop (。 in UTF8 format), then the
//    string pointer is incremented to point to the next character
//    and true is returned.  Otherwise, the pointer is not
//    incremented and false is returned.
//    
//    : e3 80 82
// 
bool Date::_hasCJKCharacterForFullStop(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xe3' && *t=='\x80' &&  *u=='\x82' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

//
// _hasCJKCharacterForForwardSlash(const char **position):
//
//    If the string pointed to by "*position" starts with
//    the CJKCharacter for forward slash (／ in UTF8 format), then the
//    string pointer is incremented to point to the next character
//    and true is returned.  Otherwise, the pointer is not
//    incremented and false is returned.
//    
//    : ef bc 8f
// 
bool Date::_hasCJKCharacterForForwardSlash(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xef' && *t=='\xbc' &&  *u=='\x8f' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

//
// _hasCJKCharacterForDash(const char **position):
//
//    If the string pointed to by "*position" starts with
//    the CJKCharacter for dash (－ in UTF8 format), then the
//    string pointer is incremented to point to the next character
//    and true is returned.  Otherwise, the pointer is not
//    incremented and false is returned.
//    
//    : ef bc 8d
// 
bool Date::_hasCJKCharacterForDash(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xef' && *t=='\xbc' &&  *u=='\x8d' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

//
// _hasCJKCharacterForOpeningSquareBracket(const char **position):
//
//    If the string pointed to by "*position" starts with
//    the CJKCharacter for opening square bracket (［ in UTF8 format), then the
//    string pointer is incremented to point to the next character
//    and true is returned.  Otherwise, the pointer is not
//    incremented and false is returned.
//    
//    : ef bc bb
// 
bool Date::_hasCJKCharacterForOpeningSquareBracket(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xef' && *t=='\xbc' &&  *u=='\xbb' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

//
// _hasCJKCharacterForClosingSquareBracket(const char **position):
//
//    If the string pointed to by "*position" starts with
//    the CJKCharacter for  (］ in UTF8 format), then the
//    string pointer is incremented to point to the next character
//    and true is returned.  Otherwise, the pointer is not
//    incremented and false is returned.
//    
//    : ef bc bd
// 
bool Date::_hasCJKCharacterForClosingSquareBracket(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xef' && *t=='\xbc' &&  *u=='\xbd' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

//
// _hasUnicodeRightArrow(const char **position):
//
//    If the string pointed to by "*position" starts with
//    the Unicode right arrow U+2192 (→ in UTF8 format), then the
//    string pointer is incremented to point to the next character
//    and true is returned.  Otherwise, the pointer is not
//    incremented and false is returned.
//    
//    : e2 86 92
// 
bool Date::_hasUnicodeRightArrow(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xe2' && *t=='\x86' &&  *u=='\x92' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

//
// Korean date delimiters:  ex: 1957년12월11일
//

//
// _hasKoreanHangulForYear(const char **position):
//
//    If the string pointed to by "*position" starts with
//    the CJKCharacter for year (년 in UTF8 format), then the
//    string pointer is incremented to point to the next character
//    and true is returned.  Otherwise, the pointer is not
//    incremented and false is returned.
//    
//    : eb 85 84
// 
bool Date::_hasKoreanHangulForYear(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xeb' && *t=='\x85' &&  *u=='\x84' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

///
/// _hasKoreanHangulForMonth(const char **position):
///
///    If the string pointed to by "*position" starts with
///    the CJKCharacter for year (월 in UTF8 format), then the
///    string pointer is incremented to point to the next character
///    and true is returned.  Otherwise, the pointer is not
///    incremented and false is returned.
///    
///    : ec 9b 94
/// 
bool Date::_hasKoreanHangulForMonth(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xec' && *t=='\x9b' &&  *u=='\x94' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

///
/// _hasKoreanHangulForDay(const char **position):
///
///    If the string pointed to by "*position" starts with
///    the CJKCharacter for year (일 in UTF8 format), then the
///    string pointer is incremented to point to the next character
///    and true is returned.  Otherwise, the pointer is not
///    incremented and false is returned.
///    
///    : ec 9d bc
/// 
bool Date::_hasKoreanHangulForDay(const char **position){
	
	//register const char *s=*position;
	//register const char *t=s+1;
	//register const char *u=s+2;
	
	const char *s=*position;
	const char *t=s+1;
	const char *u=s+2;
	
	if( *s && *t && *u && *s=='\xec' && *t=='\x9d' &&  *u=='\xbc' ){
		
		*position+=3;
		return true;
		
	}else{
		return false;
		
	}
	
}

///
/// _hasUTF8Character:
///
///    If the string pointed to by "*position" starts with
///    the character in UTF8Character, then the
///    string pointer is incremented to point to the next character
///    and true is returned.  Otherwise, the pointer is not
///    incremented and false is returned.
///
bool Date::_hasUTF8Character(const char **position,const char *UTF8Character){
	
	//register const char *s=*position;
	//register const char *t=UTF8Character;
	
	const char *s=*position;
	const char *t=UTF8Character;
	
	for(s=*position,t=UTF8Character; *t && *s==*t ; s++,t++);
	// Matches if t exhausted:
	if(!*t){
		
		*position=s;
		return true;
		
	}else{
		
		return false;
		
	}
	
}

//////////////////////////////////////////////////////
//
// Level 1 Functions for parsing a date or date range:
//
//////////////////////////////////////////////////////

///
/// _hasRangeDelimiter(const char **position,bool startBracketFound):
///
///    If the string pointed to by "*position" starts with
///    a valid range delimiter (" - ","r", "R", "", or "", then the
///    string pointer is incremented to point to the next character
///    and true is returned.  Otherwise, the pointer is not
///    incremented and false is returned.
///    
bool Date::_hasRangeDelimiter(const char **position,bool startBracketFound){
	
	//register const char *s=*position;
	
	const char *s=*position;
	
	//DEBUG:
	//std::cout << "In hasRangeDelimiter: " << s << std::endl;
	
	//
	// First check for ASCII delimiters:
	//
	if(*s && (*s=='R' || *s=='r')){
		
		s++;
		*position=s;
		return true;
		
	}
	//
	// Otherwise, if a start bracket was encountered, 
	// allow simple "-" even without surrounding white space:
	//
	if(startBracketFound && *s && *s=='-'){
		
		s++;
		*position=s;
		return true;
		
	}
	//
	// Otherwise, in the case of no start bracket, a "-" with surrounding
	// spaces is also considered a range delimiter:
	//
	if(*s && *s=='-' && *(s-1) && *(s-1)==' ' && *(s+1) && *(s+1)==' '){
		
		s+=2;
		*position=s;
		return true;
		
	}
	//
	// Unicode right arrow is also allowed:
	//
	if( _hasUnicodeRightArrow(position)){
		return true;
	}
	
	//
	// Otherwise, the CJK character for "to/arrive" is allowed:
	//
	if(_hasCJKCharacterForArrive(position)){
		return true;
	}
	
	//
	// Otherwise, the CJK dash character is also allowed:
	//
	return _hasCJKCharacterForDash(position);
	
}

//
// _hasCJKVersionOfGenericDateDelimiter()
//
bool Date::_hasCJKVersionOfGenericDateDelimiter(const char **position){
	
	return _hasCJKCharacterForFullStop(position) ||
	       _hasCJKCharacterForForwardSlash(position) ||
	       _hasCJKCharacterForDash(position);
	
}


//
// _hasYearDelimiter()
//
bool Date::_hasYearDelimiter(const char **position,bool *isProperCJKDate){
	
	// Assume that it is not a CJK date:
	*isProperCJKDate = false;
	// ... And check the simple ASCII case first:
	if(_hasASCIIDateDelimiter(position)) return true;
	// If we get here, we need to check the CJK cases:
	*isProperCJKDate = _hasCJKCharacterForYear(position);
	if(!*isProperCJKDate){
		// Check the Korean Hangul case:
		*isProperCJKDate = _hasKoreanHangulForYear(position);
	}
	if(*isProperCJKDate) return true;
	// Finally, check the odd-ball case:
	return _hasCJKVersionOfGenericDateDelimiter(position);
	
}

//
// _hasMonthDelimiter()
//
bool Date::_hasMonthDelimiter(const char **position){
	
	return _hasASCIIDateDelimiter(position) ||
	       _hasCJKCharacterForMonth(position) ||
	       _hasKoreanHangulForMonth(position) ||
	       _hasCJKVersionOfGenericDateDelimiter(position);
	
}

//
// _hasDayDelimiter()
//
bool Date::_hasDayDelimiter(const char **position){
	
	//
	// Currently, a day delimiter is only
	// mandatory in the case of a CJK date:
	//
	return _hasCJKCharacterForDay(position) ||
	       _hasKoreanHangulForDay(position);
	
}

//
// _skipWhiteSpace(): 
//
//    Skips white space, returning a pointer to the next character
//    after the white space.
//
void Date::_skipWhiteSpace(const char **position){
	
	//register const char *s=*position;
	
	const char *s=*position;
	for(;*s && *s==' ';s++);
	*position=s;
	
}

//
// _hasOpeningBracket()
//
bool Date::_hasOpeningBracket(const char **position){
	
	//
	// First check for ASCII version,
	// then CJK version:
	//
	//register const char *s=*position;
	const char *s=*position;
	
	if(*s && *s=='['){
		
		(*position)++;
		return true;
		
	}else{
		return _hasCJKCharacterForOpeningSquareBracket(position);
	}
	
}


//
// _hasClosingBracket()
//
bool Date::_hasClosingBracket(const char **position){
	
	//
	// First check for ASCII version,
	// then CJK version:
	//
	// register const char *s=*position;
	
	const char *s=*position;
	
	if(*s && *s==']'){
		
		(*position)++;
		return true;
		
	}else{
		return _hasCJKCharacterForClosingSquareBracket(position);
	}
	
}

///
/// _getValue()
///
///    Converts a string of digits, optionally prefixed by a '+' or '-',
///    into an integer value:
///
int Date::_getValue(const char **position){
	
	int VALUESTRINGSIZE=5;
	char valueString[VALUESTRINGSIZE+1];
	int sign=1;
	
	//register const char *s=*position;
	//register char *d;
	
	const char *s=*position;
	char *d;
	
	if(*s=='-'){
		sign=-1;
		s++;
	}
	if(*s=='+') s++;
	// Copy digits into valueString:
	for(d=valueString;*s && isdigit(*s) && d<valueString+VALUESTRINGSIZE;*d++=*s++);
	*d='\0';
	// Save incremented position in string:
	*position=s;
	
	// return the value:
	return sign*atoi(valueString);
	
}

//////////////////////////
//
// STATIC METHODS
//
//////////////////////////

///
/// isMissingValue determines whether a string represents
/// one of the missing values in the missing value lookup table.
///
bool Date::isMissingValue(std::string value){
	
	std::set<std::string>::const_iterator iter;
	iter = _dateMissingValue.find(Data::trim(value));
	if(iter != _dateMissingValue.end()) return true;
	return false;
	
}

//////////////////////////
//
// PUBLIC METHODS
//
//////////////////////////


///
/// setMissing:
///
void Date::setMissing( void ){
	
	_julian = _highEndJulian =0;
	_isApproximate=false;
	_isRange=false;
	_isMissing=true;
	
}



///
/// bool isMissing():
/// Required by virtual base class:
///
bool Date::isMissing( void ) const {
	return _isMissing;
}

///
/// Set a date from a "yyyy.mm.dd" string
/// or a date range string.
///
void Date::set(const char* dateString){
	
	if(Data::isGlobalMissingValue(dateString)) { setMissing(); return; }
	if(Date::isMissingValue(dateString))    { setMissing(); return; }
	
	//
	// Check if Null
	//
	 if(!*dateString){
		setMissing();
		return;
	}
	
	// for Warning messages:
	const char *methodName="Date::set()";
	
	//
	// Use digit converter for handling the
	// possible case of non-ASCII digits:
	//
	DigitConverter dc(dateString);
	std::string ss=dc.get();
	const char *s =ss.c_str();
	
	//
	// Defaults for boolean state vars:
	//
	_isMissing=_isApproximate=_isRange=false;
	
	//
	// Booleans for tracking brackets, CJK dates:
	//
	bool hasYearBracket  = false;
	bool hasMonthBracket = false;
	bool hasDayBracket   = false;
	bool isProperCJKDate = false;
	
	// Low range:
	int _year_low=0;
	int _month_low=0;
	int _day_low=0;
	
	// High Range:
	int _year_high=0;
	int _month_high=0;
	int _day_high=0;
	
	//
	// Set position to point to the 
	// beginning of the string:
	//
	const char *position[1];
	position[0]=s;
	
	/////////////////////////////////
	//
	// STARTING TO PARSE DATE STRING:
	//
	/////////////////////////////////
	
	//
	// Check for optional approximation indicator:
	//
	_isApproximate = _hasApproximationIndicator(position);
	
	//////////////////
	//
	// "LOW" DATE:
	//
	//////////////////
	
	// Check for optional opening bracket for year range:
	hasYearBracket = _hasOpeningBracket(position);
	//
	// Get YEAR:
	//
	_year_low = _getValue(position);
	// Check year values:
	if( ! _yearIsValid( _year_low , dateString ) ) return;
	
	// Jump to range delimiter if bracketed:
	if(hasYearBracket){
		// CJK Year delimiter is optional in this case,
		// and any other delimiters are not acceptable:
		_hasCJKCharacterForYear(position);
		goto getRangeDelimiter;
	}
	// In normal, non-bracketed case, the year delimiter is reqd.:
	if(!_hasYearDelimiter(position,&isProperCJKDate)){
		// If the year delimiter is absent, it could indicate
		// a range:
		goto getRangeDelimiter;
	}
	
	// Check for optional opening bracket for month range:
	hasMonthBracket = _hasOpeningBracket(position);
	//
	// get MONTH:
	//
	_month_low = _getValue(position);
	// Check month values:
	if( ! _monthIsValid( _month_low , dateString ) ) return;
	
	// Jump to range delimiter if bracketed:
	if(hasMonthBracket){
		// CJK Month delimiter is optional in this case,
		// and any other delimiters are not acceptable:
		_hasCJKCharacterForMonth(position);
		goto getRangeDelimiter;
	}
	// In normal, non-bracketed case, the month delimiter is reqd.:
	if(!_hasMonthDelimiter(position)){
		// If the month delimiter is absent, it could indicate
		// a range:
		goto getRangeDelimiter;
	}
	
	// Check for optional opening bracket for day range:
	hasDayBracket = _hasOpeningBracket(position);
	//
	// get DAY:
	//
	_day_low = _getValue(position);
	// Check day values:
	if(!_dayIsValid( _year_low , _month_low , _day_low , dateString ) ) return;
	
	if(hasDayBracket){
		// CJK Day delimiter is optional in this case,
		// and any other delimiters are not acceptable:
		_hasCJKCharacterForDay(position);
		goto getRangeDelimiter;
	}
	
	// In normal, non-bracketed case, the Day
	// delimiter is only required on proper CJK dates:
	if(isProperCJKDate){
		if(!_hasDayDelimiter(position)){
			
			// Warn but continue:
			Warning(methodName,
			        "The date \"%1$s\" is missing the day delimiter.",
			        dateString
			);
			
		}
	}
	
	//////////////////////////////////
	//
	// TARGET OF GOTO: RANGE DELIMITER
	//
	// getRangeDelimiter
	//
	//////////////////////////////////
	getRangeDelimiter:
	
	_skipWhiteSpace(position);
	
	// DEBUG:
	// std::cout << "AFTER READING LOW DATE: |" << *position << "|" << std::endl;
	
	if(!_hasRangeDelimiter(position,hasYearBracket || hasMonthBracket || hasDayBracket)){
		//
		// Range delimiter is required on bracketed expressions:
		//
		if(hasYearBracket ){
			setMissing();
			Warning(methodName,
			        "The date \"%1$s\" has been set to missing because a range of years was expected but there is no range delimiter present.",
			        dateString
			);
			return;
		}
		if(hasMonthBracket ){
			setMissing();
			Warning(methodName,
			        "The date \"%1$s\" has been set to missing because a range of months was expected but there is no range delimiter present.",
			        dateString
			);
			return;
		}
		if(hasDayBracket ){
			setMissing();
			Warning(methodName,
			        "The date \"%1$s\" has been set to missing because a range of days was expected but there is no range delimiter present.",
			        dateString
			);
			return;
		}
		
		//
		// MONTH UNDEFINED:
		//
		// If the month is undefined, then we actually have a range
		// representing the entire year:
		//
		if( ! _month_low ){
			_month_low     = 01;
			_day_low       = 01;
			_year_high     = _year_low;
			_month_high    = 12;
			_day_high      = 31;
			_isApproximate = _isRange = true;
			_julian        = _yearMonthDayToJulian( _year_low  , _month_low  , _day_low  );
			_highEndJulian = _yearMonthDayToJulian( _year_high , _month_high , _day_high );
			return;
		}
		
		//
		// DAY UNDEFINED:
		//
		// If the day is undefined, then we actually have a range
		// representing the entire month:
		//
		if( ! _day_low ){
			_day_low       = 01;
			_year_high     = _year_low;
			_month_high    = _month_low;
			_day_high      = _getLastDayOfMonth( _year_high , _month_high );
			_isApproximate = _isRange = true;
			_julian        = _yearMonthDayToJulian( _year_low  , _month_low  , _day_low  );
			_highEndJulian = _yearMonthDayToJulian( _year_high , _month_high , _day_high );
			return;
		}
		
		//
		// SINGLE, FULLY-SPECIFIED, NON-RANGED DATE:
		//
		// Get here if it is a single, fully-specified, non-ranged date:
		//
		_julian = _yearMonthDayToJulian( _year_low , _month_low, _day_low );
		return;
		
	}else{
		//
		// RANGE DELIMITER FOUND, SO MUST BE RANGED DATE:
		//
		_isApproximate=true;
		_isRange=true;
		
	}
	
	_skipWhiteSpace(position);
	
	//////////////////
	//
	// "HIGH" DATE:
	//
	//////////////////
	
	//
	// For bracketed month or day expressions, we
	// need to jump ahead to the appropriate location:
	//
	if(hasMonthBracket) goto getMonth;
	if(hasDayBracket  ) goto getDay;
	
	//
	// get YEAR:
	//
	_year_high = _getValue(position);
	// check year:
	if( ! _yearIsValid( _year_high , dateString) ) return;
	
	// If bracketed, check for closing bracket:
	if(hasYearBracket){
		// CJK Year delimiter is optional in this case,
		// and any other delimiters are not acceptable:
		_hasCJKCharacterForYear(position);
		if(!_hasClosingBracket(position)){
			
			Warning(methodName,
			        "The date \"%1$s\" has been set to missing because it is missing a closing bracket on a range of years.",
			        dateString
			);
			setMissing();
			return;
			
		}
		//
		// RETURN POINT ON
		// A BRACKETED YEAR EXPRESSION: RANGE OF YEARS:
		//
		if(_year_high <= _year_low){
			Warning(methodName,
			        "The date \"%1$s\" has been set to missing because the first year is greater than or equal to the second year.",
			        dateString
			);
			setMissing();
			return;
		}
		_month_low     = 01;
		_day_low       = 01;
		_month_high    = 12;
		_day_high      = 31;
		_isApproximate = _isRange = true;
		_julian        = _yearMonthDayToJulian(_year_low,_month_low,_day_low);
		_highEndJulian = _yearMonthDayToJulian(_year_high,_month_high,_day_high);
		return;
		
	}
	// In normal, non-bracketed case, the Year
	// delimiter is required,
	if(!_hasYearDelimiter(position,&isProperCJKDate)){
		// If year delimiter is not present, it could
		// represent a range, so jump to endStage:
		goto endStage;
	}
	
	//
	// 2005.08.16.ET ADDENDUM: 
	//   Check whether we need to jump to endStage
	//   because we are already at the end:
	_skipWhiteSpace(position);
	if(*position[0]=='\0') goto endStage;
	
	//////////////////
	//
	// TARGET OF GOTO:
	// 
	// getMonth:
	//
	//////////////////
	getMonth:
	
	_month_high = _getValue(position);
	// check month:
	if( ! _monthIsValid( _month_high , dateString ) ) return;
	
	// If bracketed, check for closing bracket:
	if(hasMonthBracket){
		// CJK Month delimiter is optional in this case,
		// and any other delimiters are not acceptable:
		_hasCJKCharacterForMonth(position);
		if(!_hasClosingBracket(position)){
			
			Warning(methodName,
			        "The date \"%1$s\" has been set to missing because it is missing a closing bracket on a range of months.",
			        dateString
			);
			setMissing();
			return;
			
		}
		
		//
		// RETURN POINT ON
		// A BRACKETED MONTH EXPRESSION: RANGE OF MONTHS:
		//
		if(_month_high <= _month_low){
			Warning(methodName,
			        "The date \"%1$s\" has been set to missing because the first month is greater than or equal to the second month.",
			        dateString
			);
			setMissing();
			return;
		}
		_year_high     = _year_low;
		_day_low       = 01;
		_day_high      = _getLastDayOfMonth( _year_high , _month_high );
		_isApproximate = _isRange = true;
		_julian        = _yearMonthDayToJulian( _year_low  , _month_low  , _day_low  );
		_highEndJulian = _yearMonthDayToJulian( _year_high , _month_high , _day_high );
		return;
		
	}
	// In normal, non-bracketed case, the Month
	// delimiter is required:
	if(!_hasMonthDelimiter(position)){
		
		// If month delimiter is not present, it could
		// represent a range, so jump to endStage:
		goto endStage;
		
	}
	
	//
	// 2005.08.16.ET ADDENDUM: 
	//   Check whether we need to jump to endStage
	//   because we are already at the end:
	_skipWhiteSpace(position);
	if(*position[0]=='\0') goto endStage;
	
	
	//////////////////
	//
	// TARGET OF GOTO:
	// 
	// getDay:
	//
	//////////////////
	getDay:
	
	_day_high = _getValue(position);
	//
	// Check day values:
	//
	if(hasDayBracket){
		//
		// Because we are looking at a bracketed day expression,
		// the year and month are held in _year_low and _month low
		// and _year_high and _month_high have not yet been specified:
		//
		if(!_dayIsValid( _year_low , _month_low , _day_high , dateString ) ) return;
		
	}else{
		
		if(!_dayIsValid( _year_high , _month_high , _day_high , dateString ) ) return;
		
	}
	
	if(hasDayBracket){
		// CJK Day delimiter is optional in this case,
		// and any other delimiters are not acceptable:
		_hasCJKCharacterForDay(position);
		if(!_hasClosingBracket(position)){
			
			Warning(methodName,
			        "The date \"%1$s\" has been set to missing because it is missing a closing bracket on a range of days.",
			        dateString
			);
			setMissing();
			return;
			
		}
		//
		// THIS IS THE RETURN POINT
		// FOR A BRACKETED DAY EXPRESSION: RANGE OF DAYS:
		//
		if(_day_high <= _day_low){
			Warning(methodName,
			        "The date \"%1$s\" has been set to missing because the first day is greater than or equal to the second day.",
			        dateString
			);
			setMissing();
			return;
		}
		_year_high     = _year_low;
		_month_high    = _month_low;
		_isApproximate = _isRange = true;
		_julian        = _yearMonthDayToJulian( _year_low  , _month_low  , _day_low  );
		_highEndJulian = _yearMonthDayToJulian( _year_high , _month_high , _day_high );
		return;
		
	}
	if(isProperCJKDate){
		// DEBUG
		// std::cout << "(2) CJK DATE now at :" << *position << std::endl;
		if(!_hasDayDelimiter(position)){
			// Warn, but continue:
			Warning(methodName,
			        "The date \"%1$s\" is missing a day delimiter.",
			        dateString
			);
		}	
	}
	
	//////////////////
	//
	// TARGET OF GOTO:
	// 
	// endStage:
	//
	//////////////////
	endStage:
	
	//
	// State analysis:
	//
	// 1) _year_low and _year_high are definitely present
	//
	// 2) The _month_low and _month_high may not be present
	//
	// 3) The _day_low and _day_high may not be present
	//
	
	if( ! _month_low ) _month_low  = 01;
	if( ! _month_high) _month_high = 12;
	if( ! _day_low )   _day_low    = 01;
	if( ! _day_high)   _day_high   = _getLastDayOfMonth( _year_high , _month_high );
	
	_julian        = _yearMonthDayToJulian( _year_low  , _month_low  , _day_low  );
	_highEndJulian = _yearMonthDayToJulian( _year_high , _month_high , _day_high );
	
	if( _highEndJulian <= _julian ){
		Warning(methodName,
		        "The date \"%1$s\" has been set to missing because the first date is greater than or equal to the second date.",
		        dateString
		);
		setMissing();
		return;
	}
	
	return;
	
}

///
/// Set date from std::string:
///
void Date::set(const std::string &dateString){
	
	set(dateString.c_str());
	
}

///
/// Get string representation of the date:
///
/// UNFINISHED: ostringstream may be SLOW : test and change
///
const std::string Date::get( void ) const {
	
	std::ostringstream oss;
	
	//
	// Conditionally add opening delimiter:
	//
	if(_displayDelimiters) oss << "{";
	
	if( _isMissing ){
		
		oss << ".";
		
	}else if(_calendarDisplayType==HIJRI && _julian< _IHIJRIJULIAN){
		
		Warning("Date::get()",
		        "The *display* of Islamic proleptic dates (i.e., before 0622.07.16 C.E.) has not been implemented."
		);
		
		oss << ".";
		
	}else{
		int yyyy=0,mm=0,dd=0;
		if(_calendarDisplayType==HIJRI){
			_getIslamicYearMonthDay(_julian,&yyyy,&mm,&dd);
		}else{
			_getYearMonthDay(_julian,&yyyy,&mm,&dd);
		}
		
		if(_isApproximate && !_isRange) oss << "~";
		if(_isRange){
			
			int highYY=0,highMM=0,highDD=0;
			int meanYY=0,meanMM=0,meanDD=0;
			_getYearMonthDay(_highEndJulian,&highYY,&highMM,&highDD);
			_getYearMonthDay((_julian + _highEndJulian)/2,&meanYY,&meanMM,&meanDD);
			
			if(_displayRangeMeans){
				oss << "x̄="
				    << std::setfill('0') << std::setw(4) << meanYY 
				    << '.' 
				    << std::setw(2) << meanMM
				    << '.' 
				    << std::setw(2) << meanDD
				    << " : ";
			}
			
			//oss << '{'
			oss << std::setfill('0') << std::setw(4) << yyyy 
			    << '.' 
			    << std::setw(2) << mm 
			    << '.' 
			    << std::setw(2) << dd 
			    << "→" 
			    << std::setfill('0') << std::setw(4) << highYY
			    << '.'
			    << std::setw(2) << highMM
			    << '.'
			    << std::setw(2) << highDD;
			//    << '}';
		}else{
			//oss << '{' 
			oss << std::setfill('0') << std::setw(4) << yyyy 
			    << '.' 
			    << std::setw(2) << mm 
			    << '.' 
			    << std::setw(2) << dd;
			//    << '}' ;
		}
	}
	
	//
	// Conditionally add closing delimiter:
	//
	if(_displayDelimiters) oss << "}";
	
	return (const std::string) oss.str();
	
}

std::ostream& operator<<(std::ostream& s,const Date& d){
	
	return s << d.get();
	
}

///////////////////////////
//
// REVISED PRIVATE METHODS:
//
///////////////////////////

///
/// Checks for errors in year yyyy, month mm, day dd
/// for Gregorian civil calendar:
/// 

//
// _yearIsValid()
//
bool Date::_yearIsValid( int year , const char *dateString ){
	
	const char *methodName="Date::_yearIsValid()";
	if( year < -4713 ){
		
		Warning(methodName,
		        "The date \"%1$s\" has been set to missing because the year precedes the start of the current Julian period on January 1, 4713 BCE.",
		        dateString
		);
		
		setMissing();
		return false;
	}
	if( year ==0 ){
		Warning(methodName,"The date \"%1$s\" has been set to missing because there is no year zero in the Gregorian calendar.",dateString);
		setMissing();
		return false;
	}
	if( year > 3267 ){
		Warning(methodName,"The date \"%1$s\" has been set to missing because the year exceeds the current Julian period which ends on 3268-01-22 GC.",dateString);
		setMissing();
		return false;
	}
	return true;
}

//
// _monthIsValid()
//
bool Date::_monthIsValid( int month , const char *dateString ){
	
	if( month<1 || month>12){
		Warning("Date::_monthIsValid()","The date \"%1$s\" has been set to missing because the month is not valid.",dateString);
		setMissing();
		return false;
	}
	return true;
	
}


//
// _dayIsValid()
//
bool Date::_dayIsValid(int year, int month, int day , const char *dateString ){
	
	// for warnings:
	const char *methodName="Date::_dayIsValid()";
	// DEBUG:
	// std::cout << "in _dayIsValid:" << year << "." << month << "." << day << std::endl; 
	
	// Check if DAY is valid:
	if( day < 1 ){
		Warning(methodName,"The date \"%1$s\" has been set to missing because the day is not valid.",dateString);
		setMissing();
		return false;
	}
	if( month==2 && _isLeapYear(year)){
		if( day > 29 ){
			Warning(methodName,"The date \"%1$s\" has been set to missing because there are only 29 days in this month.",dateString);
			setMissing();
			return false;
		}
	}else if(day > _daysInMonth[month-1]){
		// Of course month array is zero-offset:
		Warning(methodName,
		        "The date \"%1$s\" has been set to missing because there are only %2$02i days in this month.",
		        dateString,
		        _daysInMonth[month-1]
		);
		setMissing();
		return false;
	}
	// Get here if everything is OK:
	return true;
	
}

///
/// Date::Date(const Date *d)
///
///   Copy construct a date from a const pointer to a date:
///
Date::Date(const Date *d){
	
	_isMissing      = d->_isMissing;
	_isApproximate  = d->_isApproximate;
	_isRange        = d->_isRange;
	_julian         = d->_julian;
	_highEndJulian  = d->_highEndJulian;
	
}

///
/// Prefix ++
///
Date& Date::operator ++(){
	
	if(!_isMissing){
		
		++_julian;
		if(_isRange) ++_highEndJulian;
		
	}
	
	return *this;
	
}

///
/// Postfix ++
///
Date Date::operator ++(int){
	
	// Save the return value:
	Date returnValue(this);
	
	if(!_isMissing){
		
		_julian++;
		
		if(_isRange){
			
			_highEndJulian++;
			
		}
		
	}
	
	return returnValue;
	
}

///
/// Prefix --
///
Date& Date::operator --(){
	
	if(!_isMissing){
		
		--_julian;
		if(_isRange) --_highEndJulian;
		
	}
	
	return *this;
	
}

///
/// Postfix --
///
Date Date::operator --(int){
	
	// Save the return value:
	Date returnValue(this);
	
	if(!_isMissing){
		
		_julian--;
		
		if(_isRange){
			
			_highEndJulian--;
			
		}
		
	}
	
	return returnValue;
	
}

///
/// operator+=
///
Date& Date::operator+=( const Number & b ){
	
	if( b._isMissing ) setMissing();
	if(   _isMissing ) return *this;
	
	//
	// Initialize the return value assuming the default case
	// of scalars:
	//
	_julian += (long) b._value;
	if( b._isApproximate ) _isApproximate = true;
	
	if(_isRange || b._isRange){
		
		_isRange=true;
		
		if(_isRange && b._isRange){
			
			_highEndJulian += (long) b._highEndValue;
			
		}else if(_isRange){
			
			_highEndJulian += (long) b._value;
			
		}else{
			
			_highEndJulian = _julian + (long) b._highEndValue;
			
		}
		
	}
	
	return *this;
	
}

///
/// operator -=
///
Date& Date::operator-=( const Number & b ){
	
	if( b._isMissing ) setMissing();
	if(   _isMissing ) return *this;
	
	if(b._isApproximate) _isApproximate = true;
	
	// Handle simple case where both are scalers:
	if( !(_isRange || b._isRange)){
		
		// Construct from scalers:
		_julian -= (long) b._value;
		return *this;
		
	}
	//
	// Get here if one or both are ranges:
	//
	if( _isRange && b._isRange ){
		// BOTH ARE RANGED:
		
		// Set the minimum value:
		_julian -= (long) b._highEndValue;
		// Toggle as a range:
		_isRange = true;
		// Set the maximum value:
		_highEndJulian -= (long) b._value;
		return *this;
		
	}
	// GET HERE IF ONLY ONE IS RANGED:
	
	// Set the maximum value:
	if( _isRange ){
		
		// THIS IS RANGED, BUT B IS NOT:
		// Set the minimum value:
		_julian        -= (long) b._value;
		_highEndJulian -= (long) b._value;
		
		
	}else{
		// GET HERE IF B IS RANGED, BUT THIS IS NOT:
		// Toggle as a range:
		
		_isRange = true;
		_highEndJulian  =  _julian;
		_julian        -= (long) b._highEndValue;
		_highEndJulian -= (long) b._value;
		
	}
	
	return *this;
	
}


///
/// operator+():
/// 
Date Date::operator+(const Number &n) const{ 
	
	if( _isMissing || n._isMissing ) return Date::MISSING;
	
	//
	// Initialize the return value assuming the default case
	// of scalars:
	//
	Date result;
	result._isMissing = false;
	result._julian    = _julian + (long) n._value;
	
	if( _isRange || n._isRange ){
		
		result._isRange = true;
		
		if( _isRange && n._isRange){
			
			result._highEndJulian = _highEndJulian + (long) n._highEndValue;
			
		}else if(_isRange){
			
			result._highEndJulian = _highEndJulian + (long) n._value;
			
		}else{
			
			result._highEndJulian = _julian + (long) n._highEndValue;
			
		}
		
	}
	
	if( _isApproximate || n._isApproximate ) result._isApproximate = true;
	
	return result;
	
}

///
/// operator-():
/// 
Date Date::operator-(const Number &n) const{ 
	
	if( _isMissing || n._isMissing ) return Date::MISSING;
	
	Date result;
	result._isMissing     = false;
	result._isApproximate = _isApproximate || n._isApproximate;
	
	// Handle simple case where both are scalers:
	if( !(_isRange || n._isRange)){
		
		// Construct from scalers:
		result._isRange         = false;
		result._julian        = _julian - (long) n._value;
		return result;
		
	}
	//
	// Get here if one or both are ranges:
	//
	result._isRange = true;
	
	if( _isRange && n._isRange ){
		// BOTH ARE RANGED:
		
		// Set the minimum value:
		result._julian = _julian - (long) n._highEndValue;
		
		// Set the maximum value:
		result._highEndJulian = _highEndJulian - (long) n._value;
		
		return result;
		
	}
	// GET HERE IF ONLY ONE IS RANGED:
	
	if( _isRange){
		// THIS IS RANGED, BUT B IS NOT:
		
		// Set the minimum value:
		result._julian = _julian - (long) n._value;
		
		// Set the maximum value:
		result._highEndJulian = _highEndJulian - (long) n._value;
		
		return result;
		
	}
	
	// GET HERE IF B IS RANGED, BUT THIS IS NOT:
	
	// Set minimum:
	result._julian = _julian - (long) n._highEndValue;
	
	// set the maximum:
	result._highEndJulian = _julian - (long) n._value;
	
	return result;
	
}

///
/// operator-():
/// 
Number Date::operator-(const Date &d) const{ 
	
	if( _isMissing || d._isMissing ) return Number::MISSING;
	
	// for warnings:
	const char *methodName="Date::operator-()";
	
	Number result;
	result._isMissing     = false;
	result._isApproximate = _isApproximate || d._isApproximate;
	
	// Handle simple case where both are scalers:
	if( !(_isRange || d._isRange)){
		
		// Construct from scalers:
		result._isRange       = false;
		result._value         = (double) _julian - d._julian;
		
		// Check validity of the result:
		if(result._value < 0){
			Warning(methodName,
			        "In the expression, %1$s - %2$s, the second date is greater than the first and therefore the result has been set to missing",
			        get().c_str(),
			        d.get().c_str()
			);
			return Number::MISSING;
		}
		
		return result;
		
	}
	//
	// Get here if one or both are ranges:
	//
	result._isRange = true;
	
	if( _isRange && d._isRange ){
		// BOTH ARE RANGED:
		
		// Set the minimum value:
		result._value        = (double) _julian - d._highEndJulian;
		
		// Set the maximum value:
		result._highEndValue = (double) _highEndJulian - d._julian;
		
		// Check validity of the result:
		if(result._value < 0 || result._highEndValue <0 ){
			Warning(methodName,
			        "In the expression, %1$s - %2$s, one or more dates in the second range is greater than in the first range and therefore the result has been set to missing.",
			        get().c_str(),
			        d.get().c_str()
			);
			return Number::MISSING;
		}
		
		return result;
		
	}
	// GET HERE IF ONLY ONE IS RANGED:
	
	if( _isRange){
		// THIS IS RANGED, BUT B IS NOT:
		
		// Set the minimum value:
		result._value        = (double) _julian - d._julian;
		
		// Set the maximum value:
		result._highEndValue = (double) _highEndJulian - d._julian;
		
		// Check validity of the result:
		if(result._value < 0 || result._highEndValue <0 ){
			Warning(methodName,
			        "In the expression, %1$s - %2$s, the second date is greater than one or more of the dates in the first range and therefore the result has been set to missing.",
			        get().c_str(),
			        d.get().c_str()
			);
			return Number::MISSING;
		}
		
		return result;
		
	}
	
	// GET HERE IF B IS RANGED, BUT THIS IS NOT:
	
	// Set minimum:
	result._value        = (double) _julian - d._highEndJulian;
	
	// set the maximum:
	result._highEndValue = (double) _julian - d._julian;
	
	// Check validity of the result:
	if(result._value < 0 || result._highEndValue <0 ){
		Warning(methodName,
		        "In the expression, %1$s - %2$s, one or more dates in the second range is greater than the first date and therefore the result has been set to missing.",
		        get().c_str(),
		        d.get().c_str()
		);
		return Number::MISSING;
	}
	
	return result;
	
}

///
///  operator <
///
///  NOTA BENE: This is used for SORTING only
///
bool Date::operator<(const Data& b) const{
	
	//
	// If of differing data types, sort by the data type itself:
	//
	if(getDataType() != b.getDataType()) return getDataType() < b.getDataType();
	
	//
	// Get here if b is really a Date:
	//
	Date* pb = static_cast<Date*>(const_cast<Data* const>(&b));
	
	//
	// Sort MISSING values as LARGER than everything else:
	//
	if( _isMissing ) return false;
	if( pb->_isMissing ) return true;
	
	long d1,d2;
	//
	// For ranges, use the mean of the range for sorting:
	//
	if( _isRange ) d1 = (_julian + _highEndJulian)/2;
	else           d1 = _julian;
	
	if( pb->_isRange ) d2 = ( pb->_julian + pb->_highEndJulian)/2;
	else               d2 = pb->_julian;
	
	//
	// Even if d1==d2, We still
	// want to differentiate ranges that are different
	// even when the mean is the same:
	//
	if(d1==d2){
		if( _isRange && pb->_isRange ){
			//
			// Case where both are ranges: Sort so that
			// *smaller* ranges are first:
			//
			return (_highEndJulian - _julian) < (pb->_highEndJulian - pb->_julian);
			
		}else if( _isRange || pb->_isRange ){
			//
			// Case where only one is a range: Sort
			// so that discrete values appear before
			// ranges:
			//
			return pb->_isRange;
			
		}else{
			//
			// Case where neither is a range:
			//
			return false;
		}
	}
	
	return d1 < d2;
	
}

//
// Saved for future reference:
//
//bool Date::operator<(const Data& b) const{
//	
//	if(getDataType() != b.getDataType()) return getDataType() < b.getDataType();
//	Date* pb = static_cast<Date*>(const_cast<Data* const>(&b));
//	if( _isMissing || pb->_isMissing ) return false;
//	if( _isRange || pb->_isRange ){
//		if( _intersectionIsNotNull( *pb ) ) return false;
//		if(_isRange){
//			return _highEndJulian < pb->_julian;
//		}else{
//			return _julian < pb->_julian;
//		}
//	}
//	return _julian < pb->_julian;
//	
//}
//


///
/// operator ==
///
bool Date::operator==(const Data& b) const{
	Date* pb = dynamic_cast<Date*>(const_cast<Data* const>(&b));
	if(pb){
		if( _isMissing && pb->_isMissing ) return true;
		if( _isRange || pb->_isRange ) return _intersectionIsNotNull( *pb );
		return _julian == pb->_julian;
	}
	return false;
}


///
/// clone method:
///
Date* Date::clone() const {
	
	return new Date(*this);
	
}

///
/// This is a static method: Determines if the presented string
/// is a valid Date.
///
bool Date::isA(const std::string& s){
	
	Date test(s);
	// if test ends up missing, then it's not a Date
	// else it is a Date:
	return !test.isMissing();
	
}

//
// static method displayRangeMeans()
//
void Date::displayRangeMeans(bool displayThem){
	_displayRangeMeans = displayThem;
}

//
// static method displayDelimiters()
//
void Date::displayDelimiters(bool displayThem){
	_displayDelimiters = displayThem;
}


