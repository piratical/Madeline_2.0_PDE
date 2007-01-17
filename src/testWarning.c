#include "Warning.h"
#include <iostream>
#include <iostream>

using namespace std;

int main(void){

	char message[200]="testWarning now";
	char* m;
	m = message;	
	cout << "TESTING WARNINGS : DEFAULT STATE IS THAT WARNINGS ARE NOT SUPPRESSED." << endl;
	Warning("%04i.%02i.%02i: This is a test of %s",2005,9,16,m);
	cout << "NOW SUPPRESSING WARNINGS ... " << endl;
	Warning::suppressWarnings(true);
	Warning("%04i.%02i.%02i: This is a test of %s",2005,9,16,"testWarning.c");
	Warning::suppressWarnings(false);
	cout << "... END OF SUPPRESSION. " << endl;
	return 0;
}

