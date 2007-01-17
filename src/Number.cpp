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
// 2005.03.10.RK.ET
//
// 2005.07.01.ET: Completed a number of revisions to handle 
//                ranged values correctly.
//


#include <math.h>
#include <limits.h>
#include "Number.h"
#include "DigitConverter.h"
#include "Warning.h"
#include "stdlib.h"
#include "string.h"


#include <iostream>

///////////////////////////////////
//
// STATIC AND CONST STATIC MEMBERS:
//
///////////////////////////////////

/// Value of PI
const double Number::PI          = M_PI;
/// Value of PI/2
const double Number::PI_OVER_TWO = M_PI_2;
/// Value of 3*PI/2:
const double Number::THREE_PI_OVER_TWO = M_PI+M_PI_2;
/// Value of 2*PI:
const double Number::TWO_PI      = M_PI*2.0;
/// Value of E
const double Number::E           = M_E;

/// Value of -PI/2:
const double Number::NEGATIVE_PI_OVER_TWO = -1*M_PI/2;
/// Value of -3PI/2:
const double Number::NEGATIVE_THREE_PI_OVER_TWO = -3*M_PI/2;

// Minimum Difference:
const double Number::MINIMUM_DIFFERENCE = 3E-8;

// Square Root of Two:
const double Number::SQRT_TWO = M_SQRT2;

// Missing Number
const Number Number::MISSING;

std::set<std::string> Number::_numberMissingValue;
///////////////////////////////////
//
// PRIVATE METHODS:
//
///////////////////////////////////

//
//
// Only call on ranges where there is a possibility
// of the low value > high value as the result of a
// mathematical operation:
//
void Number::_checkSwapLowAndHighValue( void ) {
	
	if( _value > _highEndValue){
		double temp=_highEndValue;
		_highEndValue = _value;
		_value        = temp;
	}
	
}


//
// _min():
//
//   Return the minimum of a, b, c, d :
//
inline double Number::_min(double a, double b, double c, double d) const{
	
	double min = (double) INT_MAX;
	if(a < min) min = a;
	if(b < min) min = b;
	if(c < min) min = c;
	if(d < min) min = d;
	return min;
	
}

//
// _max():
//
//   Return the maximum of a, b, c, d :
//
inline double Number::_max(double a, double b, double c, double d) const{
	
	double max = (double) INT_MIN;
	if(a > max) max = a;
	if(b > max) max = b;
	if(c > max) max = c;
	if(d > max) max = d;
	return max;
	
}

///////////////////////////////////
//
// STATIC METHODS:
//
///////////////////////////////////

///
/// isMissingValue() determines whether an input string is
/// present in the missing value lookup table and should therefore
/// be treated as a missing value.
///
bool Number::isMissingValue(std::string value){
	
	std::set<std::string>::const_iterator iter;
	iter = _numberMissingValue.find(Data::trim(value));
	if(iter != _numberMissingValue.end()) return true;
	return false;
	
}

//
// Default to not displaying range means:
//
bool Number::_displayRangeMeans=false;

//
// static method displayRangeMeans
//
void Number::displayRangeMeans(bool displayThem){
	_displayRangeMeans=displayThem;
}

//
// _differByLessThanMinimum(): Values are considered
// equal if they differ by less than MINIMUM_DIFFERENCE:
//
bool Number::_differByLessThanMinimum(const Number &b) const{
	
	if( _isMissing || b._isMissing ) return false;
	return (fabs( _value - b._value) <= Number::MINIMUM_DIFFERENCE);
	
}

//
// _intersectionIsNotNull(): is true if there is any overlap between
// the two number ranges
//
bool Number::_intersectionIsNotNull(const Number& b) const{
	
	if( _isMissing || b._isMissing ) return false;
	if( _isRange && b._isRange ) {
		if( b._highEndValue < _value || b._value > _highEndValue ||
		    _highEndValue < b._value || _value > b._highEndValue)
			return false;
		return true;    
	} 
	if( _isRange || b._isRange ){
		const Number* r;
		const Number* n;  
		if(_isRange){
			r = this;
			n = &b;
		}else{
			r = &b;
			n = this;
		}
		if( n->_value >= r->_value && n->_value <= r->_highEndValue ){
			return true;
		}
		return false;
	}
	//
	// We get here if both are numbers (neither is a range)
	//
	return _differByLessThanMinimum(b);
	
}

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

///
/// Number( const Number *n)
///
/// Copy construct a number from a const pointer to a number:
///
Number::Number( const Number *n){
	
	_isMissing      = n -> _isMissing;
	_value          = n -> _value;
	_highEndValue   = n -> _highEndValue;
	_isApproximate  = n -> _isApproximate;
	_isRange        = n -> _isRange;
	
}

///
/// setMissing(): set a Number to missing.
///
void Number::setMissing( void ){
	
	_isMissing = true;
	_isApproximate = _isRange = false;
	_highEndValue  = _value   = 0;
	
}


