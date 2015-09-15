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
// UniqueList.cpp
//

#include "UniqueList.h"

// DEBUG:
#include <iostream>

//
// Constructor:
//
UniqueList::UniqueList(const std::string & name){
	
	_name=name;
	_count=_countMissing=_levels=0;
	
}

//
// insert():
//
void UniqueList::insert(const Data *dp){
	
	//
	// Increment summary counters:
	//
	if(dp->isMissing()) _countMissing++;
	else                _count++;
	
	UNIQUELIST::iterator it = _uniqueList.find(dp);
	
	if( it!=_uniqueList.end() ){
		//
		// Repeat of key:
		// increment item counter:
		//
		(*it).second.operator++();
		
		// DEBUG:
		// std::cout << "SEARCHING FOR " << dp->get() << " FOUND " << (*it).first->get() << std::endl;
		//
		
	}else{
		//
		// New entry:
		//
		if(!dp->isMissing()) _levels++;
		
		CategoricalInformation info;
		info.setLabel( dp->get() );
		++info;
		_uniqueList.insert(std::pair<const Data*,CategoricalInformation>(dp,info));
	}
}

//
// assignOrdinals()
// Assign ordinals only after having traversed a complete list
//
void UniqueList::assignOrdinals(void){
	
	UNIQUELIST::iterator it;
	unsigned int i;
	// Ordinals start at one:
	for(i=1, it=_uniqueList.begin();it!=_uniqueList.end();it++,i++){
		
		(*it).second.setOrdinal(i);
		
	}
	
}

//
// getOrdinalForKey
//
// Find the ordinal corresponding to a given key:
//
unsigned UniqueList::getOrdinalForKey(const Data *dp) const{
	
	UNIQUELIST::const_iterator it = _uniqueList.find(dp);
	//
	// Ordinals start at 1, so 0 means the key was not
	// found ...
	//
	if( it == _uniqueList.end() ) return 0;
	return it->second.getOrdinal();
	
}

//
// getOrdinalAndLabelForKey
//
// Find the ordinal and label corresponding to a given key:
//
void UniqueList::getOrdinalAndLabelForKey(const Data *dp,std::string& label,unsigned& ordinal) const{
	
	UNIQUELIST::const_iterator it = _uniqueList.find(dp);
	//
	// Ordinals start at 1, so 0 means the key was not
	// found ... but this should not occur in practice:
	//
	if( it != _uniqueList.end() ){
		ordinal = it->second.getOrdinal();
		label = it->second.getLabel();
	}else{
		ordinal=0;
		label=".";
	}
}

//
// printResults:
//
void UniqueList::printResults(void) const{
	
	std::cout << _name << " has " << _levels << " non-missing levels\n";
	std::cout << _name << " has " << _count  << " non-missing entries\n";
	std::cout << _name << " has " << _countMissing  << " missing entries\n";
	
	UNIQUELIST::const_iterator it;
	UNIQUELIST::const_iterator iLastEntry = _uniqueList.end(); iLastEntry--;
	unsigned int i;
	for(i=1, it=_uniqueList.begin();it!=_uniqueList.end();it++,i++){
		
		std::cout << i << ".\t" << (*it).first->get();
		std::cout << "\t" << (*it).second.getLabel();
		std::cout << "\t" << (*it).second.getCount();
		std::cout << "\tOrd=" << (*it).second.getOrdinal();
		std::cout << "\t" << ( 100.0 * (*it).second.getCount() / ( _count + _countMissing) ) << "%";
		if(it==iLastEntry){
			std::cout << "\t.";
		}else{
			std::cout << "\t" << ( 100.0 * (*it).second.getCount() / _count ) << "%";
		}
		std::cout << std::endl;
		
	}
}

//
// getLabels():
//
std::vector<std::string> UniqueList::getLabels(void) const{
	
	std::vector<std::string> labels;
	UNIQUELIST::const_iterator it=_uniqueList.begin();
	while(it != _uniqueList.end()){
		// Ignore missing
		if(!(*it).first->isMissing())
			labels.push_back((*it).second.getLabel());
		++it;
	}
	return labels;
}

