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

#ifndef LABELSET_INCLUDED
#define LABELSET_INCLUDED

#include "DataTable.h"
#include "Individual.h"
#include "DataTypes.h"

#include "Utf8String.h"

class LabelSet{
	
private:
	unsigned _numberOfLabels;
	const DataTable * _pedigreeTable;
public:
	LabelSet(){}
	LabelSet(const DataTable *const pedigreeTable);
	unsigned getNumberOfLabels(void) const;
	
	std::vector<UTF8String> getLabelSet(Individual* individual) const;
	std::vector<UTF8String> getLabelSubsetByType(DATATYPE type,Individual* individual) const;
	std::vector<UTF8String> getRandomLabelSubsetByType(DATATYPE type) const;
	std::vector<UTF8String> getLabelNameSet(void) const;
	
};

#endif
