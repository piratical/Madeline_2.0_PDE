//
// testUniqueList.cpp
//

#include "UniqueList.h"
#include "Data.h"
#include "String.h"
#include "Number.h"
#include "Date.h"
#include "Boolean.h"
#include "Gender.h"
#include "Genotype.h"
#include "Haplotype.h"
#include "Affected.h"
#include "Warning.h"

//#include "Date.h"

int main(void){
	
	Warning::suppressWarnings(true);
	
	std::cout << "\nSTRING\nExpected results for String List are: A occurs 8x, U occurs 5x, \".\" occurs 5 times\n\n";
	UniqueList myStringList("String List");
	
	String s("U");
	String sa("U");
	String s2("A");
	String s3("A");
	String m(".");
	String m2(" ");
	String m3("                   .            ");
	String m4("                ");
	String m5("RA");
	String m6("RU");
	
	myStringList.insert(&m2);
	myStringList.insert(&m);
	myStringList.insert(&s2);
	myStringList.insert(&s);
	myStringList.insert(&s3);
	myStringList.insert(&s);
	myStringList.insert(&s2);
	myStringList.insert(&s2);
	myStringList.insert(&m3);
	myStringList.insert(&s2);
	myStringList.insert(&s);
	myStringList.insert(&m2);
	myStringList.insert(&s);
	myStringList.insert(&sa);
	myStringList.insert(&s2);
	myStringList.insert(&s2);
	myStringList.insert(&s2);
	myStringList.insert(&m4);
	myStringList.insert(&m5);
	myStringList.insert(&m6);
	
	//
	// Assign the ordinals:
	//
	myStringList.assignOrdinals();
	
	myStringList.printResults();
	//
	// Show ordinal results for certain keys:
	//
	std::cout << m5 << " is at ordinal position " << myStringList.getOrdinalForKey(&m5) << "." << std::endl;
	std::cout << m6 << " is at ordinal position " << myStringList.getOrdinalForKey(&m6) << "." << std::endl;
	
	std::cout << "\nNUMBER\nExpected results for NUMBER List are:\n";
	std::cout << "xbar=0.75 : 2x\nxbar=3.5 : 3x\n";
	std::cout << "12.7 : 5x\n17.23 : 3x\n19.1245 : 2x\n";
	std::cout << "MISS : 3x\n\n";
	
	UniqueList numberList("Number List");
	
	Number n00(" ");
	Number n01("12.7");
	Number n02("12.70000000001");
	Number n03("17.23");
	Number n04("19.1245");
	Number n05("12.7");
	Number n06("[2 R 5]");
	Number n07("[-1.0 R 2.50]");
	Number n08("12.7");
	Number n09("[2.0 R 5.0]");
	Number n10("19.124500");
	Number n11("17.23");
	Number n12(".");
	Number n13("17.230");
	Number n14("[2-5]");
	Number n15(".");
	Number n16("12.7");
	Number n17("[-1.0 R 2.5]");
	Number n18("0.75");
	Number n19("[-2.0 R 3.5]");
	Number n20("[-0.5 R 2.0]");
	
	numberList.insert(&n00);
	numberList.insert(&n01);
	numberList.insert(&n02);
	numberList.insert(&n03);
	numberList.insert(&n04);
	numberList.insert(&n05);
	numberList.insert(&n06);
	numberList.insert(&n07);
	numberList.insert(&n08);
	numberList.insert(&n09);
	numberList.insert(&n10);
	numberList.insert(&n11);
	numberList.insert(&n12);
	numberList.insert(&n13);
	numberList.insert(&n14);
	numberList.insert(&n15);
	numberList.insert(&n16);
	numberList.insert(&n17);
	numberList.insert(&n18);
	numberList.insert(&n19);
	numberList.insert(&n20);
	
	Number::displayRangeMeans(true);
	numberList.printResults();
	Number::displayRangeMeans(false);
	
	UniqueList dateList("Date List");
	
	Date d00("1967.09.02");
	Date d01(".");
	Date d02(" ");
	Date d03("   ");
	Date d04("1934.13.33");
	Date d05("1934.12");
	Date d06("1934.06 - 1935.06");
	Date d07("1990.04.12");
	Date d08("2004.08.03");
	Date d09("2000.12.31");
	Date d10("2000.12.31");
	Date d11("1967.09.02");
	Date d12("1967.09.01");
	Date d13("1967.02.28");
	Date d14("2000.12.31");
	Date d15("1934.12");
	Date d16("1934.06 - 1935.06");
	Date d17("1967.02.29");
	Date d18("2004.08.02 - 2004.08.04");
	Date d19("2004.08.01 - 2004.08.05");
	
	dateList.insert(&d00);
	dateList.insert(&d01);
	dateList.insert(&d02);
	dateList.insert(&d03);
	dateList.insert(&d04);
	dateList.insert(&d05);
	dateList.insert(&d06);
	dateList.insert(&d07);
	dateList.insert(&d08);
	dateList.insert(&d09);
	dateList.insert(&d10);
	dateList.insert(&d11);
	dateList.insert(&d12);
	dateList.insert(&d13);
	dateList.insert(&d14);
	dateList.insert(&d15);
	dateList.insert(&d16);
	dateList.insert(&d17);
	dateList.insert(&d18);
	dateList.insert(&d19);
	
	Date::displayRangeMeans(true);
	dateList.printResults();
	Date::displayRangeMeans(false);
	
	UniqueList boolList("Boolean List");
	
	Boolean b00("f");
	Boolean b01("T");
	Boolean b02("f");
	Boolean b03(" ");
	Boolean b04("t");
	Boolean b05("t");
	Boolean b06("f");
	Boolean b07("F");
	Boolean b08("F");
	Boolean b09(".");
	Boolean b10("t");
	Boolean b11("f");
	Boolean b12("t");
	Boolean b13(".");
	Boolean b14("t");
	Boolean b15("f");
	Boolean b16(".");
	
	boolList.insert(&b00);
	boolList.insert(&b01);
	boolList.insert(&b02);
	boolList.insert(&b03);
	boolList.insert(&b04);
	boolList.insert(&b05);
	boolList.insert(&b06);
	boolList.insert(&b07);
	boolList.insert(&b08);
	boolList.insert(&b09);
	boolList.insert(&b10);
	boolList.insert(&b11);
	boolList.insert(&b12);
	boolList.insert(&b13);
	boolList.insert(&b14);
	boolList.insert(&b15);
	boolList.insert(&b16);
	
	boolList.printResults();
	
	UniqueList genderList("GENDER List");
	
	Gender g00("m");
	Gender g01("m");
	Gender g02("f");
	Gender g03(" ");
	Gender g04("m");
	Gender g05("m");
	Gender g06("f");
	Gender g07("m");
	Gender g08("F");
	Gender g09(".");
	Gender g10("m");
	Gender g11("F");
	Gender g12("F");
	Gender g13(".");
	Gender g14("M");
	Gender g15("F");
	Gender g16("男");
	Gender g17("女");
	Gender g18("男");
	Gender g19("男");
	Gender g20("女");
	
	genderList.insert(&g00);
	genderList.insert(&g01);
	genderList.insert(&g02);
	genderList.insert(&g03);
	genderList.insert(&g04);
	genderList.insert(&g05);
	genderList.insert(&g06);
	genderList.insert(&g07);
	genderList.insert(&g08);
	genderList.insert(&g09);
	genderList.insert(&g10);
	genderList.insert(&g11);
	genderList.insert(&g12);
	genderList.insert(&g13);
	genderList.insert(&g14);
	genderList.insert(&g15);
	genderList.insert(&g16);
	genderList.insert(&g17);
	genderList.insert(&g18);
	genderList.insert(&g19);
	genderList.insert(&g20);
	
	genderList.printResults();
	
	UniqueList genotypeList("GENOTYPE List");
	
	Genotype gn00("102/104");
	Genotype gn01("98/100");
	Genotype gn02("100/102");
	Genotype gn03("104/110");
	Genotype gn04("./.");
	Genotype gn05(".");
	Genotype gn06(" ");
	Genotype gn07("0/0");
	Genotype gn08("110/118");
	Genotype gn09("110/118");
	Genotype gn10("98/100");
	Genotype gn11("100/102");
	Genotype gn12("100/98");
	Genotype gn13("102/100");
	Genotype gn14(" 98 / 100 ");
	Genotype gn15("A/C");
	Genotype gn16("A/A");
	Genotype gn17("C/G");
	Genotype gn18("C/C");
	Genotype gn19("G/C");
	Genotype gn20("a/c");
	Genotype gn21("3/2");
	Genotype gn22("2/2");
	
	genotypeList.insert(&gn00);
	genotypeList.insert(&gn01);
	genotypeList.insert(&gn02);
	genotypeList.insert(&gn03);
	genotypeList.insert(&gn04);
	genotypeList.insert(&gn05);
	genotypeList.insert(&gn06);
	genotypeList.insert(&gn07);
	genotypeList.insert(&gn08);
	genotypeList.insert(&gn09);
	genotypeList.insert(&gn10);
	genotypeList.insert(&gn11);
	genotypeList.insert(&gn12);
	genotypeList.insert(&gn13);
	genotypeList.insert(&gn14);
	genotypeList.insert(&gn15);
	genotypeList.insert(&gn16);
	genotypeList.insert(&gn17);
	genotypeList.insert(&gn18);
	genotypeList.insert(&gn19);
	genotypeList.insert(&gn20);
	genotypeList.insert(&gn21);
	genotypeList.insert(&gn22);
	
	genotypeList.printResults();
	
	UniqueList haplotypeList("HAPLOTYPE List");
	
	Haplotype hp00("1:4");
	Haplotype hp01("1:4");
	Haplotype hp02("1\\2");
	Haplotype hp03("1/2");
	Haplotype hp04("1\\2");
	Haplotype hp05(".");
	Haplotype hp06(" ");
	Haplotype hp07("1/2");
	Haplotype hp08("1+8");
	Haplotype hp09("1+8");
	Haplotype hp10("1/2");
	Haplotype hp11("1\\2");
	Haplotype hp12("1|2");
	Haplotype hp13("1|2");
	Haplotype hp14("2|1");
	Haplotype hp15("2|1");
	Haplotype hp16("2:1");
	Haplotype hp17("1:2");
	Haplotype hp18("1+8");
	Haplotype hp19("8\\1");
	Haplotype hp20("10|12");
	
	haplotypeList.insert(&hp00);
	haplotypeList.insert(&hp01);
	haplotypeList.insert(&hp02);
	haplotypeList.insert(&hp03);
	haplotypeList.insert(&hp04);
	haplotypeList.insert(&hp05);
	haplotypeList.insert(&hp06);
	haplotypeList.insert(&hp07);
	haplotypeList.insert(&hp08);
	haplotypeList.insert(&hp09);
	haplotypeList.insert(&hp10);
	haplotypeList.insert(&hp11);
	haplotypeList.insert(&hp12);
	haplotypeList.insert(&hp13);
	haplotypeList.insert(&hp14);
	haplotypeList.insert(&hp15);
	haplotypeList.insert(&hp16);
	haplotypeList.insert(&hp17);
	haplotypeList.insert(&hp18);
	haplotypeList.insert(&hp19);
	haplotypeList.insert(&hp20);
	
	haplotypeList.printResults();
	
	UniqueList affectedList("AFFECTED List");
	
	Affected af00("A");
	Affected af01("U");
	Affected af02("a");
	Affected af03(" ");
	Affected af04("u");
	Affected af05("u");
	Affected af06("u");
	Affected af07("u");
	Affected af08("A");
	Affected af09(".");
	Affected af10("a");
	Affected af11("a");
	Affected af12("a");
	Affected af13(".");
	Affected af14("i");
	Affected af15("I");
	Affected af16(".");
	
	affectedList.insert(&af00);
	affectedList.insert(&af01);
	affectedList.insert(&af02);
	affectedList.insert(&af03);
	affectedList.insert(&af04);
	affectedList.insert(&af05);
	affectedList.insert(&af06);
	affectedList.insert(&af07);
	affectedList.insert(&af08);
	affectedList.insert(&af09);
	affectedList.insert(&af10);
	affectedList.insert(&af11);
	affectedList.insert(&af12);
	affectedList.insert(&af13);
	affectedList.insert(&af14);
	affectedList.insert(&af15);
	affectedList.insert(&af16);
	
	affectedList.printResults();
	
	return 0;
	
	
}

