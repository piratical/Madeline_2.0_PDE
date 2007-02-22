/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Adrian Marrs and Ritu Khanna
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
// DataColumn:
//

#include "DataColumn.h"
#include "utility.h"

#include <iostream>

////////////////////////////////////////
//
// CONSTRUCTORS/DESTRUCTORS
//
////////////////////////////////////////

DataColumn::DataColumn(const std::string &name,DATATYPE columnType, unsigned long rows,unsigned ordinal ){
	 
	_name=name; 
	_columnType = columnType;
	_rows = rows;
	_ordinal=ordinal;
	_showOnPedigree = false;
	_uniqueList = 0;
	
	//
	// Allocate the elements of the vector:
	//
	switch(_columnType){
	case BOOLEAN:
		for(unsigned int k=0; k < _rows; k++) _data.push_back(new Boolean());
		break;
	case DATE:
		for(unsigned int k=0; k < _rows; k++) _data.push_back(new Date());
		break;
	case GENDER:
		for(unsigned int k=0; k < _rows; k++) _data.push_back(new Gender());
		break;
	case GENOTYPE:
		for(unsigned int k=0; k < _rows; k++) _data.push_back(new Genotype());
		break;
	case HAPLOTYPE:
		for(unsigned int k=0; k < _rows; k++) _data.push_back(new Haplotype());
		break;
	case NUMBER:
		for(unsigned int k=0; k < _rows; k++) _data.push_back(new Number());
		break;
	case AFFECTED:
		for(unsigned int k=0; k < _rows; k++) _data.push_back(new Affected());
		break;
	case LIVINGDEAD:
		for(unsigned int k=0; k < _rows; k++) _data.push_back(new LivingDead());
		break;
	case SAMPLED:
		for(unsigned int k=0; k < _rows; k++) _data.push_back(new Sampled());
		break;
	case PROBAND:
		for(unsigned int k=0; k < _rows; k++) _data.push_back(new Proband());
		break;
	case TWIN:
		for(unsigned int k=0; k < _rows; k++) _data.push_back(new Twin());
		break;
	//
	// UNCLASSIFIED / MISSING CASES:
	//
	case ALLELE:
		//
		// ALLELE columns from a Parser are now converted
		// into GENOTYPE columns before DataColumn ever sees
		// anything, so this should really never occur:
		//
		Warning("DataColumn::DataColumn()",
		        "NOTA BENE: Allele column will be treated as a string column."
		);
		
	case STRING:
	case ANY:
	case UNCLASSIFIED:
	case MISSING:
	default:
		for(unsigned int k=0; k < _rows; k++) _data.push_back(new String());
		
		break;
	}
	
}

DataColumn::DataColumn(){
	
	_name="";
	_columnType = ANY;
	_rows = 0;
	_ordinal=0;
	_showOnPedigree = false;
	_uniqueList = 0;
	
}

//
// Destructor:
//
DataColumn::~DataColumn(){
	
	//
	// Delete the data elements themselves: 
	//
	
	std::vector<Data*>::iterator it;
	for(it = _data.begin(); it!= _data.end();it++) delete (*it);
	
	
	//
	// Delete the uniqueList if it exists:
	//
	if(_uniqueList) delete _uniqueList;
	
}


////////////////////////////////////////
//
// PUBLIC METHODS:
//
////////////////////////////////////////

//
// set():
//
void DataColumn::set(std::vector<std::string> & vec){
	
	//
	// Account for the possibility that the 
	// vector could be shorter or longer than _rows:
	// 
	unsigned int rows = ( vec.size() < _rows )? vec.size():_rows;
	
	for(unsigned int i=0;i< rows;i++){
		
		_data[i]->set(vec[i]);
		
	}
}

//
// setRow():
//
void DataColumn::setRow( unsigned long row , const std::string&  value){
	
	if( row >= _rows ){
		
		Warning("DataColumn::setRow()",
		        "Cannot set row %1$i because the column only has %2$i elements",
		         row+1,
		         _rows
		);
		
		return;
	}
	// Get here if it is OK to assign:
	_data[row]->set(value);
	
}

