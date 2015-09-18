/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna and Adrian Marrs
// Copyright (c) 2006 by the
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

// 2006.12.01.ET
/// ColorSeries.cpp 
///
/// The color series class creates 
/// both monochromatic and
/// bichromatic color series
///

#include "ColorSeries.h"

//DEBUG:
#include <iostream>

//
// Static Members:
//
// => Note that the initial offset of 0.00 takes into account
//    the fact that we adjust the "white" endpoint to be "off white"
//    for monochromatic series that use these customizations, so in
//    reality the first stop is not going to really be at 0.00 "pure white"
//    but at whatever off-white point that "offsetStart" is set to.
//
double ColorSeries::_oneSaturation[]    = {1.0};
double ColorSeries::_twoSaturations[]   = {0.00,1.0};
double ColorSeries::_threeSaturations[] = {0.00,0.48,1.0};
double ColorSeries::_fourSaturations[]  = {0.00,0.30,0.65,1.0};
double ColorSeries::_fiveSaturations[]  = {0.00,0.24,0.48,0.65,1.0};

//
// Vector of vectors of saturations:
//
double *ColorSeries::_saturations[]={
	NULL,
	_oneSaturation,
	_twoSaturations,
	_threeSaturations,
	_fourSaturations,
	_fiveSaturations
};

//
// Constructor:
//
ColorSeries::ColorSeries(unsigned levels,const DrawingColor &endColor,const DrawingColor &startColor){
	
	_levels   = levels;
	
	_endColor   = endColor;
	_startColor = startColor;
	
	//
	// Create series:
	//
	// => When the start color is white,
	//    we want to adjust it to actually be a very light
	//    shade of the end color. If we don't adjust it, then
	//    the pure "white" level looks out-of-place in the series.
	//    startOffset determines how far away from "white" to go:
	//
	double startOffset=0.12;
	// 
	// => (When the start color is not white, then we just
	//    create a purely linear bichromatic color series
	//    from the start to end colors.)
	// 
	double startH,startS,startV,endH,endS,endV,deltaH,deltaS,deltaV;
	startH = startColor.getHue();
	startS = startColor.getSaturation();
	startV = startColor.getValue();
	endH = endColor.getHue();
	endS = endColor.getSaturation();
	endV = endColor.getValue();
	deltaH = endH - startH;
	deltaS = endS - startS;
	deltaV = endV - startV;
	
	// Check whether the start color is white:
	bool startColorIsWhite = ( startS==0.0 && startV==1.0);
	
	if(startColorIsWhite){
		//
		// Handle achromatic end points correctly:
		// If the start color is white, then the Hue is
		// undefined, so use the hue of the ending color
		// in order to get a smooth monochromatic color
		// series:
		//
		// if( startH == -1){
		//
		startH = endH;
		startS += startOffset*deltaS;
		startV += startOffset*deltaV;
		// Recalculate the deltas:
		deltaH = endH - startH;
		deltaS = endS - startS;
		deltaV = endV - startV;
	}
	
	
	double h,s,v,fraction;
	
	for(unsigned i=0;i<_levels;i++){
		
		if( startColorIsWhite ){
			//
			// MONOCHROMATIC SERIES:
			//
			// When the start color is white and there
			// are five or fewer steps in the series,
			// use non-linearly spaced colors from the
			// saturations table because it looks a
			// little better to the human eye this way:
			//
			if( _levels<=5){
				
				h = startH;
				s = startS + deltaS * _saturations[_levels][i];
				v = startV + deltaV * _saturations[_levels][i];
			}else{
				fraction = (double)i/(double)(_levels-1);
				h = startH; 
				s = startS + deltaS * fraction;
				v = startV + deltaV * fraction;
			}
		}else{
			//
			// BICHROMATIC SERIES
			//
			// For bichromatic series, always use linear spacing
			// even for series with few levels:
			//
			fraction = (double)i/(double)(_levels-1);
			h = startH + deltaH * fraction;
			s = startS + deltaS * fraction;
			v = startV + deltaV * fraction;
			
		}
		_colorSeries.push_back( DrawingColor(endColor.getName(),h,s,v));
	}
}

