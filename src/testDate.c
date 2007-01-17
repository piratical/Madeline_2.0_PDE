#include <iostream>
#include "Date.h"
#include "Warning.h"

using namespace std;

int main( void ){

	Date aaa("1967.12.06");
	std::cout << "1967.12.06                            =" << aaa << std::endl;
	Date bbb("1967.12.12 - 26");
	std::cout << "1967.12.12 - 26                       =" << bbb << std::endl;
	
	Date ccc("1987.04.[13R29]");
	std::cout << "1987.04.[13R29]                       =" << ccc << std::endl;
	Date ddd("1987.04.[23R29");
	std::cout << "1987.04.[23R29                        =" << ddd << std::endl;
	Date eee("1987.04.[29 - 23]");
	std::cout << "1987.04.[29 - 23]                     =" << eee << std::endl;
	Date fff("1935/06/[12 22]");
	std::cout << "1935/06/[12 22]                       =" << fff << std::endl;
	Date ggg("1945.01[11 r 12]");
	std::cout << "1945.01[11 r 12]                      =" << ggg << std::endl;
	
	Date hhh("1945/01/[11R12a]");
	std::cout << "1945/01/[11R12a]                      =" << hhh << std::endl;
	Date iii("1945-01-11 - 12 b");
	std::cout << "1945-01-11 - 12 b                     =" << iii << std::endl;
	Date jjj("1945-01-[11R12]-1");
	std::cout << "1945-01-[[11R12]-1                    =" << jjj << std::endl;
	Date kkk("1945-01-11 - 12-03");
	std::cout << "1945-01-11 - 12-03                    =" << kkk << std::endl;
	Date lll("1945-01-11 - 12a");
	std::cout << "1945-01-11 - 12a                      =" << lll << std::endl;
	Date mmm("1945.01.[11R12] 1");
	std::cout << "1945.01.[11R12] 1                     =" << mmm << std::endl;
	
	Date nnn("1945/22/23");
	std::cout << "1945/22/23                            =" << nnn << std::endl;
	Date ooo("1999/02/29");
	std::cout << "1999/02/29                            =" << ooo << std::endl;
	Date ppp("1999/03/32");
	std::cout << "1999/03/32                            =" << ppp << std::endl;
	Date qqq("~1999/03/12 - 22");
	std::cout << "~1999/03/12 - 22                      =" << qqq << std::endl;
        Date rrr("~ -0500/[09 r 12]");
	std::cout << "~ -0500/[09 r 12]                     =" << rrr << std::endl;
	Date sss("~ [-0500 R +0500]");
	std::cout << "~ [-0500 R +0500]                     =" << sss << std::endl; 

	Date xxx("1987/ 02/ 12");
	std::cout << "1987/ 02/ 12                          =" << xxx << std::endl;
	Date yyy("1987.02. 05");
	std::cout << "1987.02. 05                           =" << yyy << std::endl;
	Date zzz("1987.04.12 -  26");
	std::cout << "1987.04.12 -  26                      =" << zzz << std::endl;
	Date aa("1967 - 1969");
	std::cout << "1967 - 1969                           =" << aa << std::endl;
	
	Date bb("1967-12");
	std::cout << "1967-12                               =" << bb << std::endl;
	
	Date cc("1967-02 - 04");
	std::cout << "1967-02 - 04                          =" << cc << std::endl;
	
	Date dd("1983.[07-09]");
	std::cout << "1983.[07-09]                          =" << dd << std::endl;
	
	Date ee("1983.[07-09");
	std::cout << "1983.[07-09                           =" << ee << std::endl;
	
	Date ff("1983.[ 09 r  07  ]");
	std::cout << "1983.[ 09 r 07 ]                      =" << ff << std::endl;
	
	Date gg("1945");
	std::cout << "1945                                  =" << gg << std::endl;
	
	Date hh("1945[06-08]");
	std::cout << "1945[06-08]                           =" << hh << std::endl;
	
	Date ii("1945-[06 08]");
	std::cout << "1945-[06 08]                          =" << ii << std::endl;
	Date jj("1945-[06-08]z");
	std::cout << "1945-[06-08]z                         =" << jj << std::endl;
	
	Date kk("1945-[06-08]-03");
	std::cout << "1945-[06-08]-03                       =" << kk << std::endl;
	
	Date ll("1945-[06-08a]");
	std::cout << "1945-[06-08a]                         =" << ll << std::endl;
	
	Date mm("1945-03 - 04 a");
	std::cout << "1945-03 - 04 a                        =" << mm << std::endl;
	
	Date nn("1945-03 - 04a");
	std::cout << "1945-03 - 04a                         =" << nn << std::endl;
	
	Date oo("1945-03 - 04-02");
	std::cout << "1945-03 - 04-02                       =" << oo << std::endl;
	
	Date a("[1999 - 2000]");
	std::cout << "[1999 - 2000]                         =" << a << std::endl;
	Date b(" [2000- 2001]");
	std::cout << " [2000- 2001]                         =" << b << std::endl;
	Date c(" ~[-0300 r -0200]");
	std::cout << "~[-0300 r -0200]                      =" << c << std::endl;
	Date d(" [1978  1979]");
	std::cout << "[1978 1979]                           =" << d << std::endl;
	
	Date e(" [1978 r 1979");
	std::cout << "[1978 r 1979                          =" << e << std::endl;
	
	Date f("[1980 R 1979]");
	std::cout << "[1980 R 1979]                         =" << f << std::endl;
	
	Date g("1980r1981]");
	std::cout << "1980r1981                             =" << g << std::endl;
	
	Date h("1980 - 1983");
	std::cout << "1980 - 1983                           =" << h << std::endl;
	
	Date i("1980 - 1984   a");
	std::cout << "1980 - 1984  a                        =" << i << std::endl;
	
	Date j("[1980 R 1984]-03");
	std::cout << "[1980 R 1984]-03                      =" << j << std::endl;
	
	Date k("[1980 R 1984 2]");
	std::cout << "[1980 R 1984 2]                       =" << k << std::endl;
	
	Date l("[1980 R 1984]x");
	std::cout << "[1980 R 1984]x                        =" << l << std::endl;
	
	Date m("1983-07-09");
	std::cout << "1983-07-09                            =" << m << std::endl;
	
	Date n("1983a-04-04");
	std::cout << "1983a-04-04                           =" << n << std::endl;
	Date o("1983-03a-03");
	std::cout << "1983-03a-03                           =" << o << std::endl;
	
	Date p("1983-02-02a");
	std::cout << "1983-02-02a                           =" << p << std::endl;
	
	Date q("1983 - 1985a");
	std::cout << "1983 - 1985a                          =" << q << std::endl;
	
	Date r("1900 - 2000-12");
	std::cout << "1900 - 2000-12                        =" << r << std::endl;

	Date s("~ -0300.01.05 - -0299.06.05");
	std::cout << "~ -0300.01.05 - -0299.06.05           =" << s << std::endl;
	Date t("1967.12.03 - 1968.03.04");
	std::cout << "1967.12.03 - 1968.03.04               =" << t << std::endl;
	Date u("1967-12-03 - 1979.20");
	std::cout << "1967-12-03 - 1979.20                  =" << u << std::endl;
	Date v("1967-12-03 - 1964-12-04");
	std::cout << "1967-12-03 - 1964-12-04               =" << v << std::endl;
	Date w("1967/12/03 - 1968/[03-06]");
	std::cout << "1967/12/03 - 1968/[03-06]             =" << w << std::endl;
	Date x("1967/12/[03-04] - 1968/04/23");
	std::cout << "1967/12/[03-04] - 1968/04/23          =" << x << std::endl;
       
	Date y("~1987-08-21");
	std::cout << "~1987-08-21                           =" << y << std::endl; 
	Date z("[1900 - 2000]");
	std::cout << "[1900 - 2000]                         =" << z << std::endl;
	Date hindi("१९८३-०९-०७");
	std::cout << "1983-09-07 in hindi                   =" << hindi << std::endl;
	Date invhindi("१९३२-०९-[२३-३४]");
	std::cout << "1932-09-[23-34] in hindi              =" << invhindi << std::endl;
	
	// ADDED: on 2005-07-19
	std::cout << "Testing < Operator " << std::endl;
	std::cout << "(1967/12/03 - 1968/[03-06] < invhindi) :" << (w < invhindi) << std::endl;
	std::cout << "(invhindi < hindi)                     :" << (invhindi < hindi) << std::endl;
	std::cout << "(~1987-08-21 < [1900-2000])            :" << (y < z) << std::endl;
	Date d1("1987-08-23");
	Date d2("1987-08-20");
	Date d3("~1987-08-27");
	Date d4("[1987-1990]");
	Date d5("1987-08");
	
	std::cout << "aaa is                                 :" << aaa << std::endl;
	std::cout << "d1 is                                  :" << d1 << std::endl;
	std::cout << "d2 is                                  :" << d2 << std::endl;
	std::cout << "d3 is                                  :" << d3 << std::endl;
	std::cout << "d4 is                                  :" << d4 << std::endl;
	std::cout << "d5 is                                  :" << d5 << std::endl;
	std::cout << "(d1 < ~1987-08-21)                     :" << (d1 < y) << std::endl;
	std::cout << "(d2 < ~1987-08-21)                     :" << (d2 < y) << std::endl; 
	std::cout << "(d3 < ~1987-08-21)                     :" << (d3 < y) << std::endl; 
	std::cout << "(d2 < d1)                              :" << (d2 < d1) << std::endl;
	std::cout << "(1967/12/[03-04]-1968/04/23 < invhindi :" << (x < invhindi) << std::endl;
	std::cout << "(1967/12/[03-04]-1968/04/23 < d2       :" << (x < d2) << std::endl;
	std::cout << "(1967/12/[03-04]-1968/04/23 < 1900-2000:" << (x < z) << std::endl;
	std::cout << "(1967/12/[03-04]-1968/04/23 < d4       :" << (x < d4) << std::endl;
	std::cout << "d4 < aaa                               :" << (d4 < aaa) << std::endl;
	std::cout << "d4 < d5                                :" << (d4 < d5) << std::endl;
	std::cout << "d3 < d5                                :" << (d3 < d5) << std::endl;
	std::cout << "Testing == Operator " << std::endl;
	std::cout << "(1967/12/03 - 1968/[03-06] == invhindi):" << (w == invhindi) << std::endl;
	std::cout << "(invhindi == hindi)                    :" << (invhindi == hindi) << std::endl;
	std::cout << "(~1987-08-21 == [1900-2000])           :" << (y == z) << std::endl;
	std::cout << "(d1 == ~1987-08-21)                    :" << (d1 == y) << std::endl;
	std::cout << "(d2 == ~1987-08-21)                    :" << (d2 == y) << std::endl;
	std::cout << "(d5 == ~1987-08-21)                    :" << (d5 == y) << std::endl;
	std::cout << "(d2 == d1)                             :" << (d2 == d1) << std::endl;
	std::cout << "(1967/12/[03-04]-1968/04/23 ==invhindi :" << (x == invhindi) << std::endl;
	std::cout << "(1967/12/[03-04]-1968/04/23 == d2      :" << (x == d2) << std::endl;
	std::cout << "(1967/12/[03-04]-1968/04/23 ==1900-2000:" << (x == z) << std::endl;
	std::cout << "(1967/12/[03-04]-1968/04/23 == d4      :" << (x == d4) << std::endl;
	std::cout << "d4 == aaa                              :" << (d4 == aaa) << std::endl;
	std::cout << "d1 == aaa                              :" << (d1 == aaa) << std::endl;
	std::cout << "d4 == d5                               :" << (d4 == d5) << std::endl;
	std::cout << "d4 == d3                               :" << (d4 == d3) << std::endl;
	Date d6("0000-00-00");
	std::cout << "d6=0000-00-00                          :" << d6 << std::endl;
	Date d7(".");
	std::cout << "d7=.                                   :" << d7 << std::endl;
	Date::addDateMissingValue("0000-00-00");
	std::cout << "Added 0000-00-00 as missing value" << std::endl;
	d7.set("1984");
	std::cout << "d7=1984                                :" << d7 << std::endl;
	d7.set("0000-00-00");
	std::cout << "d7=0000-00-00                          :" << d7 << std::endl;
	
	std::cout << "*** NEW TEST DATE *** " << std::endl;
	std::cout << "*** NEW TEST DATE *** " << std::endl;
	std::cout << "*** NEW TEST DATE *** " << std::endl;

	Date test("-0200.10.12");
	cout << test << endl;
	Date D("1923.10.31");
	
	cout << "ORIGINAL      :" << D << endl;
	D++;
	cout << "POST INCREMENT:" << D << endl;
	D--;
	cout << "POST DECREMENT:" << D << endl;
	++D;
	cout << "PRE  INCREMENT:" << D << endl;
	--D;
	cout << "PRE  DECREMENT:" << D << endl;
	D+=15;
	cout << "+=15 days     :" << D << endl;
	D-=15;
	cout << "-=15 days     :" << D << endl;
	
	Date E("1923.02 r 1930.07");
	cout << "E ORIGINAL    :" << E << endl;
	E+=15;
	cout << "E +=15 days   :" << E << endl;
	E-=15;
	cout << "E -=15 days   :" << E << endl;
	
	Number N("10-15");
	E+=N;
	cout << "E += [10-15] days   : EXPECTED={1923.02.11 r 1930.08.15} ACTUAL=" << E << endl;
	E-=N;
	cout << "E -= [10-15] days   : EXPECTED={1923.01.27 r 1930.08.05} ACTUAL=" << E << endl;
	
	Date F("1970 r 1980");
	cout << "F                   : " << F << endl;
	cout << "F + [10-15] days    : EXPECTED={1970.01.11 r 1981.01.15} ACTUAL=" << (F+N) << endl;
	cout << "F - [10-15] days    : EXPECTED={1969.12.17 r 1979.12.21} ACTUAL=" << (F-N) << endl;
	
	Date G("1963.07.04");
	Date H("1959.08.12");
	cout << "G" << G << " - H" << H << " = EXPECTED: 1,422 ACTUAL: " << (G-H) << endl;
	
	Date GG("1963.07");
	Date HH("1959");
	cout << "GG" << GG << " - H" << H << " = EXPECTED: [1419 r 1449] ACTUAL: " << (GG-H) << endl;
	cout << "GG" << GG << " - HH" << HH << " = EXPECTED: [1278 r 1672] ACTUAL: " << (GG-HH) << endl;
	
	cout << "HH" << HH << " - GG" << GG << " = EXPECTED: . ACTUAL: " << (HH-GG) << endl;
	cout << "H" << H << " - G" << G << " = EXPECTED: . ACTUAL: " << (H-G) << endl;
	cout << "H" << H << " - GG" << GG << " = EXPECTED: . ACTUAL: " << (H-GG) << endl;
	
	Date I("1959.07.12 - 1959.09.12");
	cout << "H" << H << " - I" << I << " = EXPECTED: . ACTUAL: " << (H-I) << endl;
	
	Date W("1967/12/03 - 1968/[03-06]");
	std::cout << "1967/12/03 - 1968/[03-06]             =" << W << std::endl;
	
	//
	// 2005.09.09 TESTING THE "isA" method:
	//
	std::string D1("1967.05.04");
	// No 29th in Feb. 1967:
	std::string D2("1967.02.29");
	// Ranged dates:
	std::string D3("1967-05-[01r15]");
	std::string D4("1923 - 1925");
	std::string D5("abcd/ef/gh");
	std::string D6(".");
	std::string D7("      ");
	
	Warning::suppressWarnings(true);
	std::cout << "Classification: " << D1 << " : " << Date::isA(D1) << std::endl;
	std::cout << "Classification: " << D2 << " : " << Date::isA(D2) << std::endl;
	std::cout << "Classification: " << D3 << " : " << Date::isA(D3) << std::endl;
	std::cout << "Classification: " << D4 << " : " << Date::isA(D4) << std::endl;
	std::cout << "Classification: " << D5 << " : " << Date::isA(D5) << std::endl;
	std::cout << "Classification: " << D6 << " : " << Date::isA(D6) << std::endl;
	std::cout << "Classification: " << D7 << " : " << Date::isA(D7) << std::endl;
	Warning::suppressWarnings(false);

	std::cout << "*** TEST ASIAN DATES ***" << std::endl;
	std::cout << "*** TEST ASIAN DATES ***" << std::endl;
	std::cout << "*** TEST ASIAN DATES ***" << std::endl;
	
	
	Date da("2003年03月09日");
	cout << "da: 2003年03月09日: " << da << endl;
	Date db("2003年4月8日");
	cout << "db: 2003年4月8日: " << db << endl;

	Date dc("2003到2005");
	cout << "dc: 2003到2005: " << dc << endl;
	Date dcc("2003到2005年");
	cout << "dcc: 2003到2005年: " << dcc << endl;

	Date dccc("2003年02到2003年07月");
	cout << "dccc: 2003年02到2003年07月: " << dccc << endl;
	Date de("2003年02到07月");
	cout << "de: 2003年02到07月: This should fail: " << de << endl;
	Date dee("2003年02月到07月");
	cout << "dee: 2003年02月到07月: This should also fail: " << dee << endl;
	Date deee("2003年［02到07］月");
	cout << "deee: 2003年［02到07］月: But this should be OK : " << deee << endl;

	Date df("2003年08月01日到15日");
	cout << "df: 2003年08月01日到15日: This should fail: " << df << endl;
	Date dff("2003年08月[01日到15]日");
	cout << "dff: 2003年08月[01到15]日: This should be OK: " << dff << endl;
	Date dfff("2003年08月[01日到15日]");
	cout << "dfff: 2003年08月[01日到15日]: This seems to work. Wow!: " << dfff << endl;

	Date dg("2003。05。05");
	cout << "dg: 2003。05。05: " << dg << endl;
	Date dh("2004／06／06");
	cout << "dh: 2004／06／06: " << dh << endl;
	Date di("2005-07-07");
	cout << "di: 2005-07-07: " << di << endl;
	Date dj("2006-[10-12]");
	cout << "dj: 2006-[10-12]: " << dj << endl;
	Date djj("2006－［10－12］");
	cout << "djj: 2006－［10－12］" << djj << endl;
	Date dk("1895→1902");
	cout << "dk: 1895→1902: " << dk << endl;
	Date dl("1895.12.05→1922.07");
	cout << "dl: 1895.12.05→1922.07:" << dl << endl;
	Date dm("1967년09월11일");
	cout << "dm: 1967년09월11일: " << dm << endl;
	Date dn("1967년[09-12]월");
	cout << "dn: 1967년[09-12]월: " << dn << endl;

	Date doo("2043年[08月到10月]");
	cout << "doo: 2043年[08月到10月]: This seems to work. Wow!: " << doo << endl;
	Date dp("[2003年到2008年]");
	cout << "dp:[2003年到2008年]: This seems to work. Wow!: " << dp << endl;


	return 0;
	
}
