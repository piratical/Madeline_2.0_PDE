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
// 2005.09.08.ET.RK
//

//
// Twin.cpp
//
#include "Twin.h"

//
// Twin::getTwinType
//
const std::string Twin::getTwinType( void ) const { 
	
	switch(_twinType){
		case MONOZYGOTIC_TWIN: return "M";
		case DIZYGOTIC_TWIN:   return "D";
		case ZYGOSITY_UNKNOWN_TWIN: return "U";
		case MISSING_TWIN: return ".";
	}
	return ".";
	
}

//
// _validateTwinMarker:
//
bool Twin::_validateTwinMarker(std::string twinMarker){
	if(twinMarker == ".") return true;
	if(twinMarker.size() == 1 && isalpha(twinMarker[0]))
		return true;
	return false;



}

