/////////////////////////////////////////////////////////
///
/// This file is part of the MADELINE 2 program 
/// written by Edward H. Trager and Ritu Khanna
/// Copyright (c) 2005 by the
/// Regents of the University of Michigan.
/// All Rights Reserved.
/// 
/// The latest version of this program is available from:
/// 
///   http://eyegene.ophthy.med.umich.edu/madeline/
///   
/// Released under the GNU General Public License.
/// A copy of the GPL is included in the distribution
/// package of this software, or see:
/// 
///   http://www.gnu.org/copyleft/
///   
/// ... for licensing details.
/// 
/////////////////////////////////////////////////////////

//
// DelimitedTableParser.h
//

#ifndef DELIMITEDTABLEPARSER_INCLUDED
#define DELIMITEDTABLEPARSER_INCLUDED

#include <iostream>
#include "Exception.h"
#include "ColumnClassifier.h"

#include "TableParser.h"

/*!
 *
 * This class is used to parse tables that are tab 
 * or comma delimited. 
 * The results end up in the _element vector.
 * 
 * The class use the data members of the TableParser
 * base class.  No additional private members are
 * required.
 * 
*/


class DelimitedTableParser : public TableParser{
	
public:
	
	DelimitedTableParser(){}
	
	// Read in the file/string, parse it, and put the results in the _element vector.
	void readFile(const char* filename);
	void readString(const std::string &inString);
	
	void display(void);

	// clear variables for a clean read.
	void reset();
	
	unsigned getColumnCount( void ){ return _columns; };
	unsigned long getNumberOfRows( void ){ return _rows; }
	
};
#endif
