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
// DataTable.cpp
//

#include "DataTable.h"
#include "Compressor.h"
#include "RandomGenerator.h"
#include "ColumnClassifier.h"
#include "RandomSampleSizes.h"

#include <iostream>
#include <cctype>

#include "utility.h"

///////////////////////////////////
//
// CONST STATIC MEMBERS:
//
///////////////////////////////////

FieldLabels DataTable::labels;

///////////////////////////////////
//
// CONSTRUCTORS/DESTRUCTORS:
//
///////////////////////////////////

//
// DataTable constructor:
//
DataTable::DataTable(TableParser &parser){
	//
	// set optional core column indices to default:
	//
	_affectedColumnIndex    = COLUMN_IS_MISSING;
	_deceasedColumnIndex    = COLUMN_IS_MISSING;
	_dobColumnIndex         = COLUMN_IS_MISSING;
	_dzTwinColumnIndex      = COLUMN_IS_MISSING;
	_mzTwinColumnIndex      = COLUMN_IS_MISSING;
	_probandColumnIndex     = COLUMN_IS_MISSING;
	_sampledColumnIndex     = COLUMN_IS_MISSING;
	_superscriptColumnIndex = COLUMN_IS_MISSING;
	_consultandColumnIndex  = COLUMN_IS_MISSING;
	_carrierColumnIndex     = COLUMN_IS_MISSING;
	
	//
	// Set ColorSeriesStack, blackAndWhiteSeriesStack pointer to null
	// by default:
	//
	_colorSeriesStack = 0;
	_blackAndWhiteSeriesStack = 0;
	//
	// Set number of rows and columns:
	//
	_rows    = parser.getNumberOfRows();
	_columns = parser.getColumnCount();
	
	// initialize column types to UNCLASSIFIED
	_finalTypes.insert(_finalTypes.end(),_columns,UNCLASSIFIED);
	
	const std::vector<std::string> * pTitles        = parser.getTitles();
	const std::vector<std::string> * pElements      = parser.getContents();
	const std::vector<char>        * pDeclaredTypes = parser.getTypes();
	
	//
	// setTypes() examines declared types and column names
	// in order to set DATATYPE types:
	//
	_setDeclaredTypesFromParser(pDeclaredTypes);
	_determineTableType(pTitles);
	_classifyRemainingColumns(pElements,pTitles);
	
	unsigned columnIndex = 0;
	unsigned rowIndex;
	
	const std::vector<std::string> &elements      = ( *pElements      );
	const std::vector<std::string> &titles        = ( *pTitles        );
	
	std::pair< std::set<DataColumn *,ltstr>::iterator , bool > thisPair;
	
	for(unsigned i=0; i<_columns; i++){
		
		if(_finalTypes[i]==ALLELE){
			
			///////////////////////////////////////
			//
			// SPECIAL PAIRED ALLELE COLUMNS CASE:
			//
			///////////////////////////////////////
			//
			// Throw exception if the subsequent column is not a matching allele column:
			//
			if(i==_columns-1 || _finalTypes[i+1]!=ALLELE || titles[i]!=titles[i+1]){
				
				throw Exception("DataTable::DataTable()","Because column %i. %s is declared as an ALLELE column, the subsequent column must be a matching ALLELE column.",(i+1),titles[i].c_str() );
				
			}
			//
			// Insert into columnSet both columns,
			// making both columns genotypes column,
			// and adding "_2nd" to the second column:
			//
			thisPair  = _columnSet.insert(new DataColumn(titles[i]              , GENOTYPE , _rows, i   ));
			std::string SecondAlleleColumnTitle = titles[i];
			SecondAlleleColumnTitle += "_2";
			std::pair< std::set<DataColumn *,ltstr>::iterator , bool > nextPair;
			nextPair  = _columnSet.insert(new DataColumn(SecondAlleleColumnTitle, GENOTYPE , _rows, i+1 ));
			//
			// Check for duplicated column titles:
			//
			if(!thisPair.second) throw Exception("DataTable::DataTable()","Column %i. %s appears to have the same name as a previous column.",(i+1),titles[i].c_str()            );
			if(!nextPair.second) throw Exception("DataTable::DataTable()","Column %i. %s appears to have the same name as a previous column.",(i+2),SecondAlleleColumnTitle.c_str() );
			//
			// Add both also into columnVector:
			//
			_columnVector.push_back(thisPair.first);
			_columnVector.push_back(nextPair.first);
			
			rowIndex=0;
			std::string genotypeFromAlleles;
			for(unsigned j=columnIndex; j<elements.size(); j+=_columns){
				
				genotypeFromAlleles =  elements[j];
				genotypeFromAlleles += "/";
				genotypeFromAlleles += elements[j+1];
				
				(*thisPair.first)->setRow(rowIndex,genotypeFromAlleles);
				(*nextPair.first)->setRow(rowIndex,genotypeFromAlleles);
				rowIndex++;
				
			}
			//
			// Skip over the next column since we've already taken it as an allele column:
			//
			columnIndex+=2;
			i++;
			
		}else{
			////////////////
			//
			// NORMAL CASE:
			//
			////////////////
			//
			// Insert into columnSet:
			//
			thisPair  = _columnSet.insert(new DataColumn(titles[i],_finalTypes[i], _rows, i));
			//
			// Check for the rare but possible case of 
			// duplicated column titles:
			//
			if(!thisPair.second){
				//
				// Not allowed:
				//
				throw Exception("DataTable::DataTable()","Column %i. %s appears to have the same name as a previous column.",(i+1),titles[i].c_str() );
			}
			//
			// ... and also into columnVector:
			//
			_columnVector.push_back(thisPair.first);
			
			rowIndex=0;
			for(unsigned j=columnIndex; j<elements.size(); j+=_columns){
				
				(*thisPair.first)->setRow(rowIndex,elements[j]);
				rowIndex++;
				
			}
			columnIndex++;
		}
		
	}
	
	//
	// setColorSeriesStack:
	//
	_setColorSeriesStack();
	
}

