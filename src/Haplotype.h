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
// Haplotype.h
//

#ifndef HAPLOTYPE_INCLUDED
#define HAPLOTYPE_INCLUDED

#include <string>
#include <iostream>
#include "Data.h"
#include "Number.h"

class Haplotype : public Data{
public:
	
	// RECOMBINATION INFORMATION enumeration:
	enum RECOMBINATION_INFO{ MISSING_INFO='.',NO_INFORMATION=':',NO_RECOMBINATION='|',MATERNAL_RECOMBINATION='/',PATERNAL_RECOMBINATION='\\',BOTH_RECOMBINE='+' };
	
private:
	static std::set<std::string> _haplotypeMissingValue;
	bool _isMissing;
	std::string _haplotype;
	
	RECOMBINATION_INFO _recombination;
	
	unsigned char _getRecombinationInfoSortOrder(void) const;
public:
	
	//
	// Static methods:
	//
	static void addHaplotypeMissingValue(std::string value){ _haplotypeMissingValue.insert(value); }
	static bool isMissingValue(std::string value);
	
	// Constructors:
	Haplotype(){ setMissing(); }
	Haplotype(const char *h){ set(h); }
	Haplotype(const std::string& h){ set(h.c_str()); }
	
	//  Methods required by Data virtual base class:
	bool isMissing( void ) const { return _isMissing; }
	void setMissing( void );
	void set( const char *haplotype );
	void set( const std::string &haplotype);
	const std::string get( void ) const; 
	
	//
	// Operators:
	// ADDED: on 2005-07-19
	bool operator<( const Data& b) const;
	bool operator==( const Data& b) const;
	// ADDED: on 2005-07-15
	const DATATYPE getDataType( void ) const { return HAPLOTYPE; }
	virtual Haplotype* clone() const;
	
	// Additional methods:
	const std::string  getAlleleFromMom( void ) const;
	const std::string  getAlleleFromDad( void ) const;
	Haplotype::RECOMBINATION_INFO getRecombinationInfo( void ) const;
	
};

std::ostream &operator<<(std::ostream &s,const Haplotype &g);

#endif
