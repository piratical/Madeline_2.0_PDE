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
// DataTable.h
//

#ifndef DATATABLE_INCLUDED
#define DATATABLE_INCLUDED

#include "FileTypeClassifier.h"
#include "DataColumn.h"

#include "TableParser.h"

#include "FieldLabels.h"

#include "ColorSeriesStack.h"

#include <set>
#include <string>
#include <vector>

/*!
 *	This class is used to store data tables that have been read in 
 *	from files. The files are read from a Parser class, classified, 
 *	and then have their columns classified.
 *	
 *	This class is used as a container for all types of data tables
 *	encountered in Madeline.  At the same time, some aspects of the
 *	class are specific to pedigree tables (the most common table type
 *	encountered in Madeline).  Due to the way that the table classi-
 *	fication code works, we felt it was better --without being very
 *	expensive-- to add some pedigree-table-specific functionality 
 *	directly into DataTable rather than have to break out separate 
 *	subclasses for each table type.
 *	
*/

class DataTable {
	
public:
	
	enum TableType { PEDIGREE, GENETICMAP, DECOMPOSED, RESULTS, ALLELEFREQUENCY, UNKNOWNTABLETYPE };
	static const int COLUMN_IS_MISSING = -1;
	
private:
	
	// "less" function for _columnSet:
	struct ltstr
	{
		bool operator()(DataColumn *c1, DataColumn *c2) const
		{
			//
			// Comparisons done without regard to case:
			//
			return c1->getCaseInvariantName() < c2->getCaseInvariantName();
		}
	};
	
	TableType _tabletype;
	
	//const char* _fileName;
	FileTypeClassifier _fileTypeClassifier;
	FileTypeClassifier::FILE_TYPE _fileType;
	const char* _fileTypeName;
	
	// Columns (Fields):
	// _columnSet provides lookup by name:
	std::set<DataColumn *,ltstr> _columnSet;
	
	// _columnVector provides lookup by index/ordinal:
	std::vector< std::set<DataColumn *,ltstr>::iterator > _columnVector;
	
	//
	// This is the vector of final types
	// which may differ from declared types:
	// 
	std::vector<DATATYPE> _finalTypes;
	
	//
	// Number of rows and columns:
	//
	unsigned long _rows; // number of rows
	unsigned _columns;   // number of columns
	
	//
	// Which columns are displayed?
	//
	std::vector<unsigned> _iconColumns;  // columns shown on icon;
	std::vector<unsigned> _labelColumns; // columns shown as labels or for output;
	
	ColorSeriesStack *_colorSeriesStack;
	ColorSeriesStack *_blackAndWhiteSeriesStack;
	
	//
	// Which optional core data columns exist?
	// 
	int _affectedColumnIndex;
	int _deceasedColumnIndex;
	int _dobColumnIndex;
	int _dzTwinColumnIndex;
	int _mzTwinColumnIndex;
	int _probandColumnIndex;
	int _sampledColumnIndex;
	int _superscriptColumnIndex;
	int _consultandColumnIndex;
	int _carrierColumnIndex;
	
	//
	// Private methods:
	//
	void _setDeclaredTypesFromParser(const std::vector<char> *pDeclaredTypes);
	void _determineTableType(const std::vector<std::string> *pTitles);
	void _classifyRemainingColumns(const std::vector<std::string> *pElements, const std::vector<std::string> *pTitles);
	void _setColorSeriesStack(void);
	
	
public:
	
	static FieldLabels labels;
	//
	// Constructors
	//
	DataTable(TableParser &parser);
	
	//
	// Destructor:
	//
	~DataTable();
	
	//
	// addColumn: add a column to the table
	//
	void addColumn( DataColumn *column);
	
	//
	// getColumnOrdinal(): returns the columns physical position:
	//
	unsigned getColumnOrdinal ( const char *name ) const;
	
	//
	// getColumnName(): returns the name of the column at physical position 'ordinal':
	//
	std::string getColumnName ( unsigned ordinal ) const;
	
	//
	// getNumberOfColumns(): Returns the number of columns in the table.
	//
	unsigned getNumberOfColumns( void ) const;
	
	
	//
	// get a column by index/ordinal:
	//
	DataColumn* getColumn( unsigned ordinal ) const;
	
	//                                                                                      
	// get a column by name:
	//
	DataColumn* getColumn(const std::string &name ) const;
	
	void deleteFrontColumn();
	void display();
	void printInputTableAsTabDelimited(std::string filename) const;
	
	// get the Table type
	const TableType getTableType( void ) const { return _tabletype; };
	std::string getTableTypeAsString(void) const;
	
	// get the number of rows
	const unsigned getNumberOfRows( void ) const { return _rows; };
	
	// check if a column exists:
	bool columnExists(const std::string &name) const;
	
	// toggle the columns for pedigree
	void toggleColumnsForPedigree(const std::vector<std::string> &columns);
	unsigned getNumberOfShowOnPedigreeColumns() const;
	Data* getDataAtIndex(const std::string name, unsigned long index) const;
	Data* getDataAtIndex(unsigned columnIndex, unsigned long index) const;

	// get the indices for optional core columns:
	// -1 if not present
	inline int getAffectedColumnIndex(void) const { return _affectedColumnIndex; }
	inline int getDeceasedColumnIndex(void) const { return _deceasedColumnIndex; }
	inline int getDOBColumnIndex(void) const { return _dobColumnIndex; }
	inline int getDZTwinColumnIndex(void)  const { return _dzTwinColumnIndex; }
	inline int getMZTwinColumnIndex(void) const { return _mzTwinColumnIndex; }
	inline int getProbandColumnIndex(void) const { return _probandColumnIndex; }
	inline int getSampledColumnIndex(void) const { return _sampledColumnIndex; }
	inline int getSuperscriptColumnIndex(void) const { return _superscriptColumnIndex; }
	inline int getConsultandColumnIndex(void)  const { return _consultandColumnIndex; }
	inline int getCarrierColumnIndex(void)     const { return _carrierColumnIndex; }
	
	//
	// getIconColumnCount
	//
	unsigned getIconColumnCount(void) const;
	//
	// getIconColumnIndex
	//
	unsigned getIconColumnIndex(unsigned nth) const;
	//
	// getLabelColumnIndex
	//
	unsigned getLabelColumnIndex(unsigned nth) const;
	
	const std::vector<unsigned> * getIconColumnVector(void) const;
	const std::vector<unsigned> * getLabelColumnVector(void) const;
	
	ColorSeriesStack *getColorSeriesStack(void) const;
	ColorSeries *getColorSeriesFromStack(unsigned nth) const;
	ColorSeries *getBlackAndWhiteSeriesFromStack(unsigned nth) const;
	
};

#endif
