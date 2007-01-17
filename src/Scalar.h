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
// 2005.07.17.RK.ET
//

#ifndef SCALAR_INCLUDED
#define SCALAR_INCLUDED

#include "Variable.h"
//! Scalar class implements Scalar variables.
/*! 
    Scalar variables are strongly typed. 
*/
class Scalar : public Variable{
private:
	//! Value of the scalar variable
	Data* _val;
	
public:
	//
	// Constructors/Destructors:
	//
	Scalar(const std::string & name) : Variable(name) { }
	Scalar(const std::string & name,const short valueType,const std::string & value) : Variable(name,valueType){ if(valueType != ANY) set(valueType,value);}
	
	~Scalar(){ 
		if(_valueType != ANY) delete _val;
	}
	// Copy Constructor:
	Scalar(const Scalar& s) : Variable(s._name,s._valueType),_val(s._val->clone()) { }
	
	//
	// Assignment Operator:
	//
	Scalar& operator= (const Scalar& s);
	//
	// Methods required by Variable virtual base class:
	//
	const std::string get( void ) const;
	const std::string getValueType() const;
	const std::string getVariableType() const { return "Scalar"; }
	
	//
	// Additional methods not present in the virtual base class:
	//
	void set(const short valueType,const std::string value);
	
	//
	// Friend functions:
	//
	friend bool operator==(const Scalar& a, const Scalar& b);
	friend bool operator<(const Scalar& a,const Scalar& b);
	
};

	std::ostream& operator<<(std::ostream& out,const Scalar& var);

#endif