//
// 2015.09.15.ET: Constructor for a custom color series:
//                Allows defining up to 16 colors using HTML/CSS hex triplets or sextuplets
//                e.g., "#ff0" or "#fed379", etc.
//
ColorSeries::ColorSeries(
            const std::string &c0,const std::string &c1,const std::string &c2,const std::string &c3,
            const std::string &c4,const std::string &c5,const std::string &c6,const std::string &c7,
            const std::string &c8,const std::string &c9,const std::string &cA,const std::string &cB,
            const std::string &cC,const std::string &cD,const std::string &cE,const std::string &cF){
	
	if(c0[0]=='#') _colorSeries.push_back( DrawingColor("c0",c0) );
	if(c1[0]=='#') _colorSeries.push_back( DrawingColor("c1",c1) );
	if(c2[0]=='#') _colorSeries.push_back( DrawingColor("c2",c2) );
	if(c3[0]=='#') _colorSeries.push_back( DrawingColor("c3",c3) );
	
	if(c4[0]=='#') _colorSeries.push_back( DrawingColor("c4",c4) );
	if(c5[0]=='#') _colorSeries.push_back( DrawingColor("c5",c5) );
	if(c6[0]=='#') _colorSeries.push_back( DrawingColor("c6",c6) );
	if(c7[0]=='#') _colorSeries.push_back( DrawingColor("c7",c7) );
	
	if(c8[0]=='#') _colorSeries.push_back( DrawingColor("c8",c8) );
	if(c9[0]=='#') _colorSeries.push_back( DrawingColor("c9",c9) );
	if(cA[0]=='#') _colorSeries.push_back( DrawingColor("cA",cA) );
	if(cB[0]=='#') _colorSeries.push_back( DrawingColor("cB",cB) );
	
	if(cC[0]=='#') _colorSeries.push_back( DrawingColor("cC",cC) );
	if(cD[0]=='#') _colorSeries.push_back( DrawingColor("cD",cD) );
	if(cE[0]=='#') _colorSeries.push_back( DrawingColor("cE",cE) );
	if(cF[0]=='#') _colorSeries.push_back( DrawingColor("cF",cF) );
	
	_levels = _colorSeries.size();
	// DEBUG: std::cerr << "Custom series constructed with " << _levels << " levels." << std::endl;
}

//
// get():
//
DrawingColor ColorSeries::get(unsigned level) const{
	
	if(level > _levels-1){
		// return the default color (which is white):
		return DrawingColor();
	}
	return _colorSeries[level];
	
}

//
// getColorAtLevel():
//
std::string ColorSeries::getColorAtLevel(unsigned level) const{
	
	if(level > _levels-1){
		// return the default color (which is white):
		return DrawingColor().get();
	}
	return _colorSeries[level].get();
	
}

//
// useBlackInkAtLevel():
//
bool ColorSeries::useBlackInkAtLevel(unsigned level) const{
	
	if(level > _levels-1){
		// return the default color (which is white),
		// so this should return black ink:
		return DrawingColor().useBlackInk();
	}
	return _colorSeries[level].useBlackInk();
	
}

//
// reversedSeriesGetColorAtLevel():
//
std::string ColorSeries::reversedSeriesGetColorAtLevel(unsigned level) const{
	if(level > _levels-1){
		// return the default color (which is white):
		return DrawingColor().get();
	}
	unsigned reversedLevel = _levels - 1 - level; 
	return _colorSeries[reversedLevel].get();
	
}

//
// reversedSeriesUseBlackInkAtLevel():
//
bool ColorSeries::reversedSeriesUseBlackInkAtLevel(unsigned level) const{
	
	if(level > _levels-1){
		// return the default color (which is white),
		// so this should return black ink:
		return DrawingColor().useBlackInk();
	}
	unsigned reversedLevel = _levels - 1 - level; 
	return _colorSeries[reversedLevel].useBlackInk();
	
}

