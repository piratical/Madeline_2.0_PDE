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
// 2007.04.26.ET
//

//
// Carrier.cpp
//

#include "Carrier.h"

///////////////////////////////////
//
// STATIC AND CONST STATIC MEMBERS:
//
///////////////////////////////////

std::map<std::string,bool> Carrier::_lookupTable;
CarrierMapLoader CarrierMapLoader::carrierMapLoader;

///////////////////////////////////
//
// STATIC METHODS:
//
///////////////////////////////////

void Carrier::addLookupTableEntry(std::string label, bool value ){
	
	_lookupTable.insert(std::pair<std::string,bool>(label,value));
	
}

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// set(): from char*
//
void Carrier::set(const char *value){
	
	_isMissing = true;
	std::map<std::string,bool>::const_iterator iter = _lookupTable.find(value);
	if(iter != _lookupTable.end()){
		_value = iter->second;
		_isMissing = false;
	}
	
}

//
// set(): from string
//
void Carrier::set(const std::string &value){
	
	set( value.c_str() );
	
}

//
// set(): from Enum
//
void Carrier::set(CARRIER carrier){
	
	if(carrier==Carrier::MISSING) _isMissing=true; 
	else{
		_isMissing = false;
		if(carrier==Carrier::IS_CARRIER) _value = true;
		else                             _value = false; 
	}
	
}



