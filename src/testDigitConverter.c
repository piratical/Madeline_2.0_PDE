
#include "DigitConverter.h"
#include "Number.h"
#include "Genotype.h"
#include "Date.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, const char * argv[]){
	
	const char *s1="12.24";
	const char *s2="١٢.٣٤٥٦٧٨٩";
	const char *s3="८९०,१२३,४५६.७८९";
	const char *s4="一九五〇到二〇〇三年";
	const char *s5="๒๒๕๔.๐๓.๑๗";
	const char *s6="１２３/１２３";
	const char *s7="۰٫۱۲۳۴";
	const char *s8="٤٥٦٬٤٣٤٬٥٥٦٬٤٥٦";
	const char *s9="១២៣/៤៥៦";
	
	cout << "=== TESTING DigitConverters constructed from const char * s ===" << endl;
	DigitConverter d1(s1);
	DigitConverter d2(s2);
	DigitConverter d3(s3);
	DigitConverter d4(s4);
	DigitConverter d5(s5);
	DigitConverter d6(s6);
	DigitConverter d7(s7);
	DigitConverter d8(s8);
	DigitConverter d9(s9);
	cout << d1 << endl;
	cout << d2 << endl;
	cout << d3 << endl;
	cout << d4 << endl;
	cout << d5 << endl;
	cout << d6 << endl;
	cout << d7 << endl;
	cout << d8 << endl;
	cout << d9 << endl;
	
	cout << "=== TESTING DigitConverters constructed from std::string ===" << endl;
	std::string S1(s1);
	std::string S2(s2);
	std::string S3(s3);
	std::string S4(s4);
	std::string S5(s5);
	std::string S6(s6);
	std::string S7(s7);
	std::string S8(s8);
	std::string S9(s9);
	DigitConverter D1(S1);
	DigitConverter D2(S2);
	DigitConverter D3(S3);
	DigitConverter D4(S4);
	DigitConverter D5(S5);
	DigitConverter D6(S6);
	DigitConverter D7(S7);
	DigitConverter D8(S8);
	DigitConverter D9(S9);
	cout << D1 << endl;
	cout << D2 << endl;
	cout << D3 << endl;
	cout << D4 << endl;
	cout << D5 << endl;
	cout << D6 << endl;
	cout << D7 << endl;
	cout << D8 << endl;
	cout << D9 << endl;
	
	
	cout << "=== TESTING DigitConverter in Numbers ===" << endl;
	Number n1(s1);
	Number n2(s2);
	Number n7(s7);
	cout << "Numeric Input: " << s1 << " Numeric Value: " << n1 << endl;
	cout << "Numeric Input: " << s2 << " Numeric Value: " << n2 << endl;
	cout << "Numeric Input: " << s7 << " Numeric Value: " << n7 << endl;
	
	
	cout << "=== TESTING DigitConverter in Genotypes ===" << endl;
	Genotype g6(s6);
	Genotype g9(s9);
	cout << "Genotype input: " << s6 << " Genotype value: " << g6 << endl; 
	cout << "Genotype input: " << s9 << " Genotype value: " << g9 << endl; 
	
	
	cout << "=== TESTING DigitConverter in Genotypes ===" << endl;
	Date Date4(s4);
	Date Date5(s5);
	
	cout << "Date Input: " << s4 << " Date value: " << Date4 << endl;
	cout << "Date Input: " << s5 << " Date value: " << Date5 << endl;

	cout << "=== COMPREHENSIVELY TESTING ALL NON-ASCII digits  ===" << endl;
	std::string ss02("〇一二三四五六七八九");
	DigitConverter dc02(ss02);
	cout << "Chinese - IN: " <<  ss02 << " OUT: " << dc02 << endl;

	std::string ss04("零壹貳叄肆伍陸柒捌玖");
	DigitConverter dc04(ss04);
	cout << "Chinese Financial- IN: " <<  ss04 << " OUT: " << dc04 << endl;

	std::string ss05("么贰參〤〥陆〧〨〩");
	DigitConverter dc05(ss05);
	cout << "Chinese Alt. Fin. + HangZhou- IN: " <<  ss05 << " OUT: " << dc05 << endl;

	std::string ss01("፩፪፫፬፭፮፯፰፱");
	DigitConverter dc01(ss01);
	cout << "Ethiopian - IN: " <<  ss01 << " OUT: " << dc01 << endl;

	std::string ss03("	๐๑๒๓๔๕๖๗๘๙");
	DigitConverter dc03(ss03);
	cout << "Thai - IN: " <<  ss03 << " OUT: " << dc03 << endl;
		

	std::string ss06("۰۱۲۳۴۵۶۷۸۹");
	DigitConverter dc06(ss06);
	cout << "Arabic - IN: " <<  ss06 << " OUT: " << dc06 << endl;
	std::string ss07("	၀၁၂၃၄၅၆၇၈၉");
	DigitConverter dc07(ss07);
	cout << "Myanmar - IN: " <<  ss07 << " OUT: " << dc07 << endl;
	std::string ss08("០១២៣៤៥៦៧៨៩");
	DigitConverter dc08(ss08);
	cout << "Khmer - IN: " <<  ss08 << " OUT: " << dc08 << endl;

	return 0;
	
}
