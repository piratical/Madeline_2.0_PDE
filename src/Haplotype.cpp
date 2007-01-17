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
// 2005.08.17.ET.RK
//

//
// Haplotype.cpp
//
#include <stdlib.h>
#include "Haplotype.h"
#include "Warning.h"
#include "DigitConverter.h"

std::set<std::string> Haplotype::_haplotypeMissingValue;

///////////////////////////////////
//
// PRIVATE METHODS:
//
///////////////////////////////////


///////////////////////////////////
//
// STATIC METHODS:
//
///////////////////////////////////
bool Haplotype::isMissingValue(std::string value){
	
	std::set<std::string>::const_iterator iter;
	iter = _haplotypeMissingValue.find(Data::trim(value));
	if(iter != _haplotypeMissingValue.end()) return true;
	return false;
	
}

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// setMissing():
//
void Haplotype::setMissing( void ){ 
	
	_isMissing=true; 
	_haplotype="."; 
	_recombination=MISSING_INFO;
}

//
// set(): from a const char*
//
void Haplotype::set( const char *haplotype ){
	
	if(Data::isGlobalMissingValue(haplotype)) { setMissing(); return; }
	if(Haplotype::isMissingValue(haplotype))   { setMissing(); return; }
	
	_recombination=MISSING_INFO;
	_isMissing=true;
	
	// Set the haplotype by stripping out all intervening white space:
	for(const char *h=haplotype;*h;h++){
		if(*h!=' '){
			
			_haplotype+=*h;
			
			// Set recombination information:
			switch((RECOMBINATION_INFO)*h){
			case NO_INFORMATION:
			case NO_RECOMBINATION:
			case MATERNAL_RECOMBINATION:
			case PATERNAL_RECOMBINATION:
			case BOTH_RECOMBINE:
				_recombination=(RECOMBINATION_INFO)*h;
				_isMissing=false;
				break;
			// The compiler complains if we do not have
			// this case, so here it is even though it is
			// unnecessary:
			case MISSING_INFO:
				break;
			}
			
		}
	}
	// terminate the trimmed haplotype:
	_haplotype+='\0';
	
	
}

//
// set() : from a std::string
//
void Haplotype::set( const std::string &haplotype ){
	
	set(haplotype.c_str());
	
}

//
// get(): a std::string
//
const std::string Haplotype::get( void ) const{
	
	if(_isMissing) return ".";
	return _haplotype;
	
}

//
// Private _getRecombinationInfoSortOrder():
// Returns unsigned:
unsigned char Haplotype::_getRecombinationInfoSortOrder(void) const{
	
	switch(_recombination){
	// Sort MISSING as greater than anything else:
	case MISSING_INFO:
		return 6;
	case NO_INFORMATION:
		return 1;
	case NO_RECOMBINATION:
		return 2;
	case MATERNAL_RECOMBINATION:
		return 3;
	case PATERNAL_RECOMBINATION:
		return 4;
	case BOTH_RECOMBINE:
		return 5;
	}
	// Treat anything else just like MISSING:
	return 6;
}

//
// operator < 
//
bool Haplotype::operator<(const Data& b) const{
	
	//
	// If of differing data types, sort by the data type itself:
	//
	if(getDataType() != b.getDataType()) return getDataType() < b.getDataType();
	
	//
	// Get here if b is really a Genotype:
	//
	Haplotype* pb = static_cast<Haplotype*>(const_cast<Data* const>(&b));
	
	//
	// Sort MISSING values as LARGER than everything else:
	//
	if( _isMissing ) return false;
	if( pb->_isMissing ) return true;
	
	//
	// This forces numeric sorting
	//
	// NOTA BENE: THIS IS INSUFFICIENT FOR DEALING WITH ACTUAL OUTPUT
	// FROM MERLIN, I.E. "2,1A : A1,2" WILL NOT BE HANDLED CORRECTLY!
	
	unsigned A =   atoi(getAlleleFromMom().c_str())*100000 
	             + _getRecombinationInfoSortOrder()* 10000 
	             + atoi(getAlleleFromDad().c_str());
	             
	unsigned B =   atoi(pb->getAlleleFromMom().c_str())*100000 
	             + pb->_getRecombinationInfoSortOrder()* 10000 
	             + atoi(pb->getAlleleFromDad().c_str());
	             
	return A < B;
	
}

//
// operator == 
//
bool Haplotype::operator==(const Data& b) const{
	
	
	if(getDataType() != b.getDataType()) return getDataType() < b.getDataType();
	Haplotype* pb = static_cast<Haplotype*>(const_cast<Data* const>(&b));
	if( _isMissing || pb->_isMissing ) return false;
	return _haplotype == pb->_haplotype;
	
}

//
// Stream operator << :
//
std::ostream &operator<<(std::ostream &s,const Haplotype &h){
	
	return s << h.get();
	
}

//
// clone:
//
Haplotype* Haplotype::clone() const {
	
	return new Haplotype(*this);
	
}


//
// getAlleleFromMom():
//
//    - The mom's allele is the *first* allele:
//
const std::string Haplotype::getAlleleFromMom( void ) const{
	
	if(_isMissing) return ".";
	std::string::size_type position = _haplotype.find((char)_recombination);
	return _haplotype.substr(0,position);
	
	
}


//
// getAlleleFromDad():
//
//    - The dad's allele is the *second* allele:
//
const std::string Haplotype::getAlleleFromDad( void ) const{
	
	if(_isMissing) return ".";
	std::string::size_type position = _haplotype.find((char)_recombination);
	return _haplotype.substr(position+1);
	
}

//
// getRecombinationInfo: as an enum
//
Haplotype::RECOMBINATION_INFO Haplotype::getRecombinationInfo( void ) const{
	
	return _recombination;
	
}


