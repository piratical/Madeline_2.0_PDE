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
// 2005.02.04.ET
//

#ifndef COLUMNCLASSIFIER_INCLUDED
#define COLUMNCLASSIFIER_INCLUDED

#include <string>
#include "DataTypes.h"

//
// Originally written as the column classifier for
// Madeline flat file tables, this class is now used
// to provide uniform column classification behaviour
// in the DataTable class.  The way the parameters
// of the scan method and other methods are set up
// reflects the original history of the class' use
// with the Madeline flat file tables.
// 
class ColumnClassifier{
	
private:
	
	static const char _THOUSANDS_SEPARATOR=',';
	static const char _DECIMAL_CHARACTER='.';
	
	int _missingOrDotCounter;
	int _numericCounter;
	int _dateCounter;
	int _genotypeCounter;
	int _genderCounter;
	int _totalCounter;
	
	const char *test;
	const char *_stt;
	const char *_end;
	
	
	bool _isEmptyOrDot(const char *p, int stt, int end);
	bool _isGender    (const char *p, int stt, int end);
	bool _isNumeric   (const char *p, int stt, int end);
	bool _isGenotype  (const char *p, int stt, int end);
	bool _isDate      (const char *p, int stt, int end);
	bool _isASCII(const char *p,int stt, int end);
	
public:
	
	ColumnClassifier(){ resetCounters(); }
	void scan( const char *p, int stt, int end);
	void scan( const char *s);
	void scan( const std::string &s);
	
	void resetCounters( void ){
		
		_missingOrDotCounter = _numericCounter = _dateCounter =0;
		_genotypeCounter = _genderCounter = _totalCounter = 0;
		
	}
	
	DATATYPE classify( void );
	
	
};

#endif