//
// Destructor:
//
DataTable::~DataTable()
{
	
	// std::cout << "~Datacolumn start" << _columnSet.size() << std::endl;
	
	//
	// Remove data columns:
	//
	std::set<DataColumn*,ltstr>::iterator it;
	for(it = _columnSet.begin(); it != _columnSet.end() ; ++it ){
		delete *it;
	}
	
	//
	// If there is a ColorSeriesStack, remove it:
	//
	if(_colorSeriesStack) delete _colorSeriesStack;
	if(_blackAndWhiteSeriesStack) delete _blackAndWhiteSeriesStack;
	
	//std::cout << "~Datacolumn end" << std::endl;
	
	return;
}




///////////////////////////////////
//
// PRIVATE METHODS:
//
///////////////////////////////////

//
// _setDeclaredTypesFromParser:
//
void DataTable::_setDeclaredTypesFromParser(const std::vector<char> *pDeclaredTypes){
	
	
	// Set declared types from parser:
	for(unsigned k=0; k < _columns ; k++)
	{
		switch((*pDeclaredTypes)[k])
		{
		case 'S':	_finalTypes[k] = STRING;
				break;
		case 'N':	_finalTypes[k] = NUMBER;
				break;
		case 'X':	_finalTypes[k] = GENDER;
				break;
		case 'G':	_finalTypes[k] = GENOTYPE;
				break;
		case 'H':	_finalTypes[k] = HAPLOTYPE;
				break;
		case 'D':	_finalTypes[k] = DATE;
				break;
		case 'A':	_finalTypes[k] = ALLELE;
				break;
		case 'B':	_finalTypes[k] = BOOLEAN;
				break;
		default:	_finalTypes[k] = UNCLASSIFIED;
				break;
		}
	}
	
}

