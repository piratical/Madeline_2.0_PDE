/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna, and Adrian Marrs
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


//! The Number class handles numeric values, ranges of numeric values, and missing values.

///
/// The Number class handles exact numeric values, approximate numeric values, and
/// missing numeric values.  Approximate numeric values can be of two types: numeric
/// ranges and approximate unranged numeric values.  Of these, numeric ranges are 
/// preferred because they provide inclusive lower and upper bounds on a measurement.  
/// (For example, it is more informative if we can say that a patient's fasting blood 
/// glucose level is "80-87" g/dl than if we merely say it is "approximately" "~85").
///
/// Note that the constructor handles non-ASCII digits seamlessly: See the DigitConverter
/// class for details.
/// 
/// Unranged approximate values are indicated by a "~" prefix. Ranged values are
/// indicated by a dash "-", "r", "R" or arrow separating the inclusive lower and
/// upper bounds.
/// 
/// In addition to standard mathematical and trigonometric methods,
/// some methods useful in linkage analysis, such as obtaining the LOD score 
/// from a p-value or converting haldane centiMorgans to a recombination fraction 
/// are also provided.
/// 
#ifndef NUMBER_INCLUDED
#define NUMBER_INCLUDED

#include <string>
#include <iostream>
#include <sstream>
#include "Data.h"

class Number : public Data{
private:

	static std::set<std::string> _numberMissingValue;
	
	static bool _displayRangeMeans;
	
	//
	// Private Data:
	//
	bool _isMissing;
	double _value;
	double _highEndValue;
	bool _isApproximate;
	bool _isRange;
	//
	// Private Methods:
	//
	void _checkSwapLowAndHighValue( void );
	inline double _min(double a, double b, double c, double d) const;
	inline double _max(double a, double b, double c, double d) const;
	inline double _sin(double v) const;
	inline double _haldane_cM_To_Theta(double ) const;
	inline double _theta_To_Haldane_cM(double ) const;
	inline double _kosambi_cM_To_Theta(double ) const;
	inline double _theta_To_Kosambi_cM(double ) const;
	inline double _lod(double ) const;
	bool _differByLessThanMinimum(const Number &b) const;
	bool _intersectionIsNotNull(const Number& test) const;
	
public:
	
	friend class Date;
	//
	// Static methods:
	//
	static void addNumberMissingValue(std::string value){ _numberMissingValue.insert(value); }
	static bool isMissingValue(std::string value);
	static void displayRangeMeans(bool);
	//
	// Public constants:
	//
	const static double PI;
	const static double PI_OVER_TWO;
	const static double THREE_PI_OVER_TWO;
	const static double TWO_PI;
	const static double E;
	
	const static double NEGATIVE_PI_OVER_TWO;
	const static double NEGATIVE_THREE_PI_OVER_TWO;
	
	const static double SQRT_TWO;
	
	const static Number MISSING;
	
	//
	// Values which differ by less than _MINIMUM_DIFFERENCE are treated as
	// equal:
	//
	const static double MINIMUM_DIFFERENCE;
	
	//
	// Constructors:
	//
	
	/// Default constructor:
	Number(){ _isMissing=true; _value=0; _highEndValue=0;_isApproximate=false; _isRange=false; }
	/// Constructor for a scaler precise or approximate value:
	Number( double value , bool isApproximate = false ){ 
		set(value); _isApproximate = isApproximate; 
	}
	// Construct from char * or std::string:
	Number( const char *value ){ set(value); }
	Number( const std::string &value ){ set(value.c_str()); }
	// Copy construct from a pointer to Number:
	Number( const Number *);
	
	//  Methods required by Data virtual base class:
	bool isMissing( void ) const { return _isMissing; }
	void setMissing( void );
	void set( const std::string &value);
	void set( const char *value );
	const std::string get( void ) const;
	
	//
	// Additional Setters not present in virtual base class:
	//
	void set( double value );
	bool isApproximate( void ) const { return _isApproximate; }
	
	
	//
	// Operators:
	// 
	Number operator+( const Number &b ) const;
	Number& operator+=( const Number &b );
	Number& operator++();
	Number operator++( int );
	Number operator-( const Number &b ) const;
	Number& operator-=( const Number &b );
	Number& operator--();
	Number operator--( int );
	Number operator*( const Number &b ) const;
	Number& operator*=( const Number &b );
	Number operator/( const Number &b ) const;
	Number& operator/=( const Number &b );
	//
	// Trigonometric Functions
	//
	Number getSin() const;
	Number getCos() const;
	Number getTan() const;
	Number getAsin() const;
	Number getAcos() const;
	Number getAtan() const;
	Number getSinh() const;
	Number getCosh() const;
	Number getTanh() const;
	//
	// Numeric functions:
	//
	Number getFloor() const;
	Number getCeiling() const;
	Number getRound() const;
	Number getExponent() const;
	Number getSquareRoot() const;
	Number getPower(const Number& b) const;
	Number getAbsoluteValue() const;
	Number getInverse() const;
	Number getLog10() const;
	Number getLog() const;
	//
	// Genetic functions:
	//
	Number getLod() const;
	Number haldaneToTheta() const;
	Number thetaToHaldane() const;
	Number kosambiToTheta() const;
	Number thetaToKosambi() const;
	//
	// Operators:
	//
	bool operator==(const Data& b) const; 
	bool operator<(const Data& b) const;
	bool operator<=(const Number& b) const;
	bool operator>=(const Number& b) const;
	bool operator!=(const Number& b) const;
	bool operator>(const Number& b) const;
	
	// ADDED: on 2005-07-15
	const DATATYPE getDataType( void ) const { return NUMBER; }
	virtual Number* clone() const;
	
	// ADDED: on 2005-09-09
	// Determines whether a string is a valid Number string
	static bool isA(const std::string& s);
	
};

// ostream insertion << operator for Numbers:
std::ostream& operator<<(std::ostream& s,const Number& n);

#endif

