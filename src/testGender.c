#include <iostream>
#include "Gender.h"
#include "Warning.h"
#include <string>
using namespace std;

int main(void){
	Gender a("M");
	cout << "M                   =" << a << endl;
	Gender b("F");
	cout << "F                   =" << b << endl;
	Gender c;
	cout << "Missing             =" << c << endl;
	c.set("m");
	cout << "set('m')            =" << c << endl;
	c.set("f");
	cout << "set('f')            =" << c << endl;
	Gender d("♀");
	cout << "♀                   =" << d << endl;
	Gender e("♂");
	cout << "♂                   =" << e << endl;
	d.set("女");
	cout << "女                  =" << d << endl;
	e.set("男");
	cout << "男                  =" << e << endl; 
	d.set("雌");
	cout << "雌                  =" << d << endl;
	e.set("雄");
	cout << "雄                  =" << e << endl;
	d.set(Gender::MALE);
	cout << "Gender::MALE        =" << d << endl;
		Gender ind1("F");
	std::cout << "ind1 F            =" << ind1 << std::endl;
	Gender ind2("m");
	std::cout << "ind2 m            =" << ind2 << std::endl;
	b.set("f");
	std::cout << "B after set f     =" << b << std::endl;
	Gender::addLookupTableEntry("FEMALE",true);
	std::cout << "Added FEMALE to the Gender map " << std::endl;
	Gender ind3("FEMALE");
	std::cout << "ind3 FEMALE       =" << ind3 << std::endl;
	Gender ind4,ind5;
	std::cout << "ind3==ind4         " << (ind3==ind4) << std::endl;
	std::cout << "ind3==ind1         " << (ind3==ind1) << std::endl;
	std::cout << "ind3==ind2         " << (ind3==ind2) << std::endl;
	std::cout << "ind4==ind5         " << (ind4==ind5) << std::endl;
	std::cout << "ind3 < ind4        " << (ind3 < ind4) << std::endl;
	std::cout << "ind3 < ind1        " << (ind3 < ind1) << std::endl;
	std::cout << "ind3 < ind2        " << (ind3 < ind2) << std::endl;
	std::cout << "ind4 < ind5        " << (ind4 < ind5) << std::endl;
	std::cout << "ind4 < ind3        " << (ind4 < ind3) << std::endl;
	std::cout << "ind2 < ind3        " << (ind2 < ind3) << std::endl;
	Gender ind6(".");
	std::cout << "ind6               " << ind6 << std::endl;
	std::cout << "ind6.isMissing()   " << ind6.isMissing() << std::endl;
	ind6.set(false);
	std::cout << "ind6.set(false)    " << ind6 << std::endl;
	std::cout << "ind6.getDataType() " << ind6.getDataType() << std::endl;
	std::cout << "ind6.getBoolean()  " << ind6.getBoolean() << std::endl;
	std::cout << "ind3.getBoolean()  " << ind3.getBoolean() << std::endl;
	std::cout << "ind1.getBoolean()  " << ind1.getBoolean() << std::endl;
	std::cout << "ind3               " << ind3 << std::endl;
	ind3.setMissing();
	std::cout << "ind3.setMissing()  " << ind3 << std::endl;
	std::cout << "ind3.getEnum       " << ind3.getEnum() << std::endl;
	std::cout << "ind6.getEnum       " << ind6.getEnum() << std::endl;
	std::cout << "ind1.getEnum       " << ind1.getEnum() << std::endl;
	ind5.set("男");
	std::cout << "ind5 男            " << ind5 << std::endl;
	ind4.set(Gender::MALE);
	std::cout << "ind4.set(MALE)     " << ind4 << std::endl;
	
	// 2005.09.12.ET: Classification using isA():
	std::string  g1("m");
	std::string  g2("f");
	std::string  g3("g");
	std::string  g4("男");
	std::string  g5("女");
	std::string  g6("fm");
	std::string  g7("♂");
	std::string  g8("♀");
	std::string  g9("MF");

	Warning::suppressWarnings(true);
	std::cout << "Classification: " << g1 << " : " << Gender::isA(g1) << std::endl;
	std::cout << "Classification: " << g2 << " : " << Gender::isA(g2) << std::endl;
	std::cout << "Classification: " << g3 << " : " << Gender::isA(g3) << std::endl;
	std::cout << "Classification: " << g4 << " : " << Gender::isA(g4) << std::endl;
	std::cout << "Classification: " << g5 << " : " << Gender::isA(g5) << std::endl;
	std::cout << "Classification: " << g6 << " : " << Gender::isA(g6) << std::endl;
	std::cout << "Classification: " << g7 << " : " << Gender::isA(g7) << std::endl;
	std::cout << "Classification: " << g8 << " : " << Gender::isA(g8) << std::endl;
	std::cout << "Classification: " << g9 << " : " << Gender::isA(g9) << std::endl;

	//
	return 0;
}
