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
// ColorSeriesStack.h  2006.12.26.ET
//

#ifndef ColorSeriesStack_INCLUDED
#define ColorSeriesStack_INCLUDED

#include <vector>
#include "DrawingMetrics.h"
#include "ColorSeries.h"
#include "DrawingColor.h"

class ColorSeriesStack{
	
public:
	
	enum SERIESTYPE{ BLACKANDWHITE, MONOCHROMATIC, BICHROMATIC };
	
private:
	
	SERIESTYPE _type;
	
	//unsigned _n;
	std::vector<ColorSeries *> _colorSeriesStack;
	
	
public:
	
	// Constructor:
	ColorSeriesStack( SERIESTYPE type = BICHROMATIC );
	
	// Destructor:
	~ColorSeriesStack();
	
	void add(unsigned levels);
	
	ColorSeries *get(unsigned nth) const;
	
	void setBichromatic(bool seriesAreBichromatic);
	
};


#endif
