//////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager and Ritu Khanna
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

#ifndef LEGEND_INCLUDED
#define LEGEND_INCLUDED

#include "DrawingLabelManager.h"
#include "SVG.h"

class Legend{
	
protected:
	double _width;
	double _height;
public:
	
	Legend() {}
	virtual ~Legend(){};
	
	virtual void draw(std::ostringstream& os,SVG* psvg,DrawingLabelManager* plm,const double x,const double y)=0;
	
	
	double getWidth(void) const { return _width; }
	double getHeight(void) const { return _height; }
	
	
};

#endif
