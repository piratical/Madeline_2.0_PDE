#include "UnicodeScript.h"
#include <iostream>

using namespace std;

int main(){
	
	// Test default constructor:
	UnicodeScript a;
	
	// Test ordinary constructor:
	UnicodeScript b(LATIN,"Latin","Latina","AQXpqz");
	
	// Test copy constructor:
	a = b;
	
	cout << "Sample for B is " << b.getSample() << endl;
	
	cout << "Sample for A is " << a.getSample() << endl;
	
	return 0;
}

