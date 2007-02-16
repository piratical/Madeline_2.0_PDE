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
// PedigreeSet.cpp
//

#include "PedigreeSet.h"
#include "DataColumn.h"
#include "LabelSet.h"
#include "DrawingMetrics.h"
#include <algorithm>
#include <set>
#include <map>

//////////////////////////////////////
//
// DESTRUCTORS:
//
//////////////////////////////////////

PedigreeSet::~PedigreeSet()
{
	
	
	std::set<Pedigree*,comparePedigrees>::iterator pit = _pedigrees.begin();
	while(pit != _pedigrees.end()){
		delete *pit;
		++pit;
	}
	_pedigrees.clear();
	
	
}

//////////////////////////////////////
//
// PRIVATE METHODS
//
//////////////////////////////////////

//
// _checkParentChildDOB():
//
void PedigreeSet::_checkParentChildDOB(){
	
	std::set<Pedigree*,comparePedigrees>::const_iterator it = _pedigrees.begin();
	while(it != _pedigrees.end()){
		(*it)->checkParentChildDOB();
		++it;
	}
	
}

//
// _computeWidths():
//
void PedigreeSet::_computeWidths(const std::string& sortField,bool dobSortOrder ){
	
	std::set<Pedigree*,comparePedigrees>::const_iterator it = _pedigrees.begin();
	while(it != _pedigrees.end()){
		(*it)->computePedigreeWidth(sortField,dobSortOrder);
		
		++it;
	}
	
	
}

//
// _determineFoundingGroups():
//
void PedigreeSet::_determineFoundingGroups(){
	
	std::set<Pedigree*,comparePedigrees>::const_iterator it = _pedigrees.begin();
	while(it != _pedigrees.end()){
		(*it)->determineFoundingGroups();
		++it;
	}
	
}

//
// _establishConnections():
//
void PedigreeSet::_establishConnections(){
	
	std::set<Pedigree*,comparePedigrees>::const_iterator it = _pedigrees.begin();
	while(it != _pedigrees.end()){
		(*it)->establishIndividualConnections();
		++it;
	}
	
}

//
// _setCoreOptionalFields():
//
void PedigreeSet::_setCoreOptionalFields(const DataTable* pedigreeTable){
	
	std::set<Pedigree*,comparePedigrees>::const_iterator it = _pedigrees.begin();
	while(it != _pedigrees.end()){
		(*it)->setCoreOptionalFields(pedigreeTable);
		++it;
	}
	
}


//////////////////////////////////////
//
// PUBLIC METHODS
//
//////////////////////////////////////

//
// addPedigreesFromDataTable():
//
void PedigreeSet::addPedigreesFromDataTable(const DataTable * p_pedigreeTable, unsigned tableIndex,const std::string& sortField){
	
	//
	// Use const reference for convenience:
	// (so I don't have to change the code
	//  since the parameter has changed from a
	//  const DataTable & to a const DataTable * )
	//
	const DataTable & pedigreeTable = (*p_pedigreeTable);
	
	std::cout << "Start of addPedigreesFromDataTable" << std::endl;
	//
	// Get all the core columns from the datatable:
	//
	DataColumn *familyIdColumn = pedigreeTable.getColumn(pedigreeTable.labels.FamilyIdField);
	//familyIdColumn->printData();
	DataColumn *individualIdColumn = pedigreeTable.getColumn(pedigreeTable.labels.IndividualIdField);
	//individualIdColumn->printData();
	DataColumn *motherIdColumn = pedigreeTable.getColumn(pedigreeTable.labels.MotherIdField);
	DataColumn *fatherIdColumn = pedigreeTable.getColumn(pedigreeTable.labels.FatherIdField);
	DataColumn *genderColumn   = pedigreeTable.getColumn(pedigreeTable.labels.GenderField);
	//
	// Insert the Pedigrees in a set:
	//
	std::string currentFamily;
	int numberOfRows = familyIdColumn->getNumberOfRows();
	int index=0;
	while(index < numberOfRows){
		
		currentFamily = familyIdColumn->get(index);
		if(currentFamily == "."){
			Warning("PedigreeSet::addPedigreesFromDataTable()",
			        "Family Id is missing for individual %s and will be ignored.",
			        individualIdColumn->get(index).c_str()
			);
			index++;
			continue;
		}
		std::pair<std::set<Pedigree*,comparePedigrees>::iterator,bool> pp;
		pp = _pedigrees.insert(new Pedigree(currentFamily,tableIndex));
		if(pp.second){
			for(int i=0;i<familyIdColumn->getNumberOfRows();i++) { 
				if(currentFamily.compare(familyIdColumn->get(i)) == 0){
					(*pp.first)->addIndividual(individualIdColumn->get(i),motherIdColumn->get(i),fatherIdColumn->get(i),genderColumn->get(i),i,tableIndex,pedigreeTable);
				}
			}
		}
		index++;
	}
	
	// Set the core optional fields on the individuals
	_setCoreOptionalFields(p_pedigreeTable);
	
	_establishConnections();
	
	bool dobPresent = false;
	if(pedigreeTable.getDOBColumnIndex() != DataTable::COLUMN_IS_MISSING){
		_checkParentChildDOB();
		dobPresent = true;
	}
	_determineFoundingGroups();
	bool sortFieldPresent = false;
	if(sortField != std::string("") && sortField != pedigreeTable.labels.DOBField){
		// Check to see if the field actually exists in the data table
		if(pedigreeTable.columnExists(sortField)){
			_computeWidths(sortField);
			sortFieldPresent = true;
		}else{
			Warning("PedigreeSet::addPedigreesFromDataTable()",
			        "Field '%s' specified for sorting the siblings does not exist in the Pedigree Table. Default ordering will be used.",
			        sortField.c_str()
			);
		}
	}
	if(!sortFieldPresent){
		if(dobPresent){
			std::cout << "Siblings are ordered by DOB." << std::endl;
			_computeWidths(pedigreeTable.labels.DOBField,true);
			DrawingMetrics::setDisplayBirthOrder(true);
		}else{    
			std::cout << "Default ordering of siblings." << std::endl;
			_computeWidths(std::string(""));
		}
	}
	std::cout << "End of addPedigreesFromDataTable" << std::endl;
	
}

//
// draw():
//
void PedigreeSet::draw(const DataTable *const pedigreeTable){
	
	std::cout << "Start of draw" << std::endl;
	//
	// Make a note if there is only one pedigree to be drawn: 
	//
	if( _pedigrees.size()==1 ) DrawingMetrics::setHasOnlyOnePedigreeState(true);
	
	//
	// Instantiate a LabelSet object for a pedigree set:
	//
	LabelSet labelSet(pedigreeTable);
	std::cout << "No of labels: " << labelSet.getNumberOfLabels() << std::endl;
	
	//
	// Iterate over the pedigrees and draw them:
	//
	std::set<Pedigree*,comparePedigrees>::const_iterator it = _pedigrees.begin();
	while(it != _pedigrees.end()){
		(*it)->draw(&labelSet);
		++it;
	}
	std::cout << "End of draw" << std::endl;
	
}

//
// display():
//
void PedigreeSet::display(){
	
	std::set<Pedigree*,comparePedigrees>::const_iterator it = _pedigrees.begin();
	while(it != _pedigrees.end()){
		std::cout << "Pedigree Id " << (*it)->getId() << " Table Id" << (*it)->getTableId() << std::endl; 
		// Display Individuals in Pedigree Id
		(*it)->display();
		it++;
	}
	
}