///
/// set() from const char *:
/// 
/// This method accepts : (1) An ordinary number string
///                       (2) An approximate number string indicated by using "~" as a prefix.
///                       (3) A range string following the pattern shown below:
///
/// Pattern: "  ~  [ +  123.456e+2  -  +  123.589e+2  ]"
///          "       -         E-   r  -         E-    "
///
void Number::set(const char *value){
	
	// for warnings:
	const char *methodName="Number::set(const char *)";
	
	if(Data::isGlobalMissingValue(value)) { setMissing(); return; }
	if(Number::isMissingValue(value)) {  setMissing(); return; }
	
	// buffer for parsing string:
	char buffer[SMALL_STRING_BUFFER_SIZE+1];
	char *s;
	char *firstNumber;
	char *secondNumber;
	
	//////////////////////
	// Starting defaults:
	//////////////////////
	bool startingBracketFound=false;
	bool rangeExpected=false;
	_isApproximate=false;
	_isRange=false;
	_isMissing=true;
	_value=0;
	_highEndValue=0;
	///////////////////////////////////////////
	// Copy DigitConverter-normalized string
	// to buffer for parsing
	///////////////////////////////////////////
	strncpy(buffer,DigitConverter(value).get().c_str(),SMALL_STRING_BUFFER_SIZE);
	buffer[SMALL_STRING_BUFFER_SIZE]='\0';
	s=buffer;
	
	// SKIP INITIAL WHITE SPACE:
	for(;*s && *s==' ';s++); if(!*s){ setMissing(); return; }
	
	///////////////////////////////////////////
	// Check for "~" indicating approximation:
	///////////////////////////////////////////
	if(*s=='~'){
		_isApproximate=true;
		s++;
	}
	// SKIP ANY ADDITIONAL WHITE SPACE:
	for(;*s && *s==' ';s++); if(!*s){ setMissing(); return; }
	
	///////////////////////////////////////////
	// Check for square starting bracket:
	///////////////////////////////////////////
	if(*s=='['){
		startingBracketFound=true;
		rangeExpected=true;
		s++;
	}
	// SKIP ANY ADDITIONAL WHITE SPACE:
	for(;*s && *s==' ';s++); if(!*s){ setMissing(); return; }
	
	//
	// Should be at the beginning of the FIRST NUMBER:
	// Set to MISSING if *s not a subset of [+,-,0-9]:
	//
	if(isdigit(*s) || *s=='-' || *s=='+'){
		firstNumber=s;
	}else{
		setMissing();
		Warning(methodName,"\"%s\" does not look like a number and has been set to missing.",value);
		return;
	}
	
	///////////////////////////////////////////////////////////
	// Skip past FIRST NUMBER, possibly in scientific notation:
	///////////////////////////////////////////////////////////
	if(*s && (*s=='+' || *s=='-')) s++;
	for(;*s && isdigit(*s); s++);
	if(*s && *s=='.') s++;
	for(;*s && isdigit(*s); s++);
	if(*s && (*s=='e' || *s=='E')){
		s++;
		// Only if the number is in scientific notation:
		if(*s && (*s=='+' || *s=='-')) s++;
		for(;*s && isdigit(*s); s++);
	}
	
	// SKIP ANY ADDITIONAL WHITE SPACE:
	for(;*s && *s==' ';s++); 
	// If we have reached the end of the string, then it is not a
	// range and we can convert it and are done:
	if(!*s){
		if(rangeExpected){
			setMissing();
			Warning(methodName,"\"%s\" does not specify a complete range. The number has been set to missing.",value);
			return;
		}else{
			_isMissing     = false;
			_isRange       = false;
			_value         = atof(firstNumber);
			return;
		}
	}
	
	/////////////////////////////////////////////////////////
	// If we get here, we need to be on RANGE INDICATOR:
	// one of "-", "r", "R", or "→" (Unicode right arrow in UTF-8):
	// 
	// Here we also convert the range indicator to NULL TERMINATOR
	// so we can safely convert the first number of the pair:
	/////////////////////////////////////////////////////////
	if(*s && (*s=='-' || *s=='r' || *s=='R')){
		rangeExpected=true;
		*s='\0';
		s++;
	}else if(*s && *s=='\xe2' && *(s+1) && *(s+1)=='\x86' && *(s+2) && *(s+2)=='\x92'){
		rangeExpected=true;
		*s='\0';
		s+=3;
	}else{
		setMissing();
		Warning(methodName,"\"%s\" does not specify a complete range. The number has been set to missing.",value);
		return;
	}
	
	// SKIP ANY ADDITIONAL WHITE SPACE AFTER THE RANGE INDICATOR:
	for(;*s && *s==' ';s++); 
	if(!*s){ 
		setMissing(); 
		Warning(methodName,"Range expected in \"%s\". The number has been set to missing.",value);
		return; 
	}
	
	//
	// Should be at the beginning of the SECOND NUMBER:
	// Set to MISSING if *s not a subset of [+,-,0-9]:
	//
	if(isdigit(*s) || *s=='-' || *s=='+'){
		secondNumber=s;
	}else{
		setMissing();
		Warning(methodName,"Missing second number in range \"%s\". The number has been set to missing.",value);
		return;
	}
	
	///////////////////////////////////////////////////////////
	// Skip past SECOND NUMBER, possibly in scientific notation:
	///////////////////////////////////////////////////////////
	if(*s && (*s=='+' || *s=='-')) s++;
	for(;*s && isdigit(*s); s++);
	if(*s && *s=='.') s++;
	for(;*s && isdigit(*s); s++);
	if(*s && (*s=='e' || *s=='E')){
		s++;
		if(*s && (*s=='+' || *s=='-')) s++;
		for(;*s && isdigit(*s); s++);
	}
	
	// SKIP ANY ADDITIONAL WHITE SPACE:
	for(;*s && *s==' ';s++);
	
	////////////////////////////////////////////////////////////
	// If opening bracket, then there must be a closing bracket:
	////////////////////////////////////////////////////////////
	if(startingBracketFound && *s!=']'){
		setMissing();
		Warning(methodName,"Closing bracket not found in \"%s\". The number has been set to missing.",value);
		return;
	}
	
	////////////////////////////////////////////////////////////
	// Get here if it is OK to convert the numbers in the range:
	////////////////////////////////////////////////////////////
	if(*s) *s='\0';
	_value         = atof(firstNumber);
	_highEndValue  = atof(secondNumber);
	
	if(_highEndValue <= _value){
		setMissing();
		Warning(methodName,"In \"%s\", the high value is less than or equal to the low value. The number has been set to missing.",value);
		return;
	}else{
		_isMissing     = false;
		_isApproximate = true;
		_isRange       = true;
	}
	
}




///
/// set() from std::string:
///
void Number::set(const std::string &value){
	
	set(value.c_str());
	
}

///
/// get(): return a std::string
///
//
// UNFINISHED: Test speed of sprintf() vs. ostringstream here.
//             If stringstream is too slow, use sprintf
//
const std::string Number::get( void ) const{
	
	std::ostringstream oss;
	
	if(_isMissing){
		return ".";
	}
	
	if(_isApproximate && !_isRange) oss << "~";
	
	if(_isRange){
		
		if(_displayRangeMeans){
			oss << "x̄=" << (0.5*(_value+_highEndValue)) << " ";
		}
		oss << "[" << _value << " → " << _highEndValue << "]";
		
	}else{
		
		oss << _value;
		
	}
	
	return oss.str();
	
}

///
/// set() from double:
///
void Number::set( double value ){ 
	
	_value=value; _highEndValue=0; _isMissing=false; _isApproximate=false; _isRange=false;
	if(Data::isGlobalMissingValue(get()) || Number::isMissingValue(get())){
		setMissing(); 
	}
	
}

///
/// operator+():
/// 
Number Number::operator+(const Number &b) const{ 
	
	if( _isMissing || b._isMissing ) return Number::MISSING;
	
	//
	// Initialize the return value assuming the default case
	// of scalars:
	//
	Number returnValue( _value + b._value );
	
	if(_isRange || b._isRange){
		
		returnValue._isRange=true;
		
		if(_isRange && b._isRange){
			
			returnValue._highEndValue = _highEndValue + b._highEndValue;
			
		}else if(_isRange){
			
			returnValue._highEndValue = _highEndValue + b._value;
			
		}else{
			
			returnValue._highEndValue = _value + b._highEndValue;
			
		}
		
	}
	
	if( _isApproximate || b._isApproximate ) returnValue._isApproximate = true;
	
	return returnValue;
	
}

///
/// operator+=():
/// 
Number & Number::operator+=(const Number &b){ 
	
	if( b._isMissing ) setMissing();
	if(   _isMissing ) return *this;
	
	//
	// Initialize the return value assuming the default case
	// of scalars:
	//
	_value += b._value;
	if( b._isApproximate ) _isApproximate = true;
	
	if(_isRange || b._isRange){
		
		_isRange=true;
		
		if(_isRange && b._isRange){
			
			_highEndValue += b._highEndValue;
			
		}else if(_isRange){
			
			_highEndValue += b._value;
			
		}else{
			
			_highEndValue = _value + b._highEndValue;
			
		}
		
	}
	
	return *this;
	
}

