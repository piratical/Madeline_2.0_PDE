/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager
// Copyright (c) 2016 by the
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
// RandomId.h
//
// 2016.02.01.ET
//

#ifndef RANDOM_ID_INCLUDED
#define RANDOM_ID_INCLUDED

#include <iostream>
#include <string>
#include <random>

class RandomId{
private:
	
	static const int consonantCount=19;
	static const int vowelCount=5;
	
	static const char uconsonant[];
	static const char uvowel[];
	static const char lconsonant[];
	static const char lvowel[];
	
public:
	
	RandomId(){};
	
	std::string get(void){
		
		std::random_device seed;
		std::mt19937 gen(seed());
		std::uniform_int_distribution<int> cdist(0,consonantCount-1);
		std::uniform_int_distribution<int> vdist(0,vowelCount-1);
		
		std::string result("!");
		
		result.append(1,uconsonant[cdist(gen)]);
		result.append(1,uvowel[vdist(gen)]);
		result.append(1,uconsonant[cdist(gen)]);
		result.append(1,uvowel[vdist(gen)]);
		result.append(1,uconsonant[cdist(gen)]);
		result.append(1,uvowel[vdist(gen)]);
		result.append(1,'!');
		
		return result;
		
	}
	
};

#endif

