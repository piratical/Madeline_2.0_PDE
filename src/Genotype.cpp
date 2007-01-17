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
// 2005.03.07.ET.RK
//

//
// Genotype.cpp
//
#include <stdlib.h>
#include "Genotype.h"
#include "Warning.h"
#include "DigitConverter.h"

std::set<std::string> Genotype::_genotypeMissingValue;

///////////////////////////////////
//
// PRIVATE METHODS:
//
///////////////////////////////////

//
// _orderAlleles(): The normal convention is to have the lesser allele shown first:
//
void Genotype::_orderAlleles( void ){
	if( _allele1 > _allele2 ){
		unsigned int temp= _allele1;
		_allele1 = _allele2;
		_allele2 = temp;
	}
}

//
// _setNormalizedStringRepresentation(): Convert genotype to the normalized form d{1,}/d{1,} or [ACGT]/[ACGT]
//
void Genotype::_setNormalizedStringRepresentation( void ){
	
	if(_isMissing) return;
	if( _isSNP ){
		_genotype  = _snpOrdinalToCString( _allele1 );
		_genotype += _normalDelimiter;
		_genotype += _snpOrdinalToCString( _allele2 );
		
	}else{
		_genotype  = _itoa(_allele1);
		_genotype += _normalDelimiter;
		_genotype += _itoa(_allele2);
	}
	if(Data::isGlobalMissingValue(_genotype)) { setMissing(); return; }
	if(Genotype::isMissingValue(_genotype))   { setMissing(); return; }
	
}

//
// _snpOrdinalToCString(): Convert unsigned int to a SNP ordinal.
//
const char *Genotype::_snpOrdinalToCString( unsigned int allele ){
	
	if(allele==SNP_A_ORDINAL) return "A";
	if(allele==SNP_C_ORDINAL) return "C";
	if(allele==SNP_G_ORDINAL) return "G";
	if(allele==SNP_T_ORDINAL) return "T";
	return ".";
	
}

//
// _itoa(): Convert unsigned int to a char*
//
const char *Genotype::_itoa( unsigned int i ) const{
	
	static char hold[7];
	sprintf(hold,"%i",i);
	return (const char *) hold;
	
}

//
// _setAllele(): set an allele from a char* input
//
const char* Genotype::_setAllele(const char* s, unsigned &value, bool &snp){
	
	char *d;
	char allele[GENERAL_STRING_BUFFER_SIZE];
	
	// skip white space preceding the allele:
	for(;*s && (*s==' ' || *s=='\t');s++);
	
	// Process SNP A/C/G/T alleles:
	if      (*s=='A' || *s=='a'){ value = SNP_A_ORDINAL;  ++s; snp=true; }
	else if (*s=='C' || *s=='c'){ value = SNP_C_ORDINAL;  ++s; snp=true; }
	else if (*s=='G' || *s=='g'){ value = SNP_G_ORDINAL;  ++s; snp=true; }
	else if (*s=='T' || *s=='t'){ value = SNP_T_ORDINAL;  ++s; snp=true; }
	else{
		
		// Process numeric allele:
		for( d=allele ; *s && isdigit(*s) ; *d++=*s++ );
		*d='\0';
		value=atoi(allele);
	}
	//
	// Skip white space after the allele
	//
	for(;*s && (*s==' ' || *s=='\t');s++);
	return s;
	
}


///////////////////////////////////
//
// STATIC METHODS:
//
///////////////////////////////////
bool Genotype::isMissingValue(std::string value){
	
	std::set<std::string>::const_iterator iter;
	iter = _genotypeMissingValue.find(Data::trim(value));
	if(iter != _genotypeMissingValue.end()) return true;
	return false;
	
}

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// set(): from a const char*
//
void Genotype::set( const char *genotype ){
	
	if(Data::isGlobalMissingValue(genotype)) { setMissing(); return; }
	if(Genotype::isMissingValue(genotype))   { setMissing(); return; }
	
	// for Warnings:
	const char *methodName="Genotype::set()";
	
	const char *s;
	bool isSNP1=false;
	bool isSNP2=false;
	
	_isMissing=false;
	//
	// Process the first allele
	//
	char temp[GENERAL_STRING_BUFFER_SIZE];
	strncpy(temp,DigitConverter(genotype).get().c_str(),GENERAL_STRING_BUFFER_SIZE);
	s = temp;
	s = _setAllele(s,_allele1,isSNP1);
	if(_allele1 <= 0){
		setMissing();
		//
		// It seems generally too annoying to see these messages:
		//
		//Warning(methodName,"%1$s is an invalid genotype and has been set to missing.",genotype);
		//
		return;
	}
	//
	// Skip the delimiter
	//
	if(*s==_normalDelimiter || *s==_alternateDelimiter){
		s++;
	}else{
		// Delimiter not found.
		setMissing();
		Warning(methodName,"Invalid character found instead of a delimiter in genotype %1$s.",genotype);
		return;
	}
	//
	// Process the second allele
	//
	s = _setAllele(s,_allele2,isSNP2);
	if(*s != '\0' || _allele2 <=0){
		// Additional characters found after the allele
		Warning(methodName,"%1$s is not a valid genotype and has been set to missing.",genotype);
		setMissing();
		return;
	}
	if(isSNP1 && isSNP2) _isSNP = true;
	else 
	if(isSNP1 || isSNP2){
		Warning(methodName,"%1$s is not a valid SNP genotype.",genotype);
		setMissing();
		return;
	}else _isSNP=false;
	_orderAlleles();
	//
	// Set normalized string representation of allele:
	//
	_setNormalizedStringRepresentation();
	
}

