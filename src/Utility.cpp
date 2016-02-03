/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager and Ritu Khanna
// Copyright (c) 2005 by the
// Regents of the University of Michigan.
// All Rights Reserved.
// 
// The latest version of this program is available from:
// 
//   http://eyegene.ophthy.med.umich.edu/madeline/
//   
// Released under the GNU General Public License.
// A copy of the GPL is included in the distribution
// package of this software, or see:
// 
//   http://www.gnu.org/copyleft/
//   
// ... for licensing details.
// 
/////////////////////////////////////////////////////////
//
// utility.cpp
//
// 2016.02.02.ET Augmented and revised.
//
#include "Utility.h"

namespace utility{
//
// dtoa() : Takes a double and returns a std::string
//
std::string dtoa(double value){
	
	std::stringstream ss;
	ss << value;
	return ss.str();
	
}

//
// split(): Takes a string, tokenizes it on the white space
//          and returns a std::vector of the tokens
//
std::vector<std::string> split(const std::string& s){
	
	std::istringstream is(s);
	return std::vector<std::string> (std::istream_iterator<std::string>(is),std::istream_iterator<std::string>());
	
}

//
// stringToUpper: Returns an upper-cased transformed copy of the string
//
std::string stringToUpper(const std::string &label){
	std::string upperCase = label;
	std::transform(upperCase.begin(),upperCase.end(),upperCase.begin(),(int(*)(int))toupper);
	return upperCase;
	
}

//
// binaryPatternToInteger:
//
// NOTA BENE: The current implementation does 
//            not check if the pattern is too long.
//            (It's fine for short patterns like "001010"
//            for which it is intended)
//
int binaryPatternToInteger(const std::string &pattern){
	
	unsigned pow=1;
	unsigned sum=0;
	for(int i=pattern.length()-1;i>=0;--i,pow<<=1){
		int bit = pattern[i] -'0';
		if(bit){
			if(bit!=1){
				// Bit pattern contains a character other than 0 or 1
				return 0;
			}else{
				// Keep running sum:
				sum += ( bit * pow );
			}
		}
	}
	return sum;
}

//
// looksLikeABinaryPattern
//
// returns true if string is composed entirely of 0's and 1's
//
bool looksLikeABinaryPattern(const std::string &pattern){
	
	for(int i=0;i<pattern.length();i++){
		if(! (pattern[i]=='0' || pattern[i]=='1') ){
			return false;
		}
	}
	return true;
}

}

