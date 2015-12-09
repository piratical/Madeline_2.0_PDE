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
// MadelineTable.cpp
//

#include "MadelineTable.h"

// DEBUG:
// #include <iostream>

#include "ColumnClassifier.h"
#include "utility.h"
#include <string>

///////////////////////////////////
//
// CONSTRUCTORS/DESTRUCTORS:
//
///////////////////////////////////

//
// Constructor:
//
MadelineTable::MadelineTable(){
	
	_buffer=0;
	_header=0;
	_data  =0;
	_lineInfo=0;
	_columnOffset=0;
	_fill=0;
	
	_lines=0;
	_length=0;
	
	_dataRows=0;
	_dataColumns=0;
	
	_headerRows=0;
	_firstHeaderRow=0;
	_lastHeaderRow=0;
	_firstDataRow=0;
	_lastDataRow=0;
	
}

// Destructor:
MadelineTable::~MadelineTable(){
	
	if(_buffer)       delete [] _buffer ;
	if(_lineInfo)     delete [] _lineInfo;
	if(_fill)         delete [] _fill;
	if(_columnOffset) delete [] _columnOffset;
	
	// DEBUG:
	//std::cerr << "Calling MadelineTable destructor ..." << std::endl;
}

///////////////////////////////////
//
// PRIVATE METHODS:
//
///////////////////////////////////

//
// _readFile(const char *fileName)
//
// Reads the data table into memory.
//
void MadelineTable::_readFile(const char *fileName){
	
	try{
		std::ifstream is;
		// Open file:
		is.open(fileName,std::ios::binary);
		// Get length of file:
		is.seekg(0,std::ios::end);
		_length=is.tellg();
		// *** DEBUG: ***
		// std::cerr << "_length = " << _length << " bytes ..." << std::endl;
		
		is.seekg(0,std::ios::beg);
		// allocate buffer:
		_buffer = new char [ _length + 1];
		// read entire block of data:
		is.read(_buffer,_length);
		
		_buffer[_length] = '\0';
		
		// Close file:
		is.close();
	}catch( ... ){
		throw Exception("MadelineTable::_readFile()","Unable to open or read file, “%s”.",fileName);
	}
}

//
// _readString:
//
void MadelineTable::_readString(const std::string inString){
	
	try{
		_length = inString.length();
		
		
		_buffer = new char [ _length + 1];
		
		strcpy(_buffer, inString.c_str());
		
		_buffer[_length] = '\0';
		
		
	}catch( ... ){
		throw Exception("MadelineTable::_readFile()","Unable to open or read file");
	}
}

//
// _getLineCount( void )
//
// Determines the number of lines in the data table.
//
void MadelineTable::_getLineCount( void ){
	
	char *b;
	
	// Count carriage returns:
	for(_lines=0, b=_buffer;*b;b++) if(*b=='\n') _lines++;
	
	// Last line may not have a carriage return at the end:
	b--;
	if(*b!='\n') _lines++;
	
	
}

//
// _allocateLineInformationVector( void )
//
// Allocates a vector of L
//
void MadelineTable::_allocateLineInformationVector( void ){
	
	_lineInfo = new LineInformation[ _lines ];
	
}

//
// _assignLinePointers:
//
void MadelineTable::_assignLinePointers( void ){
	
	unsigned long i; // line counter
	char *b;
	char *stt;
	
	for(i=0,stt=b=_buffer;*b;){
		//
		// 2015.12.09.ET: Now handles four possibilities: CR, LF, CR-LF, and LF-CR
		//
		if( *b=='\r' || *b=='\n' ){
			// Found end of a line:
			if(*b=='\r'){
				// '\r' case:
				*b++='\0';
				// Check for additional end-of-line character:
				if(*b=='\n'){
					*b++='\0';
				}
			}else{
				// '\n' case:
				*b++='\0';
				// Check for additional end-of-line character:
				if(*b=='\r'){
					*b++='\0';
				}
			}
			// Set pointer to beginning of this line
			_lineInfo[i].setBeginning(stt);
			// 2015.12.09.ET DEBUG:
			//std::cout << i << ". [" << _lineInfo[i].getLine() << "]" << std::endl;
			i++;
			// Set start for the next round:
			stt=b;
		}else{
			// Just increment pointer:
			b++;
		}
	}
	
}

