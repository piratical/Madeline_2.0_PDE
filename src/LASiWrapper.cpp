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
// 2006.03.28.RK
//

//
// LASiWrapper.cpp
//
// -- Wraps LASi methods and postscript document giving the user the flexibility
// to compile without LASi, Pango and Freetype2
// 
#include "LASiWrapper.h"
#include "UnicodeScript.h"
#include "Utf8String.h"

#ifdef LASI
using namespace LASi;
#endif

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// setFont:
//
void LASiWrapper::setFont(const char* font){ 
	#ifdef LASI
		_doc1.osBody() << LASi::setFont( font ); 
	#endif
}

//
// setFontSize:
//
void LASiWrapper::setFontSize(double fontSize){ 
	#ifdef LASI
		_doc1.osBody() << LASi::setFontSize(fontSize); 
	#endif
}

//
// getDimensions:
//
void LASiWrapper::getDimensions(std::string sampleString,double* lineHeight,double* xAdvance, double* yMinimum, double* yMaximum){
	#ifdef LASI
		_doc1.get_dimensions(sampleString,lineHeight,xAdvance,yMinimum,yMaximum); 
	#else
		*lineHeight = _lineHeight;
		*yMinimum = _yMinimum;
		*yMaximum = _yMaximum;
		UTF8String label = sampleString;
		*xAdvance = _avgCharWidth * label.unicodeValueCount();
	#endif
}

