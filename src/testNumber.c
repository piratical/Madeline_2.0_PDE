#include <iostream>
#include <iomanip>
#include <math.h>

#include "Number.h"
#include "Warning.h"

using namespace std;

int main(){

	std::cout << "*** TESTING NUMBER CLASS : UNRANGED TESTS ***" << std::endl;
	std::cout << "*** TESTING NUMBER CLASS : UNRANGED TESTS ***" << std::endl;
	std::cout << "*** TESTING NUMBER CLASS : UNRANGED TESTS ***" << std::endl;
	std::cout << "PI                           =" << std::setw(28) << "3.1415" << std::setw(28) << Number::PI << std::endl;
	std::cout << "MISSING                      =" << std::setw(28) << "." << std::setw(28) << Number::MISSING << std::endl; 

	Number inv("Hello");
	std::cout << "Invalid Number               =" << std::setw(28) << "." << std::setw(28) << inv << std::endl;
	std::cout << "Cos(.)                       ="  << std::setw(28) << "." << std::setw(28) << inv.getCos() <<  std::endl;
	std::cout << "Sqrt(.)                      =" << std::setw(28) << "." << std::setw(28) << inv.getSquareRoot() << std::endl;
	std::cout << "Log(.)                       =" << std::setw(28) << "." << std::setw(28) << inv.getLog() << std::endl;
	std::cout << "(.)++                        =" << std::setw(28) << "." << std::setw(28) << (inv++) << std::endl;
	std::cout << "++(.)                        =" << std::setw(28) << "." << std::setw(28) << (++inv) << std::endl;

	Number hindi("१३.२");

	std::cout << "Hindi Number                 =" << std::setw(28) << "13.2" << std::setw(28) << hindi << std::endl;
	std::cout << ".+=13.2                      =" << std::setw(28) << "."    << std::setw(28) << (inv+=hindi) << std::endl;
	std::cout << "./13.2                       =" << std::setw(28) << "."    << std::setw(28) << (inv/hindi) << std::endl;
	Number invhindi("आकाश26"); // Akash26
	std::cout << "Hindi Number2                =" << std::setw(28) << "." << std::setw(28) << invhindi << std::endl;
	std::cout << ".+.                          =" << std::setw(28) << "." << std::setw(28) << (inv+invhindi) << std::endl;
	std::cout << ".+=.                         =" << std::setw(28) << "." << std::setw(28) << (inv+=invhindi) << std::endl;
	std::cout << "TRIGONOMETRIC FUNCTIONS:" << std::endl;
	Number tri(3*Number::PI/2);
	std::cout << "Sin(3*pi/2)                  =" << std::setw(28) <<  "-1" << std::setw(28) << tri.getSin() << std::endl;
	std::cout << "Cos(3*pi/2)                  =" << std::setw(28) << "0" << std::setw(28) << tri.getCos() << std::endl;
	std::cout << "Tan(3*pi/2)                  =" << std::setw(28) << "." << std::setw(28) << tri.getTan() << std::endl;
	Number testpi(Number::PI);
	std::cout << "Sin(pi)                      =" << std::setw(28) << "0" << std::setw(28) << testpi.getSin() << std::endl;
	std::cout << "Cos(pi)                      =" << std::setw(28) << "-1" << std::setw(28) << testpi.getCos() << std::endl;
	std::cout << "Tan(pi)                      =" << std::setw(28) << "0" << std::setw(28) << testpi.getTan() << std::endl;
	Number test3pi(Number::PI*3);
	Number tri2(0.5);
	std::cout << "Asin (0.5)                   =" << std::setw(28) << "0.5236" << std::setw(28) << tri2.getAsin() << std::endl;
	std::cout << "Acos (0.5)                   =" << std::setw(28) << "1.047" << std::setw(28) << tri2.getAcos() << std::endl;
	std::cout << "Atan (0.5)                   =" << std::setw(28) << "0.4636" << std::setw(28) << tri2.getAtan() << std::endl;
	std::cout << "Asin (pi)                    =" << std::setw(28) << "." << std::setw(28) << testpi.getAsin() << std::endl;
	std::cout << "Sinh (0.5)                   =" << std::setw(28) << "0.521" << std::setw(28) << tri2.getSinh() << std::endl;
	std::cout << "Cosh (0.5)                   =" << std::setw(28) << "1.127" << std::setw(28) << tri2.getCosh() << std::endl;
	std::cout << "Tanh (0.5)                   =" << std::setw(28) << "0.4621" << std::setw(28) << tri2.getTanh() << std::endl;
	std::cout <<  "Sin(pi)==Sin(3pi)?          =" << std::setw(28) << "1" << std::setw(28) << (testpi.getSin() == test3pi.getSin()) << std::endl;
	std::cout <<  "Sin(3pi/2)!=Cos(3pi)?       =" << std::setw(28) << "0" << std::setw(28) << (tri.getSin() != test3pi.getCos()) << std::endl;
	std::cout << "NUMERIC FUNCTIONS: " << std::endl;
	Number a(0.5);
	Number neg(-10);
	Number zero(0.0);
	Number b(4.2);
	Number c(3.9);
	Number d(100);
	Number e(0.125);
	Number one(1);
	std::cout << "Log(0.5)                     =" << std::setw(28) << "-0.693" <<  std::setw(28) << a.getLog() << std::endl;
	std::cout << "Log(-10)                     =" << std::setw(28) << "." << std::setw(28) <<  neg.getLog() << std::endl;
	std::cout << "Log10(0.5)                   =" << std::setw(28) << "-0.3010" << std::setw(28)  << a.getLog10() << std::endl;
	std::cout << "Log10(-10)                   =" << std::setw(28) << "." << std::setw(28) << neg.getLog10() << std::endl;
	std::cout << "Abs(-10)                     =" << std::setw(28) << "10" << std::setw(28) << neg.getAbsoluteValue() << std::endl;
	std::cout << "Abs(0.5)                     =" << std::setw(28) <<  "0.5" << std::setw(28) << a.getAbsoluteValue() << std::endl;
	std::cout << "Sqrt(-10)                    =" << std::setw(28) << "." << std::setw(28) << neg.getSquareRoot() << std::endl;
	std::cout << "Sqrt(0.5)                    =" << std::setw(28) << "0.7071" << std::setw(28) << a.getSquareRoot() << std::endl;
	std::cout << "Power(-10^0.5)               =" << std::setw(28) << "." << std::setw(28) << neg.getPower(a) << std::endl;
	std::cout << "Power(0.5^-10)               =" << std::setw(28) << "1024" << std::setw(28) << a.getPower(neg) << std::endl;
	std::cout << "Exp(-10)                     =" << std::setw(28) << "4.54e-5" << std::setw(28) << neg.getExponent() << std::endl;
	std::cout << "Inverse(-10)                 =" << std::setw(28) << "-0.1" << std::setw(28) << neg.getInverse() << std::endl;
	std::cout << "Inverse(0)                   =" << std::setw(28) << "." << std::setw(28) << zero.getInverse() << std::endl;
	std::cout << "Floor(3.9)                   =" << std::setw(28) << "3" << std::setw(28) << c.getFloor() << std::endl;
	std::cout << "Floor(4.2)                   =" << std::setw(28) << "4" << std::setw(28) << b.getFloor() << std::endl;
	std::cout << "Ceil(3.9)                    =" << std::setw(28) << "4" << std::setw(28) << c.getCeiling() << std::endl;
	std::cout << "Ceil(4.2)                    =" << std::setw(28) << "5" << std::setw(28) << b.getCeiling() << std::endl;
	std::cout << "Round(4.2)                   =" << std::setw(28) << "4" << std::setw(28) << b.getRound() << std::endl;
	std::cout << "Round(3.9)                   =" << std::setw(28) << "4" << std::setw(28) << c.getRound() << std::endl;

	std::cout << "OPERATORS : " << std::endl;
	std::cout << "-10+4.2                      =" << std::setw(28) << "-5.8" << std::setw(28) << (neg+b) << std::endl;
	std::cout << "3.9-4.2                      =" << std::setw(28) << "-0.3" << std::setw(28) << (c-b) << std::endl;
	std::cout << "3.9*(-10)                    =" << std::setw(28) << "39" <<  std::setw(28) << (c*neg) << std::endl; 
	std::cout << "4.2/-10                      =" << std::setw(28) << "-.42" << std::setw(28) <<  (b/neg) << std::endl;
	std::cout << "4.2/0                        =" << std::setw(28) << "." << std::setw(28) << (b/zero) << std::endl;
	std::cout << "4.2++                        =" << std::setw(28) << "4.2" <<  std::setw(28) << b++ << std::endl;
	std::cout << "3.9--                        =" << std::setw(28) << "3.9" << std::setw(28) << c--  << std::endl;
	std::cout << "--(4.2++)                    =" << std::setw(28) << "4.2" << std::setw(28) << --b << std::endl;
	std::cout << "++(3.9--)                    =" << std::setw(28) << "3.9" << std::setw(28) << ++c << std::endl;
	std::cout << "3.9*=(-10)                   =" << std::setw(28) << "-39" << std::setw(28) << (c*=neg) << std::endl;
	std::cout << "that/(-10)                   =" << std::setw(28) << "3.9" << std::setw(28) <<  (c/=neg) << std::endl; 
	std::cout << "4.2+=(-10)                   =" << std::setw(28)<< "-5.8" << std::setw(28) << (b+=neg) << std::endl;
	std::cout << "that-=(-10)                  =" << std::setw(28) << "4.2" << std::setw(28) <<  (b-=neg) << std::endl;
	std::cout << "4.2/=0                       =" << std::setw(28) << "." << std::setw(28) << (b/=zero) << std::endl;
	Number sci(4e5);
	std::cout << "Scientific Number            =" << std::setw(28) << "4e5" << std::setw(28) << sci << std::endl;
	std::cout << "Log10(4e5)                   =" << std::setw(28) << "5.6020" << std::setw(28) << sci.getLog10() << std::endl;
	std::cout << "4e5-10                       =" << std::setw(28) << "399990" << std::setw(28) << (sci+neg) << std::endl;
	
	std::cout << "GENETIC METHODS: " << std::endl;
	std::cout << "Lod(1)                       =" << std::setw(28) << "0" << std::setw(28) << one.getLod() << std::endl;
	std::cout << "Lod(0.5)                     =" << std::setw(28) << "0.3010" << std::setw(28) << a.getLod() << std::endl;
	std::cout << "Lod(-10)                     =" << std::setw(28) << "." << std::setw(28) << neg.getLod() << std::endl;	
	std::cout << "haldaneToTheta(100)          =" << std::setw(28) << "0.432" << std::setw(28) << d.haldaneToTheta() << std::endl;
	std::cout << "haldaneToTheta(-10)          =" << std::setw(28) << "." << std::setw(28) << neg.haldaneToTheta() << std::endl;
	std::cout << "thetaToHaldane(0.5)          =" << std::setw(28) << "." << std::setw(28) << a.thetaToHaldane() << std::endl;
	std::cout << "thetaToHaldane(0.125)        =" << std::setw(28) << "14.4" << std::setw(28) << e.thetaToHaldane() << std::endl;
	std::cout << "thetaToHaldane(-10)          =" << std::setw(28) << "." << std::setw(28) << neg.thetaToHaldane() << std::endl;
	std::cout << "kosambiToTheta(100)          =" << std::setw(28) << "0.482" << std::setw(28) << d.kosambiToTheta() << std::endl;
	std::cout << "kosambiToTheta(-10)          =" << std::setw(28) << "." << std::setw(28) << neg.kosambiToTheta() << std::endl;
	std::cout << "thetaToKosambi(0.5)          =" << std::setw(28) << "." << std::setw(28) << a.thetaToKosambi() << std::endl;
	std::cout << "thetaToKosambi(0.125)        =" << std::setw(28) << "12.8" << std::setw(28) << e.thetaToKosambi() << std::endl;
	std::cout << "thetaToKosambi(-10)          =" << std::setw(28) << "." << std::setw(28) << neg.thetaToKosambi() << std::endl;
	
	std::cout << "*** TESTING NUMBER CLASS : RANGE TESTS ***" << std::endl;
	std::cout << "*** TESTING NUMBER CLASS : RANGE TESTS ***" << std::endl;
	std::cout << "*** TESTING NUMBER CLASS : RANGE TESTS ***" << std::endl;
	std::cout << "APPROXIMATIONS AND RANGES" << std::endl;
	
	Number app("~6");
	Number two(2);
	Number app1("~3");
	Number app2("~5.4");
	c=Number(3.9);
	std::cout << "~6                           =" << std::setw(28) << "~6" << std::setw(28) << app.get() << std::endl;
	std::cout << "Tan(~6)                      =" << std::setw(28) << "~-0.2910" << std::setw(28) << app.getTan() << std::endl; 
	std::cout << "Log(~6)                      =" << std::setw(28) << "~1.7917" << std::setw(28) << app.getLog() << std::endl;
	std::cout << "Floor( ~5.4 )                =" << std::setw(28) << "~5" << std::setw(28) << app2.getFloor() << std::endl;
	std::cout << "Lod(~6)                      =" << std::setw(28) << "~0.7781" << std::setw(28) << app.getLod() << std::endl;
	std::cout << "Pow(~6,2)                    =" << std::setw(28) << "~36" << std::setw(28) << app.getPower(two) << std::endl;
	std::cout << "Pow(2,~6)                    =" << std::setw(28) << "~64" <<  std::setw(28) << two.getPower(app) << std::endl;
	std::cout << "~6 + 3.9                     =" << std::setw(28) << "~9.9" << std::setw(28) << (app+c) << std::endl;
	std::cout << "~6 - .                       =" << std::setw(28) << "." << std::setw(28) << (app-inv) << std::endl;
	std::cout << "3.9 * ~6                     =" << std::setw(28) << "~23.4" << std::setw(28) << (c*app) << std::endl;
	std::cout << "~3 / ~6                      =" << std::setw(28) << "~0.5" << std::setw(28) << (app1/app) << std::endl;
	std::cout << "++~3                         =" << std::setw(28) << "~4" << std::setw(28) << ++app1 << std::endl;
	std::cout << "~4++                         =" << std::setw(28) << "~4" << std::setw(28) << app1++ << std::endl;
	std::cout << "~4++ now is                  =" << std::setw(28) << "~5" << std::setw(28) << app1 << std::endl;
	std::cout << "~5.4+=3.9                    =" << std::setw(28) << "~9.3" << std::setw(28) << (app2+=c) << std::endl;
	std::cout << "~6/=~5                       =" << std::setw(28) << "~1.2" << std::setw(28) << (app1/=app) << std::endl;
	Number range("[6-8]");
	Number ra("~[3-5]");
	Number ra1("-3r5");
	Number ra2("-5--3");
	Number ra3("-2--1");
	std::cout << "[6-8]                        =" << std::setw(28) << "[6-8]" << std::setw(28) << range.get() << std::endl;
	std::cout << "Tan[6-8]                     =" << std::setw(28) << "[-0.29r-6.79]" << std::setw(28) << range.getTan() << std::endl; 
	std::cout << "Sqrt[3-5]                    =" << std::setw(28) << "[1.732r2.23]" << std::setw(28) << ra.getSquareRoot() << std::endl;
	std::cout << "Lod[3-5]                     =" << std::setw(28) << "[0.4771r0.6989]" << std::setw(28) << ra.getLod() << std::endl;
	std::cout << "Range " << range << "C " << c  << std::endl;

	std::cout << "[6-8] + 3.9                  =" << std::setw(28) <<  "[9.9-11.9]" << std::setw(28) << (range+c) << std::endl;
	std::cout << "3.9 - [6-8]                  =" << std::setw(28) <<  "[-2.1r-4.1]" << std::setw(28) << (c-range) << std::endl;
	std::cout << "~6 + [6-8]                   =" << std::setw(28) << "[12-14]" << std::setw(28) << (app+range) << std::endl;
	std::cout << "[6-8] * ~6                   =" << std::setw(28) << "[36-48]" << std::setw(28) << (range*app) << std::endl;
	std::cout << "[6-8] / [3-5]                =" << std::setw(28) <<  "[2-1.6]" << std::setw(28) << (range/ra) << std::endl;
	std::cout << "~[3-5]++                     =" << std::setw(28) << "[3-5]" << std::setw(28) << ra++ << std::endl;
	std::cout << "~[3-5]++ now is              =" << std::setw(28) << "[4-6]" << std::setw(28) <<  ra.get() << std::endl;
	std::cout << "++[4-6]                      =" << std::setw(28) << "[5-7]" << std::setw(28) << ++ra << std::endl;
	std::cout << "[5-7]+=[6-8]                 =" << std::setw(28) << "[11-15]" << std::setw(28) << (ra+=range) << std::endl;
	
	std::cout << "[6-8]/=~6                    =" << std::setw(28) << "[1-1.3]" << std::setw(28) << (range/=app) << std::endl;
	std::cout << "Sqrt[-3 r 5]                 =" << std::setw(28) << "." << std::setw(28) << ra1.getSquareRoot() << std::endl;
	std::cout << "Sqrt[-5 - -3]                =" << std::setw(28) << "." << std::setw(28) << ra2.getSquareRoot() << std::endl; 
	range*=app;
	std::cout << "[6-8] ^ 2                    =" << std::setw(28) << "[36-64]" << std::setw(28) << range.getPower(two) << std::endl;
	std::cout << "2 ^ [6-8]                    =" << std::setw(28) << "[64-256]" << std::setw(28) << two.getPower(range) << std::endl;
	std::cout << "[-5r-3] ^ 2                  =" << std::setw(28) << "[9-25]" << std::setw(28) << ra2.getPower(two) << std::endl;
	std::cout << "2 ^ [-5r-3]                  =" << std::setw(28) << "[0.03125r0.0125]" << std::setw(28) << two.getPower(ra2) << std::endl;
	std::cout << "[6-8]^[-2r-1]                =" << std::setw(28) << "[0.277r0.125]"<< std::setw(28) << range.getPower(ra3) << std::endl; 
	
	std::cout << "COMPARISON OPERATORS" << std::endl;
	Number aa(25.6), bb(29.5), cc, dd("~25.6"), ee;
	
	std::cout << "aa=" << aa << ", bb=" << bb << ", cc=" << cc << ", dd=" << dd << ", ee=" << ee << std::endl;
	std::cout << "aa < bb ? :                  =" << std::setw(28) << "1" <<  std::setw(28) << (aa<bb) << std::endl;
	std::cout << "aa > bb ? :                  =" << std::setw(28) << "0" << std::setw(28) << (aa>bb) << std::endl;
	std::cout << "aa == bb ? :                 =" << std::setw(28) << "0" << std::setw(28) << (aa==bb) << std::endl;
	std::cout << "aa == dd ? :                 =" << std::setw(28) << "1" << std::setw(28) << (aa==dd) << std::endl;
	std::cout << "aa != dd ? :                 =" << std::setw(28) << "0" << std::setw(28) << (aa!=dd) << std::endl;
	std::cout << "aa != bb ? :                 =" << std::setw(28) << "1" << std::setw(28) << (aa!=bb) << std::endl;
	std::cout << "aa >= bb ? :                 =" << std::setw(28) << "0" << std::setw(28) << (aa>=bb) << std::endl;
	std::cout << "aa <= bb ? :                 =" << std::setw(28) << "1" << std::setw(28) << (aa<=bb) << std::endl;
	std::cout << "aa >= dd ? :                 =" << std::setw(28) << "1" << std::setw(28) << (aa>=dd) << std::endl;
	
	std::cout << "aa != cc ? :                 =" << std::setw(28) << "1" << std::setw(28) << (aa!=cc) << std::endl;
	std::cout << "aa == cc ? :                 =" << std::setw(28) << "0" << std::setw(28) << (aa==cc) << std::endl;
	std::cout << "aa <= cc ? :                 =" << std::setw(28) << "0" << std::setw(28) << (aa<=cc) << std::endl;
	std::cout << "aa >= cc ? :                 =" << std::setw(28) << "0" << std::setw(28) << (aa>=cc) << std::endl;
	std::cout << "aa < cc ? :                  =" << std::setw(28) << "0" << std::setw(28) << (aa<cc) << std::endl;
	std::cout << "aa > cc ? :                  =" << std::setw(28) << "0" << std::setw(28) << (aa>cc) << std::endl;
	std::cout << "cc != ee ? :                 =" << std::setw(28) << "0" << std::setw(28) << (cc!=ee) << std::endl;
	std::cout << "cc == ee ? :                 =" << std::setw(28) << "1" << std::setw(28) << (cc==ee) << std::endl;
	std::cout << "cc >= ee ? :                 =" << std::setw(28) << "1" << std::setw(28) << (cc>=ee) << std::endl;
	std::cout << "cc <= ee ? :                 =" << std::setw(28) << "1" << std::setw(28) << (cc<=ee) << std::endl;
	std::cout << "cc < ee ? :                  =" << std::setw(28) << "0" << std::setw(28) << (cc<ee) << std::endl;
	std::cout << "cc > ee ? :                  =" << std::setw(28) << "0" << std::setw(28) << (cc>ee) << std::endl;

	std::cout << "dd >= ee ? :                 =" << std::setw(28) << "0" << std::setw(28) << (dd>=ee) << std::endl;
	std::cout << "dd <= ee ? :                 =" << std::setw(28) << "0" << std::setw(28) << (dd<=ee) << std::endl;
	std::cout << "dd < ee ? :                  =" << std::setw(28) << "0" << std::setw(28) << (dd<ee) << std::endl;
	std::cout << "dd > ee ? :                  =" << std::setw(28) << "0" << std::setw(28) << (dd>ee) << std::endl;
	std::cout << "dd != ee ? :                 =" << std::setw(28) << "1" << std::setw(28) << (dd!=ee) << std::endl;
	std::cout << "dd == ee ? :                 =" << std::setw(28) << "0" << std::setw(28) << (dd==ee) << std::endl;
	
	Number aaa("27-33"), bbb("32-35"), ccc("~14"), ddd("10-12"), eee,fff(12),hhh(45),iii(4);
	
	std::cout << "aaa=" << aaa << ", bbb=" << bbb << ", ccc=" << ccc << ", ddd=" << ddd << ", eee=" << eee << std::endl;
	std::cout << "aaa                          =" << std::setw(28) << "[27r33]" <<  std::setw(28) << aaa << std::endl;
	std::cout << "bbb                          =" << std::setw(28) << "[32r35]" <<  std::setw(28) << bbb << std::endl;
	std::cout << "aaa < bbb ? :                =" << std::setw(28) << "0" << std::setw(28) <<  (aaa<bbb) << std::endl;
	std::cout << "aaa > bbb ? :                =" << std::setw(28) << "0" << std::setw(28) << (aaa>bbb) << std::endl;
	std::cout << "aaa <= bbb ? :               =" << std::setw(28) << "1" << std::setw(28) << (aaa<=bbb) << std::endl;
	std::cout << "aaa >= bbb ? :               =" << std::setw(28) << "1" << std::setw(28) << (aaa>=bbb) << std::endl;
	std::cout << "aaa == bbb ? :               =" << std::setw(28) << "1" << std::setw(28) << (aaa==bbb) << std::endl;
	std::cout << "aaa != bbb ? :               =" << std::setw(28) << "0" << std::setw(28) << (aaa!=bbb) << std::endl;
	std::cout << "bbb < aaa ? :                =" << std::setw(28) << "0" << std::setw(28) << (bbb<aaa) << std::endl;
	std::cout << "bbb > aaa ? :                =" << std::setw(28) << "0" << std::setw(28) << (bbb>aaa) << std::endl;
	std::cout << "bbb <= aaa ? :               =" << std::setw(28) << "1" << std::setw(28) << (bbb<=aaa) << std::endl;
	std::cout << "bbb >= aaa ? :               =" << std::setw(28) << "1" << std::setw(28) << (bbb>=aaa) << std::endl;
	
	std::cout << "aaa                          =" << std::setw(28) << "[27r33]" << std::setw(28) << aaa << std::endl;
	std::cout << "ddd                          =" << std::setw(28) << "[10r12]" << std::setw(28) << ddd << std::endl;

	std::cout << "aaa < ddd ? :                =" << std::setw(28) << "0" << std::setw(28) << (aaa<ddd) << std::endl;
	std::cout << "aaa > ddd ? :                =" << std::setw(28) << "1" << std::setw(28) << (aaa>ddd) << std::endl;
	std::cout << "aaa <= ddd ? :               =" << std::setw(28) << "0" << std::setw(28) << (aaa<=ddd) << std::endl;
	std::cout << "aaa >= ddd ? :               =" << std::setw(28) << "1" << std::setw(28) << (aaa>=ddd) << std::endl;
	std::cout << "aaa == ddd ? :               =" << std::setw(28) << "0" << std::setw(28) << (aaa==ddd) << std::endl;
	std::cout << "aaa != ddd ? :               =" << std::setw(28) << "1" << std::setw(28) << (aaa!=ddd) << std::endl;
	std::cout << "ddd < aaa ? :                =" << std::setw(28) << "1" << std::setw(28) << (ddd<aaa) << std::endl;
	std::cout << "ddd > aaa ? :                =" << std::setw(28) << "0" << std::setw(28) << (ddd>aaa) << std::endl;
	std::cout << "ddd <= aaa ? :               =" << std::setw(28) << "1" << std::setw(28) << (ddd<=aaa) << std::endl;
	std::cout << "ddd >= aaa ? :               =" << std::setw(28) << "0" << std::setw(28) << (ddd>=aaa) << std::endl;

	std::cout << "fff                          =" << std::setw(28) << "12" << std::setw(28) << fff << std::endl;
	std::cout << "ddd                          =" << std::setw(28) << "[10-12]" << std::setw(28) << ddd << std::endl;	
	std::cout << "fff < ddd ? :                =" << std::setw(28) << "0" << std::setw(28) << (fff<ddd) << std::endl;
	std::cout << "fff > ddd ? :                =" << std::setw(28) << "0" << std::setw(28) << (fff>ddd) << std::endl;
	std::cout << "fff <= ddd ? :               =" << std::setw(28) << "1" << std::setw(28) << (fff<=ddd) << std::endl;
	std::cout << "fff >= ddd ? :               =" << std::setw(28) << "1" << std::setw(28) << (fff>=ddd) << std::endl;
	std::cout << "fff == ddd ? :               =" << std::setw(28) << "1" << std::setw(28) << (fff==ddd) << std::endl;
	std::cout << "fff != ddd ? :               =" << std::setw(28) << "0" << std::setw(28) << (fff!=ddd) << std::endl;
	std::cout << "ddd < fff ? :                =" << std::setw(28) << "0" << std::setw(28) << (ddd<fff) << std::endl;
	std::cout << "ddd > fff ? :                =" << std::setw(28) << "0" << std::setw(28) << (ddd>fff) << std::endl;
	std::cout << "ddd <= fff ? :               =" << std::setw(28) << "1" << std::setw(28) << (ddd<=fff) << std::endl;
	std::cout << "ddd >= fff ? :               =" << std::setw(28) << "1" << std::setw(28) << (ddd>=fff) << std::endl;

	std::cout << "hhh                          =" << std::setw(28) << "45" << std::setw(28) << hhh << std::endl;
	std::cout << "ddd                          =" << std::setw(28) << "[10r12]" << std::setw(28) << ddd << std::endl;
	std::cout << "hhh < ddd ? :                =" << std::setw(28) << "0" << std::setw(28) << (hhh<ddd) << std::endl;
	std::cout << "hhh > ddd ? :                =" << std::setw(28) << "1" << std::setw(28) << (hhh>ddd) << std::endl;
	std::cout << "hhh <= ddd ? :               =" << std::setw(28) << "0" << std::setw(28) << (hhh<=ddd) << std::endl;
	std::cout << "hhh >= ddd ? :               =" << std::setw(28) << "1" << std::setw(28) << (hhh>=ddd) << std::endl;
	std::cout << "hhh == ddd ? :               =" << std::setw(28) << "0" << std::setw(28) << (hhh==ddd) << std::endl;
	std::cout << "hhh != ddd ? :               =" << std::setw(28) << "1" << std::setw(28) << (hhh!=ddd) << std::endl;
	std::cout << "ddd < hhh ? :                =" << std::setw(28) << "1" << std::setw(28) << (ddd<hhh) << std::endl;
	std::cout << "ddd > hhh ? :                =" << std::setw(28) << "0" << std::setw(28) << (ddd>hhh) << std::endl;
	std::cout << "ddd <= hhh ? :               =" << std::setw(28) << "1" << std::setw(28) << (ddd<=hhh) << std::endl;
	std::cout << "ddd >= hhh ? :               =" << std::setw(28) << "0" << std::setw(28) << (ddd>=hhh) << std::endl;

	std::cout << "ddd                          =" << std::setw(28) << "[10-12]" << std::setw(28) << ddd << std::endl;
	std::cout << "iii                          =" << std::setw(28) << "4" << std::setw(28) << iii << std::endl;
	std::cout << "ddd < iii ? :                =" << std::setw(28) << "0" << std::setw(28) << (ddd<iii) << std::endl;
	std::cout << "ddd > iii ? :                =" << std::setw(28) << "1" << std::setw(28) << (ddd>iii) << std::endl;
	std::cout << "ddd <= iii ? :               =" << std::setw(28) << "0" << std::setw(28) << (ddd<=iii) << std::endl;
	std::cout << "ddd >= iii ? :               =" << std::setw(28) << "1" << std::setw(28) << (ddd>=iii) << std::endl;
	std::cout << "ddd == iii ? :               =" << std::setw(28) << "0" << std::setw(28) << (ddd==iii) << std::endl;
	std::cout << "ddd != iii ? :               =" << std::setw(28) << "1" << std::setw(28) << (ddd!=iii) << std::endl;
	std::cout << "iii < ddd ? :                =" << std::setw(28) << "1" << std::setw(28) << (iii<ddd) << std::endl;
	std::cout << "iii > ddd ? :                =" << std::setw(28) << "0" << std::setw(28) << (iii>ddd) << std::endl;
	std::cout << "iii <= ddd ? :               =" << std::setw(28) << "1" << std::setw(28) << (iii<=ddd) << std::endl;
	std::cout << "iii >= ddd ? :               =" << std::setw(28) << "0" << std::setw(28) << (iii>=ddd) << std::endl;
	ddd=aaa;
	std::cout << "aaa                          =" << std::setw(28) << "[27-33]" << std::setw(28) << aaa << std::endl;
	std::cout << "ddd                          =" << std::setw(28) << "[27-33]" << std::setw(28) << ddd << std::endl;

	std::cout << "aaa < ddd ? :                =" << std::setw(28) << "0" << std::setw(28) << (aaa<ddd) << std::endl;
	std::cout << "aaa > ddd ? :                =" << std::setw(28) << "0" << std::setw(28) << (aaa>ddd) << std::endl;
	std::cout << "aaa <= ddd ? :               =" << std::setw(28) << "1" << std::setw(28) << (aaa<=ddd) << std::endl;
	std::cout << "aaa >= ddd ? :               =" << std::setw(28) << "1" << std::setw(28) << (aaa>=ddd) << std::endl;
	std::cout << "aaa == ddd ? :               =" << std::setw(28) << "1" << std::setw(28) << (aaa==ddd) << std::endl;
	std::cout << "aaa != ddd ? :               =" << std::setw(28) << "0" << std::setw(28) << (aaa!=ddd) << std::endl;
	std::cout << "ddd < aaa ? :                =" << std::setw(28) << "0" << std::setw(28) << (ddd<aaa) << std::endl;
	std::cout << "ddd > aaa ? :                =" << std::setw(28) << "0" << std::setw(28) << (ddd>aaa) << std::endl;
	std::cout << "ddd <= aaa ? :               =" << std::setw(28) << "1" << std::setw(28) << (ddd<=aaa) << std::endl;
	std::cout << "ddd >= aaa ? :               =" << std::setw(28) << "1" << std::setw(28) << (ddd>=aaa) << std::endl;
	std::cout << "*** TESTING NUMBER CLASS : RANGE TESTS -- ED'S ADDITIONS FROM newTestNumber ***" << std::endl;
	std::cout << "*** TESTING NUMBER CLASS : RANGE TESTS -- ED'S ADDITIONS FROM newTestNumber ***" << std::endl;
	std::cout << "*** TESTING NUMBER CLASS : RANGE TESTS -- ED'S ADDITIONS FROM newTestNumber ***" << std::endl;

	Number A("3.92");
	Number B("[-72 r +19]");
	Number C("[-28 r 81]");
	Number D("2");
	Number E(".");
	
	cout << "B + A  : " << B << " + " << A << " = EXPECTED: [-68.08 r 22.92] ACTUAL: " << (B+A) << endl;
	cout << "B + C  : " << B << " + " << C << " = EXPECTED: [-100 r 100] ACTUAL: " << (B+C) << endl;
	cout << "B += D : " << B << " + " << D << " = EXPECTED: [-70 r 21] ACTUAL: ";
	B+=D;
	cout << B << endl;
	cout << "B += C : " << B << " + " << C << " = EXPECTED: [-98 r 102] ACTUAL: ";
	B+=C;
	cout << B << endl;
	
	cout << "A + E  : " << A << " + " << E << " = EXPECTED: . ACTUAL: " << (A+E) << endl;
	cout << "E + B  : " << E << " + " << B << " = EXPECTED: . ACTUAL: " << (E+B) << endl;
	cout << "B + E  : " << B << " + " << E << " = EXPECTED: . ACTUAL: " << (B+E) << endl;
	
	++D;
	cout << " ++D   : EXPECTED: 3 ACTUAL: " << D << endl;
	D++;
	cout << " D++   : EXPECTED: 4 ACTUAL: " << D << endl;
	
	++C;
	cout << " ++C   : EXPECTED: [-27 r 82] ACTUAL: " << C << endl;
	C++;
	cout << " C++   : EXPECTED: [-26 r 83] ACTUAL: " << C << endl;
	
	Number F("-220.1");
	Number G("-101.5");
	Number H("[-15 r +27.2]");
	Number I("[100.10 - 1000.20]");
	
	cout << "F - G  : " << F << " - " << G << " = EXPECTED: -118.6 ACTUAL: " << (F-G) << endl;
	cout << "E - F  : " << E << " - " << F << " = EXPECTED: . ACTUAL: " << (E-F) << endl;
	cout << "F - E  : " << F << " - " << E << " = EXPECTED: . ACTUAL: " << (F-E) << endl;
	cout << "H - I  : " << H << " - " << I << " = EXPECTED: [-1015.20 r -72.9] ACTUAL: " << (H-I) << endl;
	cout << "I - H  : " << I << " - " << H << " = EXPECTED: [72.9 r 1015.2] ACTUAL: " << (I-H) << endl;
	cout << "H - F  : " << H << " - " << F << " = EXPECTED: [205.1 r 247.3] ACTUAL: " << (H-F) << endl;
	cout << "F - H  : " << F << " - " << H << " = EXPECTED: [-247.3 r -205.1] ACTUAL: " << (F-H) << endl;
	
	Number J("[1000 r 3000]");
	Number K("[3000 r 5000]");
	Number L("-456.00123");
	
	cout << " F-=G  : " << F << " -= " << G << " = EXPECTED:  -118.6 ACTUAL: ";
	F-=G;
	cout << F << endl;
	
	cout << " J-=G  : " << J << " -= " << G << " = EXPECTED:  [1101.5 r 3101.5] ACTUAL: ";
	J-=G;
	cout << J << endl;
	 
	cout << " G-=J  : " << G << " -= " << J << " = EXPECTED:  [-3203.0 r -1203.0] ACTUAL: ";
	G-=J;
	cout << G << endl;
	
	--K;
	cout << " --K   : EXPECTED: [2999 r 4999] ACTUAL: " << K << endl;
	K--;
	cout << " K--   : EXPECTED: [2998 r 4998] ACTUAL: " << K << endl;
	
	--L;
	cout << " --L   : EXPECTED: -457.00123 ACTUAL: " << L << endl;
	L--;
	cout << " L--   : EXPECTED: -458.00123 ACTUAL: " << L << endl;
	
	Number M("~ -27.25");
	Number N("[-4 r 25]");
	Number O("[-25 r +3]");
	Number P("10e3");
	
	cout << "M*M  : " << M << " * " << M << " = EXPECTED: ~742.5625 ACTUAL: " << (M*M) << endl;
	cout << "M*N  : " << M << " * " << N << " = EXPECTED: [-681.25 r 109] ACTUAL: " << (M*N) << endl;
	cout << "N*O  : " << N << " * " << O << " = EXPECTED: [-625 r 100] ACTUAL: " << (N*O) << endl;
	
	cout << "P*=M : " << P << " *=" << M << " = EXPECTED: ~ -272500 ACTUAL: ";
	P*=M;
	cout << P << endl;
	
	cout << "M*=N  : " << M << " *=" << N << " = EXPECTED: [-681.25 r 109] ACTUAL: ";
	M*=N;
	cout << M << endl;
	
	cout << "N*=O  : " << N << " *=" << O << " = EXPECTED: [-625 r 100] ACTUAL: ";
	N*=O;
	cout << N << endl;
	
	Number Q("~0.00");
	Number R("-12");
	Number S("[-144 r 288]");
	Number T("[-12 r 3]");
	Number U("[+12 r +24]");
	Number V("[0.0 r 120]");
	Number W("[-120 r 0.0]");
	Number X("30 r 50");
	
	cout << "Q/Q  : " << Q << " / " << Q << " = EXPECTED: . <undefined> ACTUAL: " << (Q/Q) << endl;
	cout << "Q/R  : " << Q << " / " << R << " = EXPECTED: . <undefined> ACTUAL: " << (Q/R) << endl;
	cout << "R*Q  : " << R << " / " << Q << " = EXPECTED: . <infinity> ACTUAL: " << (R/Q) << endl;
	cout << "R/R  : " << R << " / " << R << " = EXPECTED: 1 ACTUAL: " << (R/R) << endl;
	cout << "T/R  : " << T << " / " << R << " = EXPECTED: [-0.25 r 1] ACTUAL: " << (T/R) << endl;
	cout << "R/T  : " << R << " / " << T << " = EXPECTED: . <undefined> ACTUAL: " << (R/T) << endl;
	
	cout << "Q/S  : " << Q << " / " << S << " = EXPECTED: . <undefined> ACTUAL: " << (Q/S) << endl;
	cout << "S/T  : " << S << " / " << T << " = EXPECTED: . <undefined> ACTUAL: " << (S/T) << endl;
	
	cout << "U/V  : " << U << " / " << V << " = EXPECTED: . <infinity> ACTUAL: " << (U/V) << endl;
	cout << "U/W  : " << U << " / " << W << " = EXPECTED: . <infinity> ACTUAL: " << (U/W) << endl;
	cout << "V/W  : " << V << " / " << W << " = EXPECTED: . <undefined> ACTUAL: " << (V/W) << endl;
	cout << "U/S  : " << U << " / " << S << " = EXPECTED: . <undefined> ACTUAL: " << (U/S) << endl;
	
	cout << "T/=X : " << T << " /=" << X << " = EXPECTED: [-0.40 r 0.10] ACTUAL: ";
	T/=X;
	cout << T << endl;
	
	cout << "R/X  : " << R << " / " << X << " = EXPECTED: [-0.40 r -0.24] ACTUAL: " << (R/X) << endl;
	cout << "R/=X : " << R << " /=" << X << " = EXPECTED: [-0.40 r -0.24] ACTUAL: ";
	R/=X;
	cout << R << endl;
	
	cout << "U/=W : " << U << " /=" << W << " = EXPECTED: . <undefined> ACTUAL: ";
	U/=W;
	cout << U << endl;
	
	Number S1("7.0 r 11.725");
	cout << "sin(S1): sin(" << S1 <<") = EXPECTED: [-1 r 1] ACTUAL: " << S1.getSin() << endl;
	
	Number S2("0.7853981634");
	cout << "sin(S2=pi/4): sin(" << S2 <<") = EXPECTED: 1/sqrt(2) ACTUAL: " << S2.getSin() << endl;
	S2*=-1;
	cout << "sin(-S2=-pi/4): sin(" << S2 <<") = EXPECTED: -1/sqrt(2) ACTUAL: " << S2.getSin() << endl;
	S2*=-1;
	S2+=1.570796327;
	cout << "sin(S2+pi/2): sin(" << S2 <<") = EXPECTED: 1/sqrt(2) ACTUAL: " << S2.getSin() << endl;
	S2*=-1;
	cout << "sin(-(S2+pi/2)): sin(" << S2 <<") = EXPECTED: -1/sqrt(2) ACTUAL: " << S2.getSin() << endl;
	
	Number S3("0.785398 r 2.35619");
	cout << "sin(S3=pi/4 R 3pi/4): sin(" << S3 <<") = EXPECTED: [1/sqrt(2) r 1] ACTUAL: " << S3.getSin() << endl;
	
	Number S4("0.785398 r 3.141592654");
	cout << "sin(S4=pi/4 R pi): sin(" << S4 <<") = EXPECTED: [0 r 1] ACTUAL: " << S4.getSin() << endl;
	
	Number S5("2.35619449 r 4.71238898");
	cout << "sin(S5=3pi/4 R 3pi/2): sin(" << S5 <<") = EXPECTED: [-1 r 1/sqrt(2)] ACTUAL: " << S5.getSin() << endl;
	
	Number S6("0.6981317008 r 0.872664626");
	cout << "sin(S6=40 d. R 50 d.): sin(" << S6 <<") = EXPECTED: [sin(40d) r sin(50d)] ACTUAL: " << S6.getSin() << endl;
	
	Number S7("-0.7853981634 r 0.872664626");
	cout << "sin(S7=-pi/4 r +50deg.): sin(" << S7 <<") = EXPECTED: [-1/sqrt(2) r sin(50)=0.766044] ACTUAL: " << S7.getSin() << endl;
	
	Number S8("-5.497787144 r -0.7853981634");
	cout << "sin(S8=-7pi/4 r +pi/4): sin(" << S8 <<") = EXPECTED: [-1 r +1] ACTUAL: " << S8.getSin() << endl;
	
	Number S9("-3.926990817 r -2.35619449");
	cout << "sin(S9=-5pi/4 r -3pi/4): sin(" << S9 <<") = EXPECTED: [-1/sqrt(2) r +1/sqrt(2)] ACTUAL: " << S9.getSin() << endl;
	
	Number S10("0.5235987756 r 1.047197551");
	cout << "sin(S10=30deg r 60deg): sin(" << S10 <<") = EXPECTED: [ 0.5 r 0.866025] ACTUAL: " << S10.getSin() << endl;
	S10 *= -1;
	cout << "sin(-S10=-60deg r -30deg): sin(" << S10 <<") = EXPECTED: [-0.866025 r -0.5] ACTUAL: " << S10.getSin() << endl;
	
	Number C1("0.5235987756");
	cout << "cos( 30deg): cos(" << C1 <<") = EXPECTED: 0.866025  ACTUAL: " << C1.getCos() << endl; 
	Number C2("1.047197551");
	cout << "cos( 60deg): cos(" << C2 <<") = EXPECTED: 0.5       ACTUAL: " << C2.getCos() << endl; 
	Number C3("0.7853981634");
	cout << "cos( 45deg): cos(" << C3 <<") = EXPECTED: 1/sqrt(2) ACTUAL: " << C3.getCos() << endl; 
	Number C4("1.570796327");
	cout << "cos( 90deg): cos(" << C4 <<") = EXPECTED: 0.00      ACTUAL: " << C4.getCos() << endl; 
	Number C5("2.094395102");
	cout << "cos(120deg): cos(" << C5 <<") = EXPECTED: -0.5      ACTUAL: " << C5.getCos() << endl; 
	Number C6("2.617993878");
	cout << "cos(150deg): cos(" << C6 <<") = EXPECTED: -0.866025 ACTUAL: " << C6.getCos() << endl; 
	Number C7("3.141592654");
	cout << "cos(180deg): cos(" << C7 <<") = EXPECTED: -1.0      ACTUAL: " << C7.getCos() << endl; 
	Number C8("3.665191429");
	cout << "cos(210deg): cos(" << C8 <<") = EXPECTED: -0.866025 ACTUAL: " << C8.getCos() << endl; 
	C1 *= -1;
	C2 *= -1;
	C3 *= -1;
	C4 *= -1;
	C5 *= -1;
	C6 *= -1;
	C7 *= -1;
	C8 *= -1;
	cout << "cos( -30deg): cos(" << C1 <<") = EXPECTED: 0.866025  ACTUAL: " << C1.getCos() << endl; 
	cout << "cos( -60deg): cos(" << C2 <<") = EXPECTED: 0.50      ACTUAL: " << C2.getCos() << endl; 
	cout << "cos( -45deg): cos(" << C3 <<") = EXPECTED: 1/sqrt(2) ACTUAL: " << C3.getCos() << endl; 
	cout << "cos( -90deg): cos(" << C4 <<") = EXPECTED: 0         ACTUAL: " << C4.getCos() << endl; 
	cout << "cos(-120deg): cos(" << C5 <<") = EXPECTED: -0.5      ACTUAL: " << C5.getCos() << endl; 
	cout << "cos(-150deg): cos(" << C6 <<") = EXPECTED: -0.866025 ACTUAL: " << C6.getCos() << endl; 
	cout << "cos(-180deg): cos(" << C7 <<") = EXPECTED: -1.00     ACTUAL: " << C7.getCos() << endl; 
	cout << "cos(-210deg): cos(" << C8 <<") = EXPECTED: -0.866025 ACTUAL: " << C8.getCos() << endl; 
	
	Number C9("94.24777961 r 160.2212253");
	cout << "cos(30pi r 51pi): cos(" << C9 << ") = EXPECTED: [-1 r +1] ACTUAL: " << C9.getCos() << endl;
	cout << "sin(30pi r 51pi): sin(" << C9 << ") = EXPECTED: [0  r +1] ACTUAL: " << C9.getSin() << endl;
	cout << "tan(30pi r 51pi): tan(" << C9 << ") = EXPECTED: [. <div by zero>] ACTUAL: " << C9.getTan() << endl;
	
	Number C10("0.7853981634 r 1.570796327");
	cout << "tan(pi/4 r pi/2): tan(" << C10 << ") = EXPECTED: [. <div by zero>] ACTUAL: " << C10.getTan() << endl;
	
	Number C11("0.7853981634 r 1.570796326");
	cout << "tan(pi/4 r less than but quite close to pi/2): tan(" << C11 << ") = EXPECTED: [1 r a very large number] ACTUAL: " << C11.getTan() << endl;
	
	Number C12("0.5235987756 r 1.047197551");
	cout << "tan(30d f 60d): tan(" << C12 << ") = EXPECTED: [0.57735 r 1.73205] ACTUAL: " << C12.getTan() << endl;
	
	Number C13("-1 r +1");
	cout << "asin(-1 r +1): asin(" << C13 << ") = EXPECTED: [-PI/2 r +PI/2] ACTUAL: " << C13.getAsin() << endl;
	
	Number C14("-0.7071067812 r +0.7071067812");
	cout << "asin(-1/sqrt(2) r 1/sqrt(2)): asin(" << C14 << ") = EXPECTED: [-PI/4 r +PI/4] ACTUAL: " << C14.getAsin() << endl;
	
	Number C15("0 r +0.7071067812");
	cout << "asin(0 r 1/sqrt(2)): asin(" << C15 << ") = EXPECTED: [0 r +PI/4] ACTUAL: " << C15.getAsin() << endl;
	
	Number C16("-0.7071067812 r 0");
	cout << "asin(-1/sqrt(2) r 0): asin(" << C16 << ") = EXPECTED: [-PI/4 r 0] ACTUAL: " << C16.getAsin() << endl;
	
	Number C17("-0.7071067812 r 1.01");
	cout << "asin(-1/sqrt(2) r 1.01): asin(" << C17 << ") = EXPECTED: . <domain error> ACTUAL: " << C17.getAsin() << endl;
	
	Number C18("-2.1 r 0");
	cout << "asin(-2.1 r 0): asin(" << C18 << ") = EXPECTED: . <domain error> ACTUAL: " << C18.getAsin() << endl;
	
	Number C19("6.9");
	cout << "asin(6.9): asin(" << C19 << ") = EXPECTED: . <domain error> ACTUAL: " << C19.getAsin() << endl;
	
	cout << "acos(-1 r +1): acos(" << C13 << ") = EXPECTED: [0 r PI] ACTUAL: " << C13.getAcos() << endl;
	cout << "acos(-1/sqrt(2) r 1/sqrt(2)): acos(" << C14 << ") = EXPECTED: [PI/4 r 3PI/4] ACTUAL: " << C14.getAcos() << endl;
	cout << "acos(0 r 1/sqrt(2)): acos(" << C15 << ") = EXPECTED: [PI/4 r PI/2] ACTUAL: " << C15.getAcos() << endl;
	cout << "acos(-1/sqrt(2) r 0): acos(" << C16 << ") = EXPECTED: [Pi/2 r 3PI/4]  ACTUAL: " << C16.getAcos() << endl;
	cout << "acos(-1/sqrt(2) r 1.01): acos(" << C17 << ") = EXPECTED: . <domain error> ACTUAL: " << C17.getAcos() << endl;
	cout << "acos(-2.1 r 0): acos(" << C18 << ") = EXPECTED: . <domain error> ACTUAL: " << C18.getAcos() << endl;
	cout << "acos(6.9): acos(" << C19 << ") = EXPECTED: . <domain error> ACTUAL: " << C19.getAcos() << endl;
	
	Number C20("-10 r +1.0");
	cout << "atan(-10 r +1.0): atan(" << C20 << ") = EXPECTED: [-1.471127 r PI/4] ACTUAL: " << C20.getAtan() << endl;
	
	Number C21("-1e6 r +1e6");
	cout << "atan(-1e6 r +1e6): atan(" << C21 << ") = EXPECTED: [close to -PI/2 r close to PI/2] ACTUAL: " << C21.getAtan() << endl;
	
	Number C22("0 r +1e4");
	cout << "atan(0 +1e4): atan(" << C22 << ") = EXPECTED: [ 0 r close to PI/2] ACTUAL: " << C22.getAtan() << endl;
	
	Number C23("-1e4 r 0");
	cout << "atan(-1e4 r 0): atan(" << C23 << ") = EXPECTED: [close to -PI/2 r 0] ACTUAL: " << C23.getAtan() << endl;
	
	Number C24("0");
	cout << "cosh(0): cosh(" << C24 << ") = EXPECTED: 1 ACTUAL: " << C24.getCosh() << endl;
	
	Number C25("-5 r +2");
	cout << "cosh(-5 r +2): cosh(" << C25 << ") = EXPECTED: [1 r 74.209948] ACTUAL: " << C25.getCosh() << endl;
	
	Number C26("-2 r 3");
	cout << "cosh(-2 r 3): cosh(" << C26 << ") = EXPECTED: [1 r 10.067662] ACTUAL: " << C26.getCosh() << endl;
	
	Number C27("0.5 r 3");
	cout << "cosh(0.5 r 3): cosh(" << C27 << ") = EXPECTED: [1.127625 r 10.067662] ACTUAL: " << C27.getCosh() << endl;
	
	Number C28("-3 r -0.5");
	cout << "cosh(-3 r -0.5): cosh(" << C28 << ") = EXPECTED: [1.127625 r 10.067662] ACTUAL: " << C28.getCosh() << endl;
	
	cout << "sinh(0): sinh(" << C24 << ") = EXPECTED: 0 ACTUAL: " << C24.getSinh() << endl;
	cout << "sinh(-5 r +2): sinh(" << C25 << ") = EXPECTED: [-74.20321058 r 3.626860408] ACTUAL: " << C25.getSinh() << endl;
	cout << "sinh(0.5 r 3): sinh(" << C27 << ") = EXPECTED: [0.5210953 r 10.01787493] ACTUAL: " << C27.getSinh() << endl;
	cout << "sinh(-3 r -0.5): sinh(" << C28 << ") = EXPECTED: [-10.01787493 r -0.5210953] ACTUAL: " << C28.getSinh() << endl;
	
	cout << "tanh(0): tanh(" << C24 << ") = EXPECTED: 0 ACTUAL: " << C24.getTanh() << endl;
	cout << "tanh(-5 r +2): tanh(" << C25 << ") = EXPECTED: [very close to -1 r 0.964027] ACTUAL: " << C25.getTanh() << endl;
	cout << "tanh(0.5 r 3): tanh(" << C27 << ") = EXPECTED: [0.462117 r very close to 1 (0.995054)] ACTUAL: " << C27.getTanh() << endl;
	cout << "tanh(-3 r -0.5): tanh(" << C28 << ") = EXPECTED: [-0.995054 r -0.46211715] ACTUAL: " << C28.getTanh() << endl;
	
	cout << "floor(-3 r -0.5): floor(" << C28 << ") = EXPECTED: [-3 r -1] ACTUAL: " << C28.getFloor() << endl;
	cout << "floor(0.5 r 3): floor(" << C27 << ") = EXPECTED: [0 r 3] ACTUAL: " << C27.getFloor() << endl;
	Number C29("-1.9999 r 3.99999");
	cout << "floor(-1.999 r 3.9999): floor(" << C29 << ") = EXPECTED: [-2 r 3] ACTUAL: " << C29.getFloor() << endl;
	Number C30("-1.9999 r -1.5432");
	cout << "floor(-1.999 r -1.5432): floor(" << C30 << ") = EXPECTED: [-2 r -2] ACTUAL: " << C30.getFloor() << endl;
	
	cout << "ceiling(-3 r -0.5): ceiling(" << C28 << ") = EXPECTED: [-3 r 0] ACTUAL: " << C28.getCeiling() << endl;
	cout << "ceiling(0.5 r 3): ceiling(" << C27 << ") = EXPECTED: [1 r 3] ACTUAL: " << C27.getCeiling() << endl;
	cout << "ceiling(-1.999 r 3.9999): ceiling(" << C29 << ") = EXPECTED: [-1 r 4] ACTUAL: " << C29.getCeiling() << endl;
	cout << "ceiling(-1.999 r -1.5432): ceiling(" << C30 << ") = EXPECTED: [-1 r -1] ACTUAL: " << C30.getCeiling() << endl;
	
	Number C31("1.5 r 2.5");
	Number C32("3.5 r 4.5");
	Number C33("7.5 r 7.9");
	cout << "round(1.5 r 2.5): round(" << C31 << ") = EXPECTED: [2 r 3] ACTUAL: " << C31.getRound() << endl;
	cout << "round(3.5 r 4.5): round(" << C32 << ") = EXPECTED: [4 r 5] ACTUAL: " << C32.getRound() << endl;
	cout << "round(7.5 r 7.9): round(" << C33 << ") = EXPECTED: [8 r 8] ACTUAL: " << C33.getRound() << endl;
	
	Number SR01("-2 r 3");
	Number SR02("0 r 9");
	Number SR03("25 r 100");
	cout << "sqrt(-2 r 3): sqrt(" << SR01 << ") = EXPECTED: . <domain error> ACTUAL: " << SR01.getSquareRoot() << endl;
	cout << "sqrt(0 r 9): sqrt(" << SR02 << ") = EXPECTED: [0-3] ACTUAL: " << SR02.getSquareRoot() << endl;
	cout << "sqrt(25 r 100): sqrt(" << SR03 << ") = EXPECTED: [5-10] ACTUAL: " << SR03.getSquareRoot() << endl;
	
	cout << "abs(-2 r 3): abs(" << SR01 << ") = EXPECTED: [0-3] ACTUAL: " << SR01.getAbsoluteValue() << endl;
	cout << "abs(0 r 9): abs(" << SR02 << ") = EXPECTED: [0-9] ACTUAL: " << SR02.getAbsoluteValue() << endl;
	
	cout << "inv(-2 r 3): inv(" << SR01 << ") = EXPECTED: . <domain error> ACTUAL: " << SR01.getInverse() << endl;
	cout << "inv(0 r 9): inv(" << SR02 << ") = EXPECTED: . <domain error> ACTUAL:  " << SR02.getInverse() << endl;
	cout << "inv(25 r 100): inv(" << SR03 << ") = EXPECTED: [1/100 r 1/25] ACTUAL: " << SR03.getInverse() << endl;
	
	// Logarithms:
	Number L01("-12");
	Number L02("1000");
	Number L03("-5 r -2");
	Number L04("-2 r 2.718281828");
	Number L05("2.718281828 r 10000");
	
	cout << "log10(-12): log10(" << L01 << ") = EXPECTED: . ACTUAL: " << L01.getLog10() << endl;
	cout << "log10(1000): log10(" << L02 << ") = EXPECTED: 3 ACTUAL: " << L02.getLog10() << endl;
	cout << "log10(-5 r -2): log10(" << L03 << ") = EXPECTED: . ACTUAL: " << L03.getLog10() << endl;
	cout << "log10(-2 r 2.7182818): log10(" << L04 << ") = EXPECTED: . ACTUAL: " << L04.getLog10() << endl;
	cout << "log10(2.781828 r 10000): log10(" << L05 << ") = EXPECTED: [0.4342944819 r 4] ACTUAL: " << L05.getLog10() << endl;
	
	cout << "log(-12): log(" << L01 << ") = EXPECTED: . ACTUAL: " << L01.getLog() << endl;
	cout << "log(1000): log(" << L02 << ") = EXPECTED: 6.907755279 ACTUAL: " << L02.getLog() << endl;
	cout << "log(-5 r -2): log(" << L03 << ") = EXPECTED: . ACTUAL: " << L03.getLog() << endl;
	cout << "log(-2 r 2.7182818): log(" << L04 << ") = EXPECTED: . ACTUAL: " << L04.getLog() << endl;
	cout << "log(2.781828 r 10000): log(" << L05 << ") = EXPECTED: [1 r 9.21034] ACTUAL: " << L05.getLog() << endl;
	
	Number M01("0.0001");
	Number M02("-0.0001");
	Number M03("0.0001 r 0.001");
	Number M04("0.1 r 100");
	
	cout << "log10(0.0001 r 0.001): log10(" << M03 << ") = EXPECTED: [-4 r -3] ACTUAL: " << M03.getLog10() << endl;
	cout << "LOD(0.0001): LOD(" << M01 << ") = EXPECTED: 4 ACTUAL: " << M01.getLod() << endl;
	cout << "LOD(-0.0001): LOD(" << M02 << ") = EXPECTED: . ACTUAL: " << M02.getLod() << endl;
	cout << "LOD(0.0001 r 0.001): LOD(" << M03 << ") = EXPECTED: [3 r 4]  ACTUAL: " << M03.getLod() << endl;
	cout << "LOD(0.1 r 100): LOD(" << M04 << ") = EXPECTED: [-2 r 1] ACTUAL: " << M04.getLod() << endl;
	
	Number H01("-0.01");
	Number H02("0.00");
	Number H03("34.6573593");
	Number H04("1e6");
	Number H05("-0.0001 r 34.6573593");
	Number H06("34.65733593 r 1e6");
	
	cout << "haldaneToTheta(" << H01 << ") = EXPECTED: . <domain error> ACTUAL: " << H01.haldaneToTheta() << endl;
	cout << "haldaneToTheta(" << H02 << ") = EXPECTED: 0 ACTUAL: " << H02.haldaneToTheta() << endl;
	cout << "haldaneToTheta(" << H03 << ") = EXPECTED: 1/4 (0.25) ACTUAL: " << H03.haldaneToTheta() << endl;
	cout << "haldaneToTheta(" << H04 << ") = EXPECTED: close to 0.5 ACTUAL: " << H04.haldaneToTheta() << endl;
	cout << "haldaneToTheta(" << H05 << ") = EXPECTED: . <domain error> ACTUAL: " << H05.haldaneToTheta() << endl;
	cout << "haldaneToTheta(" << H06 << ") = EXPECTED: [1/4 r 1/2]      ACTUAL: " << H06.haldaneToTheta() << endl;
	
	Number h01("-0.01 r 0.25");
	Number h02("0.25 r 0.4999");
	Number h03("0.25 r 0.712");
	Number h04("0.25 r 0.44");
	Number h05("0.25 r 0.5");
	
	cout << "thetaToHaldane(" << h01 << ") = EXPECTED: . <domain error> ACTUAL: " << h01.thetaToHaldane() << endl;
	cout << "thetaToHaldane(" << h02 << ") = EXPECTED: [34.657 r 425.8596] ACTUAL: " << h02.thetaToHaldane() << endl;
	cout << "thetaToHaldane(" << h03 << ") = EXPECTED: . <domain error> ACTUAL: " << h03.thetaToHaldane() << endl;
	cout << "thetaToHaldane(" << h04 << ") = EXPECTED: [34.657 r 106.0131768] ACTUAL: " << h04.thetaToHaldane() << endl;
	cout << "thetaToHaldane(" << h05 << ") = EXPECTED: . <infinity> ACTUAL: " << h05.thetaToHaldane() << endl;
	
	cout << "thetaToKosambi(" << h01 << ") = EXPECTED: . <domain error> ACTUAL: " << h01.thetaToKosambi() << endl;
	cout << "thetaToKosambi(" << h02 << ") = EXPECTED: [27.4653 r 230.2560092] ACTUAL: " << h02.thetaToKosambi() << endl;
	cout << "thetaToKosambi(" << h03 << ") = EXPECTED: . <domain error> ACTUAL: " << h03.thetaToKosambi() << endl;
	cout << "thetaToKosambi(" << h04 << ") = EXPECTED: [27.4653 r 68.78838] ACTUAL: " << h04.thetaToKosambi() << endl;
	cout << "thetaToKosambi(" << h05 << ") = EXPECTED: . <infinity> ACTUAL: " << h05.thetaToKosambi() << endl;
	
	Number E01("-1");
	Number E02("0");
	Number E03("+1");
	Number E04("-1 r 0");
	Number E05("0 r +1");
	Number E06("-2 r +2");
	
	cout << "exp(" << E01 << ") = EXPECTED: 1/e (0.36787) ACTUAL: " << E01.getExponent() << endl;
	cout << "exp(" << E02 << ") = EXPECTED: 1 ACTUAL: " << E02.getExponent() << endl;
	cout << "exp(" << E03 << ") = EXPECTED: e (2.718281828) ACTUAL: " << E03.getExponent() << endl;
	cout << "exp(" << E04 << ") = EXPECTED: [1/e r 1] ACTUAL: " << E04.getExponent() << endl;
	cout << "exp(" << E05 << ") = EXPECTED: [1 r e] ACTUAL: " << E05.getExponent() << endl;
	cout << "exp(" << E06 << ") = EXPECTED: [0.1353352 r 7.389056] ACTUAL: " << E06.getExponent() << endl;
	
	Number P01("-2.3");
	Number P02("2.3");
	Number E07("-3");
	Number E08("-3.2");
	Number E09("3");
	Number E10("-3.2 r 2.3");
	Number P03("-2.3 r -1.5");
	Number P04("1.5 r 2.3");
	
	cout << "Power: " << P01 << "^" << E07 << " = EXPECTED: -0.0821895 ACTUAL: " << (P01.getPower(E07)) << endl;
	cout << "Power: " << P01 << "^" << E08 << " = EXPECTED: . <domain error> ACTUAL: " << (P01.getPower(E08)) << endl;
	cout << "Power: " << P01 << "^" << E09 << " = EXPECTED: -12.167 ACTUAL: " << (P01.getPower(E09)) << endl;
	
	cout << "Power: " << P02 << "^" << E07 << " = EXPECTED: 0.0821895 ACTUAL: " << (P02.getPower(E07)) << endl;
	cout << "Power: " << P02 << "^" << E08 << " = EXPECTED: 0.0695778372 ACTUAL: " << (P02.getPower(E08)) << endl;
	cout << "Power: " << P02 << "^" << E09 << " = EXPECTED: 12.167 ACTUAL: " << (P02.getPower(E09)) << endl;
	
	cout << "Power: " << P03 << "^" << E07 << " = EXPECTED: [-0.296296 r -0.08218952] ACTUAL: " << (P03.getPower(E07)) << endl;
	cout << "Power: " << P03 << "^" << E08 << " = EXPECTED: . <domain error> ACTUAL: " << (P03.getPower(E08)) << endl;
	cout << "Power: " << P03 << "^" << E09 << " = EXPECTED: [-12.167 r -3.375] ACTUAL: " << (P03.getPower(E09)) << endl;
	
	cout << "Power: " << P04 << "^" << E07 << " = EXPECTED: [0.08218952 r 0.296296] ACTUAL: " << (P04.getPower(E07)) << endl;
	cout << "Power: " << P04 << "^" << E08 << " = EXPECTED: [0.069577 r 0.273217] ACTUAL: " << (P04.getPower(E08)) << endl;
	cout << "Power: " << P04 << "^" << E09 << " = EXPECTED: [3.375 r 12.167] ACTUAL: " << (P04.getPower(E09)) << endl;
	
	cout << "Power: " << P04 << "^" << E10 << " = EXPECTED: [0.069577 r 6.79163] ACTUAL: " << (P04.getPower(E10)) << endl;
	Number PP;
	cout << "PP: " << PP ;
	PP++;
	cout <<	" PP++: " << PP << endl;
	
	cout << "---------------------------------------------------------" << endl;
	cout << "TESTING TANGENT AT -2PI, -3PI/2, -PI, -PI/2, 0, PI/2 ... " << endl;
	cout << "---------------------------------------------------------" << endl;
	Number PA = -2.0L * M_PI;
	Number PIOVERTWO = M_PI_2;
	for(int i=0;i<9;i++){
		cout << "PA = " << PA ;
		cout << " tan(PA) = " << PA.getTan() << endl;
		PA+= PIOVERTWO;
	}
	
	cout << "---------------------------------------------------------" << endl;
	cout << "TESTING isA() method                                 ... " << endl;
	cout << "---------------------------------------------------------" << endl;
	
	std::string N1(".");
	std::string N2("     ");
	std::string N3("157.93");
	std::string N4("12.45 - 13.67");
	std::string N5("~24.3");
	std::string N6("[0.056 r 0.897]");
	std::string N7("110/112");
	std::string N8("110-112");
	
	Warning::suppressWarnings(true);
	cout << "Classifying: " << N1 << " : " << Number::isA(N1) << endl;
	cout << "Classifying: " << N2 << " : " << Number::isA(N2) << endl;
	cout << "Classifying: " << N3 << " : " << Number::isA(N3) << endl;
	cout << "Classifying: " << N4 << " : " << Number::isA(N4) << endl;
	cout << "Classifying: " << N5 << " : " << Number::isA(N5) << endl;
	cout << "Classifying: " << N6 << " : " << Number::isA(N6) << endl;
	cout << "Classifying: " << N7 << " : " << Number::isA(N7) << endl;
	cout << "Classifying: " << N8 << " : " << Number::isA(N8) << endl;
	Warning::suppressWarnings(false);
	
	Number testSine("0 r 3.141592654");
	cout << "Sine([0 PI])=" << testSine.getSin() << endl;

	return 0;
	
}

