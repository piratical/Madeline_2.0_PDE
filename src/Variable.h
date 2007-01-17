/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager,Ritu Khanna and Adrian Marrs
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
#ifndef VARIABLE_INCLUDED
#define VARIABLE_INCLUDED

#include <iostream>

#include "Data.h"
#include "Date.h"
#include "Genotype.h"
#include "Number.h"
#include "String.h"
#include "Boolean.h"
#include "Warning.h"
//! Variable class implements variables
/*! Variable is a virtual base class. Scalar and AssociativeArray classes are derived from it.
*/
class Variable{
	
protected:
	std::string _name; /*!< The name of a variable. */
	short _valueType;  /*!< The value type of a variable. */
	
public:
	//
	// Constructors/Destructors:
	//
	Variable(const std::string & name) : _name(name),_valueType(ANY) { }
	Variable(const std::string & name,const short valueType) : _name(name),_valueType(valueType) { }
	virtual ~Variable() { }
	
	const std::string getName() const { return _name; }
	// Get a display (string) version of the Variable:
	virtual const std::string get( void ) const=0;
	virtual const std::string getValueType() const=0; 
	virtual const std::string getVariableType() const=0;
	
};

#endif
