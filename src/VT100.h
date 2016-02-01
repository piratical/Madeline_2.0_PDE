/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna, and Adrian Marrs
// Copyright (c) 2005, 2016 by the
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
// VT100.h
//
// 2016.01.28.ET
//

#ifndef VT100_INCLUDED$
#define VT100_INCLUDED$

#include <iostream>
#include <unistd.h>
#include <stdio.h>

//
// vt100 command namespace and enum:
//
namespace vt100{
	enum command{
		stopColor,
		startBlack,
		startRed,
		startGreen,
		startYellow,
		startBlue,
		startMagenta,
		startCyan,
		startWhite
	};
};

//
// Static class VTStreamState
// maintains information about the
// state of the standard streams
// and whether they are really
// a terminal device or not
//
class VTStreamState{
private:
	static VTStreamState vtStreamState;
	
public:
	static const std::streambuf *coutBuf;
	static const std::streambuf *cerrBuf;
	static const bool coutIsATerminal;
	static const bool cerrIsATerminal;
	
public:
	
	// Constructor:
	VTStreamState();
	
};

//
// Stream operator << for the vt100::commands:
// 
// (This is where the actual work is done)
//
std::ostream & operator << (std::ostream &os,enum vt100::command cmd);

#endif
