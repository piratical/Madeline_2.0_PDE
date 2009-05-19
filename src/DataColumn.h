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
// DataColumn.h
//

#ifndef DATACOLUMN_INCLUDED
#define DATACOLUMN_INCLUDED

#include <string>

// DEBUG:
#include <iostream>

#include "DataTypes.h"

// Data is the virtual base class:
#include "Data.h"
#include "Date.h"
#include "Gender.h"
#include "Genotype.h"
#include "Haplotype.h"
#include "Number.h"
#include "String.h"
#include "Affected.h"
#include "LivingDead.h"
#include "Sampled.h"
#include "Proband.h"
#include "Consultand.h"
#include "Carrier.h"
#include "RelationshipEnded.h"
#include "Sterility.h"
#include "Infertility.h"
#include "Twin.h"

#include "Exception.h"
#include "Warning.h"
#include <vector>

#include "UniqueList.h"

/*! This class is used to store columns of information, which are then stored inside the Data Table
 *
 *
 *
*/

//
// DataColumn:
//
class DataColumn {
	
private:
	std::string _name;                // column name
	int _ordinal;                     // physical position of the column 
	DATATYPE _columnType;             // Type of data
	
	unsigned long _rows;               // Number of values (i.e., rows) in the column
	std::vector<Data*> _data;
	
	bool _showOnPedigree;             // Flag indicating that the column should appear on the pedigree.
	bool _showOnIcon;                 // Flag indicating that the column should appear on the icon.
	
	UniqueList *_uniqueList;
	
	
public:
	
	//
	// Constructors:
	//
	DataColumn();

	DataColumn(const std::string &name,DATATYPE columnType=MISSING, unsigned long rows=0, unsigned  ordinal=0 );
	
	// Destructor:
	~DataColumn();
	
	// Setters:
	void setColumnType   ( DATATYPE columnType    ){ _columnType=columnType; };
	void setName         ( const char *name       ){ _name=name;             };
	void setName         ( const std::string &name){ _name=name;
	             };
	void setShowOnPedigree(bool value);
	void setShowOnIcon(bool value);
	
	void set(std::vector<std::string> & vec);
	void setRow( unsigned long row , const std::string & value);

	bool True() const {return true;}
	bool False() const {return false;}
	
	// Getters:
	DATATYPE getColumnType     ( void ) const { return _columnType; };
	std::string getColumnTypeAsString(void) const;
	int      getOrdinal        ( void ) const { return _ordinal;    };
	int      getNumberOfRows   ( void ) const { return _rows;       };
	std::string getName        ( void ) const { return _name;       };
	std::string getCaseInvariantName( void ) const;
	bool getShowOnPedigree(void) const;
	bool getShowOnIcon(void) const;
	
	// DEBUGGING:
	void printData( void );
	void printData( unsigned long i );
	
	// get the ith row / element from the column
	std::string get( unsigned long i) const;
	Data* getDataAtIndex(unsigned long i) const;
	std::vector<Data*> getData(){ return _data; }
	
	//
	// This cannot be a const method because it needs
	// to create the private _uniqueList if not yet present:
	// 
	const UniqueList *const getUniqueList(void);

};

#endif
