//
// testUnicodeConverters.cpp
//
// (1) Tests UTF-16 to UTF-8 Conversion by converting a UTF-16-encoded file
// (2) Tests UTF-32 to UTF-8 Conversion by converting a short string
//

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "Utf8String.h"

//
// main
//
int main( int argc, const char *argv[]){
	
	//
	// Must provide file name argument:
	//
	if(argc!=2){
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		std::cerr << "HINT: Use \"data/UTF16SampleTextFile.txt\" for testing!" << std::endl;
		exit(1);
	}
	
	std::ifstream readFile;
	readFile.open(argv[1],std::ios::in|std::ios::binary|std::ios::ate);
	
	char *memblock;
	std::ifstream::pos_type size;
	if(readFile.is_open()){
		
		size = readFile.tellg();
		std::cout << "Memblock size is now " << size << std::endl;
		memblock = new char[ size ];
		readFile.seekg (0, std::ios::beg);
		readFile.read(memblock,size);
		readFile.close();
	}
	
	//
	// UTF16 size is 1/2 since there
	// are two bytes per character (normally -- surrogate pairs
	// are the exception of course)
	//
	unsigned utf16size = size;
	utf16size/=2;
	std::cout << "utf16size is " << utf16size << std::endl;
	
	std::basic_string<UTF16> utf16;
	UTF16 *stt = (UTF16 *) memblock;
	UTF16 *end = stt + utf16size;
	
	//
	// Check whether the file looks like a UTF-16 file:
	//
	if     (*stt==0xfeff) std::cout << "File begins with a Big Endian BOM." << std::endl;
	else if(*stt==0xffef) std::cout << "File begins with a Little Endian BOM." << std::endl;
	else                  std::cout << "File has no BOM : Unable to detect format." << std::endl;
	
	//
	// Since no byte swapping is performed, this code won't actually
	// work on files created on a platform with a different Endianess:
	//
	for(UTF16 *p=stt;p<end;p++){
		std::cout << std::hex << *p << std::endl;
		if(*p==0xfeff || *p==0xfffe ) continue;
		//
		// Add byte swapping here if you want to do
		// more rigorous testing:
		//
		utf16.push_back(*p);
	}
	
	//
	// Construct a UTF8String from a UTF16 string:
	//
	UTF8String utf8(utf16);
	
	//
	// The following of course prints correctly on
	// modern UTF-8-based *Nix terminals:
	//
	std::cout << "====CONSTRUCTOR===\n";
	std::cout << "The UTF-16 file contents are:" << std::endl;
	std::cout << utf8 << std::endl;
	delete memblock;
	
	// Also check UTF8 from UTF32 constructor:
	std::basic_string<UTF32> utf32;
	utf32.push_back(0x7ae0);
	utf32.push_back(0x5b50);
	utf32.push_back(0x6021);
	
	UTF8String utf8_2(utf32);
	std::cout << "====CONSTRUCTOR===\n";
	std::cout << "The UTF-32 string contents are:" << std::endl;
	std::cout << utf8_2 << std::endl;
	
	//
	// Also verify append and assignment operators:
	//
	UTF8String utf8_3("Hello, World!");
	UTF8String utf8_4("Hello, World!");
	
	utf8_3 += utf32;
	utf8_4 += utf16;
	std::cout << "====APPEND TO END VIA += ====\n";
	std::cout << "The contents from a +=UTF32 are:" << std::endl;
	std::cout << utf8_3 << std::endl;
	
	std::cout << "====APPEND TO END VIA += ====\n";
	std::cout << "The contents from a +=UTF16 are:" << std::endl;
	std::cout << utf8_4 << std::endl;
	
	utf8_3 = utf32;
	utf8_4 = utf16;
	std::cout << "====ASSIGNMENT====\n";
	std::cout << "The contents from a UTF32 assignment are:" << std::endl;
	std::cout << utf8_3 << std::endl;
	
	std::cout << "====ASSIGNMENT====\n";
	std::cout << "The contents from a UTF16 assignment are:" << std::endl;
	std::cout << utf8_4 << std::endl;
	
	
	return 0;
	
}
