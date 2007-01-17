#include "Genotype.h"
#include <iostream>
#include "Number.h"
#include "Warning.h"
#include <string>

using namespace std;

int main(int argc, char *argv[]){

	Data* ppbh = new Genotype();
	ppbh->set("0 /0");
	cout << ppbh->get() << endl;
	delete ppbh;
	Genotype aa("12  /  56nm");
	cout << "12/56nm                            =" << aa << endl;
	Genotype bb("  A / CB     ");
	cout << "A/CB                               =" << bb << endl;
	Genotype cc("  A / C    B ");
	cout << "A/C B                              =" << cc << endl;
	Genotype dd("A  / C   n"); 
	cout << "A /C n                             =" << dd << endl;
	Genotype ee("A / C  ");
	cout << "A/C                                =" << ee << endl;
	Genotype ff("A / C");
	cout << "A/C                                =" << ff << endl;
	Genotype gg("12b/13");
	cout << "12b/13                             =" << gg << endl;
	Genotype a("117|109");
	cout << "117|109                            =" << a << endl;
	Genotype b("125/121");
	cout << "125/121                            =" << b << endl;
	Genotype c("      14       |       18          ");
	cout << " 14     |    18                    =" << c << endl;
	Genotype g("-4/5");
	cout << "-4/5                               =" << g << endl;
	Genotype h("4/0");
	cout << "4/0                                =" << h << endl;
	Genotype i("h/l");
	cout << "h/l                                =" << i << endl;
	Genotype j("4/l");
	cout << "4/l                                =" << j << endl;
	Genotype k(0,4);
	cout << "0/4                                =" << k << endl;
	Genotype snp("T/G");
	cout << "T/G                                =" << snp << endl;
	snp.setAllele1(Genotype::SNP_A_ORDINAL);
	cout << "T/G after setAllele1 is            =" << snp << endl;
	Genotype l(10,-12);
	cout << "10/-12                             =" << l << endl;
	Genotype m; 
	cout << "Missing                            =" << m << endl;
	m.setAllele1(24);
	cout << "Missing after setAllele1(24)       =" << m << endl;
	m.setAllele2(26);
	cout << "Missing after setAllele2(26)       =" << m << endl;
	m.set("G/T");
	cout << "24/26 after set G/T                =" << m << endl;
	m.setAllele1(3);
	cout << "G/T after setAllele1(3)            =" << m << endl;
	Genotype n("A/CD");
	cout << "A/CD                               =" << n << endl;
	n.setAllele1("G");
	cout << "A/CD after setAllele1('G')         =" << n << endl;
	n.setAllele1("23");
	cout << "A/CD after setAllele1('23')        =" << n << endl;
	n.set("A/C");
	cout << "A/C                                =" << n << endl;
	n.setAllele1("G");
	cout << "A/C after setAllele1('G')          =" << n << endl;
	n.setAllele2("T");
	cout << "G/C after setAllele2('T')          =" << n << endl;
	n.setAllele1("96");
	cout << "G/T after setAllele1(96)           =" << n << endl;
	Genotype o(34,38);
	cout << "34/38                              =" << o.get() << endl;
	cout << "getAllele1                         =" << o.getAllele1() << endl;
	cout << "getAllele2                         =" << o.getAllele2() << endl;
	n.set(96,102);
	cout << "96/102                             =" << n << endl;
	n.setAllele1("G");
	cout << "96/102 after setAllele1('G')       =" << n << endl;
	o.setAllele1("T");
	cout << "34/38 after setAllele1('T')        =" << o << endl;
	cout << "getAllele1                         =" << o.getAllele1() << endl;
	cout << "getAllele2                         =" << o.getAllele2() << endl;
	o.setAllele1(4);
	o.setAllele2(-8);
	cout << "4/-8                               =" << o << endl;
	cout << "getAllele1                         =" << o.getAllele1() << endl;
	cout << "getAllele2                         =" << o.getAllele2() << endl;
	Genotype p("೪೮/೫೦");
	cout << "Genotype Number in Kannada 48/50   =" << p << endl;
	p.setAllele1("५४");
	cout << "48/50 after setAllele1(54) in Hindi=" << p << endl;
	p.setAllele2("५६");
	cout << "After setAllele2(56) in Hindi      =" << p << endl;
	Genotype q(12,14);
	cout << "12/14                              =" << q << endl;
	cout << "12/14+=2                           =" << (q+=2) << endl;
	cout << "14/16+=-13                         =" << (q+=-13) << endl;
	cout << "1/2+=-1                            =" << (q+=-1) << endl;
	cout << ".+=1                               =" << (q+=1) << endl;
	cout << "121/125-=4                         =" << (b-=4) << endl;

	cout << "117/121-=-20                       =" << (b-=-20) << endl;
	cout << "137/141-=140                       =" << (b-=140) << endl;
	cout << ".-=1                               =" << (b-=1)   << endl;
	cout << "14/18+=Number(6)                   =" << (c+=Number(6)) << endl;	
	cout << "20/24+=Number('6.7')               =" << (c+=Number("6.7")) << endl;
	cout << "26/30+=Number('~5')                =" << (c+=Number("~5")) << endl;
	c.set(26,30);
	cout << "26/30+=Number('[4-5]')             =" << (c+=Number("[4-5]")) << endl;
	cout << ".+=Number(4)                       =" << (c+=Number(4)) << endl;
	c.set(54,60);
	cout << "54/60-=Number(4)                   =" << (c-=Number(4)) << endl;
	Number nn;
	cout << "50/56-=Number                      =" << (c-=nn) << endl;
	cout << "50/56-=Number('5.6')               =" << (c-=Number(5.6)) << endl;
	cout << "45/51-=Number(45)                  =" << (c-=Number(45)) << endl;
	c.set(45,51);
	cout << "45/51-=Number('~5')                =" << (c-=Number("~5")) << endl;
	cout << "45/51-=Number('[4-5]')             =" << (c-=Number("[4-5]")) << endl;
	cout << ".-=Number(4)                       =" << (c-=Number(4)) << endl;
	c.set("a/c");
	cout << "a/c                                =" << c << endl;
	c.set(67,69);
	cout << "a/c after set(67,69)               =" << c << endl;
	c.setAllele1("64");
	cout << "67/69 after setAllele1('64')       =" << c << endl;
	c.setAllele1("65m n");
	cout << "64/69 after setAllele1('65 mn')    =" << c << endl;	

	Genotype d;

	d+=187;
	d+=181;
	cout << "d: " << d << endl;

	Genotype e;
	e.set(12,17);
	cout << "e: " << e << endl;
	
	Genotype f("0/12");
	cout << "f: " << f << endl;
	cout << "f is missing? " << f.isMissing() << endl;
	
	f.set(1234,1111);
	cout << "f: " << f << endl;
	cout << "f is missing? " << f.isMissing() << endl;
	
	snp.setAllele2(Genotype::SNP_C_ORDINAL);
	cout << "SNP: " << snp << endl;
	snp.setAllele1(Genotype::SNP_T_ORDINAL);
	snp.setAllele2(0);
	cout << "SNP: " << snp << endl;
	snp.setAllele1(12);
	cout << "SNP TEST: " << snp << endl;
	snp.set("G/0");
	cout << "SNP: " << snp << endl;
	snp.set("Y/T");
	cout << "SNP: " << snp << endl;
	cout << "SNP allele1 is: " << snp.getAllele1() << endl;
	cout << "SNP allele2 is: " << snp.getAllele2() << endl;
	
	Genotype t1(" . ");
	cout << "t1: " << t1 << endl;
	t1.setAllele1(4);
	t1.setAllele2(5);
	cout << "t1: " << t1 << endl;
	Genotype t2(" 0   /  0");
	cout << "t2: " << t2 << endl;
	Genotype t3("./.");
	cout << "t3: " << t3 << endl;
	Genotype t4(" .   /    .  ");
	cout << "t4: " << t4 << endl;

	// ADDED: on 2005-07-19 to test < operator:
	cout << "Test < operator" << endl;
	Genotype l1(" 120/120 ");
	Genotype l2(" 120/124 ");
	Genotype l3(" 120/120 ");
	Genotype l4(" C/T ");
	Genotype l5(" C/C ");
	Genotype l6(" C/T ");
	Genotype l7(" 118/120 ");
	Genotype l8(" 1/1 ");
	Genotype l9(" T/T ");
	cout << "l1 is  : " << l1 << endl;
	cout << "l2 is  : " << l2 << endl;
	cout << "l3 is  : " << l3 << endl;
	cout << "l4 is  : " << l4 << endl;
	cout << "l5 is  : " << l5 << endl;
	cout << "l6 is  : " << l6 << endl;
	cout << "l7 is  : " << l7 << endl;
	cout << "l8 is  : " << l8 << endl;
	cout << "l9 is  : " << l9 << endl;
	cout << "l1 < l2: " << (l1 < l2) << endl;
	cout << "l1 < l3: " << (l1 < l3) << endl;
	cout << "l1 < l7: " << (l1 < l7) << endl;
	cout << "l1 < t2: " << (l1 < t2) << endl;
	cout << "t2 < t3: " << (t2 < t3) << endl;
	cout << "l4 < l2: " << (l4 < l2) << endl;
	cout << "l8 < l4: " << (l8 < l4) << endl;
	cout << "l4 < l8: " << (l4 < l8) << endl;
	cout << "l4 < t2: " << (l4 < t2) << endl;
	cout << "l5 < l4: " << (l5 < l4) << endl;
	cout << "l4 < l6: " << (l4 < l6) << endl;
	cout << "l4 < l9: " << (l4 < l9) << endl;
	cout << "Test == operator" << endl;
	cout << "l1 == l3 " << (l1 == l3) << endl;
	cout << "l1 == l7 " << (l1 == l7) << endl;
	cout << "l1 == t4 " << (l1 == t4) << endl;
	cout << "t2 == t3 " << (t2 == t3) << endl;
	cout << "l8 == l9 " << (l8 == l9) << endl;
	cout << "l4 == l6 " << (l4 == l6) << endl;
	cout << "l4 == l5 " << (l4 == l5) << endl;
	cout << "l4 == t3 " << (l4 == t3) << endl; 
	Genotype l10("9/10");
	cout << "l10=9/10 " << l10 << endl;
	Genotype::addGenotypeMissingValue("9/9");

	cout << "Added 9/9 as missing value" << endl;
	l10.setAllele2("9");
	cout << "l10.setAllele2(9) " << l10 << endl;
	l10.set("10/12");
	cout << "l10=10/12         " << l10 << endl;
	l10.set(9,9);
	cout << "l10.set(9,9)      " << l10 << endl;
	Genotype::addGenotypeMissingValue("x|x");
	cout << "Added x|x as missing value" << endl;
	Genotype l11("x|x");
	cout << "l11(x|x)          " << l11 << endl;

	//2005.09.12.ET Addenda: Testing isA() method:
	std::string g1("1/2");
	std::string g2("A/C");
	std::string g3("   G    /    T    ");
	std::string g4(" 91 / 111 ");
	std::string g5(" A / D ");
	std::string g6("220/226");
	std::string g7("1984/1986");
	std::string g8("1984/12");
	std::string g9("12/1984");
	std::string gA("1984/12/10");
	
	Warning::suppressWarnings(true);
	std::cout << "Classification: " << g1 << " : " << Genotype::isA(g1) << std::endl;
	std::cout << "Classification: " << g2 << " : " << Genotype::isA(g2) << std::endl;
	std::cout << "Classification: " << g3 << " : " << Genotype::isA(g3) << std::endl;
	std::cout << "Classification: " << g4 << " : " << Genotype::isA(g4) << std::endl;
	std::cout << "Classification: " << g5 << " : " << Genotype::isA(g5) << std::endl;
	std::cout << "Classification: " << g6 << " : " << Genotype::isA(g6) << std::endl;
	std::cout << "Classification: " << g7 << " : " << Genotype::isA(g7) << std::endl;
	std::cout << "Classification: " << g8 << " : " << Genotype::isA(g8) << std::endl;
	std::cout << "Classification: " << g9 << " : " << Genotype::isA(g9) << std::endl;
	std::cout << "Classification: " << gA << " : " << Genotype::isA(gA) << std::endl;
	Warning::suppressWarnings(false);
	
	return 0;

}
