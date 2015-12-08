#include "SampleQuantity.h"
#include <iostream>

using namespace std;

int main( void ){
	
	SampleQuantity sq1();
	std:: cout << sq1.get();
	std:: cout << sq1.getBoolean();
	std:: cout << sq1.getValue();
	
	SampleQuantity sq2("Y");
	std:: cout << sq2.get();
	std:: cout << sq2.getBoolean();
	std:: cout << sq2.getValue();
	
	SampleQuantity sq3("N");
	std:: cout << sq3.get();
	std:: cout << sq3.getBoolean();
	std:: cout << sq3.getValue();
	
	SampleQuantity sq4(".  ");
	std:: cout << sq4.get();
	std:: cout << sq4.getBoolean();
	std:: cout << sq4.getValue();
	
	SampleQuantity sq5("0.50");
	std:: cout << sq5.get();
	std:: cout << sq5.getBoolean();
	std:: cout << sq5.getValue();
	
	SampleQuantity sq6("1.50");
	std:: cout << sq6.get();
	std:: cout << sq6.getBoolean();
	std:: cout << sq6.getValue();
	return 1;
}