///
/// _determineTableType: This method determines the type of the data table.
///	This method also classifies the column types of known core columns
///
void DataTable::_determineTableType(const std::vector<std::string> *pTitles) {
	
	
	//
	// Variables used to determine the Table Type:
	//
	int pedigreecount   = 0;
	int geneticcount    = 0;
	int decomposedcount = 0;
	int resultscount    = 0;
	int allelecount     = 0;
	bool geneticPosition = false;
	int geneticPositionCount = 0;
	
	_tabletype = UNKNOWNTABLETYPE;           // reset table type
	
	//
	// For convenience, we declare a reference 
	// rather than use the pointer directly:
	//
	const std::vector<std::string> &titles        =  (*pTitles        );
	
	//
	// PART ONE:
	//
	// (1) Iterate over all column titles, and add up the number of "hits" 
	//     to determine which table type it is.
	//
	// (2) At the same time, set column types based on 
	//     core field names -- but don't yet set column 
	//     types for unrecognized fields.
	//
	// (3) If the Affected column is present, also push its column index
	//     onto the iconColumns vector.
	//
	// (4) If the IndividualId column is present, push its column index
	//     onto the labelColumns vector.
	//
	for(unsigned i = 0; i < _columns ; i++)
	{
		
		std::string tempString = stringToUpper(titles[i]);
		
		if(tempString == labels.FamilyIdField)
		{
			
			pedigreecount++;
			decomposedcount++;
			_finalTypes[i] = STRING;
			
		}
		else if(tempString == labels.IndividualIdField){
			
			pedigreecount++;
			decomposedcount++;
			_finalTypes[i] = STRING;
			//
			// Push this column as the default label
			// displayed on pedigree drawings:
			//
			_labelColumns.push_back(i);
			
		}
		else if(tempString == labels.FatherIdField || tempString == labels.MotherIdField)
		{
			
			pedigreecount++;
			_finalTypes[i] = STRING;
			
		}
		else if(tempString == labels.GenderField)
		{
			
			pedigreecount++;
			_finalTypes[i] = GENDER;
			
		}
		else if(tempString == labels.ChromosomeField)
		{
			
			geneticcount++;
			_finalTypes[i] = STRING;
			
		}
		else if(tempString == labels.MarkerNameField)
		{
			
			geneticcount++;
			decomposedcount++;
			allelecount++;
			_finalTypes[i] = STRING;
			
		}
		else if(tempString == labels.PositionField)
		{
			geneticcount++;
			resultscount++;
			geneticPosition = true;
			_finalTypes[i] = NUMBER;
		}
		else if(tempString == labels.PositionFemaleField || tempString == labels.PositionMaleField)
		{
			geneticcount++;
			geneticPositionCount++;
			_finalTypes[i] = STRING;
		}
		else if(tempString == labels.Allele1Field || tempString == labels.Allele2Field)
		{
			decomposedcount++;
			_finalTypes[i] = NUMBER;
		}
		else if(tempString == labels.Score1Field )
		{
			resultscount++;
			_finalTypes[i] = NUMBER;
		}
		else if(tempString == labels.FrequencyField || tempString == labels.AlleleField)
		{
			allelecount++;
			_finalTypes[i] = NUMBER;
		}
		else if(tempString == labels.DOBField)
		{
			
			// Optional core field
			_finalTypes[i]  = DATE;
			_dobColumnIndex = i;
		}
		else if(tempString == labels.MZTwinField || tempString == labels.DZTwinField)
		{
			
			// Optional core field
			_finalTypes[i] = STRING;
			if(tempString == labels.MZTwinField) _mzTwinColumnIndex = i;
			else                                 _dzTwinColumnIndex = i;
		}
		else if(tempString.substr(0,labels.AffectedField.size()) == labels.AffectedField)
		{
			//
			// NOTA BENE: *Any* field that starts with the
			// Affected field's prefix --i.e., "AFFECTED"--
			// is treated as an affection field and pushed onto
			// the _iconColumns stack:
			//
			
			// Optional core field
			_finalTypes[i] = AFFECTED;
			//
			// The first seen affected column becomes the default:
			//
			if(_affectedColumnIndex==COLUMN_IS_MISSING) _affectedColumnIndex = i;
			//
			// Push this column index as a
			// column to be used for
			// shading the icons on pedigree
			// drawings:
			//
			_iconColumns.push_back(i);
			
		}
		else if(tempString == labels.SampledField)
		{
			
			// Optional core field
			_finalTypes[i] = SAMPLED;
			_sampledColumnIndex = i;
			
		}
		else if(tempString == labels.SuperscriptField)
		{
			
			_finalTypes[i] = STRING;
			_superscriptColumnIndex = i;
			
		}
		else if(tempString == labels.DeceasedField)
		{
			
			// Optional core field
			_finalTypes[i] = LIVINGDEAD;
			_deceasedColumnIndex = i;
			
		}
		else if(tempString == labels.ProbandField)
		{
			
			// Optional core field
			_finalTypes[i] = PROBAND;
			_probandColumnIndex = i;
			
		}
		else if(tempString == labels.ConsultandField)
		{
			
			// Optional core field:
			// Treat consultand just like proband:
			_finalTypes[i] = CONSULTAND;
			_consultandColumnIndex = i;
			
		}
		else if(tempString == labels.CarrierField)
		{
			
			// Optional core field:
			_finalTypes[i] = CARRIER;
			_carrierColumnIndex = i;
			
		}
	}
	
	
	//
	// PART TWO:
	//
	// If any of the counts match up to the given table type, 
	// set the table type accordingly:
	//
	if(pedigreecount == labels.PEDIGREE_TABLE_CORE_COLUMN_COUNT)             _tabletype = PEDIGREE;
	else if(decomposedcount == labels.DECOMPOSED_TABLE_CORE_COLUMN_COUNT)    _tabletype = DECOMPOSED;
	else if(allelecount == labels.ALLELE_FREQUENCY_TABLE_CORE_COLUMN_COUNT)  _tabletype = ALLELEFREQUENCY;
	else if(resultscount == labels.RESULTS_TABLE_CORE_COLUMN_COUNT)          _tabletype = RESULTS;
	else if(geneticcount > labels.GENETIC_TABLE_CORE_COLUMN_MINIMUM && 
	        geneticcount < labels.GENETIC_TABLE_CORE_COLUMN_MAXIMUM)
	{
		if(geneticPosition)
		{
			_tabletype = GENETICMAP;
		}
		else if(!geneticPosition && 
		         geneticPositionCount == labels.GENETIC_TABLE_CORE_COLUMN_MINIMUM)
		{
			_tabletype = GENETICMAP;
		}
	}
	
	
}

