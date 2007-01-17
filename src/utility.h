#ifndef UTILITY_INCLUDED
#define UTILITY_INCLUDED

#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <algorithm>

std::string dtoa(double value);
std::vector<std::string> split(const std::string& s);

//
// dtoa() : Takes a double and returns a std::string
//
inline std::string dtoa(double value){
	
	std::stringstream ss;
	ss << value;
	return ss.str();
	
}

//
// split(): Takes a string, tokenizes it on the white space
//          and returns a std::vector of the tokens
//
inline std::vector<std::string> split(const std::string& s){
	
	std::istringstream is(s);
	return std::vector<std::string> (std::istream_iterator<std::string>(is),std::istream_iterator<std::string>());
	
}

//
// stringToUpper: Returns an upper-cased transformed copy of the string
//
inline std::string stringToUpper(const std::string &label){
	std::string upperCase = label;
	std::transform(upperCase.begin(),upperCase.end(),upperCase.begin(),(int(*)(int))toupper);
	return upperCase;
	
}


#endif
