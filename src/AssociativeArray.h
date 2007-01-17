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
#ifndef ASSOCIATIVEARRAY_INCLUDED
#define ASSOCIATIVEARRAY_INCLUDED

#include "Variable.h"
#include <map>


struct compare {
	
	bool operator() (Data* a, Data* b) const {
		return *a < *b;
	}
	
};

//! AssociativeArray implements Associative Array variables
/*! Associative Array variables are strongly typed. Key values cannot be set to
    missing. Ranges and Approximations are not allowed as key values. 
    Keys are sorted alphanumerically depending on the Data type.
*/ 
class AssociativeArray : public Variable{
private:
	std::map<Data*,Data*,compare> _map; /*!< Stores key-value pairs */
	short _keyType;                     /*!< Data type of the key */
	
	bool _keyIsValid(Data*,const std::string key);
public:
	//
	// Constructors/Destructors:
	//
	AssociativeArray(const std::string & name,const short keyType,const short valueType) : Variable(name,valueType), _keyType(keyType){ }
	
	~AssociativeArray(){ 
		std::map<Data*,Data*,compare>::const_iterator p;
		p=_map.begin();
		while(p != _map.end()){
			delete (p->first);
			delete (p->second);
			p++;
		}
		_map.clear();
	}
	
	// Copy Constructor:
	AssociativeArray(const AssociativeArray& s) : Variable(s._name,s._valueType),_keyType(s._keyType) {
		std::map<Data*,Data*,compare>::const_iterator p;
		p = s._map.begin();
		while(p != s._map.end()){
			_map.insert(std::pair<Data*,Data*>(p->first->clone(),p->second->clone()));
			p++;
		}
	}
	//
	// Assignment Operator:
	//
	AssociativeArray& operator= (const AssociativeArray& s);
	//
	//  Methods required by Variable virtual base class:
	//
	const std::string get( void ) const;
	const std::string getValueType() const;
	const std::string getVariableType() const { return "AssociativeArray"; }
	//
	// Additional methods not present in the virtual base class:
	//
	void insert(const short keyType,const std::string key,const short valueType,const std::string value);
	const std::string getKeyType() const;
	//
	// Friend functions:
	//
	friend bool operator==(const AssociativeArray& a, const AssociativeArray& b);
};

	std::ostream& operator<< (std::ostream& out, const AssociativeArray& var);

#endif
