#include "AssociativeArray.h"

int main(){
	
	AssociativeArray assoc1("seasons",ANY,ANY);
	assoc1.insert(ANY,"1",STRING,"Spring");
	assoc1.insert(STRING,"Third",STRING,"Autumn");
	assoc1.insert(STRING,"Fourth",STRING,"Winter");
	assoc1.insert(STRING,".",STRING,"Any");
	std::cout << "Value Type:" << assoc1.getValueType() << std::endl;
	std::cout << assoc1 << std::endl;
	// Testing Copy Constructor
	std::cout << "Testing Copy Constructor:" << std::endl;
	AssociativeArray mz_twin("MZ Twin",NUMBER,STRING);
	mz_twin.insert(NUMBER,"1",STRING,"A");
	mz_twin.insert(NUMBER,"2",STRING,"B");
	mz_twin.insert(NUMBER,"3",STRING,"C");
	mz_twin.insert(NUMBER,"3",STRING,".");
	std::cout << "Value Type: " << mz_twin.getValueType() << std::endl;
	std::cout << mz_twin <<  std::endl;
	AssociativeArray mz_twin_copy(mz_twin);
	std::cout << "MZ_Twin Copy: " << mz_twin_copy.getValueType() << std::endl;
	std::cout << mz_twin_copy << std::endl;

	// Testing combination of key and values using |
	std::cout << "Testing Key and Value combinations using |: " << std::endl;
	AssociativeArray isMale("isMale",BOOLEAN|STRING,BOOLEAN|NUMBER);
	isMale.insert(BOOLEAN,"m",BOOLEAN,"M");
	isMale.insert(STRING,"Male",BOOLEAN,"T");
	isMale.insert(BOOLEAN,"f",BOOLEAN,"F");
	isMale.insert(STRING,"Female",BOOLEAN,"F");
	isMale.insert(STRING,"Femme",BOOLEAN,"f");
	isMale.insert(STRING,"Man",NUMBER,"1");
	isMale.insert(STRING,"Woman",NUMBER,"2");
	isMale.insert(BOOLEAN,"T",STRING,"male");
	std::cout << "isMale: " << isMale.getValueType() << std::endl;
	std::cout << isMale << std::endl;

	// Testing assignment operation
	std::cout << "Testing assignment operation:" << std::endl;
	assoc1=mz_twin;
	std::cout << "assoc1: " << assoc1.getValueType() << std::endl;
	std::cout << assoc1 << std::endl;
	AssociativeArray mz_twin_assign("MZ Twin Assign",NUMBER,STRING);
	std::cout << "mz_twin_assign: " << mz_twin_assign.getValueType() << std::endl;
	std::cout << mz_twin_assign << std::endl;
	mz_twin_assign = mz_twin;
	std::cout << mz_twin_assign << std::endl; 
	
	AssociativeArray d1S100("D1S100",STRING,GENOTYPE);
	d1S100.insert(STRING,"M100",GENOTYPE,"120/122");
	d1S100.insert(STRING,"M101",GENOTYPE,"118/120");
	d1S100.insert(STRING,"M102",GENOTYPE,"116/122");
	d1S100.insert(STRING,"M103",GENOTYPE,"/");
	std::cout << "D1S100: " << d1S100.getValueType() << std::endl;
	std::cout << d1S100 << std::endl;

	AssociativeArray dob("DOB",STRING,DATE);
	dob.insert(STRING,"M100",DATE,"1956/12/12");
	dob.insert(STRING,"M101",DATE,"1945");
	dob.insert(STRING,"M102",DATE,"1943-02-24");
	dob.insert(STRING,"M103",DATE,"194606-]");
	std::cout << "DOB: " << dob.getValueType() << std::endl;
	std::cout << dob << std::endl;

	// Testing Number ranges
	std::cout << "Testing Number Ranges as Keys:" << std::endl;
	AssociativeArray eye_pressure("Eye Pressure",NUMBER,STRING|NUMBER);
	eye_pressure.insert(NUMBER,"[12-18]",STRING,"OD");
	eye_pressure.insert(NUMBER,"18",STRING,"OS");
	eye_pressure.insert(NUMBER,"10",STRING,"OD"); 
	eye_pressure.insert(NUMBER,"~20",NUMBER,"2");
	eye_pressure.insert(NUMBER,"2",NUMBER,"2");
	eye_pressure.insert(NUMBER,"24",NUMBER,".");
	std::cout << "Added 99 as Number Missing Value" << std::endl;
	Number::addNumberMissingValue("99");
	eye_pressure.insert(NUMBER,"36",NUMBER,"99");
	
	
	std::cout << "Eye Pressure: " << eye_pressure.getValueType() << std::endl;
	std::cout << eye_pressure << std::endl;
	std::cout << "dob==d1S100? " << (dob==d1S100) << std::endl;
	std::cout << mz_twin << std::endl;
	std::cout << mz_twin_copy << std::endl;
	std::cout << "mz_twin==mz_twin_copy? " << (mz_twin==mz_twin_copy) << std::endl;
	return 0;
} 