//
// _determineNumberOfBlocks:
//
void MadelineTable::_determineNumberOfBlocks( void ){
	
	unsigned long i;
	
	for(i=_blocks=0;i< _lines;i++)
	{
		// skip empty lines at top:
		for(;i< _lines && _lineInfo[i].getType()==LineInformation::EMPTY_LINE;i++);
		
		
		// return if no more lines to process:
		if(i== _lines) return;
		// otherwise, increment block counter:
		_blocks++;
		// std::cerr << "CURRENT BLOCK: " << _blocks << std::endl;
		// now skip non-empty lines representing the block:
		for(;i< _lines && _lineInfo[i].getType()==LineInformation::DIRTY_LINE;i++);
		
	}
	
	//
	// Fail if block count is too high or too low:
	//
	if(_blocks<2){
		throw Exception("MadelineTable::_DetermineNumberOfBlocks()","File has less than two blocks of data");
	}
	if(_blocks>2){
		throw Exception("MadelineTable::_DetermineNumberOfBlocks()","File has more than two blocks of data");
	}
	
}

//
// _assignRecordTypes:
//
void MadelineTable::_assignRecordTypes( void ){
	
	unsigned i=0;
	if(getBlockCount()==2){
		// Skip empty lines at top:
		for(;i< _lines && _lineInfo[i].getType()==LineInformation::EMPTY_LINE;i++);
		// Store first header row index:
		_firstHeaderRow = i;
		// Now mark the non-empty lines as the HEADER block:
		for(;i< _lines && _lineInfo[i].getType()==LineInformation::DIRTY_LINE;i++){
			_lineInfo[i].setType(LineInformation::HEADER_LINE);
		}
		// Store last header row index and number of (filled) header rows:
		_headerRows = i - _firstHeaderRow;
		_lastHeaderRow = i-1;
	}
	// Now at or before the DATA block:
	// Skip any empty lines:
	for(;i< _lines && _lineInfo[i].getType()==LineInformation::EMPTY_LINE;i++);
	// First, store pointer to beginning of DATA block:
	_data     = _lineInfo[i].getLine();
	_firstDataRow = i;
	// Arbitrarily assume lines are of same length in DATA block: use one as standard:
	_length   = _lineInfo[i].getLength();
	// Mark the non-empty lines as the DATA block:
	for(;i< _lines && _lineInfo[i].getType()==LineInformation::DIRTY_LINE;i++){
		_lineInfo[i].setType(LineInformation::DATA_LINE);
	}
	// Set number of rows in the data table:
	_dataRows = i - _firstDataRow;
	_lastDataRow = --i;
	// DEBUG:
	// std::cerr << "*** FIRST HEADER ROW     : " << _firstHeaderRow << std::endl;
	// std::cerr << "*** LAST  HEADER ROW     : " << _lastHeaderRow << std::endl;
	// std::cerr << "*** NO. OF HEADER ROWS   : " << _headerRows << std::endl;
	// 
	// std::cerr << "*** NUMBER OF DATA ROWS: " << _dataRows   << std::endl;
	// std::cerr << "*** LENGTH OF DATA ROW : " << _length << std::endl;
	// std::cerr << "*** FIRST DATA ROW     : " << _firstDataRow << std::endl;
	// std::cerr << "*** LAST  DATA ROW     : " << _lastDataRow << std::endl;
}

//
// _checkDataBlockRectangularity:
//
void MadelineTable::_checkDataBlockRectangularity( void ){
	
	for(unsigned long i=_firstDataRow;i<=_lastDataRow;i++){
	
		if( _lineInfo[i].getLength()!= _length && _lineInfo[i].getType()!=LineInformation::EMPTY_LINE){
			throw Exception("MadelineTable::_CheckDataBlockRectangularity()","Lengths of rows in the DATA block are inconsistent");
		}
	}
}

//
// _calculateColumnFillRates:
//
void MadelineTable::_calculateColumnFillRates( void ){
	
	_fill = new unsigned long[ _length ];
	for(int j= 0; j< _length;) _fill[j++]=0;
	
	// Outer loop through the rows:
	for(unsigned long i= _firstDataRow; i<= _lastDataRow ; i++){
		// Inner loop through the columns in each DATA row:
		for(int j= 0; j< _length; j++){
			_fill[j]+= ( _lineInfo[i].getLine()[j] != ' ' );
		}
	}
	
	// DEBUG:
	//std::cerr << "FILL RATES:" << std::endl;
	//for(int j= 0; j< _length; j++) std::cerr << (j+1) << ". " << _fill[j] << std::endl;
	
}

//
// _determineNumberOfColumns:
//
void MadelineTable::_determineNumberOfColumns( void ){
	
	// Skip initial white space, if any:
	int j;
	for(j=0;j< _length && !_fill[j];j++);
	// Now count fields:
	for(_dataColumns=0;j< _length;){
		
		// Skip the filled part of the column:
		for(;j< _length && _fill[j];j++);
		// Increment the column counter:
		_dataColumns++;
		// Skip the empty part between columns:
		for(;j< _length && !_fill[j];j++);
		
	}
	// DEBUG:
	//std::cerr<< "*+*+ NUMBER OF COLUMNS: " << _dataColumns << std::endl;
}