///
/// Prefix ++
///
Number& Number::operator ++(){
	
	if(!_isMissing){
		
		++_value;
		if(_isRange) ++_highEndValue;
		
	}
	
	return *this;
	
}


///
/// Postfix ++
///
Number Number::operator ++(int){
	
	// Save the return value:
	Number returnValue(this);
	
	if(!_isMissing){
		
		_value++;
		
		if(_isRange){
			
			_highEndValue++;
			
		}
		
	}
	
	return returnValue;
	
}

///
/// operator-():
/// 
Number Number::operator-(const Number &b) const{ 
	
	if( _isMissing || b._isMissing ) return Number::MISSING;
	
	// Handle simple case where both are scalers:
	if( !(_isRange || b._isRange)){
		
		// Construct from scalers:
		return Number( _value - b._value, _isApproximate || b._isApproximate );
		
	}
	//
	// Get here if one or both are ranges:
	//
	if( _isRange && b._isRange ){
		// BOTH ARE RANGED:
		
		// Set the minimum value:
		Number returnValue( _value - b._highEndValue , _isApproximate || b._isApproximate );
		// Toggle as a range:
		returnValue._isRange = true;
		// Set the maximum value:
		returnValue._highEndValue = _highEndValue - b._value;
		return returnValue;
		
	}
	// GET HERE IF ONLY ONE IS RANGED:
	
	if( _isRange){
		// THIS IS RANGED, BUT B IS NOT:
		// Set the minimum value:
		Number returnValue( _value - b._value , _isApproximate || b._isApproximate );
		// Toggle as a range:
		returnValue._isRange = true;
		// Set the maximum value:
		returnValue._highEndValue = _highEndValue - b._value;
		return returnValue;
		
	}
	
	// GET HERE IF B IS RANGED, BUT THIS IS NOT:
	
	// Set minimum:
	Number returnValue( _value - b._highEndValue, _isApproximate || b._isApproximate );
	// Toggle as a range:
	returnValue._isRange = true;
	// set the maximum:
	returnValue._highEndValue = _value - b._value;
	return returnValue;
	
}

///
/// operator-=():
///
Number& Number::operator-=(const Number &b){
	
	if( b._isMissing ) setMissing();
	if(   _isMissing ) return *this;
	
	if(b._isApproximate) _isApproximate = true;
	
	// Handle simple case where both are scalers:
	if( !(_isRange || b._isRange)){
		
		// Construct from scalers:
		_value -= b._value;
		return *this;
		
	}
	//
	// Get here if one or both are ranges:
	//
	if( _isRange && b._isRange ){
		// BOTH ARE RANGED:
		
		// Set the minimum value:
		_value -= b._highEndValue;
		// Toggle as a range:
		_isRange = true;
		// Set the maximum value:
		_highEndValue -= b._value;
		return *this;
		
	}
	// GET HERE IF ONLY ONE IS RANGED:
	
	
	// Set the maximum value:
	if( _isRange ){
		
		// THIS IS RANGED, BUT B IS NOT:
		// Set the minimum value:
		_value -= b._value;
		_highEndValue -= b._value;
		
		
	}else{
		// GET HERE IF B IS RANGED, BUT THIS IS NOT:
		// Toggle as a range:
		_isRange = true;
		_highEndValue =  _value;
		_value        -= b._highEndValue;
		_highEndValue -= b._value;
		
	}
	
	return *this;
	
}

///
/// Prefix --
///
Number& Number::operator --(){
	
	if(!_isMissing){
		--_value;
		if(_isRange) --_highEndValue;
	}
	return *this;
	
}

///
/// Postfix --
///
Number Number::operator --(int){
	
	
	// Save the return value:
	Number returnValue(this);
	
	if(!_isMissing){
		
		_value--;
		
		if(_isRange){
			
			_highEndValue--;
			
		}
		
	}
	
	return returnValue;
	
}


///
/// operator*():
///
Number Number::operator*(const Number &b) const{
	
	if(b._isMissing || _isMissing) return Number::MISSING;
	
	// Handle simple case where both numbers are scalars:
	if(!(_isRange || b._isRange)){
		
		return Number( _value * b._value, _isApproximate || b._isApproximate );
		
	}
	// Get here if one or both numbers are ranges:
	
	Number returnValue;
	returnValue._isMissing     = false;
	returnValue._isApproximate = true;
	returnValue._isRange       = true;
	
	if( _isRange && b._isRange ){
		// BOTH ARE RANGED:
		
		double e = ( _value * b._value );
		double f = ( _value * b._highEndValue );
		double g = ( _highEndValue * b._value );
		double h = ( _highEndValue * b._highEndValue );
		
		returnValue._value        = _min(e,f,g,h);
		returnValue._highEndValue = _max(e,f,g,h);
		return returnValue;
		
	}
	
	// One or the other number is ranged:
	
	returnValue._value = _value * b._value;
	
	if( _isRange ){
		// THIS IS RANGED:
		
		returnValue._highEndValue = _highEndValue * b._value;
		
	}else{
		// B IS RANGED:
		
		returnValue._highEndValue = _value * b._highEndValue;
		
	}
	// Check whether we need to swap:
	returnValue._checkSwapLowAndHighValue();
	return returnValue;
	
}

///
/// operator*=():
///
Number& Number::operator*=(const Number &b){
	
	if( b._isMissing ) setMissing();
	if(   _isMissing ) return *this;
	
	if(b._isApproximate) _isApproximate = true;
	
	// Handle simple case where both numbers are scalars:
	if(!(_isRange || b._isRange)){
		
		_value *= b._value;
		return *this;
		
	}
	// Get here if one or both numbers are ranges:
	
	_isRange       = true;
	
	if( _isRange && b._isRange ){
		// BOTH ARE RANGED:
		
		double e = ( _value * b._value );
		double f = ( _value * b._highEndValue );
		double g = ( _highEndValue * b._value );
		double h = ( _highEndValue * b._highEndValue );
		
		_value        = _min(e,f,g,h);
		_highEndValue = _max(e,f,g,h);
		return *this;
		
	}
	
	// One or the other number is ranged:
	
	_value *= b._value;
	
	if( _isRange ){
		// THIS IS RANGED:
		
		_highEndValue *= b._value;
		
	}else{
		// B IS RANGED:
		
		_highEndValue = _value * b._highEndValue;
		
	}
	// Check whether we need to swap:
	_checkSwapLowAndHighValue();
	
	return *this;
	
}

