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
// 2014.07.01.ET
//

//
// Pregnancy.cpp
//

#include "Pregnancy.h"

///////////////////////////////////
//
// STATIC AND CONST STATIC MEMBERS:
//
///////////////////////////////////

std::map<std::string,bool> Pregnancy::_lookupTable;
PregnancyMapLoader PregnancyMapLoader::pregnancyMapLoader;

///////////////////////////////////
//
// STATIC METHODS:
//
///////////////////////////////////

void Pregnancy::addLookupTableEntry(std::string label, bool value ){
	
	_lookupTable.insert(std::pair<std::string,bool>(label,value));
	
}

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

void Pregnancy::set(const char *value){
	
	_isMissing = true;
	std::map<std::string,bool>::const_iterator iter = _lookupTable.find(value);
	if(iter != _lookupTable.end()){
		_value = iter->second;
		_isMissing = false;
	}
	
}

void Pregnancy::set(const std::string &value){
	
	set( value.c_str() );
	
}

void Pregnancy::set(PREGNANCY pregnancy){
	
	if(pregnancy==Pregnancy::MISSING) _isMissing=true; 
	else{
		_isMissing = false;
		if(pregnancy==Pregnancy::IS_PREGNANCY) _value = true;
		else                             _value = false; 
	}
	
}



