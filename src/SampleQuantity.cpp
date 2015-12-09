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
// 2015.12.07.ET
//

//
// SampleQuantity.cpp
//
#include "SampleQuantity.h"
#include "stdlib.h"  // for strtod()
#include "Warning.h"

std::map<std::string,bool> SampleQuantity::_lookupTable;
SampleQuantityMapLoader SampleQuantityMapLoader::sampleQuantityMapLoader;
//
// _set:
//
void SampleQuantity::_set(void){
	// Trim the value:
	Data::trim(_value);
	// Check for missing:
	if(Data::isGlobalMissingValue(_value)){
		// Normalize the missing value
		_value        = ".";  
		_isMissing    = true;
		_booleanValue = false;
		_doubleValue  = 0.0;
	}else if(_value[0]>='0' && _value[0]<='9'){
		// Starts with a digit:
		char *pEnd;
		_doubleValue = strtod(_value.c_str(),&pEnd);
		// Restrict range of allowed value to fraction between 0 to 1 inclusive:
		if(_doubleValue<0.0 || _doubleValue>1.0){
			Warning("SampleQuantity::_set()","Sample quantity “%.3g” is not a fraction between 0 and 1 inclusive and will be set to 0.",_doubleValue);
			_doubleValue=0;
		}
		_isMissing    = false;
		_booleanValue = true;
	}else{
		// Anything else sets booleanValue to true but doubleValue still zero:
		std::map<std::string,bool>::const_iterator iter = _lookupTable.find(_value);
		if(iter != _lookupTable.end()){
			_booleanValue = iter->second;
		}else{
			Warning("SampleQuantity::_set()","Sample quantity marker “%s” is unrecognized and will be interpreted as FALSE.",_value.c_str());
			_booleanValue = false;
		}
		_isMissing    = false;
		_doubleValue  = 0;
	}
}