//
// set() : from a std::string
//
void Genotype::set( const std::string& genotype ){
	
	set(genotype.c_str());
	
}

//
// get(): a std::string
//
const std::string Genotype::get( void ) const{
	
	if(_isMissing) return ".";
	return _genotype;
	
}

//
// set(): from ints
//
void Genotype::set(const int allele1, const int allele2){
	
	if(allele1 <= 0 || allele2 <= 0){
		setMissing();
		Warning("Genotype::set(int,int)","%1$d/%2$d is an invalid Genotype and has been set to missing.",allele1,allele2);
		return;
	}
	_allele1 = allele1;
	_allele2 = allele2;
	_isSNP=false;
	_isMissing=false;
	_orderAlleles();
	_setNormalizedStringRepresentation();
	
}

//
// setAllele1(): from int
//
void Genotype::setAllele1( const int allele1 ){
	
	// for warnings:
	const char *methodName="Genotype::setAllele1()";
	
	if(allele1 <= 0){
		setMissing();
		Warning(methodName,"%d is an invalid allele1 and has been set to missing.",allele1);
		return;
	}
	if(_isSNP){
		setMissing();
		Warning(methodName,"%d is an invalid SNP allele1 and has been set to missing.",allele1);
		return;
	}
	_allele1=allele1; 
	if(_allele2){
		_isMissing=false;
		_orderAlleles();
	}
	_setNormalizedStringRepresentation();
	
}

//
// setAllele2(): from int
//
void Genotype::setAllele2( const int allele2 ){ 
	
	// for warnings:
	const char *methodName="Genotype::setAllele2()";
	
	if(allele2 <= 0){
		setMissing();
		Warning(methodName,"%d is an invalid allele2 and has been set to missing.",allele2);
		return;
	}
	if(_isSNP){
		setMissing();
		Warning(methodName,"%d is an invalid SNP allele2 and has been set to missing.",allele2);
		return;
	}
	_allele2=allele2; 
	if(_allele1){
		_isMissing=false;
		_orderAlleles();
	}
	_setNormalizedStringRepresentation();
	
}

//
// setAllele1() : from const char*
//
void Genotype::setAllele1( const char* allele1){
	
	// for warnings:
	const char *methodName="Genotype::setAllele1(const char *)";
	
	bool snp=false;
	char temp[GENERAL_STRING_BUFFER_SIZE];
	strncpy(temp,DigitConverter(allele1).get().c_str(),GENERAL_STRING_BUFFER_SIZE);
	const char *d = temp;
	d = _setAllele(d,_allele1,snp);
	if(_isMissing) return;
	//
	// Check whether a SNP allele is assigned a SNP ordinal.
	//
	if(_isSNP && !snp || !_isSNP && snp){
		setMissing();
		Warning(methodName,"%s does not match the given genotype and has been set to missing.",allele1);
		return;
	}
	if(*d != '\0' || _allele1 <= 0){
		// Additional characters found after the allele
		Warning(methodName,"%s is not a valid allele1 and has been set to missing.",allele1);
		setMissing();
		return;
	}
	if(_allele2){
		_isMissing=false;
		_orderAlleles();
	}
	_setNormalizedStringRepresentation();
	
}

//
// setAllele2(): from const char*
//
void Genotype::setAllele2( const char* allele2){
	
	// for warnings:
	const char *methodName="Genotype::setAllele2()";
	
	bool snp=false;
	char temp[GENERAL_STRING_BUFFER_SIZE];
	strncpy(temp,DigitConverter(allele2).get().c_str(),GENERAL_STRING_BUFFER_SIZE);
	const char* d= temp;
	d = _setAllele(d,_allele2,snp);
	if(_isMissing) return;
	//
	// Check whether a SNP allele is assigned a SNP ordinal.
	//
	if(_isSNP && !snp || !_isSNP && snp){
		setMissing();
		Warning(methodName,"%s does not match the given genotype and has been set to missing.",allele2);
		return;
	}
	if(*d != '\0' || _allele2 <= 0){
		// Additional characters found after the allele
		Warning(methodName,"%s is not a valid allele2 and has been set to missing.",allele2);
		setMissing();
		return;
	}
	if(_allele1){
		_isMissing=false;
		_orderAlleles();
	}
	_setNormalizedStringRepresentation();
	
}


//
// setAllele1(): from std::string
//
void Genotype::setAllele1( const std::string &allele1){
	
	setAllele1(allele1.c_str());
	
}