///
/// get: Get data at index as a string.
///
std::string DataColumn::get( unsigned long i) const{
	
	return  _data[i]->get();
	
}

///
/// getDataAtIndex:
///
Data* DataColumn::getDataAtIndex( unsigned long i) const{
	
	return  _data[i];
	
}

//
// setShowOnPedigree:
//
void DataColumn::setShowOnPedigree(bool value){
	
	_showOnPedigree = value;
	
}

//
// getShowOnPedigree:
//
bool DataColumn::getShowOnPedigree(void) const{
	
	return _showOnPedigree;
	
}

//
// setShowOnIcon:
//
void DataColumn::setShowOnIcon(bool value){
	
	_showOnIcon = value;
	
}

//
// getShowOnIcon:
//
bool DataColumn::getShowOnIcon(void) const{
	
	return _showOnIcon;
	
}

///
/// getUniqueList(): Creates a list of unique data values present in the data column.
/// This cannot be a const method because it needs
/// to create the private _uniqueList if not yet present:
/// 
const UniqueList *const DataColumn::getUniqueList(void){
	
	//
	// If the list does not yet exist,
	// create it:
	//
	if( ! _uniqueList ){
		std::string listName="Unique values in ";
		listName+= _name;
		_uniqueList=new UniqueList(listName);
		//
		// Iterate over all the entries in
		// the Data column:
		//
		for(unsigned long i=0; i< _rows; i++){
			_uniqueList->insert( _data[i] );
		}
		//
		// Set the ordinals:
		//
		_uniqueList->assignOrdinals();
	}
	
	
	//
	// return a const pointer to the list (const):
	//
	return _uniqueList;
	
}

//
// getColumnTypeAsString():
//
std::string DataColumn::getColumnTypeAsString(void) const{
	
	std::string type;
	
	switch(_columnType){
		
	case ANY:
		type="ANY";
		break;
	case BOOLEAN:
		type="BOOLEAN";
		break;
	case DATE:
		type="DATE";
		break;
	case GENDER:
		type="GENDER";
		break;
	case ALLELE:
		type="ALLELE";
		break;
	case GENOTYPE:
		type="GENOTYPE";
		break;
	case HAPLOTYPE:
		type="HAPLOTYPE";
		break;
	case NUMBER:
		type="NUMBER";
		break;
	case STRING:
		type="STRING";
		break;
	case AFFECTED:
		type="AFFECTED";
		break;
	case UNCLASSIFIED:
		type="UNCLASSIFIED";
		break;
	case MISSING:
		type="MISSING";
		break;
	case SAMPLED:
		type="SAMPLED";
		break;
	case PROBAND:
		type="PROBAND";
		break;
	case LIVINGDEAD:
		type="LIVINGDEAD";
		break;
	case TWIN:
		type="TWIN";
		break;
	default:
		type="UNRECOGNIZED_TYPE";
		break;
	}
	return type;
	
}

///
/// getCaseInvariantName() : Used for sorting and searching
///
std::string DataColumn::getCaseInvariantName( void ) const{
	
	return stringToUpper(_name);
	
}


// DEBUGGING:
//
// printData():
//
void DataColumn::printData( void ){
	
	// Show ordinal starting from 1 instead of from zero:
	std::cout << "COLUMN " << ( _ordinal+1 ) << " is called " << _name << "\n";
	std::cout << "TYPE  :" << getColumnTypeAsString() << "\n";
	std::cout << "ROWS  :" << _rows << "\n";
	
	const UniqueList *const uList = getUniqueList();
	std::cout << "This column has " << uList->getLevels() << " distinct non-missing levels.\n";
	std::cout << "There are " << uList->getCount() << " non-missing values and " << uList->getCountMissing() << " missing values.\n\n";
	
	for(unsigned long i=0; i< _rows; i++){
		std::cout << _data[i]->get() << "\t";
		if((i+1)%10==0) std::cout << std::endl;
		
	}
	std::cout << "\n" << std::endl;
}

//
// printData:
//
void DataColumn::printData( unsigned long i ){
	
	std::cout << _data[i]->get();
	
}
