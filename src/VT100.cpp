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
// VT100.cpp
//
// 2016.01.28.ET
//

#include "VT100.h"

VTStreamState VTStreamState::vtStreamState;
const std::streambuf *VTStreamState::coutBuf;
const std::streambuf *VTStreamState::cerrBuf;
const bool VTStreamState::coutIsATerminal=isatty(fileno(stdout));
const bool VTStreamState::cerrIsATerminal=isatty(fileno(stderr));

VTStreamState::VTStreamState(){
	
	coutBuf=std::cout.rdbuf();
	cerrBuf=std::cerr.rdbuf();
	
}
std::ostream & operator << (std::ostream &os, enum vt100::command cmd){
	
	//
	// If the stream is cout or cerr and if it really is a terminal
	// (and not some file redirection), then print out the v100::command.
	// Otherwise, just return the reference to the ostream.
	//
	std::streambuf const *osBuf = os.rdbuf();
	if( (osBuf==VTStreamState::coutBuf && VTStreamState::coutIsATerminal)
	    || 
	    (osBuf==VTStreamState::cerrBuf && VTStreamState::cerrIsATerminal)
	){
		switch(cmd){
		case vt100::stopColor:
			os << "\x1b[0m";
			break;
		case vt100::startBlack:
			os << "\x1b[1;30m";
			break;
		case vt100::startRed:
			os << "\x1b[1;31m";
			break;
		case vt100::startGreen:
			os << "\x1b[1;32m";
			break;
		case vt100::startYellow:
			os << "\x1b[1;33m";
			break;
		case vt100::startBlue:
			os << "\x1b[1;34m";
			break;
		case vt100::startMagenta:
			os << "\x1b[1;35m";
			break;
		case vt100::startCyan:
			os << "\x1b[1;36m";
			break;
		case vt100::startWhite:
			os << "\x1b[1;37m";
			break;
		default:
			break;
		}
	}
	return os;
}

