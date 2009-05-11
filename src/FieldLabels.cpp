/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna, and Adrian Marrs
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
// FieldLabels.cpp
//

#include "FieldLabels.h"

//////////////////////////////////////
//
// CONSTRUCTORS:
//
//////////////////////////////////////

FieldLabels::FieldLabels()
{
	FamilyIdField = "FAMILYID";			// Shared with Decomposed Table
	IndividualIdField = "INDIVIDUALID";	// Shared with Decomposed Table
	FatherIdField = "FATHER";
	MotherIdField = "MOTHER";
	GenderField = /*"SEX"*/"GENDER";

	//Genetic Map Table Labels
	ChromosomeField = "CHROMOSOME";
	MarkerNameField = "MARKERNAME";		// Shared with Decomposed Table and Allele Frequency Table
	PositionField = "POSITION";			// Shared with Results Table
	PositionFemaleField = "POSITION_FEMALE";
	PositionMaleField = "POSITION_MALE";

	//Decomposed Table Labels
	Allele1Field = "ALLELE1";
	Allele2Field = "ALLELE2";

	//Results Table Labels
	Score1Field = "SCORE";
	Score2Field = "SCORE2";
	Score3Field = "SCORE3";
	Score4Field = "SCORE4";
	Score5Field = "SCORE5";

	//Allele Frequency Table Labels
	FrequencyField = "FREQUENCY";
	AlleleField = "ALLELE";

	// Optional Core Field Labels
	DOBField         = "DOB";
	MZTwinField      = "MZTWIN";
	DZTwinField      = "DZTWIN";
	AffectedField    = "AFFECTED";
	DeceasedField    = "DECEASED";
	ProbandField     = "PROBAND";
	SampledField     = "SAMPLED";
	SuperscriptField = "SUPERSCRIPT";
	ConsultandField  = "CONSULTAND";
	CarrierField     = "CARRIER";
	RelationshipEndedField = "RELATIONSHIPENDED";
	
	
	PEDIGREE_TABLE_CORE_COLUMN_COUNT = 5;
	GENETIC_TABLE_CORE_COLUMN_MINIMUM = 2;
	GENETIC_TABLE_CORE_COLUMN_MAXIMUM = 6;
	ALLELE_FREQUENCY_TABLE_CORE_COLUMN_COUNT = 3;
	RESULTS_TABLE_CORE_COLUMN_COUNT = 3;
	DECOMPOSED_TABLE_CORE_COLUMN_COUNT = 3;
	//RESULTS_TABLE_CORE_COLUMN_COUNT = 2;
	//DECOMPOSED_TABLE_CORE_COLUMN_COUNT = 2;

}

//////////////////////////////////////
//
// PUBLIC METHODS
//
//////////////////////////////////////

//
// find:
//
bool FieldLabels::find(std::string inString, DATATYPE inType)
{
/*
	switch(inType)
	{
		case ANY:			
					break;
		case BOOLEAN:		
					break;
		case DATE:			
					break;
		case GENDER:		
					break;
		case ALLELE:		
					break;
		case GENOTYPE:		
					break;
		case HAPLOTYPE:		
					break;
		case NUMBER:		
					break;
		case STRING:		
					break;
		case AFFECTED:		
					break;
		case UNCLASSIFIED:	
					break;
		case MISSING:		
					break;
		default:
	};
*/
	return true;
}
