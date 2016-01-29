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
// 2016.01.28.ET
//

#ifndef VT100_INCLUDED$
#define VT100_INCLUDED$

//
// Just a few simple VT100 terminal escape commands
// for setting the color and boldness of text output
// to a terminal:
//
namespace vt100 
	{
	const char *stopColor    = "\x1b[0m";
	const char *startBlack   = "\x1b[1;30m";
	const char *startRed     = "\x1b[1;31m";
	const char *startGreen   = "\x1b[1;32m";
	const char *startYellow  = "\x1b[1;33m";
	const char *startBlue    = "\x1b[1;34m";
	const char *startMagenta = "\x1b[1;35m";
	const char *startCyan    = "\x1b[1;36m";
	const char *startWhite   = "\x1b[1;37m";
	
}

#endif
