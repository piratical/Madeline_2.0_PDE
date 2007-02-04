 /////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna and Adrian Marrs
// Copyright (c) 2006 by the
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
// UniqueList.h
//
#include <map>
#include <vector>
#include <string>
#include "Data.h"

#ifndef UNIQUELIST_INCLUDED
#define UNIQUELIST_INCLUDED

//
// CategoricalInformation
//
// This class is used by UniqueList
// for tracking statistics on each
// level of a variable
// 
class CategoricalInformation{
	
private:
	
	unsigned int _ordinal;
	unsigned int _count;
	std::string _label;
	
public:
	
	// Constructor: 
	//
	CategoricalInformation(){ _ordinal=_count = 0; };
	CategoricalInformation(const std::string &label){ _label=label;};
	CategoricalInformation(const std::string &label,const unsigned ordinal){ _label=label; _ordinal=ordinal;};
	
	// Convenience increment operator:
	void operator++(void){ _count++; };
	
	void setLabel(const std::string &label){ _label=label; };
	void setOrdinal(const unsigned ordinal){ _ordinal=ordinal;};
	
	unsigned getCount(void) const    { return _count;   };
	unsigned getOrdinal(void) const  { return _ordinal; };
	std::string getLabel(void) const { return _label;   };
	
};


//
// UniqueList
//
class UniqueList{
	
	private:
	
	std::string _name; // Name of the uniqueListing
	unsigned _levels; // Non-missing levels
	unsigned _count;  // Non-missing summary count
	unsigned _countMissing; // Missing count
	
	//
	// Comparison Functor:
	//
	struct _compareData{
		bool operator()( const Data * const d1 , const Data * const d2 ) const{
			
			return (*d1) < (*d2);
			
		}
	};
	
	typedef std::map<const Data * const,CategoricalInformation,_compareData> UNIQUELIST;
	UNIQUELIST _uniqueList; // Map of the levels and counts
	
	public:
	
	// Constructor:
	UniqueList(const std::string & name);
	// insert:
	void insert( const Data *);
		
	unsigned getLevels(void)       const { return _levels; } /// getLevels returns the number of non-missing levels.
	unsigned getAllLevels(void)    const { return _levels+(_countMissing?1:0); } /// getAllLevels returns the number of non-missing levels and one missing level if present
	unsigned getCount(void)        const { return _count; }
	unsigned getCountMissing(void) const { return _countMissing; }
	
	// Assign ordinals only after having completed
	// source list traversal:
	void assignOrdinals(void);
	
	// Find the ordinal corresponding to a given key:
	unsigned getOrdinalForKey(const Data *dp) const;
	// Find the label corresponding to a given key:
	void getOrdinalAndLabelForKey(const Data *dp,std::string& label,unsigned& ordinal) const;
	std::vector<std::string> getLabels(void) const;
	void printResults(void) const;
	
};


#endif