///
/// operator/():
///
Number Number::operator/(const Number &b) const{
	
	// for warnings:
	const char *methodName="Number::operator/(const Number &)";
	
	if(b._isMissing || _isMissing) return Number::MISSING;
	
	// Handle simple case where both numbers are scalars:
	if(!(_isRange || b._isRange)){
		
		// CHECK FOR DIVISION BY ZERO
		if(b._value == 0){
			if(_value == 0){
				Warning(methodName,"The result of %s/%s is undefined and has therefore been set to missing",
				get().c_str(),b.get().c_str());
			}else{
				Warning(methodName,"The result of %s/%s is %c∞ and has therefore been set to missing",
				get().c_str(),b.get().c_str(),_value>0?'+':'-');
			}
			return Number::MISSING;
		}
		
		return Number( _value / b._value, _isApproximate || b._isApproximate );
		
	}
	// Get here if one or both numbers are ranges:
	
	Number returnValue;
	returnValue._isMissing     = false;
	returnValue._isApproximate = true;
	returnValue._isRange       = true;
	
	if( _isRange && b._isRange ){
		// BOTH ARE RANGED:
		
		// CHECK FOR DIVISION BY ZERO:
		if(b._value<=0 && b._highEndValue>=0){
			if(_value<=0 && _highEndValue>=0){
				Warning(methodName,"The result of %s/%s contains 0/0 and has therefore been set to missing",
				get().c_str(),b.get().c_str());
			}else{
				Warning(methodName,"The result of %s/%s contains %s/0 and has therefore been set to missing",
				get().c_str(),b.get().c_str(),get().c_str());
			}
			return Number::MISSING;
		}
		
		double e = ( _value / b._value );
		double f = ( _value / b._highEndValue );
		double g = ( _highEndValue / b._value );
		double h = ( _highEndValue / b._highEndValue );
		
		returnValue._value        = _min(e,f,g,h);
		returnValue._highEndValue = _max(e,f,g,h);
		return returnValue;
		
	}
	
	// One or the other number is ranged:
	
	if( _isRange ){
		// THIS IS RANGED:
		
		// CHECK FOR DIVISION BY ZERO
		if(b._value == 0){
			if(_value<=0 && _highEndValue>=0){
				Warning(methodName,"The result of %s/0 is undefined and has therefore been set to missing",
				get().c_str());
			}else{
				Warning(methodName,"The result of %s/0 is %c∞ and has been set to missing",
				get().c_str(),_value>0?'+':'-');
			}
			return Number::MISSING;
		}
		
		returnValue._value        = _value        / b._value;
		returnValue._highEndValue = _highEndValue / b._value;
		
	}else{
		// B IS RANGED:
		
		// CHECK FOR DIVISION BY ZERO:
		if(b._value<=0 && b._highEndValue>=0){
			if(_value==0){
				Warning(methodName,"The result of %s/%s contains 0/0 and has therefore been set to missing",
				get().c_str(),b.get().c_str());
			}else{
				Warning(methodName,"The result of %s/%s contains %s/0 and has therefore been set to missing",
				get().c_str(),b.get().c_str(),get().c_str());
			}
			return Number::MISSING;
		}
		
		returnValue._value        = _value / b._value;
		returnValue._highEndValue = _value / b._highEndValue;
		
	}
	// Check whether we need to swap:
	returnValue._checkSwapLowAndHighValue();
	return returnValue;
	
}


///
/// operator/=():
///
Number& Number::operator/=(const Number &b){
	
	if( b._isMissing ) setMissing();
	if(   _isMissing ) return *this;
	
	// for warnings:
	const char *methodName="Number::operator/=(const Number &)";
	
	if(b._isApproximate) _isApproximate = true;
	
	// Handle simple case where both numbers are scalars:
	if(!(_isRange || b._isRange)){
		
		if(b._value == 0){
			if(_value == 0){
				Warning(methodName,"The result of 0/0 is undefined and has been set to missing");
			}else{
				Warning(methodName,"The result of %s/0 is %c∞ and has been set to missing",
				get().c_str(),_value>0?'+':'-');
			}
			setMissing();
			return *this;
		}
		
		_value /= b._value;
		return *this;
		
	}
	
	// Get here if one or both numbers are ranges:
	
	if( _isRange && b._isRange ){
		// BOTH ARE RANGED:
		
		// CHECK FOR DIVISION BY ZERO:
		if(b._value<=0 && b._highEndValue>=0){
			if(_value<=0 && _highEndValue>=0){
				Warning(methodName,"The result of %s/%s contains 0/0 and has therefore been set to missing",
				get().c_str(),b.get().c_str());
			}else{
				Warning(methodName,"The result of %s/%s contains %s/0 and has therefore been set to missing",
				get().c_str(),b.get().c_str(),get().c_str());
			}
			setMissing();
			return *this;
		}
		
		double e = ( _value / b._value );
		double f = ( _value / b._highEndValue );
		double g = ( _highEndValue / b._value );
		double h = ( _highEndValue / b._highEndValue );
		
		_value        = _min(e,f,g,h);
		_highEndValue = _max(e,f,g,h);
		return *this;
		
	}
	
	// ONE OR THE OTHER OF THE TWO NUMBERS IS RANGED:
	
	if( _isRange ){
		// THIS IS RANGED:
		
		// CHECK FOR DIVISION BY ZERO
		if(b._value == 0){
			if(_value<=0 && _highEndValue>=0){
				Warning(methodName,"The result of %s/0 is undefined and has therefore been set to missing",
				get().c_str());
			}else{
				Warning(methodName,"The result of %s/0 is %c∞ and has been set to missing",
				get().c_str(),_value>0?'+':'-');
			}
			setMissing();
			return *this;
		}
		
		_value        /= b._value;
		_highEndValue /= b._value;
		
	}else{
		// B IS RANGED:
		
		// CHECK FOR DIVISION BY ZERO:
		if(b._value<=0 && b._highEndValue>=0){
			if(_value==0){
				Warning(methodName,"The result of %s/%s contains 0/0 and has therefore been set to missing",
				get().c_str(),b.get().c_str());
			}else{
				Warning(methodName,"The result of %s/%s contains %s/0 and has therefore been set to missing",
				get().c_str(),b.get().c_str(),get().c_str());
			}
			setMissing();
			return *this;
		}
		
		_isRange       = true;
		// Here we must calculate _highEndValue first before _value is modified:
		_highEndValue  = _value / b._highEndValue;
		_value        /= b._value;
		
	}
	// Check whether we need to swap:
	_checkSwapLowAndHighValue();
	
	return *this;
	
}


///////////////////////////////////
//
// Trigonometric Methods:
//
///////////////////////////////////

//
// _sin()
//
inline double Number::_sin(double v) const{
	
	if(fabs(fmod( v , Number::PI )) <= Number::MINIMUM_DIFFERENCE ){
		return 0;
	}else{
		return sin( v );
	}
}

