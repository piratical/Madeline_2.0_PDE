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
// 2005.03.27.ET
//

//
// Date.h
//

///
/// The Date class provides support for exact dates, approximate dates, and 
/// missing dates.  Approximate dates can be either unranged approximations
/// or, more common, ranged approximations.  For example, the date string 
/// "1963" will be interpreted by the class constructor as a ranged date spanning
/// from January 1, 1963 to December 31, 1963 inclusive. The date string " 1994-07" will
/// be interpreted as a ranged date spanning July 01, 1994 to July 31, 1994 inclusive.
/// In contrast, "1987-05-12" would be interpreted as an exact date and
/// "2005-02-29" as a missing date (because there were only 28 days in February of
/// 2005).
/// 
/// Dates are stored internally as Julian day numbers. Dates are expected to be input 
/// in ISO "YYYY-MM-DD" format.  Note however that the constructor handles non-ASCII 
/// digits seamlessly: See the DigitConverter class for details.
/// 
/// For dates prior to the establishment of the Gregorian calendar in 1582, a proleptic
/// calendar is used.
/// 
/// 2005.09.19 TO-DO: ED, DECIDE WHAT TO DO ABOUT NON-GREGORIAN (BUDDHIST,HIJRI) INPUT 
/// AND OUTPUT IN THE DATE CLASS.
/// 

#ifndef DATE_INCLUDED
#define DATE_INCLUDED

#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "Data.h"
#include "Number.h"

//
// Date Class:
//
class Date : public Data {
public:
	
	//
	// We use BUDDHIST_GREGORIAN here to emphasize that this
	// is not the lunar calendar but just the Gregorian with
	// years counted using the Buddhist Era convention:
	// 
	enum CALENDAR{ GREGORIAN, BUDDHIST_GREGORIAN, HIJRI };
	
	const static Date MISSING;
	
private:
	
	const static int _daysInMonth[12];
	
	static bool _displayRangeMeans;
	static bool _displayDelimiters;
	
	// Private constants needed by algorithms:
	const static long _IGREG=(15+31L*(10+12L*1582)); // Gregorian calendar adopted Oct. 15, 1582 
	const static long _IGREGJULIAN = 2299161;        // Julian day number of Oct. 15m 1582
	//
	// The start (day 1 of Muharram, 1 AH) of the Hijri Islamic calendar  was 
	// Friday July 16, 622 CE, julianDay=1,948,440.  
	// As the calendar depends on Lunar sightings by a human (rather than 
	// astronomical calculations), it can go up or down a day.
	// By using two days before here, the algorithm based on Reingold and 
	// Dershowitz (see reference below) gives the correct dates and days of 
	// week for:
	// 
	//   Friday   0632.03.06 CE == 0010.12.09 AH
	//   Tuesday  1992.12.08 CE == 1413.06.13 AH
	//   Saturday 2005.03.26 CE == 1426.02.16 AH
	//   
	const static long _IHIJRIJULIAN = 1948438;
	
	//
	// Length of date string in yyyy.mm.dd or dd.mm.yyyy format
	// or in ~-yyyy.mm.dd - yyyy.mm.dd format, plus some extra for
	// good measure:
	// 
	// -> _DATESTRINGLENGTH=72 would allow, for example, a date string
	//   like "二〇〇五-〇五-一七 -  二〇〇五-一〇-三〇" in UTF-8:
	// 
	const static unsigned int _DATESTRINGLENGTH=72;  
	const static double _DAYSINYEAR;      // Current (in year 2005) average no. of days per year
	static std::set<std::string> _dateMissingValue;
	
	// Private data:
	bool     _isMissing;                             // Boolean for missing
	bool     _isApproximate;
	bool     _isRange;
	long     _julian;                                // julian day number
	long     _highEndJulian;
	
	// STATIC MEMBER:
	static CALENDAR _calendarDisplayType;            // Default calendar
	
	//////////////////////
	//
	// Private methods:
	// 
	/////////////////////
	
	//
	// Converts Gregorian civil year yyyy, month mm, day dd 
	// to Julian day number:
	//
	long _yearMonthDayToJulian(int yyyy,int mm, int dd);
	
	//
	// Determines if a Gregorian calendar year is a leap year: 
	//
	bool _isLeapYear(int year);
	// 
	// Returns last day of a month.
	//
	int _getLastDayOfMonth(int year, int month);
	//
	// Obtain the Gregorian civil year yyyy, month mm, and day dd:
	//
	void _getYearMonthDay(const long julian, int *yyyy,int *mm,int *dd) const;
	
	//////////////////////////
	//
	// Islamic calendar stuff:
	//
	// Reference: 
	// "Calendrical Calculations'' by Nachum Dershowitz and
	// Edward M. Reingold, Software---Practice & Experience,
	// vol. 20, no. 9 (September, 1990), pp. 899--928.
	//
	//////////////////////////
	
