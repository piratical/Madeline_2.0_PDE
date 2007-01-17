/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna, and Adrian Marrs
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
// 2006.02.14.RK
//

//
// Width.h
//
//
#ifndef Width_INCLUDED
#define Width_INCLUDED

class Width{
private:
	unsigned _left;
	unsigned _right;
	unsigned _total;
	
public:
	Width(){
		_left=_right=_total=0;
	}
	// Setters:
	void setLeft(unsigned left) { _left = left; }
	void setRight(unsigned right) { _right = right; }
	void setTotal(unsigned total) { _total = total; }
	// Getters:
	unsigned getLeft(void) const { return _left; }
	unsigned getRight(void) const { return _right; }
	unsigned getTotal(void) const { return _total; }
};

#endif
