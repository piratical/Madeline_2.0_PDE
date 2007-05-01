#include "Consultand.h"

int main(){
	
	Consultand a;
	Consultand b("Y");
	Consultand c(".");
	Consultand d("N");
	Consultand e("C");
	Consultand f("c");
	std::cout << "a (should be .)  =" << a.getBoolean() << std::endl;
	std::cout << "b (should be T)  =" << b.getBoolean() << std::endl;
	std::cout << "c (should be .)  =" << c.getBoolean() << std::endl;
	std::cout << "d (should be F)  =" << d.getBoolean() << std::endl;
	std::cout << "e (should be T)  =" << e.getBoolean() << std::endl;
	std::cout << "f (should be T)  =" << f.getBoolean() << std::endl;
	
	
	return 0;
}