///
/// sin():
///
Number Number::getSin() const{
	
	// Check for missing:
	if(_isMissing) return Number::MISSING;
	
	Number returnValue;
	returnValue._isMissing     = false;
	returnValue._isApproximate = _isApproximate;
	returnValue._isRange       = _isRange;
	
	if(!_isRange){
		
		returnValue._value = _sin( _value );
		
	}else{
		//
		// CASE OF A RANGE:
		//
		double v1,v2,s1,s2;
		// Need to normalize to range [ 0 r 2*PI ] :
		v1 = _value;
		v2 = _highEndValue;
		if( v1 >= Number::TWO_PI ) v1 = fmod( v1 , Number::TWO_PI );
		if( v2 >= Number::TWO_PI ) v2 = fmod( v2 , Number::TWO_PI );
		
		// DEBUG:
		//std::cerr << "AFTER NORMALIZATION: " << v1 << " r " << v2 << std::endl;
		
		// Get sin of the endpoints:
		s1 = _sin( v1 );
		s2 = _sin( v2 );
		// Swap min, max if necessary:
		if( s2 < s1 ){
			double temp = s1;
			s1 = s2;
			s2 = temp;
		}
		// DEBUG:
		//std::cerr << "SIN OF ENDPOINTS: " << s1 << " r " << s2 << std::endl;
		
		// Correct minima, maxima if range contains pi/2, pi, 3pi/2, or 0:
		if( v1 < Number::PI_OVER_TWO       && v2 > Number::PI_OVER_TWO       && s2 < +1.0 ) s2 = +1.0;
		if( v1 < Number::THREE_PI_OVER_TWO && v2 > Number::THREE_PI_OVER_TWO && s1 > -1.0 ) s1 = -1.0;
		
		// Also must consider normalized negative ranges:
		if( v1 < NEGATIVE_PI_OVER_TWO       && v2 > NEGATIVE_PI_OVER_TWO       && s1 > -1.0 ) s1 = -1.0;
		if( v1 < NEGATIVE_THREE_PI_OVER_TWO && v2 > NEGATIVE_THREE_PI_OVER_TWO && s2 <  1.0 ) s2 = +1.0;
		
		// Assign:
		returnValue._value        = s1;
		returnValue._highEndValue = s2;
	}
	
	return returnValue;
	
}

///
/// getCos()
///
Number Number::getCos() const{
	
	Number returnValue(this);
	returnValue += Number::PI_OVER_TWO;
	return returnValue.getSin();
	
}

///
/// getTan():
///
Number Number::getTan() const{
		
	// for warnings:
	const char *methodName="Number::getTan()";
	
	// Check for missing:
	if(_isMissing) return Number::MISSING;
	
	Number returnValue;
	returnValue._isMissing     = false;
	returnValue._isApproximate = _isApproximate;
	returnValue._isRange       = _isRange;
	
	if(!_isRange){
		
		double val=fabs(_value);
		if(fmod(val,Number::PI_OVER_TWO) <= Number::MINIMUM_DIFFERENCE){
			if(fmod(val,Number::PI) <= Number::MINIMUM_DIFFERENCE){
				
				returnValue._value = 0;
				
			}else{
				Warning(methodName,"tan(%s) is %c∞ and has been set to missing",
					get().c_str(),
					_value<0?'-':'+'
				);
				return Number::MISSING;
			}
		}else{
			returnValue._value = tan( _value );
		}
		
	}else{
		//
		// CASE OF A RANGE:
		//
		double v1,v2,s1,s2;
		// Need to normalize to range [ 0 r 2*PI ] :
		v1 = _value;
		v2 = _highEndValue;
		if( v1 >= Number::TWO_PI ) v1 = fmod( v1 , Number::TWO_PI );
		if( v2 >= Number::TWO_PI ) v2 = fmod( v2 , Number::TWO_PI );
		
		// DEBUG:
		//std::cerr << "AFTER NORMALIZATION: " << v1 << " r " << v2 << std::endl;
		
		// :
		if( v1 <= Number::PI_OVER_TWO       && v2 >= Number::PI_OVER_TWO ){
			returnValue.setMissing();
			Warning(methodName,"The result of tan(%s) contains tan(Π/2) and has therefore been set to missing",get().c_str());
			return returnValue;
		}
		if( v1 <= Number::THREE_PI_OVER_TWO && v2 >= Number::THREE_PI_OVER_TWO ){
			returnValue.setMissing();
			Warning(methodName,"The result of tan(%s) contains tan(3Π/2) and has therefore been set to missing",get().c_str());
			return returnValue;
		}
		// Also must consider normalized negative ranges:
		if( v1 <= NEGATIVE_PI_OVER_TWO       && v2 >= NEGATIVE_PI_OVER_TWO ){
			returnValue.setMissing();
			Warning(methodName,"The result of tan(%s) contains tan(-Π/2) and has therefore been set to missing",get().c_str());
			return returnValue;
		}
		if( v1 <= NEGATIVE_THREE_PI_OVER_TWO && v2 >= NEGATIVE_THREE_PI_OVER_TWO ){
			returnValue.setMissing();
			Warning(methodName,"The result of tan(%s) contains tan(-3Π/2) and has therefore been set to missing",get().c_str());
			return returnValue;
		}
		
		// Get tan of the endpoints:
		if(fabs(fmod(v1,Number::PI_OVER_TWO)) <= Number::MINIMUM_DIFFERENCE ){
			Warning(methodName,"tan(%f) is %c∞ and has been set to missing",v1<0?'-':'+',v1);
			return Number::MISSING;
		}else{
			s1 = tan( v1 );
		}
		if(fabs(fmod(v2,Number::PI_OVER_TWO)) <= Number::MINIMUM_DIFFERENCE ){
			Warning(methodName,"tan(%f) is %c∞ and has been set to missing",v2<0?'-':'+',v2);
			return Number::MISSING;
		}else{
			s2 = tan( v2 );
		}
		// Swap min, max if necessary:
		if( s2 < s1 ){
			double temp = s1;
			s1 = s2;
			s2 = temp;
		}
		// DEBUG:
		//std::cerr << "TAN OF ENDPOINTS: " << s1 << " r " << s2 << std::endl;
		
		// Assign:
		returnValue._value        = s1;
		returnValue._highEndValue = s2;
	}
	
	return returnValue;
	
}