//
// _classifyRemainingColumns
//
void DataTable::_classifyRemainingColumns(const std::vector<std::string> *pElements,
	                                  const std::vector<std::string> * pTitles   ){
	
	unsigned i,j,k;
	//
	// For syntactical convenience, a reference 
	// to the elements is used:
	//
	const std::vector<std::string> &elements      =  (*pElements      );
	
	RandomGenerator  randomGenerator(RandomGenerator::PSEUDO_RANDOM);
	ColumnClassifier columnClassifier;
	
	// DEBUG:
	//std::cout << "DEBUG: STT in _classifyRemainingColumns" << std::endl;
	
	//
	// Use the column classifier class 
	// to determine the types of the remaining
	// columns that are not yet classified:
	//
	for(j=0; j<_columns; j++){
		
		//
		// If the column has already been classified 
		// based on the header labels do not reclassify:
		//
		
		// DEBUG:
		//std::cout << "DEBUG: Column " <<(j+1)<<". "<<(*pTitles)[j]<<" has column type "<< _finalTypes[j] << std::endl;
		
		if(!(_finalTypes[j]==UNCLASSIFIED || _finalTypes[j]==MISSING))  continue;
		
		//
		// First, try to classify by picking a small
		// random sample:
		//
		for(i=0;i<GENERAL_RANDOM_SAMPLE_SIZE;i++){
			//
			// Draw a random row from the table:
			//
			
			k = randomGenerator.getIntegerInRange( 0 , _rows-1 );
			columnClassifier.scan(elements[j+_columns*k]);
			
		}
		//
		// Now classify the column:
		//
		_finalTypes[j] = columnClassifier.classify();
		
		// DEBUG:
		//std::cout << "Column " << (j+1) << ". " << (*pTitles)[j] << " is " << _finalTypes[j] << " after Random Sample " << std::endl;
		
		//
		// If the column could not be classified
		// from random draws, then we read the entire
		// column.  This can occur with very sparsely
		// populated columns:
		//
		if(_finalTypes[j] == MISSING){
			
			columnClassifier.resetCounters();
			
			for(i=0; i<_rows; i++){
				columnClassifier.scan(elements[j+_columns*i]);
			}
			
			_finalTypes[j] = columnClassifier.classify();
			
			//
			// Warn user if still unclassified:
			//
			if(_finalTypes[j] == MISSING){
				
				Warning("DataTable::_classifyRemainingColumns()",
				        "Column %1$i. %2$s appears to be completely missing.",
				         (j+1),
				         (*pTitles)[j].c_str()
				);         
				
			}
			// DEBUG:
			//std::cout << "Column " << (j+1) << ". " << (*pTitles)[j] << " is " << _finalTypes[j] << " after Full Sample " << std::endl;
		}
		
		//
		// reset counters before scanning the next column:
		//
		columnClassifier.resetCounters();
		
	}
	
}

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// addColumn: add a column to the table
//
void DataTable::addColumn( DataColumn *column){
	
	// insert the DataColumn into the table:
	std::pair< std::set<DataColumn *,ltstr>::iterator , bool > thisPair = _columnSet.insert( column );
	// push the column name into the columnVector: the index tells us the column's physical position:
	_columnVector.push_back( thisPair.first );
	
}

