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
// 2005.03.07.ET
//

//
// Genotype.h
//

#ifndef GENOTYPE_INCLUDED
#define GENOTYPE_INCLUDED

#include <string>
#include <iostream>
#include "Data.h"
#include "Number.h"
//! Genotype is a data type derived from Data
/*! Genotype deals with both microsatellite marker alleles
 and SNP alleles.
*/
class Genotype : public Data{
public:
	
	enum { SNP_A_ORDINAL=1, SNP_C_ORDINAL=2, SNP_G_ORDINAL=3, SNP_T_ORDINAL=4 };
	
private:
	static std::set<std::string> _genotypeMissingValue;
	bool _isMissing;
	bool _isSNP;           /*!< Is the genotype for a SNP? */
	std::string _genotype; /*!< String representation of the genotype alleles */
	
	unsigned int _allele1;
	unsigned int _allele2;
	
	unsigned int _partial1; // partial repeat allele 1 (STR genotype)
	unsigned int _partial2; // partial repeat allele 2 (STR genotype)
	//
	// Delimiters between alleles in a genotype:
	//
	const static char _normalDelimiter='/';
	const static char _alternateDelimiter='|';
	//
	// Delimiter between an allele and a partial repeat in an STR allele:
	//
	const static char _normalPartialDelimiter='.';
	const static char _alternatePartialDelimiter=',';
	
	// The normal convention is to have the lesser allele shown first:
	void _orderAlleles( void );
	
	// Store "normalized" string representation of allele:
	void _setNormalizedStringRepresentation( void );
	
	// Convert integer allele to (static) c-string:
	const char *_itoa( unsigned int i ) const;
	const char *_snpOrdinalToCString( unsigned int allele );
	void _readAllele(const char *s,unsigned &v1,unsigned &v2);
	const char* _setAllele(const char* input, unsigned &allele, unsigned &partial, bool &snp);

public:
	
	//
	// Static methods:
	//
	static void addGenotypeMissingValue(std::string &value){ _genotypeMissingValue.insert(value); }
	static bool isMissingValue(std::string value);
	
	// Constructors:
	Genotype(){ setMissing(); }
	Genotype(const char *g){ set(g); }
	Genotype(const std::string& g){ set(g.c_str()); }
	Genotype(const int allele1, const int allele2){ _isSNP=false;set(allele1,allele2); }
	
	//  Methods required by Data virtual base class:
	bool isMissing( void ) const { return _isMissing; }
	void setMissing( void ) { _isMissing=true; _isSNP=false;_genotype=".";_allele1=_allele2=0; }
	void set( const char *genotype );
	void set( const std::string& genotype);
	const std::string get( void ) const; 
	
	//
	// Additional Setters not present in virtual base class:
	//
	void set(const int allele1,const int allele2);
	void setAllele1(const int allele1);
	void setAllele2(const int allele2);
	void setAllele1(const char* allele1);
	void setAllele2(const char* allele2);
	void setAllele1(const std::string &allele1);
	void setAllele2(const std::string &allele2);
	//
	// Additional Getters not present in virtual base class:
	//
	const Number getAllele1( void ) const { if(_isMissing) return Number::MISSING; return Number(_allele1); }
	const Number getAllele2( void ) const { if(_isMissing) return Number::MISSING; return Number(_allele2); }
	
	//
	// Operators:
	//
	Genotype& operator+=( const int allele );
	Genotype& operator-=( const int allele );
	Genotype& operator+=( const Number &allele );
	Genotype& operator-=( const Number &allele );
	
	//
	// Operators:
	bool operator<( const Data& b) const;
	bool operator==( const Data& b) const;
	const DATATYPE getDataType( void ) const { return GENOTYPE; }
	virtual Genotype* clone() const;
	
	/// Determines whether a string is a valid Genotype string
	static bool isA(const std::string& s);
	
};

std::ostream &operator<<(std::ostream &s,const Genotype &g);

#endif
