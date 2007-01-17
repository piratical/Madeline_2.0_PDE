#include "String.h"
#include <iostream>

using namespace std;

int main( void ){
	String s1;
	cout << "Empty String                       =" << s1.get() << endl;
	s1.set("	");
	cout << "Whitespaces                        =" << s1.get() << endl;
	s1.set("    	");
	cout << "Whitespaces                        =" << s1 << endl;
	s1.set("  abcd ");
	cout << " abcd                              =" << s1 << endl;
	String s2("ABCD");
	cout << "ABCD                               =" << s2 << endl;
	cout << "ABCD+  abcd                        =" << (s2+s1) << endl;
	String s3("EFGH");
	cout << "ABCD+=EFGH                         =" << (s2+=s3) << endl;
	String s4;
	cout << "''+EFGH                            =" << (s4+s3) << endl;
	cout << "EFGH+=''                           =" << (s3+=s4) << endl;
	cout << "''+''                              =" << (s3+s4) << endl;
	cout << "''+= abcd                          =" << (s4+=s1) << endl;
	s1.set("hello");
	s3.set("hello");
	String s5;
	cout << "hello==hello                       =" << (s1==s3) << endl;
	cout << "hello==''                          =" << (s1==s4) << endl;
	cout << "hello==ABCDEFGH                    =" << (s1==s2) << endl;
	cout << "''==''                             =" << (s4==s5) << endl;
	cout << "hello!=hello                       =" << (s1!=s3) << endl;
	cout << "hello!=''                          =" << (s1!=s4) << endl;
	cout << "hello!=ABCDEFGH                    =" << (s1!=s2) << endl;
	cout << "''!=''                             =" << (s4!=s5) << endl;
	cout << "hello<=hello                       =" << (s1<=s3) << endl;
	cout << "hello<=''                          =" << (s1<=s4) << endl;
	cout << "hello<=ABCDEFGH                    =" << (s1<=s2) << endl;
	cout << "ABCDEFGH<=hello                    =" << (s2<=s1) << endl;
	cout << "''<=''                             =" << (s4<=s5) << endl;
	cout << "hello>=hello                       =" << (s1>=s3) << endl;
	cout << "hello>=''                          =" << (s1>=s4) << endl;
	cout << "hello>=ABCDEFGH                    =" << (s1>=s2) << endl;
	cout << "ABCDEFGH>=hello                    =" << (s2>=s1) << endl;
	cout << "''>=''                             =" << (s4>=s5) << endl;
	cout << "hello<hello                        =" << (s1<s3) << endl;
	cout << "hello<''                           =" << (s1<s4) << endl;
	cout << "hello<ABCDEFGH                     =" << (s1<s2) << endl;
	cout << "ABCDEFGH<hello                     =" << (s2<s1) << endl;
	cout << "''<''                              =" << (s4<s5) << endl;
	cout << "hello>hello                        =" << (s1>s3) << endl;
	cout << "hello>''                           =" << (s1>s4) << endl;
	cout << "hello>ABCDEFGH                     =" << (s1>s2) << endl;
	cout << "ABCDEFGH>hello                     =" << (s2>s1) << endl;
	cout << "''>''                              =" << (s4>s5) << endl;
       	String e1("");
        cout << "e1                                 =" << e1 << endl;
        cout << "isMissing()                        =" << e1.isMissing() << endl;
        String e2(" .");
        cout << "e2                                 =" << e2 << endl;
        cout << "isMissing()                        =" << e2.isMissing() << endl;
        String e3("G");
        cout << "e3                                 =" << e3 << endl;
        cout << "isMissing()                        =" << e3.isMissing() << endl;
        String e4;
        String::addStringMissingValue("x");
        cout << "e4                                 =" << e4 << endl;
        cout << "Added x as missing value" << endl;
        e4.set("x");
        cout << "e4=x                               =" << e4 << endl;

	return 1;
}
