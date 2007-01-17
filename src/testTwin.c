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
// 2005.09.08.ET
//

//
// Twin.cpp
//

#include "Twin.h"

int main(){
	Twin t1;
	std::cout << " T1 " << t1 << " Type " << t1.getTwinType() << std::endl;
	Twin t2("A",Twin::MONOZYGOTIC_TWIN);
	Twin t3("A",Twin::MONOZYGOTIC_TWIN);
	std::cout << " T2 " << t2 << " Type " << t2.getTwinType() << std::endl;
	std::cout << " T3 " << t3 << " Type " << t3.getTwinType() << std::endl;
	Twin t4("a",Twin::DIZYGOTIC_TWIN);
	Twin t5("a",Twin::DIZYGOTIC_TWIN);
	std::cout << " T4 " << t4 << " Type " << t4.getTwinType() << std::endl;
	std::cout << " T5 " << t5 << " Type " << t5.getTwinType() << std::endl;
	Twin t6;
	t6.set("1",Twin::ZYGOSITY_UNKNOWN_TWIN);
	t1.set("1",Twin::ZYGOSITY_UNKNOWN_TWIN);
	std::cout << " T1 " << t1 << " Type " << t1.getTwinType() << std::endl;

	std::cout << " T6 " << t6 << " Type " << t6.getTwinType() << std::endl; 
	std::cout << "get t6 " << t6.get() << std::endl;
	Twin t8("",Twin::MONOZYGOTIC_TWIN);
	std::cout << " T8 " << t8 << " Type " << t8.getTwinType() << std::endl;
	return 1;

}
