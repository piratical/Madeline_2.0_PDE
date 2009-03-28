

#include <string>
#include "DataTypes.h"


class FieldLabels
{
  public:
	FieldLabels();
	bool find(std::string, DATATYPE);
	//Pedigree Table Labels
	std::string FamilyIdField;	// Shared with Decomposed Table
	std::string IndividualIdField;	// Shared with Decomposed Table
	std::string FatherIdField;
	std::string MotherIdField;
	std::string GenderField;

	//Genetic Map Table Labels
	std::string ChromosomeField;
	std::string MarkerNameField;	// Shared with Decomposed Table and Allele Frequency Table
	std::string PositionField;	// Shared with Results Table
	std::string PositionFemaleField;
	std::string PositionMaleField;

	//Decomposed Table Labels
	std::string Allele1Field;
	std::string Allele2Field;

	//Results Table Labels
	std::string Score1Field;
	std::string Score2Field;
	std::string Score3Field;
	std::string Score4Field;
	std::string Score5Field;

	//Allele Frequency Table Labels
	std::string FrequencyField;
	std::string AlleleField;

	// Optional CORE fields in a Pedigree table
	std::string DOBField;
	std::string MZTwinField;
	std::string DZTwinField;
	std::string AffectedField;
	std::string DeceasedField;
	std::string ProbandField;
	std::string SampledField;
	std::string SuperscriptField;
	std::string ConsultandField;
	std::string CarrierField;
	std::string DivorcedField;
	
	int PEDIGREE_TABLE_CORE_COLUMN_COUNT;
	int GENETIC_TABLE_CORE_COLUMN_MINIMUM;
	int GENETIC_TABLE_CORE_COLUMN_MAXIMUM;
	int ALLELE_FREQUENCY_TABLE_CORE_COLUMN_COUNT;
	int RESULTS_TABLE_CORE_COLUMN_COUNT;
	int DECOMPOSED_TABLE_CORE_COLUMN_COUNT;
};
