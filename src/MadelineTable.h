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
// MadelineTable.h
//

#ifndef MADELINETABLE_INCLUDED
#define MADELINETABLE_INCLUDED

#include <iostream>
#include <fstream>

//#include <vector>
//#include <string>
#include "TableParser.h"

#include "LineInformation.h"
#include "ColumnOffset.h"
#include "Exception.h"
#include "RandomGenerator.h"
#include "RandomSampleSizes.h"

#include "Warning.h"

class MadelineTable : public TableParser{
	
private:
	
	char *_header;
	
	// RAW BUFFER, LINES, and BLOCKS:
	char *_buffer;              // Pointer to the beginning of the data buffer
	LineInformation *_lineInfo; // vector of LineInformation objects -- one object per line of file
	unsigned long _lines;       // Total number of lines in the file (HEADER, DATA, and EMPTY lines)
	int _blocks;                // Number of blocks should be 2, i.e., one HEADER and one DATA block
	
	// DATA BLOCK:
	const char   *_data  ;          // Pointer to the beginning of the rectangular DATA block 
	unsigned long _firstDataRow;    // first row of DATA block
	unsigned long _dataRows;        // Number of rows of data
	unsigned long _lastDataRow;     // last row of DATA block
	int           _length;          // Length of one row of data in the DATA block
	
	// HEADER BLOCK:
	unsigned long _firstHeaderRow;
	unsigned long _headerRows;
	unsigned long _lastHeaderRow;
	
	unsigned      _dataColumns;
	
	unsigned long *_fill;           // Vector of column fill rates: vector length is DATA block line length
	ColumnOffset *_columnOffset;  // Vector of ColumnOffsets objects -- one object per column
	
	void _readFile(const char *fileName);
	void _readString(const std::string inString);
	void _getLineCount( void );
	void _allocateLineInformationVector( void );
	void _assignLinePointers( void );
	void _determineNumberOfBlocks( void );
	void _assignRecordTypes( void );
	void _checkDataBlockRectangularity( void );
	void _calculateColumnFillRates( void );
	void _determineNumberOfColumns( void );
	void _determineFieldBoundaries( void );
	char *_readAndNullTerminateColumnName(char *beginning,const char *headerEnd, ColumnOffset &columnOffset);
	//                                                           
	// _readColumnType()                                         
	//                                                           
	// Allowed field types are: C -- character                   
	//                          N -- numeric                     
	//                          D -- date                        
	//                          G -- genotype                    
	//                          A -- allele                      
	//                          X -- character-type gender field 
	//                                                           
	char *_readColumnType(char *beginning,const char *headerEnd, ColumnOffset &columnOffset );
	void _parseHeader( void );
	//void _classifyColumns( void );
	//void _resolveDeclaredVersusDiscoveredColumnTypes( void );
	void _fillVectors();
	
public:
	
	// debug:
	void debug( void );
	
	// getters:
	int getBlockCount( void ){ return _blocks; }
	
	// setters:
	void setFile(const char *fileName);
	void setString(std::string inString);
	
	// Constructor:
	MadelineTable();
	
	// Destructor:
	~MadelineTable();
	
	// Get Methods:
	const char *getColumnName( unsigned columnIndex ){ return _columnOffset[ columnIndex ].getName(); }
	DATATYPE  getColumnType( unsigned columnIndex ){ return _columnOffset[ columnIndex ].getDiscoveredType(); }
	
	std::string getData(unsigned columnIndex, unsigned long rowIndex);
	
	//
	// TableParser base class now has these methods:
	//
	//unsigned getColumnCount( void ){ return _columns; };
	//unsigned long getNumberOfRows( void ){ return _dataRows; }
	
	void display(void);
	
};

#endif