//
// _determineFieldBoundaries( void )
//
void MadelineTable::_determineFieldBoundaries( void ){
	
	unsigned i; // indexes the field (column)
	int j; // indexes the byte column in the flat file DATA block
	int offset=0; // Offset to beginning of field
	
	// Allocate the vector of ColumnOffsets:
	_columnOffset = new ColumnOffset [ _dataColumns ];
	
	// Skip any white space preceding first field:
	for(j=0;j<_length && !_fill[j];j++);
	
	// i indexes fields, j indexes columns in flat file:
	for(i=0;j<_length;){
		
		// Set the field offset (0 for the first column):
		_columnOffset[i].setStart(offset);
		
		// Skip the filled part of the current field: 
		// end is actually the next byte after the end
		// of the filled portion:                     
		//                                            
		for(;j<_length && _fill[j];j++);
		
		_columnOffset[i].setEnd(j);
		
		// Skip the empty part of the field on the right margin:
		for(;j<_length && !_fill[j];j++);
		// J is now at the start of the next field:
		offset=j;
		// Increment the field counter:
		i++;
	}
	
	// DEBUG:
	//std::cerr << "COLUMN OFFSET INFORMATION\n";
	//for(i=0;i<_dataColumns;i++){
	//	std::cerr << (i+1) << ". STT=" << (_columnOffset[i].getStart()+1) << " END=" << (_columnOffset[i].getEnd()+1) << std::endl;
	//}
}

//
// _fillVectors:
//
void MadelineTable::_fillVectors()
{
	
	//
	// set the base class stuff:
	//
	_columns = _dataColumns;
	_rows    = _dataRows;
	
	for(unsigned long j=0;j< _rows;j++)
	{
		for(unsigned i=0;i< _columns;i++)
		{
			//
			// Create the element vector:
			//
			_element.push_back(getData(i,j));
			
		}
	}
	
	for(unsigned i=0;i< getColumnCount();i++)
	{
		//
		// Create the column type vector:
		//
		_columnType.push_back( _columnOffset[i].getDeclaredType() );
	}
	
}

//
// _readAndNullTerminateColumnName()
//
char *MadelineTable::_readAndNullTerminateColumnName(char *beginning,const char *headerEnd, ColumnOffset &columnOffset){
	
	char *stt;
	char *end;
	
	// Skip white space:
	for(stt=beginning;stt<headerEnd && (*stt==' ' || *stt=='\0' || *stt=='\n' || *stt=='\r' || *stt=='\t');stt++);
		
	// Continue to find the end of the column name string
	// which is not yet null-terminated:
	for(end=stt;end<headerEnd && !(*end==' ' || *end=='\0' || *end=='\n' || *end=='\r' || *end=='\t');end++);
	// Null terminate the column name:
	*end='\0';

	// The trimmed label is now set:
	columnOffset.setName(stt);
	
	_title.push_back( std::string(stt) );
	
	return end;
}

///                                                           
/// _readColumnType() : sets the declared type
///                                                           
/// Allowed field types are: C -- character                   
///                          N -- numeric                     
///                          D -- date                        
///                          G -- genotype                    
///                          A -- allele                      
///                          X -- character-type gender field 
/// enum COLUMNTYPE{ ALLELE, DATE, GENDER, GENOTYPE, HAPLOTYPE, MISSING, NUMBER, STRING, UNCLASSIFIED };
///
///
char *MadelineTable::_readColumnType(char *beginning,const char *headerEnd, ColumnOffset &columnOffset ){
	
	register char a;
	register char c;
	register char *b;

	/*                   */
	/* skip white space: */
	/*                   */
	for(b=beginning;b<headerEnd && (*b==' ' || *b=='\0' || *b=='\n' || *b=='\r' || *b=='\t');b++);
	/*                                                   */
	/* accept only a single character surrounded on both */
	/* sides by white space for field type:              */
	/*                                                   */
	a=*(b-1);
	c=*(b+1);
	if(   *b 
	   && (*b=='C' || *b=='N' || *b=='D' || *b=='G' || *b=='A' || *b=='X')
	   && (a==' ' || a=='\0' || a=='\n' || a=='\r' || a=='\t')
	   && (c==' ' || c=='\0' || c=='\n' || c=='\r' || c=='\t')
	){
	
		columnOffset.setDeclaredType(*b);
		return ++b;	/* return after the type */
	}else{	
		columnOffset.setDeclaredType('\0'); /* Indicates field type not found: UNCLASSIFIED */
		return --b; /* return before the type         */
	}
}

