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

//
// ColorSeriesStack.cpp  2006.12.26.ET
//

#include "ColorSeriesStack.h"
#include "RandomGenerator.h"

//
// Constructor:
//
ColorSeriesStack::ColorSeriesStack( bool seriesAreBichromatic){
	
	_seriesAreBichromatic = seriesAreBichromatic;
	
};


//
// setBichromatic():
//
void ColorSeriesStack::setBichromatic(bool seriesAreBichromatic){
	
	_seriesAreBichromatic = seriesAreBichromatic;
	
}

//
// Destructor:
//
ColorSeriesStack::~ColorSeriesStack(){
	
	if(_colorSeriesStack.size()>0){
		
		std::vector<ColorSeries *>::const_iterator p;
		for(p=_colorSeriesStack.begin();p!=_colorSeriesStack.end();++p){
			
			delete (*p);
			
		}
	}
}

//
// add() : Add a ColorSeries with "levels" levels to the stack
//
void ColorSeriesStack::add(unsigned levels){
	
	
	if(levels<2) levels=2;
	
	ColorSeries *pCS;
	
	unsigned n = _colorSeriesStack.size();
	
	if(n<DrawingMetrics::monochromat.size()){
		//
		// Use predefined color series based on DrawingMetrics colors:
		//
		if(_seriesAreBichromatic){
			pCS = new ColorSeries(levels,DrawingMetrics::monochromat[n],DrawingMetrics::bichromat[n]);
		}else{
			pCS = new ColorSeries(levels,DrawingMetrics::monochromat[n]);
		}
	}else{
		//
		// Use random colors:
		//
		RandomGenerator r;
		DrawingColor color1;
		//
		// Randomly choose a color with at least 50% saturation and 50% value:
		// i.e., we choose relatively brighter and more saturated colors ...
		//
		color1.setFromHSV(r.getIntegerInRange(0,360),r.getIntegerInRange(50,100),r.getIntegerInRange(50,100));
		
		if(_seriesAreBichromatic){
			// Set color2 to the complement of color1:
			DrawingColor color2;
			color2.set(color1.getComplement());
			pCS = new ColorSeries(levels,color1,color2);
		}else{
			pCS = new ColorSeries(levels,color1);
		}
		
	}
	_colorSeriesStack.push_back(pCS);
	
}

//
// get()
//
ColorSeries *ColorSeriesStack::get(unsigned nth) const{
	
	return _colorSeriesStack[nth];
	
}

