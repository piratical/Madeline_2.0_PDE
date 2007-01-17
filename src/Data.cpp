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
// 2005.07.26.RK.ET
//

#include "Data.h"

///////////////////////////////////
//
// STATIC AND CONST STATIC MEMBERS:
//
///////////////////////////////////
std::set<std::string> Data::_missingValue;

// Used to suppress warnings when the "isA()" methods in derived 
// classes call "set()" methods to test whether strings conform 
// to patterns expected for that class:
//bool Data::_suppressWarnings=false;

DataMissingValueLoader DataMissingValueLoader::dataMissingValueLoader;

///////////////////////////////////
//
// STATIC METHODS:
//
///////////////////////////////////
void Data::addGlobalMissingValue(std::string value){
	
	_missingValue.insert(value);
	
}

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// isGlobalMissingValue
//
bool Data::isGlobalMissingValue(std::string value){
	
	std::set<std::string>::const_iterator iter;
	iter = _missingValue.find(Data::trim(value));
	if(iter != _missingValue.end()) return true;
	return false;
	
}

//
// NOTE BENE: trim takes a non-const *reference* to a string
//            and modifies the original string, and returns a
//            reference to the original string:
//
// Also note that the default "drop" string in Data.h contains
// numerous Unicode space characters like ZWSP, NBSP, ZWNBSP, 
// N SPACE, M SPACE, etc.:
//

std::string & Data::trim(std::string& s,const std::string& drop){
	
	// Trim left side:
	s.erase(0,s.find_first_not_of(drop));
	// Trim right side:
	s.erase(s.find_last_not_of(drop)+1);
	
	return s;
	
}

