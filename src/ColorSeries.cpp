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

double ColorSeries::_oneSaturation[]    = {1.0};
double ColorSeries::_twoSaturations[]   = {0.00,1.0};
double ColorSeries::_threeSaturations[] = {0.12,0.48,1.0};
double ColorSeries::_fourSaturations[]  = {0.12,0.30,0.65,1.0};
double ColorSeries::_fiveSaturations[]  = {0.12,0.24,0.48,0.65,1.0};

//double ColorSeries::_fiveSaturations[]={0.08,0.24,0.48,0.80,1.0};

//double ColorSeries::_twoSaturations[]={0.0,1.0};
//double ColorSeries::_threeSaturations[]={0.0,0.50,1.0};
//double ColorSeries::_fourSaturations[]={0.0,0.34,0.67,1.0};
//double ColorSeries::_fiveSaturations[]={0.0,0.25,0.50,0.75,1.0};

// Vector of vectors of saturations:
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
	// Force series to have at least two levels always:
	// if(_levels<2) _levels=2;
	
	_endColor   = endColor;
	_startColor = startColor;
	
	//
	// Create series:
	//
	double startH,startS,startV;
	startH = startColor.getHue();
	startS = startColor.getSaturation();
	startV = startColor.getValue();
	
	//
	// If the start color is not
	// white (the default), then
	// create a purely linear bichromatic
	// color series:
	//
	bool startColorIsWhite = ( startS==0.0 && startV==1.0);
	
	double endH,endS,endV;
	endH = endColor.getHue();
	endS = endColor.getSaturation();
	endV = endColor.getValue();
	
	//
	// Handle achromatic end points correctly:
	// If the start color is white, then the Hue is
	// undefined, so use the hue of the ending color
	// in order to get a smooth monochromatic color
	// series:
	//
	if( startH == -1) startH = endH;
	
	double deltaH,deltaS,deltaV;
	deltaH = endH - startH;
	deltaS = endS - startS;
	deltaV = endV - startV;
	
	double h,s,v,fraction;
	
	for(unsigned i=0;i<_levels;i++){
		
		//if( startColorIsWhite && _levels<=5){
		if( _levels<=5){
			//
			// When the start color is white and there
			// are five or fewer steps in the series,
			// use non-linearly spaced colors from the
			// saturations table:
			//
			h = startH + deltaH * _saturations[_levels][i];
			s = startS + deltaS * _saturations[_levels][i];
			v = startV + deltaV * _saturations[_levels][i];
			
		}else{
			// For bichromatic series or when there are
			// more than five levels, just use linear spacing
			// 
			// For monochromatic series, this works OK for 6
			// and maybe even 7 levels : More than 7 levels
			// is definitely not recommended:
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

