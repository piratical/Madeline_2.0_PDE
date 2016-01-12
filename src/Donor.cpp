/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna, and Adrian Marrs
// Copyright (c) 2016 by the
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
// 2016.01.11.ET
//

//
// Donor.cpp
//
#include "Donor.h"
#include "utility.h"

std::map<std::string,DONOR_TYPE> Donor::_lookupTable;
DonorMapLoader DonorMapLoader::donorMapLoader;

std::string Donor::englishString[4]={".","sperm donor","ovum donor","sperm and ovum donor"};

//
// Donor::set() base method:
//
void Donor::set(const std::string &value){
	
	std::string findMe=stringToUpper(value);
	Data::trim(findMe);
	
	std::map<std::string,DONOR_TYPE>::const_iterator iter = _lookupTable.find(findMe);
	if(iter != _lookupTable.end()){
		_value = iter->second;
	}else{
		_value = MISSING_DONOR;
	}
	
}

//
// set( const char * )
//
void Donor::set(const char *value){
	
	std::string findMe(value);
	set(findMe);
	
}

//
// Free function: ostream operator <<
//
std::ostream& operator<<(std::ostream& s, const Donor& d){
	
	return s << d.get();
	
}

//
// addDonorMapping
//
void Donor::addDonorMapping(std::string &donorValue,DONOR_TYPE donorMapping){ 
	
	Donor::_lookupTable[ donorValue ] = donorMapping;
	
}

//
// get
//
const std::string Donor::get(void) const {
	
	return englishString[_value];
	
};

//
// operator==
//
bool Donor::operator==(const Data& b) const{
	
	const Donor &d = dynamic_cast<const Donor &>(b);
	if(isMissing() && d.isMissing()){
		return true;
	}else if( isMissing() || d.isMissing() ){
		return false;
	}else if(d.getDataType()==DONOR){
		return _value==d._value;
	}else{
		return false;
	}
	
}

//
// operator<
//
bool Donor::operator<(const Data& b) const{
	
	if(getDataType() != b.getDataType()){
		return getDataType() < b.getDataType();
	}
	const Donor& d = static_cast<const Donor &>(b);
	//
	// Make MISSING sort as greater than everything else:
	//
	if( isMissing()   ) return false;
	if( d.isMissing() ) return true;
	return _value < d._value;
	
}

//
// clone:
//
Donor* Donor::clone() const{
	
	return new Donor(*this);
	
}


