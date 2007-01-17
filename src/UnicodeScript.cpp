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

#include "UnicodeScript.h"

// DEBUG:
#include <iostream>

//
// Constructor 
//
UnicodeScript::UnicodeScript(){
	
	_scriptCode  = NO_SCRIPT_CODE;
	
}



//
// Constructor:
//
UnicodeScript::UnicodeScript(SCRIPTCODE scriptCode,const std::string englishName,const std::string nativeName,const std::string sampleFontMetricsString){
	
	_scriptCode  = scriptCode;
	_englishName = englishName;
	_nativeName  = nativeName;
	_sampleFontMetricsString = sampleFontMetricsString;
	
}

std::string UnicodeScript::getSample(void) const {
	
	return _sampleFontMetricsString;
	
}

std::string UnicodeScript::getEnglishName(void) const {
	
	return _englishName;
	
}

std::string UnicodeScript::getNativeName(void) const {
	
	return _nativeName;
	
}


