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
// Last updated 2016.01.11.ET
// 

//
// Donor.h
//
#ifndef Donor_INCLUDED
#define Donor_INCLUDED

#include "Data.h"
#include "DataTypes.h"
#include <map>

//
// DONOR_TYPE
//
enum DONOR_TYPE{MISSING_DONOR,SPERM_DONOR,OVUM_DONOR,SPERM_AND_OVUM_DONOR};

class Donor : public Data {
	
private:
	
	DONOR_TYPE _value;
	
	friend class DonorMapLoader;
	static std::map<std::string,DONOR_TYPE> _lookupTable;
	static std::string englishString[];
	
public:
	
	//
	// Constructors:
	//
	Donor(){ _value = MISSING_DONOR; }
	Donor( const std::string &value ){ set(value); }
	Donor( const char* value ){ set(value); }
	//
	// Set methods:
	//
	void set(const std::string &value);
	void set(const char *value);
	
	DONOR_TYPE getDonorType(void) const { return _value; };
	const std::string get(void) const;
	
	static void addDonorMapping(std::string &donorValue,DONOR_TYPE donorMapping);
	
	bool isMissing( void ) const { return _value==MISSING_DONOR; };
	void setMissing( void ){ _value = MISSING_DONOR; };
	const DATATYPE getDataType( void ) const { return DONOR; };
	Donor* clone() const;
	// operators required by Data base class
	bool operator==(const Data&) const;
	bool operator<(const Data&) const;
	
};

//
// Donor Friend class that initializes the lookup table:
//
class DonorMapLoader
{
public:
	static DonorMapLoader donorMapLoader;
	
	DonorMapLoader(){
		Donor::_lookupTable["S"]              = SPERM_DONOR;
		Donor::_lookupTable["O"]              = OVUM_DONOR;
		Donor::_lookupTable["SO"]             = SPERM_AND_OVUM_DONOR;
		Donor::_lookupTable["OS"]             = SPERM_AND_OVUM_DONOR;
		Donor::_lookupTable["SPERM"]          = SPERM_DONOR;
		Donor::_lookupTable["OVUM"]           = OVUM_DONOR;
		Donor::_lookupTable["SPERM_AND_OVUM"] = SPERM_AND_OVUM_DONOR;
		Donor::_lookupTable["OVUM_AND_SPERM"] = SPERM_AND_OVUM_DONOR;
		Donor::_lookupTable["SPERM AND OVUM"] = SPERM_AND_OVUM_DONOR;
		Donor::_lookupTable["OVUM AND SPERM"] = SPERM_AND_OVUM_DONOR;
		Donor::_lookupTable["SPERM OVUM"]     = SPERM_AND_OVUM_DONOR;
		Donor::_lookupTable["OVUM SPERM"]     = SPERM_AND_OVUM_DONOR;
		Donor::_lookupTable["SPERM & OVUM"]   = SPERM_AND_OVUM_DONOR;
		Donor::_lookupTable["OVUM & SPERM"]   = SPERM_AND_OVUM_DONOR;
	}
};

std::ostream& operator<<(std::ostream& s,const Donor& n);

#endif
