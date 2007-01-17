/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager and Ritu Khanna
// Copyright (c) 2006 by the
// Regents of the University of Michigan.
// All Rights Reserved.
// 
// The latest version of this program is available from:
// 
//	 http://eyegene.ophthy.med.umich.edu/madeline/
//
// Released under the GNU General Public License.
// A copy of the GPL is included in the distribution
// package of this software, or see:
//
//	 http://www.gnu.org/copyleft/
//
// ... for licensing details.
//
/////////////////////////////////////////////////////////

//
// LabelSet.cpp
//

#include "LabelSet.h"

//
// RandomGenerator is needed for getRandomLabelSubsetByType() method:
//
#include "RandomGenerator.h"

//////////////////////////////////////
//
// PUBLIC METHODS
//
//////////////////////////////////////

//
// Constructor
//
LabelSet::LabelSet(const DataTable *const pedigreeTable){
	_pedigreeTable  =  pedigreeTable;
	_numberOfLabels = _pedigreeTable->getNumberOfShowOnPedigreeColumns();
}

//
// getNumberOfLabels:
//
unsigned LabelSet::getNumberOfLabels() const{
	
	return _numberOfLabels;
	
}

//
// getLabelSet:
//
std::vector<UTF8String> LabelSet::getLabelSet(Individual* individual) const{
	
	std::vector<UTF8String> labelSet;
	for(unsigned i=0;i<_pedigreeTable->getNumberOfShowOnPedigreeColumns();i++){
		labelSet.push_back(_pedigreeTable->getColumn(_pedigreeTable->getLabelColumnIndex(i))->get(individual->getRowIndex()));
	}
	
	return labelSet;
	
}

//
// getLabelSubsetByType: Get the subset of labels that are "on" which are 
//                       of a specific DATATYPE such as Genotype or String.
//                       
//
// 2006.08.04.ET addendum
//
std::vector<UTF8String> LabelSet::getLabelSubsetByType(DATATYPE type,Individual* individual) const{
	
	std::vector<UTF8String> labelSet;
	for(unsigned i=0;i<_pedigreeTable->getNumberOfShowOnPedigreeColumns();i++){
		if(_pedigreeTable->getColumn(_pedigreeTable->getLabelColumnIndex(i))->getColumnType() == type){
			labelSet.push_back(_pedigreeTable->getColumn(_pedigreeTable->getLabelColumnIndex(i))->get(individual->getRowIndex()));
		}
	}
	
	return labelSet;
	
}

//
// getRandomLabelSubsetByType: Get the subset of labels that are "on" which are 
//                       of a specific DATATYPE such as Genotype or String for 
//                       a random Individual : Used by the label manager class
//                       to determine font metrics ...
//
// 2006.08.04.ET addendum
//
std::vector<UTF8String> LabelSet::getRandomLabelSubsetByType(DATATYPE type) const{
	
	//
	// Pick a random individual from the _pedigreeTable:
	//
	RandomGenerator pickAnIndividual(RandomGenerator::PSEUDO_RANDOM);
	unsigned aRandomIndividual = pickAnIndividual.getIntegerInRange(0,_pedigreeTable->getNumberOfRows()-1);
	
	//
	// Now return the data of DATATYPE "type" for that random individual:
	//
	std::vector<UTF8String> labelSet;
	
	for(unsigned i=0;i<_pedigreeTable->getNumberOfShowOnPedigreeColumns();i++){
		 if(_pedigreeTable->getColumn(_pedigreeTable->getLabelColumnIndex(i))->getColumnType()==type ){
			
			labelSet.push_back( _pedigreeTable->getColumn(_pedigreeTable->getLabelColumnIndex(i))->get( aRandomIndividual ) );
			
		}
	}
	return labelSet;
	
}

//
// getLabelNameSet:
//
std::vector<UTF8String> LabelSet::getLabelNameSet(void) const{
	
	std::vector<UTF8String> labelNameSet;
	
	for(unsigned i=0;i<_pedigreeTable->getNumberOfShowOnPedigreeColumns();i++){
		
		labelNameSet.push_back( _pedigreeTable->getColumnName(_pedigreeTable->getLabelColumnIndex(i)));
		
	}
	return labelNameSet;
	
}
