#include "Affected.h"
#include <iostream>

using namespace std;

int main( void ){
	
	Affected s1;
	cout << "Empty Affection String             =|" << s1.get() << "|" << endl;
	s1.set("	");
	cout << "Whitespaces                        =|" << s1.get() << "|" << endl;
	s1.set("    	");
	cout << "Whitespaces                        =|" << s1 << "|" << endl;
	s1.set("                   ​abcd                ");
	cout << "'abcd' is surrounded on both sides by various Unicode space characters..." << endl;
	cout << " abcd                              =|" << s1 << "|" << endl;
	cout << " abcd's boolean attribute          =|" << s1.getBoolean() << endl;
	s1.set("A");
	cout << " 'A'                               =|" << s1 << "|" << endl;
	cout << " 'A's boolean attribute            =|" << s1.getBoolean() << endl;
	
	Affected s2("U");
	cout << " 'U'                               =|" << s2 << "|" << endl;
	cout << " 'U's boolean attribute            =|" << s2.getBoolean() << endl;
	Affected s3("a");
	cout << " 'a'                               =|" << s3 << "|" << endl;
	cout << " 'a's boolean attribute            =|" << s3.getBoolean() << endl;
	Affected s4("                  ​u                  ​                  ​");
	cout << "'u' is surrounded on both sides by various Unicode space characters..." << endl;
	cout << " 'u'                               =|" << s4 << "|" << endl;
	cout << " 'u's boolean attribute            =|" << s4.getBoolean() << endl;
	
	Affected s5("                              i                                             ");
	cout << "'i' is surrounded on both sides by various Unicode space characters..." << endl;
	cout << " 'i'                               =|" << s5 << "|" << endl;
	cout << " 'i's boolean attribute            =|" << s5.getBoolean() << endl;
	
	Affected s6("I");
	cout << " 'I'                               =|" << s6 << "|" << endl;
	cout << " 'I's boolean attribute            =|" << s6.getBoolean() << endl;
	
	Affected s7("RA");
	cout << " 'RA'                               =|" << s7 << "|" << endl;
	cout << " 'RA's boolean attribute            =|" << s7.getBoolean() << endl;
	
	cout << " Call addAffectedBooleanMapping to make 'RA' equivalent to 'affected' (T) ..." << endl;
	cout << " Call addAffectedBooleanMapping to make 'RU' equivalent to 'unaffected' (F) ..." << endl;
	
	Affected::addAffectedBooleanMapping("RA","T");
	Affected::addAffectedBooleanMapping("RU","F");
	
	Affected s8("RA");
	cout << " 'RA'                               =|" << s8 << "|" << endl;
	cout << " 'RA's boolean attribute after mapping:" << s8.getBoolean() << endl;
	
	Affected s9("RU");
	cout << " 'RU'                               =|" << s9 << "|" << endl;
	cout << " 'RU's boolean attribute after mapping:" << s9.getBoolean() << endl;
	
	return 1;
}
