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
// utility.h
//
// 2016.02.02.ET: Augmented and revised
//
#ifndef UTILITY_INCLUDED
#define UTILITY_INCLUDED

#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <algorithm>

namespace utility{

std::string dtoa(double value);
std::vector<std::string> split(const std::string& s);
std::string stringToUpper(const std::string &label);
int binaryPatternToInteger(const std::string &pattern);
bool looksLikeABinaryPattern(const std::string &pattern);

}

#endif

