
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
// ColumnTypes.h
//
// 2005.04.15.ET.RK
//

#ifndef DATAYTYPES_INCLUDED
#define DATAYTYPES_INCLUDED

enum DATATYPE {
	
	ANY=0x0000,
	BOOLEAN=0x0001,
	DATE=0x0002,
	GENDER=0x0004,
	ALLELE=0x0008,
	GENOTYPE=0x0010,
	HAPLOTYPE=0x0020,
	NUMBER=0x0040,
	STRING=0x0080,
	AFFECTED=0x0100,
	SAMPLED=0x0200,
	PROBAND=0x0400,
	LIVINGDEAD=0x0800,
	TWIN=0x1000,
	UNCLASSIFIED=0x2000, // Column has not yet been scanned ...
	MISSING=0x4000,      // Column has been scanned and still found empty/missing ...
	// 2007.04.26.ET: Adding more types here:
	CONSULTAND=0x8000,
	CARRIER=0x10000,
	RELATIONSHIPENDED=0x20000,
	INFERTILITY=0x40000,
	STERILITY=0x80000,
	PREGNANCY=0x100000,
	COLLAPSED=0x200000
};

#endif
