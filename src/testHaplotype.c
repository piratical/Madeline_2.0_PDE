#include "Haplotype.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
	
	string a="     110    :  112     ";
	string b="    3,4A | A3,4   ";
	string c=".";
	string d="     112    :   112    ";
	string e="     112    :   108    ";
	string f="    112  : 108 ";
	string g="114/116";
	string h="114,116";
	
	Haplotype ha(a);
	Haplotype hb(b);
	Haplotype hc(c);
	
	Haplotype hd(d);
	Haplotype he(e);
	Haplotype hf(f);
	Haplotype hg(g);
	Haplotype hh(h);
	
	cout << a << " = " << ha << endl;
	cout << a << " alleleFromMom = " << ha.getAlleleFromMom() << endl;
	cout << a << " alleleFromDad = " << ha.getAlleleFromDad() << endl;
	cout << a << " recombination = " << (char) ha.getRecombinationInfo() << endl;

	cout << b << " = " << hb << endl;
	cout << b << " alleleFromMom = " << hb.getAlleleFromMom() << endl;
	cout << b << " alleleFromDad = " << hb.getAlleleFromDad() << endl;
	cout << b << " recombination = " << (char) hb.getRecombinationInfo() << endl;
	
	cout << c << " = " << hc << endl;
	cout << c << " alleleFromMom = " << hc.getAlleleFromMom() << endl;
	cout << c << " alleleFromDad = " << hc.getAlleleFromDad() << endl;
	cout << c << " recombination = " << (char) hc.getRecombinationInfo() << endl;
	
	cout <<  a << " < " << e << " ? : expect true : " << (ha<he) << endl;
	cout <<  d << " < " << e << " ? : expect false: " << (hd<he) << endl;
	cout <<  e << " == " << f << " ? : expect true: " << (he==hf) << endl;
	
	cout << g << " = " << hg.get() << endl;
	cout << g << " recombination = " << (char) hg.getRecombinationInfo() << endl;
	
	cout << h << " = " << hh.get() << endl;
	cout << h << " recombination = " << (char) hh.getRecombinationInfo() << endl;
	
	return 0;

}
