#include "Boolean.h"

int main(){
	
	Boolean b;
	std::cout << "B                 =" << b << std::endl;
	Boolean flag("T");
	std::cout << "flag T            =" << flag << std::endl;
	Boolean flag2("f");
	std::cout << "flag2 f           =" << flag2 << std::endl;
	b.set("t");
	std::cout << "B after set t     =" << b << std::endl;
	Boolean::addLookupTableEntry("TRUE",true);
	std::cout << "Added TRUE to the map " << std::endl;
	Boolean flag3("TRUE");
	std::cout << "flag3 TRUE        =" << flag3 << std::endl;
	Boolean flag4,flag5;
	std::cout << "flag3==flag4       " << (flag3==flag4) << std::endl;
	std::cout << "flag3==flag        " << (flag3==flag) << std::endl;
	std::cout << "flag3==flag2       " << (flag3==flag2) << std::endl;
	std::cout << "flag4==flag5       " << (flag4==flag5) << std::endl;
	std::cout << "flag3 < flag4      " << (flag3 < flag4) << std::endl;
	std::cout << "flag3 < flag       " << (flag3 < flag) << std::endl;
	std::cout << "flag3 < flag2      " << (flag3 < flag2) << std::endl;
	std::cout << "flag4 < flag5      " << (flag4 < flag5) << std::endl;
	std::cout << "flag4 < flag3      " << (flag4 < flag3) << std::endl;
	std::cout << "flag2 < flag3      " << (flag2 < flag3) << std::endl;
	Boolean flag6(".");
	std::cout << "flag6              " << flag6 << std::endl;	
	std::cout << "flag6.getBoolean() " << flag6.getBoolean() << std::endl;
	flag6.set(false);
	std::cout << "flag6.set(false)   " << flag6 << std::endl;
	std::cout << "flag6.getBoolean() " << flag6.getBoolean() << std::endl;
	std::cout << "flag.getBoolean()  " << flag.getBoolean() << std::endl;
	return 0;
}