///
/// getColumnOrdinal(): Returns the columns physical position.
///
unsigned DataTable::getColumnOrdinal ( const char *name ) const{
	
	std::set<DataColumn *,ltstr>::const_iterator p=_columnSet.find(new DataColumn(name));
	if(p!=_columnSet.end()){
		return (*p)->getOrdinal();
	}else{
		throw Exception("dataTable::getColumnOrdinal()","Column does not exist.");
	}
	
}

///
/// getColumnName(): Returns the name of the column at physical position 'ordinal'.
///
std::string DataTable::getColumnName ( unsigned ordinal ) const{
	
	
	if( ordinal >= _columnSet.size() ){
		throw Exception("dataTable::getColumnName()","Ordinal is out of range");
	}
	
	std::set<DataColumn *,ltstr>::iterator it = _columnVector[ordinal];
	
	return (*it)->getName();
	 
}

///
/// getNumberOfColumns(): Returns the number of columns in the table.
///
unsigned DataTable::getNumberOfColumns( void ) const {
	
	return _columnSet.size(); 
	
}

///
/// getColumn(): Get a column by index/ordinal.
///
DataColumn *DataTable::getColumn( unsigned ordinal ) const{
	
	if( ordinal >= _columnSet.size() ){
		throw Exception("DataTable::getColumn()","Ordinal is out of range");
	}
	
	const std::set<DataColumn *,ltstr>::iterator it = _columnVector[ordinal];
	
	return (*it);
	 
}

///
/// getColumn(): Get a column by name.
///
DataColumn *DataTable::getColumn(const std::string &name ) const{
	
	DataColumn* tempdc = new DataColumn(stringToUpper(name));
	std::set<DataColumn *,ltstr>::const_iterator p=_columnSet.find(tempdc);
	if(p != _columnSet.end()){
		delete tempdc;
		return (*p);
	}else{
		delete tempdc;
		std::cout << name << std::endl;
		throw Exception("DataTable::getColumn()","Column %s does not exist.",name.c_str());
	}
	
}

//
// deleteFrontColumn:
//
void DataTable::deleteFrontColumn() {
	
	std::set<DataColumn*,ltstr>::const_iterator pit = _columnSet.begin();
	delete *pit;
	_columnSet.erase(pit);
	
}

///
/// columnExists(): Check if a column exists in a data table by searching the table header.
///
bool DataTable::columnExists(const std::string &name) const{
	
	DataColumn* tempdc = new DataColumn(stringToUpper(name));
	
	std::set<DataColumn *,ltstr>::const_iterator p=_columnSet.find(tempdc);
	
	delete tempdc;
	
	return (p!= _columnSet.end());
	
}


///
/// toggleColumnsForPedigree(): User can specify any number of columns to be displayed on the pedigree.
/// Mark all such columns for output display, if they exist.
///
void DataTable::toggleColumnsForPedigree(const std::vector<std::string> &columns){
	
	// Since the labels are specified by the user clear the vector  that contains
	// the default label 'Individual Id'
	_labelColumns.clear();
	for(unsigned i=0;i<columns.size();i++){
		if(columnExists(columns[i])){
			DataColumn* dc = getColumn(stringToUpper(columns[i]).c_str());
			dc->setShowOnPedigree(true);
			_labelColumns.push_back(dc->getOrdinal());
		}else Warning("DataTable::toggleColumnsForPedigree","Could not find column %s to show on the pedigree.",columns[i].c_str());
	}
	
}

///
/// getNumberOfShowOnPedigreeColumns(): Get number of data columns that will be displayed on the pedigree output.
///
unsigned DataTable::getNumberOfShowOnPedigreeColumns() const{
	
	return _labelColumns.size();
	
}

//
// getDataAtIndex:
//
Data* DataTable::getDataAtIndex(const std::string name, unsigned long index) const{
	
	return getColumn(name)->getDataAtIndex(index);
	
}

//
// getDataAtIndex:
//
Data* DataTable::getDataAtIndex(unsigned columnIndex, unsigned long index) const{
	
	return getColumn(columnIndex)->getDataAtIndex(index);
	
}


