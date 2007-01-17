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

#ifndef LABELLEGEND_INCLUDED
#define LABELLEGEND_INCLUDED

#include "DrawingMetrics.h"
#include "LabelSet.h"
#include "DrawingLabelManager.h"
#include "DrawingCanvas.h"
#include "SVG.h"
#include "Utf8String.h"
#include "Legend.h"

class LabelLegend : public Legend{
	
private:
	const LabelSet * _labelSet;
	
public:
	
	LabelLegend() {}
	void setLabelSet(const LabelSet *const labelSet){
		
		_labelSet = labelSet;
		//_width  = DrawingMetrics::getIconInterval()  + 2*DrawingMetrics::getLabelMargin();
		_width = DrawingMetrics::getIconInterval(); 
		_height = _labelSet->getNumberOfLabels()*DrawingMetrics::getLineHeight() + 2*DrawingMetrics::getLabelMargin();
		
	}
	
	void draw(std::ostringstream& os,SVG* psvg,DrawingLabelManager* plm,const double x,const double y){
		
		// lineHeight:
		double lineHeight = DrawingMetrics::getLineHeight();
		double labelMargin = DrawingMetrics::getLabelMargin();
		std::vector<UTF8String> labels = _labelSet->getLabelNameSet();
		
		// Determine a starting point to draw the key:
		double xstt = x - DrawingMetrics::getHorizontalInterval() + labelMargin;
		double ystt = y; 
		
		// Wrap labels in a group:
		psvg->startGroup(os,"legend");
		psvg->drawRectangle(os,xstt,ystt,_width-2*labelMargin,_height,"keyBox");
		ystt+= lineHeight;
		xstt+=DrawingMetrics::getHorizontalInterval() - labelMargin;
		for(unsigned i=0;i<labels.size();i++){
			psvg->drawText(os,xstt,ystt, plm->fitStringToLabelWidth( labels[i] ) );
			ystt+=lineHeight;
		}
		psvg->endGroup(os);
	}
	
};

#endif