///
/// asin(): Arc sine 
///
Number Number::getAsin() const{
	
	// for warnings:
	const char *methodName="Number::getAsin()";
	
	if(_isMissing) return Number::MISSING;
	if(_value > 1 || _value < -1){
		Warning(methodName,"asin(%s) results in a domain error and has been set to missing",get().c_str());
		return Number::MISSING;
	}
	Number temp(asin(_value));
	if(_isRange){
		if(_highEndValue > 1 || _highEndValue < -1){
			Warning(methodName,"asin(%s) results in a domain error and has been set to missing",get().c_str());
			return Number::MISSING;
		}
		temp._highEndValue=asin(_highEndValue);
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// acos(): Arc cosine
///
Number Number::getAcos() const{
	
	// for warnings:
	const char *methodName="Number::getAcos()";
	
	if(_isMissing) return Number::MISSING;
	if(_value > 1 || _value < -1){
		Warning(methodName,"acos(%s) results in a domain error and has been set to missing",get().c_str());
		return Number::MISSING;
	}
	Number temp(acos(_value));
	if(_isRange){
		if(_highEndValue > 1 || _highEndValue < -1){
			Warning(methodName,"acos(%s) results in a domain error and has been set to missing",get().c_str());
			return Number::MISSING;
		}
		temp._highEndValue=acos(_highEndValue);
		temp._checkSwapLowAndHighValue();
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// atan(): Arc tangent
///
Number Number::getAtan() const{
	
	if(_isMissing) return Number::MISSING;
	Number temp(atan(_value));
	if(_isRange){
		temp._highEndValue=atan(_highEndValue);
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// sinh(): Hyperbolic sine
///
Number Number::getSinh() const{
	
	if(_isMissing) return Number::MISSING;
	Number temp(sinh(_value));
	if(_isRange){
		temp._highEndValue=sinh(_highEndValue);
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// cosh(): Hyperbolic cosine
///
Number Number::getCosh() const{
	
	if(_isMissing) return Number::MISSING;
	Number temp(cosh(_value));
	if(_isRange){
		
		temp._highEndValue=cosh(_highEndValue);
		temp._isRange=true;
		// check whether to swap low and high endpoints:
		temp._checkSwapLowAndHighValue();
		// Since the cosh(0)==1 and cosh(x)>=1 for all x,
		// if a range crosses zero, check whether the minimum 
		// is 1:
		if(_value <0 && _highEndValue >0 && temp._value>1 ) temp._value=1;
		
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// tanh(): Hyperbolic tangent
///
Number Number::getTanh() const{
	
	if(_isMissing) return Number::MISSING;
	Number temp(tanh(_value));
	if(_isRange){
		temp._highEndValue=tanh(_highEndValue);
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///////////////////////////////////
//
// Numeric Methods:
//
///////////////////////////////////

///
/// getFloor(): returns the floor of a Number
/// 
Number Number::getFloor() const{
	
	if(_isMissing) return Number::MISSING;
	Number temp(floor(_value));
	if(_isRange){
		temp._highEndValue=floor(_highEndValue);
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// getCeiling(): returns the ceiling of a Number
///
Number Number::getCeiling() const{
	
	if(_isMissing) return Number::MISSING;
	Number temp(ceil(_value));
	if(_isRange){
		temp._highEndValue=ceil(_highEndValue);
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// getRound(): rounds a Number
///
Number Number::getRound() const{
	
	if(_isMissing) return Number::MISSING;
	Number temp(round(_value));
	if(_isRange){
		temp._highEndValue=round(_highEndValue);
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// getExponent():
///
Number Number::getExponent() const{
	
	if(_isMissing) return Number::MISSING;
	Number temp(exp(_value));
	if(_isRange){
		temp._highEndValue=exp(_highEndValue);
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// getSquareRoot():
///
Number Number::getSquareRoot() const{
	
	// for warnings:
	const char *methodName="Number::getSquareRoot()";
	
	if(_isMissing) return Number::MISSING;
	if(_value < 0){
		Warning(methodName,"sqrt(%s) results in a domain error and has been set to missing",get().c_str());
		return Number::MISSING;
	}
	Number temp(sqrt(_value));
	if(_isRange){
		if(_highEndValue < 0){
			Warning(methodName,"sqrt(%s) results in a domain error and has been set to missing",get().c_str());
			return Number::MISSING;
		}
		temp._highEndValue=sqrt(_highEndValue);
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// getPower(): 
///
Number Number::getPower(const Number& e) const{
	
	// for warnings:
	const char *methodName="Number::getPower()";
	
	if(_isMissing || e._isMissing) return Number::MISSING;
	
	//
	// NOTE BENE: The error treatment in this method is verbose in order to
	//            achieve clarity in the Warning messages presented to the user:
	//            
	
	//
	// If the base is negative, then the exponent, e, must be integral in order
	// to get an answer in the domain of the real numbers.  This also means that
	// the exponent cannot be a range, because a range by definition contains non-integral
	// real values:
	//
	if(_value<0){
		// Exponent is a scalar but is not an integer:
		if(!e._isRange && (int)e._value != e._value){
			if(_isRange){
				// Base is a range:
				Warning(methodName,"The negative values in %s raised to the non-integral exponent %s result in a domain error and therefore the result has been set to missing.",
					get().c_str(),
					e.get().c_str()
				);
			}else{
				// Base is a scalar:
				Warning(methodName,"The negative scalar %s raised to the non-integral exponent %s results in a domain error and therefore the result has been set to missing.",
					get().c_str(),
					e.get().c_str()
				);
			}
			return Number::MISSING;
		}
		// Exponent is a range and thus non-integral:
		if(e._isRange && e._value<0 ){
			if(_isRange){
				// Base is a range:
				Warning(methodName,"The negative values in %s raised to the non-integral exponent values in %s results in a domain error and therefore the result has been set to missing.",
					get().c_str(),
					e.get().c_str()
				);
			}else{
				// Base is a scalar:
				Warning(methodName,"The negative scalar %s raised to the non-integral exponent values in %s results in a domain error and therefore the result has been set to missing.",
					get().c_str(),
					e.get().c_str()
				);
			}
			return Number::MISSING;
		}
	}
	//
	// If the base is zero, then the exponent
	// must be greater than or equal to zero:
	//
	if(_value==0 && !_isRange && e._value<0){
		if(e._isRange){
			// Exponent is a range:
			Warning(methodName,"The scalar 0 raised to negative exponent values in %s results in a domain error and therefore the result has been set to missing.",
				e.get().c_str()
			);
		}else{
			// Exponent is a scalar:
			Warning(methodName,"The scalar 0 raised to the negative exponent %s results in a domain error and therefore the result has been set to missing.",
				e.get().c_str()
			);
		}
		return Number::MISSING;
	}
	//
	// Alternatively, if the base is a range containing zero,
	// then the exponent must likewise be greater than or equal to zero:
	//
	if(_isRange && _value<=0 && _highEndValue>=0 && e._value<0){
		if(e._isRange){
			// Exponent is a range:
			Warning(methodName,"Negative values in the range %s raised to negative exponent values in %s result in a domain error and therefore the result has been set to missing.",
				get().c_str(),
				e.get().c_str()
			);
		}else{
			// Exponent is a scalar:
			Warning(methodName,"Negative values in the range %s raised to the negative exponent %s results in a domain error and therefore the result has been set to missing.",
				get().c_str(),
				e.get().c_str()
			);
		}
		return Number::MISSING;
	}
	
	//
	// Get here if it is OK to calculate the result:
	//
	
	Number result;
	result._isMissing     = false;
	result._isRange       = _isRange       || e._isRange;
	result._isApproximate = _isApproximate || e._isApproximate;
	
	if(_isRange && e._isRange){
		// 
		// BOTH base and exponent are ranged:
		//
		double r = pow( _value        , e._value        );
		double s = pow( _value        , e._highEndValue );
		double t = pow( _highEndValue , e._value        );
		double u = pow( _highEndValue , e._highEndValue );
		
		result._value        = _min(r,s,t,u);
		result._highEndValue = _max(r,s,t,u);
		return result;
	}
	//
	// Now check if either the base or the exponent, but not both, are ranged:
	//
	if(_isRange){
		// BASE is ranged:
		result._value        = pow( _value        , e._value );
		result._highEndValue = pow( _highEndValue , e._value );
		result._checkSwapLowAndHighValue();
		return result;
	}
	if(e._isRange){
		// Only EXPONENT is ranged:
		result._value        = pow( _value , e._value        );
		result._highEndValue = pow( _value , e._highEndValue );
		result._checkSwapLowAndHighValue();
		return result;
	}
	//
	// Get here if both are scalars:
	//
	result._value = pow( _value , e._value );
	return result;
	
}

///
/// abs(): returns the absolute value of a Number
///
Number Number::getAbsoluteValue() const{
	
	if(_isMissing) return Number::MISSING;
	Number temp(fabs(_value));
	if(_isRange){
		temp._highEndValue=fabs(_highEndValue);
		temp._isRange=true;
		temp._checkSwapLowAndHighValue();
		if(_value <0 && _highEndValue>0 && temp._value>0) temp._value=0;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// inv(): returns the inverse of a Number
///
Number Number::getInverse() const{
	
	// for warnings:
	const char *methodName="Number::getInverse()";
	
	if(_isMissing) return Number::MISSING;
	if(!_isRange && _value==0){
		Warning(methodName,"The inverse of zero is undefined and therefore the result has been set to missing.");
		return Number::MISSING;
	}
	if(_isRange && _value<=0 && _highEndValue>=0){
		Warning(methodName,"The inverse of the range %s contains 1/0 which is undefined and therefore the result has been set to missing.",get().c_str());
		return Number::MISSING;
	}
	// Get here if it is OK to calculate the result:
	Number temp(1.0/_value);
	if(_isRange){
		temp._highEndValue=1.0/_highEndValue;
		temp._isRange=true;
		temp._checkSwapLowAndHighValue();
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// log10(): logarithm base 10
///
Number Number::getLog10() const{
	
	// for warnings:
	const char *methodName="Number::getLog10()";
	
	if(_isMissing)  return  Number::MISSING;
	if(_value<=0){
		Warning(methodName,"The logarithm of %s results in a domain error and therefore has been set to missing.",get().c_str());
		return Number::MISSING;
	}
	Number temp(log10(_value));
	if(_isRange){
		temp._highEndValue=log10(_highEndValue);
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// log(): natural logarithm (base e)
///
Number Number::getLog() const{
	
	// for warnings:
	const char *methodName="Number::getLog()";
	
	if(_isMissing)  return  Number::MISSING;
	if(_value <= 0){
		Warning(methodName,"The logarithm of %s results in a domain error and therefore has been set to missing.",get().c_str());
		return  Number::MISSING;
	}
	Number temp(log(_value));
	if(_isRange){
		temp._highEndValue=log(_highEndValue);
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///////////////////////////////////
//
// Genetic Methods:
//
///////////////////////////////////

//
// _haldane_cM_To_Theta 
//
//    Converts a Haldane centiMorgan genetic map distance
//    to the recombination fraction, theta.
//
inline double Number::_haldane_cM_To_Theta(double m) const{
	
	return 0.5*(1.0-exp(-0.02*m));
	
}

//
// _theta_To_Haldane_cM
//
//    Converts a recombination fraction, theta, to a haldane
//    centiMorgan genetic map distance.
//
inline double Number::_theta_To_Haldane_cM(double r) const{
	
	return -50.0*(log(1.0-2.0*r));
	
}

//
// _kosambi_cM_To_Theta
//
//    Converts a Kosambi centiMorgan genetic map distance
//    to the recombination fraction, theta
//
inline double Number::_kosambi_cM_To_Theta(double m) const{
	
	register double four_m;
	register double n;
	
	// Remember that m is in centiMorgans, so we need to
	// convert to Morgans and then multiply by 4:
	four_m = 0.04*m;
	n      = exp( four_m );
	
	return 0.5*( n - 1.0 )/( n + 1.0 );
	
}

//
// _theta_To_Kosambi_cM
//
//    Converts a recombination fraction, theta, to a Kosambi
//    centiMorgan genetic map distance.
//
inline double Number::_theta_To_Kosambi_cM(double r) const{
	
	register double two_r;
	
	// Here again we multiply by 100 to get centiMorgans
	// instead of Morgans:
	two_r = 2.0*r;
	return 25.0*log( ( 1+two_r )/( 1-two_r ) );
	
}

//
// _lod
//
//    Converts a P-value to a LOD score.
//
inline double Number::_lod(double m) const{
	
	return -1.0*log10(m);
	
}

///
/// Lod(): returns a p-value's equivalent LOD score.
///
Number Number::getLod() const{
	
	// for warnings:
	const char *methodName="Number::getLod()";
	
	if(_isMissing)  return  Number::MISSING;
	if(_value <= 0){
		Warning(methodName,"The LOD of %s results in a domain error and therefore has been set to missing.",get().c_str());
		return  Number::MISSING;
	}
	Number temp( _lod( _value ) );
	if(_isRange){
		temp._highEndValue = _lod( _highEndValue );
		temp._isRange=true;
		temp._checkSwapLowAndHighValue();
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}


///
/// haldaneToTheta():
///
///   Converts a Haldane genetic distance in centiMorgans
///   to a recombination fraction.
///
Number Number::haldaneToTheta() const{
	
	// for warnings:
	const char *methodName="Number::haldaneToTheta()";
	
	if(_isMissing) return Number::MISSING;
	if(_value < 0){
		Warning(methodName,"haldaneToTheta(%s) results in a domain error and therefore the result has been set to missing.",get().c_str());
		return Number::MISSING;
	}
	Number temp( _haldane_cM_To_Theta( _value ) );
	if(_isRange){
		
		temp._highEndValue = _haldane_cM_To_Theta( _highEndValue );
		temp._isRange=true;
		
	}
	
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// thetaToHaldane(): converts recombination fraction to 
/// Haldane distance in cM.
///
Number Number::thetaToHaldane() const{
	
	// for warnings:
	const char *methodName="Number::thetaToHaldane()";
	
	if(_isMissing) return Number::MISSING;
	if( _value == 0.5 ){
		Warning(methodName,"thetaToHaldane(0.5) is ∞ and therefore the result has been set to missing.");
		return Number::MISSING;
	}else
	if( _value<0 || _value > 0.5 ){
		Warning(methodName,"thetaToHaldane(%s) results in a domain error and therefore the result has been set to missing.",get().c_str());
		return Number::MISSING;
	}
	
	Number temp( _theta_To_Haldane_cM( _value ) );
	
	if( _isRange ){
		if(_highEndValue == 0.5){
			Warning(methodName,"In the range %s, thetaToHaldane(0.5) is ∞ and therefore the result has been set to missing.",get().c_str());
			return Number::MISSING;
		}else if(_highEndValue > 0.5){
			Warning(methodName,"thetaToHaldane(%s) results in a domain error and therefore the result has been set to missing.",get().c_str());
			return Number::MISSING;
		}
		temp._highEndValue = _theta_To_Haldane_cM( _highEndValue );
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// kosambiToTheta(): converts Kosambi genetic distance in cM to 
/// recombination fraction.
///
Number Number::kosambiToTheta() const{
	
	// for warnings:
	const char *methodName="Number::kosambiToTheta()";
	
	if(_isMissing) return Number::MISSING;
	if(_value < 0){
		Warning(methodName,"kosambiToTheta(%s) results in a domain error and therefore the result has been set to missing.",get().c_str());
		return Number::MISSING;
	}
	Number temp( _kosambi_cM_To_Theta( _value ) );
	if(_isRange){
		
		temp._highEndValue = _kosambi_cM_To_Theta( _highEndValue );
		temp._isRange=true;
		
	}
	
	temp._isApproximate=_isApproximate;
	return temp;
	
}

///
/// thetaToKosambi(): converts a recombination fraction to
/// Kosambi genetic distance in cM.
///
Number Number::thetaToKosambi() const{
	
	// for warnings:
	const char *methodName="Number::thetaToKosambi()";
	
	if(_isMissing) return Number::MISSING;
	if( _value == 0.5 ){
		Warning(methodName,"thetaToKosambi(0.5) is ∞ and therefore the result has been set to missing.");
		return Number::MISSING;
	}else
	if( _value<0 || _value > 0.5 ){
		Warning(methodName,"thetaToKosambi(%s) results in a domain error and therefore the result has been set to missing.",get().c_str());
		return Number::MISSING;
	}
	
	Number temp( _theta_To_Kosambi_cM( _value ) );
	
	if( _isRange ){
		if(_highEndValue == 0.5){
			Warning(methodName,"In the range %s, thetaToKosambi(0.5) is ∞ and therefore the result has been set to missing.",get().c_str());
			return Number::MISSING;
		}else if(_highEndValue > 0.5){
			Warning(methodName,"thetaToKosambi(%s) results in a domain error and therefore the result has been set to missing.",get().c_str());
			return Number::MISSING;
		}
		temp._highEndValue = _theta_To_Kosambi_cM( _highEndValue );
		temp._isRange=true;
	}
	temp._isApproximate=_isApproximate;
	return temp;
	
}



///
/// operator ==
///
bool Number::operator==(const Data& b) const{
	
	Number* pb = dynamic_cast<Number*>(const_cast<Data* const>(&b));
	if(pb){
		if( _isMissing && pb->_isMissing ) return true;
		if( _isRange || pb->_isRange ) return _intersectionIsNotNull( *pb );
		return _differByLessThanMinimum( *pb );
	}
	return false;
}

///
/// operator !=
///
bool Number::operator!=(const Number& b) const{
	
	if( _isMissing && b._isMissing ) return false;
	if( _isRange || b._isRange ) return !_intersectionIsNotNull( b );
	return ! _differByLessThanMinimum( b );
	
}

///
/// operator <=
///
bool Number::operator<=(const Number& b) const{
	
	if( _isMissing && b._isMissing ) return true;
	if( _isMissing || b._isMissing ) return false;
	if( _isRange || b._isRange ){
		if(_intersectionIsNotNull( b )) return true;
		if(_isRange) return _highEndValue <= b._value;
		else         return _value <= b._value;
	}
	if( _differByLessThanMinimum( b ) ) return true;
	return ( _value <= b._value );
}

///
/// operator >=
///
bool Number::operator>=(const Number& b) const{
	
	if( _isMissing && b._isMissing ) return true;
	if( _isMissing || b._isMissing ) return false;
	if( _isRange || b._isRange ){
		if(_intersectionIsNotNull( b )) return true;
		if(b._isRange) return _value >= b._highEndValue;
		else return _value >= b._value;
	} 
	if( _differByLessThanMinimum( b )) return true;
	return ( _value >= b._value );
	
}

///
/// operator <
///
/// NOTA BENE: This is used for SORTING ONLY
///
bool Number::operator<(const Data& b) const {
	//
	// If of differing data types, sort by the data type itself:
	//
	if(getDataType() != b.getDataType()) return getDataType() < b.getDataType();
	
	//
	// Get here if b is really a Number:
	//
	Number* pb = static_cast<Number*>(const_cast<Data* const>(&b));
	
	//
	// Sort MISSING values as LARGER than everything else:
	//
	if( _isMissing ) return false;
	if( pb->_isMissing ) return true;
	
	double v1,v2;
	//
	// For ranges, use the mean of the range for sorting:
	// 
	if( _isRange )     v1 = 0.5*(_value+_highEndValue);
	else               v1 = _value;
	
	if( pb->_isRange ) v2 = 0.5*(pb->_value+pb->_highEndValue);
	else               v2 = pb->_value;
	
	//
	// Check if values differ by less than the minimum difference:
	//
	if( fabs( v1 - v2 ) <= Number::MINIMUM_DIFFERENCE ){
		//
		// For the purposes of sorting, however, we still
		// want to differentiate ranges that are different
		// even when the mean is the same:
		//
		if( _isRange && pb->_isRange ){
			//
			// Case where both are ranges: Sort so that
			// *smaller* ranges are first:
			//
			return (_highEndValue - _value) < (pb->_highEndValue - pb->_value);
			
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
	
	return v1 < v2;
	
}

///
/// BELOW SAVED FOR FUTURE REFERENCE:
/// operator <
///
//bool Number::operator<(const Data& b) const{
//	
//	if(getDataType() != b.getDataType()) return getDataType() < b.getDataType();
//	Number* pb = static_cast<Number*>(const_cast<Data* const>(&b));
//	//
//	// Sort MISSING values as larger than everything else:
//	//
//	if( _isMissing ) return false;
//	if( pb->_isMissing ) return true;
//	if( _isRange || pb->_isRange ){
//		if( _intersectionIsNotNull( *pb ) ) return false;
//		if(_isRange){
//			return _highEndValue < pb->_value;
//		}else{
//			return _value < pb->_value;
//		}
//	}else if( _differByLessThanMinimum( *pb )){
//		return false;
//	}
//	return _value < pb->_value;
//	
//}
//

///
/// operator >
///
bool Number::operator>(const Number& b) const{
	
	if( _isMissing || b._isMissing ) return false;
	if( _isRange || b._isRange ){
		if( _intersectionIsNotNull( b ) ) return false;
		if(b._isRange){
			return _value > b._highEndValue;
		}else{
			return _value > b._value;
		}
	}else if( _differByLessThanMinimum( b )){
		return false;
	}
	return _value > b._value;
	
}


///
/// Free function: ostream operator << for Numbers
///
std::ostream& operator<<(std::ostream& s, const Number& n){
	
	return s << n.get();
	
}

///
/// virtual copy constructor:
///
Number* Number::clone() const {
	
	return new Number(*this);
	
}

///
/// This is a static method: Determines whether an input string
/// fits the pattern for a Number.
///
bool Number::isA(const std::string& s){
	
	Number test(s);
	// if test ends up missing, then it's not a Number
	// else it is a Number:
	return !test.isMissing();
	
}