//
// display:
//
void DataTable::display(){
	
	//std::cout << "\nTABLE URL=" << _url << " FN=" << _fileName << " :\n"; 
	std::cout << "\nTYPE        : " << getTableTypeAsString()  << "\n";
	std::cout << "COLUMN COUNT: " << _columns << "\n";
	std::cout << "ROW    COUNT: " << _rows << "\n\n";
	
	//
	// _columnVector is a vector of iterators from the set:
	// std::vector< std::set<DataColumn *,ltstr>::iterator >
	//
	for(unsigned i=0;i<_columns;i++){
		
		(*_columnVector[i])->printData();
		
	}
	
}

//
// printInputTableAsTabDelimited:
//
void DataTable::printInputTableAsTabDelimited(std::string filename) const{
	
	std::ofstream inputfile(filename.c_str());
	if(inputfile.is_open()){
		
		// Print the labels first:
		unsigned i,j;
		for(i=0;i<_columns-1;i++){
			inputfile << (*_columnVector[i])->getName() << "\t";
		}
		inputfile << (*_columnVector[i])->getName() << std::endl;
		// Print the data by rows
		for(j=0;j<_rows;j++){
			for(i=0;i<_columns-1;i++){
				inputfile << (*_columnVector[i])->get(j) << "\t";
			}
			inputfile << (*_columnVector[i])->get(j) << std::endl;
		}
	}
}

//
// getTableTypeAsString()
//
std::string DataTable::getTableTypeAsString(void) const {
	
	std::string type;
	switch(_tabletype){
	case PEDIGREE:
		type="pedigree";
		break;
	case GENETICMAP:
		type="genetic map";
		break;
	case DECOMPOSED:
		type="decomposed";
		break;
	case RESULTS:
		type="results";
		break;
	case ALLELEFREQUENCY:
		type="allele frequency";
		break;
	case UNKNOWNTABLETYPE:
		type="unknown";
		break;
	}
	return type;
	
}

///
/// getIconColumnCount(): Get a count of number of data fields that will be displayed on the icon.
/// By default all the fields that have 'Affected' as the prefix will be used to display on the icon.
///
unsigned DataTable::getIconColumnCount(void) const{
	return _iconColumns.size();
}

//
// getIconColumnIndex
//
unsigned DataTable::getIconColumnIndex(unsigned nth) const{
	return _iconColumns[nth];
}

//
// getLabelColumnIndex
//
unsigned DataTable::getLabelColumnIndex(unsigned nth) const{
	return _labelColumns[nth];
}

///
/// getIconColumnVector(): Return a pointer to the _iconColumns vector.
///
const std::vector<unsigned> * DataTable::getIconColumnVector(void) const{
	
	return & _iconColumns;
	
}

///
/// getlabelColumnVector(): Return a pointer to the _labelColumns vector.
///
const std::vector<unsigned> * DataTable::getLabelColumnVector(void) const{
	
	return & _labelColumns;
	
}

//
// getColorSeriesStack
//
ColorSeriesStack * DataTable::getColorSeriesStack(void) const{
	
	return _colorSeriesStack;
	
}

//
// getColorSeriesFromStack
//
ColorSeries *DataTable::getColorSeriesFromStack(unsigned nth) const{
	
	if(_colorSeriesStack) return _colorSeriesStack->get(nth);
	else                  return 0;
	
}

//
// getBlackAndWhiteSeriesFromStack
//
ColorSeries *DataTable::getBlackAndWhiteSeriesFromStack(unsigned nth) const{
	
	if(_blackAndWhiteSeriesStack) return _blackAndWhiteSeriesStack->get(nth);
	else                          return 0;
	
}


//
// _setColorSeriesStack(void);
//
void DataTable::_setColorSeriesStack(void){
	
	if(_iconColumns.size()){
		std::cout << " iconColumns size " << _iconColumns.size() << std::endl;
		//
		// Passing true to the ColorSeriesStack 
		// constructor creates bichromatic color series:
		//
		if(!_colorSeriesStack) _colorSeriesStack = new ColorSeriesStack(ColorSeriesStack::BICHROMATIC);
		if(!_blackAndWhiteSeriesStack) _blackAndWhiteSeriesStack = new ColorSeriesStack(ColorSeriesStack::BLACKANDWHITE);
		
		for(unsigned int i=0;i<_iconColumns.size();i++){
			
			unsigned levels = getColumn(_iconColumns[i])->getUniqueList()->getAllLevels();
			_colorSeriesStack->add(levels);
			_blackAndWhiteSeriesStack->add(levels);
			
		}
	}
	
}

