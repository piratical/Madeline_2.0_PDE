/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna and Adrian Marrs
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
// 2007.03.28.RK.
//

//
// LASiWrapper.h
//
// -- Wraps LASi methods and postscript document giving the user the flexibility
// to compile without LASi, Pango and Freetype2
// 
#ifndef LASiWrapper_INCLUDED
#define LASiWrapper_INCLUDED

#include <iostream>
//
// For font metrics
// and Postscript output:
// 
#ifdef LASI
#include <LASi.h>
#endif
#include "DrawingMetrics.h"

class LASiWrapper{
	
private: 
	#ifdef LASI
	LASi::PostscriptDocument _doc1;
	#endif
	double _yMaximum;
	double _yMinimum;
	double _lineHeight;
	double _avgCharWidth;
public:
	LASiWrapper(){
		// Set constant values for lineHeight, yMaximum and yMinimum if the application is compiled without LASi 
		#ifndef LASI
			_lineHeight = 13.75;
			_yMinimum = -1.87184;
			_yMaximum = 6.84582;
			_avgCharWidth = 6.0;
		#endif
	}
	
	void setFont(const char* font);
	void setFontSize(double fontSize);
	void getDimensions(std::string sampleString,double* lineHeight,double* xAdvance, double* yMinimum, double* yMaximum);
	
};

#endif