	//
	// Obtain the Julian day number from an Islamic Hijri date:
	//
	long _islamicYearMonthDayToJulian( int yyyy, int mm, int dd) const ;
	
	//
	// Returns true if it is an Islamic Hijri leap year:
	//
	bool _isIslamicLeapYear(int year) const;
	
	//
	// Returns the number of days in the Islamic month:
	//
	int _lastDayOfIslamicMonth(int year, int month) const ;
	
	//
	// _getIslamicYearMonthDay
	//
	// --> Only use if date is > IHIJRIJULIAN (start of Islamic calendar)
	//
	void _getIslamicYearMonthDay(const long julian, int *yyyy, int *mm, int *dd) const;
	
	bool _intersectionIsNotNull(const Date& b) const;
	
	///////////////////////////////////
	//
	// New Date range parsing methods:
	//
	///////////////////////////////////
	///////////////////
	//
	// Level 0 Methods:
	//
	///////////////////
	bool _hasASCIIDateDelimiter(const char **position);
	bool _hasApproximationIndicator(const char **position);
	bool _hasCJKCharacterForYear(const char **position);
	bool _hasCJKCharacterForMonth(const char **position);
	bool _hasCJKCharacterForDay(const char **position);
	bool _hasCJKCharacterForArrive(const char **position);
	bool _hasCJKCharacterForFullStop(const char **position);
	bool _hasCJKCharacterForForwardSlash(const char **position);
	bool _hasCJKCharacterForDash(const char **position);
	bool _hasCJKCharacterForOpeningSquareBracket(const char **position);
	bool _hasCJKCharacterForClosingSquareBracket(const char **position);
	bool _hasUnicodeRightArrow(const char **position);
	bool _hasKoreanHangulForYear(const char **position);
	bool _hasKoreanHangulForMonth(const char **position);
	bool _hasKoreanHangulForDay(const char **position);
	//
	// Use this for any additional characters:
	//
	bool _hasUTF8Character(const char **position,const char *UTF8Character);
	///////////////////
	//
	// Level 1 Methods:
	//
	///////////////////
	bool _hasRangeDelimiter(const char **position,bool startBracketFound);
	bool _hasCJKVersionOfGenericDateDelimiter(const char **position);
	bool _hasYearDelimiter(const char **position, bool *isProperCJKDate);
	bool _hasMonthDelimiter(const char **position);
	bool _hasDayDelimiter(const char **position);
	bool _hasOpeningBracket(const char **position);
	bool _hasClosingBracket(const char **position);
	int _getValue(const char **position);
	void _skipWhiteSpace(const char **position);
	
	///////////////////
	//
	// Utility Methods:
	//
	///////////////////
	// void _warning(const char *message);
	// void _showDate(int year, int month, int day);
	
	///////////////////
	//
	// Level 2 Methods:
	//
	///////////////////
	void _readDateString(const char *s);
	
	/////////////////////////////////////
	//
	// REVISED AND ADDED PRIVATE METHODS:
	//
	/////////////////////////////////////
	bool _yearIsValid(int year , const char *dateString );
	bool _monthIsValid(int month , const char *dateString );
	bool _dayIsValid(int year, int month, int day , const char *dateString );

public:
	//
	// Static methods:
	//
	static void addDateMissingValue(std::string value){ _dateMissingValue.insert(value); }
	static bool isMissingValue(std::string value);
	// Constructors:
	Date() { setMissing(); }           // defaults to missing
	Date(const char *d ) { set(d); }
	Date(const std::string &d){ set(d.c_str()); }
	Date(const Date *d);
	
	// Methods required by virtual base class Variable:
	bool isMissing( void ) const;
	void setMissing( void );
	void set(const char *dateString);
	void set(const std::string &dateString);
	const std::string get( void ) const;
	
	// Operators:
	Date& operator++();
	Date operator++( int );
	
	Date& operator--();
	Date operator--( int );
	
	Date& operator+=( const Number & b );
	Date& operator-=( const Number & b );
	
	Date operator+( const Number & b ) const;
	Date operator-( const Number & b ) const;
	Number operator-( const Date & b ) const;
	//
	// ADDED: on 2005-07-19
	// 
	bool operator<(const Data& b) const;
	bool operator==(const Data& b) const;
	// ADDED: on 2005-07-15
	bool isApproximate( void ) const { return _isApproximate; }
	const DATATYPE getDataType( void ) const { return DATE; } 
	virtual Date* clone() const;
	
	// ADDED: on 2005-09-09
	// Determines whether a string is a valid Date string
	static bool isA(const std::string& s);
	static double getDaysInYear(void) { return _DAYSINYEAR; }
	
	// ADDED on 2006.11.30:
	static void displayRangeMeans(bool displayThem);
	// ADDED on 2007.02.10:
	static void displayDelimiters(bool displayThem); 
};

//
// Free Functions:
//
std::ostream& operator<<(std::ostream& s,const Date& d);

#endif