//
// _parseHeader( void )
//
void MadelineTable::_parseHeader( void ){
	
	if(getBlockCount()==2){
		
		char *b;
		unsigned i;
		char type;
		char *headerStart = _lineInfo[ _firstHeaderRow ].getMalleableLine();
		const char *headerEnd   = _lineInfo[ _lastHeaderRow ].getLineEnd();
		
		for(b=headerStart,i=0;b<headerEnd && i<_dataColumns;i++){
			// (re) initialize type:
			type='\0';
			// read the column name:
			b=_readAndNullTerminateColumnName(b,headerEnd,_columnOffset[i]);
			
			// Also read the column type:
			b=_readColumnType(b,headerEnd,_columnOffset[i]);
			
		}
		// UNFINISHED: WHAT IF DECLARATIONS ARE FEWER THAN COLUMNS ?
		
	}
	// UNFINISHED: WHAT IF ONLY ONE BLOCK? HOW DO WE DEAL WITH COLUMN NAMES?
	
}


///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// getData
//
std::string MadelineTable::getData(unsigned columnIndex, unsigned long rowIndex){
	
	std::string data; 
	const char *offset, *stt, *end, *s;
	
	// Offset is a pointer to the beginning of the row of data:
	offset = _lineInfo[ _firstDataRow + rowIndex ].getLine();
	
	
	stt = offset + _columnOffset[ columnIndex ].getStart();
	// Skip past any initial white space:
	for(;*stt && *stt==' ';stt++);
	
	// 2015.12.09.ET end is supposed to be one byte past end of data:
	end = offset + _columnOffset[ columnIndex ].getEnd();
	end--;
	// Skip past any terminal white space:
	for(;*end==' ';end--);
	// Copy data:
	for(s=stt;s<=end;data+=*s++);
	return data;
}

//
// setFile():
//
void MadelineTable::setFile(const char *fileName){
	
	_readFile(fileName);
	_getLineCount();
	_allocateLineInformationVector();
	_assignLinePointers();
	_determineNumberOfBlocks();
	_assignRecordTypes();
	_checkDataBlockRectangularity();
	_calculateColumnFillRates();
	_determineNumberOfColumns();
	_determineFieldBoundaries();
	_parseHeader();
	//
	// Now Handled downstream in DataTable instead: 
	//_classifyColumns();
	// This is basically obsoleted because DataTable now handles classification: 
	//_resolveDeclaredVersusDiscoveredColumnTypes();
	//
	_fillVectors();
	// DEBUG:
	//display();
}

//
// setString():
//
void MadelineTable::setString(std::string inString){
	
	_readString(inString);
	_getLineCount();
	_allocateLineInformationVector();
	_assignLinePointers();
	_determineNumberOfBlocks();
	_assignRecordTypes();
	_checkDataBlockRectangularity();
	_calculateColumnFillRates();
	_determineNumberOfColumns();
	_determineFieldBoundaries();
	_parseHeader();
	//
	// Now Handled downstream in DataTable instead: 
	//_classifyColumns();
	// This is basically obsoleted now because DataTable now handles classification: 
	//_resolveDeclaredVersusDiscoveredColumnTypes();
	//
	_fillVectors();
	// DEBUG:
	//display();
}


//
// display:
//
void MadelineTable::display(void)
{
	for(unsigned int i=0;i<_title.size();i++){
		std::cout << _title[i] << "\t";
	}
	
	std::cout << std::endl;
	
	for(unsigned int i=0;i<_columnType.size();i++)
	{
		std::cout << i <<":" << _columnType[i] << "\t";
	}
	
	std::cout << std::endl;
	
	for(unsigned int i=0;i<_element.size();i++)
	{
		if(i % getColumnCount() == 0)
		{
			std::cout << std::endl;
		}
		std::cout << _element[i] << "\t";
	}
	
	std::cout << std::endl;
	
}

//
// debug:
//
void MadelineTable::debug( void ){
	
	//for(unsigned i=0;i< _lines;i++){
	//	
	//	std::cerr << (i+1) << ". LEN=" << _lineInfo[i].getLength() << " TABS=" << _lineInfo[i].getTabCount() << " : " << _lineInfo[i].getLine() ;
	//	if( _lineInfo[i].getType()==LineInformation::EMPTY_LINE) std::cerr << ": EMPTY";
	//	if( _lineInfo[i].getType()==LineInformation::DIRTY_LINE) std::cerr << ": DIRTY";
	//	if( _lineInfo[i].getType()==LineInformation::HEADER_LINE) std::cerr << ": HEADER";
	//	if( _lineInfo[i].getType()==LineInformation::DATA_LINE) std::cerr << ": DATA";
	//	std::cerr << std::endl;
	//	
	//}
	//std::cerr << "NUMBER OF BLOCKS: " << getBlockCount() << std::endl;
	for(unsigned i=0;i<_dataColumns;i++){
		
		std::cerr << (i+1) << ". " << _columnOffset[i].getName() << " DeclaredType=" << _columnOffset[i].getDeclaredType() << " DiscoveredType=" <<_columnOffset[i].getDiscoveredType() << std::endl;
	}             
}