//
// setAllele2(): from std::string
//
void Genotype::setAllele2( const std::string &allele2){
	
	setAllele2(allele2.c_str());
	
}

//
// operator+=(): Add an integer value to both alleles.
//
Genotype& Genotype::operator+=( const int allele ){
	
	if(_isMissing || _isSNP){
		return *this;
	}
	if(allele+_allele1 <= 0){
		Warning("Genotype::operator+=()","Adding %d to %s is invalid and has been set to missing.",allele,_genotype.c_str());
		setMissing();
		return *this;
	}
	_allele1+=(unsigned)allele;
	_allele2+=(unsigned)allele;
	_setNormalizedStringRepresentation();
	return *this;
	
}

//
// operator-=(): Subtract an integer value from both alleles.
//
Genotype& Genotype::operator-=( const int allele ){
	
	
	if(_isMissing || _isSNP){
		return *this;
	}
	if(allele > 0 && (unsigned)allele >= _allele1){
		Warning("Genotype::operator-=()","Subtracting %d from %s is invalid and has been set to missing.",allele,_genotype.c_str());
		setMissing();
		return *this;
	}
	_allele1-=(unsigned)allele;
	_allele2-=(unsigned)allele;
	_setNormalizedStringRepresentation();
	return *this;
	
}

//
// operator+=(): Add a Number to both alleles. 
//
Genotype& Genotype::operator+=( const Number &allele ){
	
	if(_isMissing || _isSNP || allele.isMissing() ){
		return *this;
	}
	if(allele.isApproximate()){
		Warning("Genotype::operator+=(Number)","Adding %s to %s is invalid and has been set to missing.",allele.get().c_str(),_genotype.c_str());
		setMissing();
		return *this;
	}
	int temp = atoi(allele.get().c_str());
	if(temp+_allele1 <= 0){
		Warning("Genotype::operator+=(Number)","Adding %s to %s is invalid and has been set to missing.",allele.get().c_str(),_genotype.c_str());
		setMissing();
		return *this;
	}
	_allele1+=(unsigned)temp;
	_allele2+=(unsigned)temp;
	_setNormalizedStringRepresentation();
	return *this;
	
}

//
// operator-=(): Subtract a Number from both alleles. 
//
Genotype& Genotype::operator-=( const Number &allele ){
	
	
	if(_isMissing || _isSNP || allele.isMissing() ){
		return *this;
	}
	if(allele.isApproximate()){
		Warning("Genotype::operator-=(Number)","Subtracting %s from %s is invalid and has been set to missing.",allele.get().c_str(),_genotype.c_str());
		setMissing();
		return *this;
	}
	int temp = atoi(allele.get().c_str());
	if(temp > 0 && (unsigned)temp >= _allele1){
		Warning("Genotype::operator-=(Number)","Subtracting %s from %s is invalid and has been set to missing.",allele.get().c_str(),_genotype.c_str());
		setMissing();
		return *this;
	}
	_allele1-=(unsigned)temp;
	_allele2-=(unsigned)temp;
	_setNormalizedStringRepresentation();
	return *this;
	
}

///
/// operator < 
///
/// NOTA BENE: This is used for SORTING ONLY
///
bool Genotype::operator<(const Data& b) const{
	
	//
	// If of differing data types, sort by the data type itself:
	//
	if(getDataType() != b.getDataType()) return getDataType() < b.getDataType();
	
	//
	// Get here if b is really a Genotype:
	//
	Genotype* pb = static_cast<Genotype*>(const_cast<Data* const>(&b));
	
	//
	// Sort MISSING values as LARGER than everything else:
	//
	if( _isMissing ) return false;
	if( pb->_isMissing ) return true;
	
	//
	// Force SNP genotypes to sort both separately and before non-SNP genotypes:
	//
	if(_isSNP && !pb->_isSNP) return true;
	
	//
	// Compare the first allele:
	//
	if( _allele1 != pb->_allele1) return _allele1 < pb->_allele1;
	
	//
	// Get here if the first alleles are identical:
	// Compare second alleles:
	//
	return _allele2 < pb->_allele2;
	
}

//
// operator == 
//
bool Genotype::operator==(const Data& b) const{
	
	Genotype* pb = dynamic_cast<Genotype*>(const_cast<Data* const>(&b));
	if(pb){
		if( _isMissing && pb->_isMissing ) return true;
		// do not compare SNP alleles to other alleles
		if(_isSNP != pb->_isSNP) return false;
		if(_allele1== pb->_allele1) return _allele2 == pb->_allele2;
	}
	return false;
	
}

//
// Stream operator << :
//
std::ostream &operator<<(std::ostream &s,const Genotype &g){
	
	return s << g.get();
	
}
//
// clone:
//
Genotype* Genotype::clone() const {
	
	return new Genotype(*this);
	
}


// ADDED: on 2005-09-09
// Determines whether a string is a valid Genotype string
bool Genotype::isA(const std::string& s){
	
	Genotype test(s);
	// if test ends up missing, then it's not a Genotype
	// else it is a Genotype:
	return !test.isMissing();
	
}


