/////////////////////////////////////////////////////////
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
//
// 2006.08.10.ET: Revised
//

//
// DrawingCanvas.cpp
//

#include "DrawingCanvas.h"
#include <fstream>

//
// Required for math definitions like TWO_PI, etc:
//
#include "Number.h"

//
// We include limits.h to set up
// starting minima and maxima for x and y coordinates:
//
#include "limits.h"

///////////////////////////////////
//
// Private Methods:
//
///////////////////////////////////

//
//_setHeader:
//
void DrawingCanvas::_setHeader(){
	
	std::stringstream width,height;
	
	width << _media.getWidth();
	height << _media.getHeight();
	
	//
	// Only print the xml declaration if the
	// document is not an embeddable fragment:
	//
	if( DrawingMetrics::getEmbeddedState()==false ){
		_header <<  "<?xml version=\"1.0\" standalone=\"no\"?>\n";
	}
	
	_header << " <svg version=\"1.1\"  id=\"svgDC\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" ";
	
	//
	// Viewbox only:
	//
	//_header << " viewBox=\"0,0," + width.str() + "," + height.str() + "\"  onload=\"init()\" >\n";
	
	//
	// Based on media width and height only:
	//
	//_header << " width=\"" + width.str() + "\" height=\"" + height.str() + "\"";
	
	//
	// Based on actual x and y extents:
	//
	_header << " width=\""; 
	_header << getXRange();
	_header << "\" height=\"";
	_header << getYRange();
	_header << "\" ";
	
	_header << " onload=\"init()\" >\n";
	
	//
	// Start of <defs> section:
	//
	_header << " <defs>\n";
	
	//
	// Append the CSS:
	//
	_setCSS();
	
	//
	// Append the Javascript
	//
	_setJavascript();
	
	//
	// Append any SVG definitions:
	//
	_setSVGDefinitions();
	//
	// End defs section:
	//
	_header << " </defs>\n";
	
	// Set up the group tag for the whole document
	
	_header << " <g class=\"pedigree\" transform=\"translate(" << getXMinimum()*-1 << ")\">\n";
	
}

//
// _setFooter:
//
void DrawingCanvas::_setFooter(){
	
	// Close the group tag for the whole document
	_footer << " </g>\n";
	_footer << " </svg>\n";
	
}

//
// _setJavascript()
//
void DrawingCanvas::_setJavascript(){
	
	//
	// Append the default javascript to the header:
	//
	_header << " <script type=\"text/javascript\" xlink:href=\"javascript/madeline.js\"></script>\n";
	
}

//
// _setCSS()
//
void DrawingCanvas::_setCSS(){
	
	//
	// Start style CDATA section:
	//
	_header << "  <style type=\"text/css\"><![CDATA[\n\n";
	
	//
	// Line Style definition:
	//
	_header << "line{\n";
	_header << "	stroke-linejoin:miter;\n";
	_header << "	stroke:#000;\n";
	_header << "	stroke-width:" << DrawingMetrics::getLineWidth() << DrawingMetrics::getLineWidthUnit() << ";\n";
	_header << "	stroke-linejoin:miter;\n";
	_header << "	stroke-linecap:square;\n";
	_header << "}\n\n";
	
	//
	// Rectangle style definition: Needed for legend -- do not comment out
	//
	_header << "rect{\n";
	//_header << "	fill:#fff;\n";
	_header << "	stroke:#000;\n";
	_header << "	stroke-width:" << DrawingMetrics::getLineWidth() << DrawingMetrics::getLineWidthUnit() << ";\n";
	_header << "	stroke-linejoin:miter;\n";
	_header << "	stroke-linecap:square;\n";
	_header << "}\n\n";
	
	//
	// Circle style definition:
	//
	//_header << "circle{\n";
	//_header << "	fill:#fff;\n";
	//_header << "	stroke:#000;\n";
	//_header << "	stroke-width:" << DrawingMetrics::getLineWidth() << DrawingMetrics::getLineWidthUnit() << ";\n";
	//_header << "}\n\n";
	
	//
	// Polyline style definition:
	//
	//_header << "polyline{\n";
	//_header << "	fill:#fff;\n";
	//_header << "	stroke:#000;\n";
	//_header << "	stroke-width:" << DrawingMetrics::getLineWidth() << DrawingMetrics::getLineWidthUnit() << ";\n";
	//_header << "}\n\n";
	
	//
	// ellipse : Used for encircled numbers on birth order
	//
	_header << "ellipse{\n";
	//_header << "	fill: #fff;\n";
	_header << "	stroke: #000;\n";
	_header << "	stroke-width:" << 0.5*DrawingMetrics::getLineWidth() << DrawingMetrics::getLineWidthUnit() << ";\n";
	_header << "}\n\n";
	
	//
	// circle, rect and polyline hovers:
	//
	double highlightWidth = 2.0*DrawingMetrics::getLineWidth();
	
	//_header << "rect:hover{\n";
	//_header << "	fill:#ffb787;\n";
	//_header << "	stroke-width:" << highlightWidth << DrawingMetrics::getLineWidthUnit() << ";\n";
	//_header << "}\n\n";
	
	//_header << "circle:hover{\n";
	//_header << "	fill:#ffb787;\n";
	//_header << "	stroke-width:" << highlightWidth << DrawingMetrics::getLineWidthUnit() << ";\n";
	//_header << "}\n\n";
	
	//_header << "polyline:hover{\n";
	//_header << "	fill:#ffb787;\n";
	//_header << "	stroke-width:" << highlightWidth << DrawingMetrics::getLineWidthUnit() << ";\n";
	//_header << "}\n\n";
	
	//
	// Text: defaults for labels
	//
	_header << "text{\n";
	_header << "	font-family:\"";
	// User-selected plus default "sans-serif" fallback font definitions:
	_header << DrawingMetrics::getFontFamily() << "\",sans-serif;\n";
	// Font size:
	_header << "	font-size:" << DrawingMetrics::getFontSize() << DrawingMetrics::getFontSizeUnit() << ";\n";
	_header << "}\n\n";
	
	//
	// .dashed:
	//
	//_header << ".dashed{\n";
	//_header << "	stroke:#930;\n";
	//_header << "}\n\n";
	
	//
	// .layer text:
	//
	_header << ".layer text{\n";
	_header << "	fill:#930;\n";
	_header << "}\n\n";
	
	//
	// .layer line:
	//
	_header << ".layer line{\n";
	_header << "	stroke:#6d7b8d;\n";
	_header << "}\n\n";
	
	//
	// .affected:
	//
	//_header << ".affected{\n";
	//_header << "	fill:#98afc7;\n";
	//_header << "}\n\n";
	
	//
	// .notSampled:
	//
	//_header << ".notSampled{\n";
	//_header << "	stroke:#808080;\n";
	//_header << "}\n\n";
	
	//
	// .affectedNotSampled:
	//
	//_header << ".affectedNotSampled{\n";
	//_header << "	stroke:#808080;\n";
	//_header << "	fill:#98afc7;\n";
	//_header << "}\n\n";
	
	//
	// .birthOrder:
	//
	_header << ".birthOrder{\n";
	_header << "	font-size: " << 2*DrawingMetrics::getFontSize()/3 << DrawingMetrics::getFontSizeUnit() << ";\n";
	_header << "}\n\n";
	
	//
	// .header:
	//
	_header << ".header{\n";
	_header << "	font-size:" << DrawingMetrics::getHeaderFontSize() << DrawingMetrics::getFontSizeUnit() << ";\n";
	_header << "	font-weight:bold;\n";
	_header << "}\n\n";
	
	//
	// keyBox (legend) style definition:
	//
	_header << ".keyBox{\n";
	_header << "	fill:#e5e5e5;\n";
	_header << "	stroke:#000;\n";
	_header << "	stroke-width:" << DrawingMetrics::getLineWidth() << DrawingMetrics::getLineWidthUnit() << ";\n";
	_header << "	stroke-linejoin:miter;\n";
	_header << "	stroke-linecap:square;\n";
	_header << "}\n\n";
	
	//
	// .mating:
	//
	_header << ".mating:hover{\n";
	_header << "	stroke-width:" << highlightWidth << DrawingMetrics::getLineWidthUnit() << ";\n";
	_header << "	stroke:#d56300;\n";
	_header << "}\n\n";
	
	//
	// .selectedIndividual:
	//
	_header << ".selectedIndividual{\n";
	_header << "	stroke-width:" << highlightWidth << DrawingMetrics::getLineWidthUnit() << ";\n";
	_header << "	stroke-linecap:square;\n";
	_header << "	stroke-linejoin:miter;\n";
	_header << "	stroke:#d00;\n";
	_header << "    fill:none;\n";
	_header << "}\n\n";
	
	//
	// .selectedIndividual:hover
	//
	_header << ".selectedIndividual:hover{\n";
	_header << "	stroke:#d00;\n";
	_header << "	fill:#ffbbbb;\n";
	_header << "	fill-opacity:0.70;\n";
	_header << "}\n\n";
	////////////////
	//
	// New classes:
	//
	////////////////
	
	//
	// .solid:
	//
	_header << ".solid{\n";
	_header << "	stroke-width:" << DrawingMetrics::getLineWidth() << DrawingMetrics::getLineWidthUnit() << ";\n";
	_header << "	stroke-linecap:square;\n";
	_header << "	stroke-linejoin:miter;\n";
	_header << "	stroke:#000;\n";
	_header << "	fill:none;\n";
	_header << "}\n\n";
	
	//
	// .dashed:
	//
	_header << ".dashed{\n";
	_header << "	stroke-width:" << DrawingMetrics::getLineWidth() << DrawingMetrics::getLineWidthUnit() << ";\n";
	_header << "	stroke-linecap:square;\n";
	_header << "	stroke-linejoin:miter;\n";
	_header << "	stroke:#98afc7;\n";
	_header << "	stroke-dasharray:2mm,1mm;\n";
	_header << "	stroke-dashoffset:1.0mm;\n";
	_header << "	fill:none;\n";
	_header << "}\n\n";
	
	_header << ".solid:hover{\n";
	_header << "	fill:#ffb787;\n";
	_header << "	fill-opacity:0.70;\n";
	_header << "	stroke:#d56300;\n";
	_header << "	stroke-width:" << highlightWidth << DrawingMetrics::getLineWidthUnit() << ";\n";
	_header << "}\n\n";
	
	_header << ".dashed:hover{\n";
	_header << "	fill:#ffb787;\n";
	_header << "	fill-opacity:0.70;\n";
	_header << "	stroke:#d56300;\n";
	_header << "	stroke-width:" << highlightWidth << DrawingMetrics::getLineWidthUnit() << ";\n";
	_header << "}\n\n";
	
	//
	// thinLine:
	//
	_header << ".thinLine{\n";
	_header << "	fill:none;\n";
	_header << "	stroke: #000;\n";
	_header << "	stroke-width:" << 0.5*DrawingMetrics::getLineWidth() << DrawingMetrics::getLineWidthUnit() << ";\n";
	_header << "}\n\n";
	
	//
	// whiteInkLetter:
	//

	//_header << ".whiteInkLetter_1{\n";
	//_header << "	font-size: " << DrawingMetrics::getFontSize() << DrawingMetrics::getFontSizeUnit() << ";\n";
	//_header << "	fill: #fff;\n";
	//_header << "}\n\n";
	
	//_header << ".whiteInkLetter_2{\n";
	//_header << "	font-size: " << DrawingMetrics::getFontSize()-2 << DrawingMetrics::getFontSizeUnit() << ";\n";
	//_header << "	fill: #fff;\n";
	//_header << "}\n\n";
	
	//_header << ".whiteInkLetter_3{\n";
	//_header << "	font-size: " << DrawingMetrics::getFontSize()-4 << DrawingMetrics::getFontSizeUnit() << ";\n";
	//_header << "	fill: #fff;\n";
	//_header << "}\n\n";
	
	_header << ".whiteInkLetter{\n";
	_header << "	font-size: " << 2*DrawingMetrics::getFontSize()/3 << DrawingMetrics::getFontSizeUnit() << ";\n";
	_header << "	fill: #fff;\n";
	_header << "}\n\n";
	
	//
	// blackInkLetter:
	//

	//_header << ".blackInkLetter_1{\n";
	//_header << "	font-size: " << DrawingMetrics::getFontSize() << DrawingMetrics::getFontSizeUnit() << ";\n";
	//_header << "}\n\n";
	
	//_header << ".blackInkLetter_2{\n";
	//_header << "	font-size: " << DrawingMetrics::getFontSize()-2 << DrawingMetrics::getFontSizeUnit() << ";\n";
	//_header << "}\n\n";
	
	//_header << ".blackInkLetter_3{\n";
	//_header << "	font-size: " << DrawingMetrics::getFontSize()-4 << DrawingMetrics::getFontSizeUnit() << ";\n";
	//_header << "}\n\n";
	
	_header << ".blackInkLetter{\n";
	_header << "	font-size: " << 2*DrawingMetrics::getFontSize()/3 << DrawingMetrics::getFontSizeUnit() << ";\n";
	_header << "	fill: #000;\n";
	_header << "}\n\n";
	
	//
	// End style CDATA section:
	//
	_header << "]]></style>\n";
	
}

//
// _setSVGDefinitions:
//
void DrawingCanvas::_setSVGDefinitions(){
	
	_header << " <marker id=\"endArrow\" viewBox=\"0 0 10 10\" refX=\"1\" refY=\"5\" markerUnits=\"strokeWidth\" orient=\"auto\" markerWidth=\"5\" markerHeight=\"4\">"; 
	_header << "  <polyline points=\"0,0 10,5 0,10 1,5\" fill=\"#000\" />";
	_header << " </marker>\n";
	
}

//
// _trackMinimaAndMaxima
//
void DrawingCanvas::_trackMinimaAndMaxima( double x , double y ){
	
	if( x < _xMinimum ) _xMinimum = x;
	if( y < _yMinimum ) _yMinimum = y;
	if( x > _xMaximum ) _xMaximum = x;
	if( y > _yMaximum ) _yMaximum = y;
	
}

//
// _drawDeceasedLine()
//
void DrawingCanvas:: _drawDeceasedLine(double x, double y){
	
	
	double diameter = DrawingMetrics::getIconDiameter();
	double radius = 0.5*diameter;
	double x1,y1,x2,y2;
	double factor = DrawingMetrics::getScalingFactor();
	x1 = x - radius - factor;
	y1 = y + radius + factor;
	x2 = x + radius + factor;
	y2 = y - radius - factor;
	_body  << "  <line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" />\n";
	
}

//
// _drawProbandArrow()
//
void DrawingCanvas:: _drawProbandArrow(double x, double y){
	
	//
	// Offset tip of arrow from corner of square icon by half a millimeter:
	//
	double diameter = DrawingMetrics::getIconDiameter();
	double padding = 0.5*DrawingMetrics::getScalingFactor();
	double ax = x - DrawingMetrics::getIconRadius() - padding;
	double ay = y + DrawingMetrics::getIconRadius() + padding;
	//
	// Scaling of arrow (3rd parameter) is adjusted to look nice:
	//
	_drawNorthEastArrow(ax,ay,0.33);
	
	double cx = ax - 3.2*DrawingMetrics::getScalingFactor() - 2.0*padding;
	double cy = ay + 3.2*DrawingMetrics::getScalingFactor();
	drawText(cx,cy,"P");
	
}

//
// _drawNorthEastArrow
//
void DrawingCanvas::_drawNorthEastArrow(double x, double y, double scale){
	
	double k  = DrawingMetrics::getScalingFactor();
	double x1 = -7.0 * scale * k; double y1 =  3.0 * scale * k;
	double x2 =  1.5 * scale * k; double y2 =  1.5 * scale * k;
	double x3 = -2.5 * scale * k; double y3 =  2.5 * scale * k;
	double x4 =  1.0 * scale * k; double y4 =  1.0 * scale * k;
	double x5 =  2.5 * scale * k; double y5 = -2.5 * scale * k;
	double x6 =  1.5 * scale * k; double y6 =  1.5 * scale * k;
	
	_body << "<path d=\"M " << x << " " << y ;
	_body << " l " << x1 << " " << y1 ;
	_body << " l " << x2 << " " << y2 ;
	_body << " l " << x3 << " " << y3 ;
	_body << " l " << x4 << " " << y4 ;
	_body << " l " << x5 << " " << y5 ;
	_body << " l " << x6 << " " << y6 ;
	_body << " z\" />\n";
	
}

//
// _drawLegends:
//
void DrawingCanvas::_drawLegends(void){
	
	// Draw the icon legend if present:
	if(_iconLegendFlag){
		_iconLegend.draw(_body,&_svg,&_labelManager,_xMinimum,_yMinimum);
		// Draw the label legend if present:
		if(_labelSet->getNumberOfLabels()){
			double iconLegendHeight = _iconLegend.getHeight();
			_labelLegend.draw(_body,&_svg,&_labelManager,_xMinimum,_yMinimum+iconLegendHeight);
		}
	}else if(_labelSet->getNumberOfLabels()){
		// Draw the label legend if present:
		_labelLegend.draw(_body,&_svg,&_labelManager,_xMinimum,_yMinimum);
	}
}

///////////////////////////////////
//
// Public Methods:
//
///////////////////////////////////

//
// Constructors:
//

DrawingCanvas::DrawingCanvas(const LabelSet *labelSet,const std::string name="a4"){
	
	_labelSet = labelSet;
	
	
	setDrawingMedia(name);
	
	//
	// Initialize the x and y minima and maxima:
	//
	_xMinimum = _yMinimum = (double) INT_MAX;
	_xMaximum = _yMaximum = (double) INT_MIN;
	
	//
	// labelManager needs to have a pointer to a valid PostscriptDocument:
	//
	_labelManager.setPostscriptDocument( & _doc );
	
	//
	// the labelManager automagically sets the correct line height:
	//
	// // _labelManager = DrawingLabelManager(labelSet);
	//
	_labelManager.setLabelSet(labelSet);
	
	_labelLegend.setLabelSet(labelSet);
	_iconLegendFlag = false;
	//
	// Must setHeader() at the very end after drawing 
	// in order to know true x and y extents for width and height:
	//
	// _setHeader();
	
	_setFooter();
	_layerFlag = false;
	
}


//
// setDrawingMedia()
//
void DrawingCanvas::setDrawingMedia(std::string name="a4"){
	
	_media  = DrawingMetrics::getDrawingMedia(name);
	
}

//
// startLayer:
//
void DrawingCanvas::startLayer(){
	
	_layerFlag = true;
	if(_layers.empty()) _layer << " <g id=\"layer0\" class=\"layer\" >\n";
	else                _layer << " <g id=\"layer" << _layers.size() << "\" class=\"layer\" >\n";
	
}

//
// endLayer:
//
void DrawingCanvas::endLayer(){
	
	_layerFlag = false;
	_layer << " </g>\n";
	_layers.push_back( std::string(_layer.str()) );
	_layer.clear();
	
}

//
// startGroup:
//
void DrawingCanvas::startGroup(const std::string& cssClass){
	
	_svg.startGroup(_body,cssClass);
	
}

//
// endGroup:
//
void DrawingCanvas::endGroup(){
	
	_svg.endGroup(_body);
	
}

//
// getWidth:
//
double DrawingCanvas::getWidth(){
	
	return _media.getWidth();
	
}

//
// getHeight:
//
double DrawingCanvas::getHeight(){
	
	return _media.getHeight();
	
}

//
// drawTitle:
//
void DrawingCanvas::drawTitle(const std::string title){
	
	_body << "  <title>" << title << "</title>\n";
	
}

//
// drawLine:
//
void DrawingCanvas::drawLine(double x1,double y1,double x2,double y2){
	
	//if(_layerFlag) _layer << "  <line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" />\n";
	//else           _body  << "  <line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" />\n";
	if(!_layerFlag) _svg.drawLine(_body,x1,y1,x2,y2);
	else            _svg.drawLine(_layer,x1,y1,x2,y2);
	
}

//
// drawPolyline:
//
void DrawingCanvas::drawPolyline(const std::string points,const std::string& id,const std::string& cssClass){
	
	/*std::ostringstream *output;
	if(_layerFlag) output = &(_layer);
	else           output = &(_body );
	*output << "  <polyline points=\"" << points << "\" ";
	if(id        != std::string("") ) *output << "id=\""    << id    <<"\" ";
	if(myclass     != std::string("") ) *output << "class=\""    << myclass    <<"\" ";
	*output << " />\n";
	*/
	if(!_layerFlag) _svg.drawPolyline(_body,points,id,cssClass);
	else            _svg.drawPolyline(_layer,points,id,cssClass);
	
}

//
// drawHorizontalLine:
//
void DrawingCanvas::drawHorizontalLine(double y, double x1, double x2){
	
	//if(_layerFlag) _layer << "  <line x1=\"" << x1 << "\" y1=\"" << y << "\" x2=\"" << x2 << "\" y2=\"" << y << "\" />\n";
	//else           _body  << "  <line x1=\"" << x1 << "\" y1=\"" << y << "\" x2=\"" << x2 << "\" y2=\"" << y << "\" />\n";
	if(!_layerFlag) _svg.drawHorizontalLine(_body,y,x1,x2);
	else            _svg.drawHorizontalLine(_layer,y,x1,x2);
	
}

//
// drawVerticalLine:
//
void DrawingCanvas::drawVerticalLine(double x, double y1, double y2,const std::string & className, const std::string & id){
	
	/*std::ostringstream *output;
	if(_layerFlag) output = &(_layer);
	else           output = &(_body );
	
	
	*output << "  <line ";
	if(className != std::string("") ) *output << "class=\"" << className <<"\" ";
	if(id        != std::string("") ) *output << "id=\""    << id    <<"\" ";
	*output << "x1=\"" << x << "\" y1=\"" << y1 << "\" x2=\"" << x << "\" y2=\"" << y2 << "\" />\n";
	*/
	if(!_layerFlag) _svg.drawVerticalLine(_body,x,y1,y2);
	else            _svg.drawVerticalLine(_layer,x,y1,y2);
	
}

//
// drawText:
//
void DrawingCanvas::drawText(double x,double y,std::string text,std::string cssClass){
	
	if(!_layerFlag) _svg.drawText(_body,x,y,text,cssClass);
	else            _svg.drawText(_layer,x,y,text,cssClass);
	
}

//
// drawIndividual:
//
void DrawingCanvas::drawIndividual(Individual* pIndividual,double x,double y,bool isDashed){
	
	std::string id = pIndividual->getId().get();
	
	if(pIndividual->hasBeenDrawn()){
		//
		// There might be an Ordinary Founder
		// that is married to more than one person in the pedigree. 
		// There might be an Original Founder who has multiple spouses
		// or an individual with a consanguinous or external loop.
		// In such cases draw a dashed individual to represent a repeat.
		//
		if(pIndividual->isOrdinaryFounder() || isDashed);
		else return;
		
	}
	
	/////////////////////////////////////
	//
	// PRELIMINARY STUFF BEFORE DRAWING:
	//
	/////////////////////////////////////
	
	//
	// Store x and y : Currently absolute positions:
	//
	if(!pIndividual->hasBeenDrawn()){
		pIndividual->setX(x);
		pIndividual->setY(y);
	}
	//
	// track the minima and maxima on the drawing so we 
	// can later calculate x and y extents (width and height):
	//
	_trackMinimaAndMaxima( x , y );
	
	//
	// Determine the CSS class of the individual: solid or dashed
	//
	std::string cssClass;
	if( pIndividual->isVirtual() ||
	    (pIndividual->hasBeenDrawn() && (pIndividual->isOrdinaryFounder() || isDashed))
	){
		cssClass="dashed";
	}else{
		cssClass="solid";
	}
	
	/////////////////////////////////////////
	//
	// DRAW SPECIAL TYPES OF INDIVIDUALS:
	//
	/////////////////////////////////////////
	
	if( pIndividual->isSpecial() ){ 
		if( pIndividual->isIndividualIndicatingNoOffspring()){
			std::cout<< pIndividual->getId().get() <<" is an indicator of no offspring" << std::endl;
			_svg.drawIconForNoChildren(_body,x,y-DrawingMetrics::getVerticalDrop2());
			return;
			
		}
		if( pIndividual->isIndividualIndicatingNonFertility()){
			
			_svg.drawIconForInfertility(_body,x,y-DrawingMetrics::getVerticalDrop2());
			return;
			
		}     
		if( pIndividual->isIndividualIndicatingTerminatedPregnancy()){
			
			_svg.drawIconForAbortedPregnancy(_body,x,y-DrawingMetrics::getVerticalDrop2(),pIndividual->getId().get(),cssClass);
			return;
			
		}
	}
	
	/////////////////////////////////////////
	//
	// DRAW ORDINARY TYPES OF INDIVIDUALS:
	//
	/////////////////////////////////////////
	
	//
	// Draw the icon shading first:
	//
	if(!pIndividual->hasBeenDrawn() && pIndividual->getGender().getEnum() != Gender::MISSING) 
		iconPie(x,y,pIndividual);
	
	//
	// Indicate sampled individuals:
	//
	if(pIndividual->isSampled()){
		_drawSampledIndicator(x,y);
	}
	
	//
	// Draw icon:
	//
	switch(pIndividual->getGender().getEnum()){
	case(Gender::MALE):
		
		_svg.drawMaleIcon(_body,x,y,pIndividual->getId().get(),cssClass);
		break;
		
	case(Gender::FEMALE):
		
		_svg.drawFemaleIcon(_body,x,y,pIndividual->getId().get(),cssClass);
		break;
		
	case(Gender::MISSING):
	default:
		
		_svg.drawGenderUnknownIcon(_body,x,y-(M_SQRT2-1)*DrawingMetrics::getIconDiameter(),pIndividual->getId().get(),cssClass);
		break;
		
	}
	
	//
	// Draw deceased line:
	//
	if(!pIndividual->hasBeenDrawn() && pIndividual->isDeceased()){
		_drawDeceasedLine(x,y);
	}
	
	//
	// Draw proband arrow:
	//
	if(!pIndividual->hasBeenDrawn() && pIndividual->isProband()){
		_drawProbandArrow(x,y);
	}
	
	//
	// Draw labels:
	//
	if(!pIndividual->hasBeenDrawn()){
		// drawLabelSet
		drawLabelSet(pIndividual);
		
	}else if(pIndividual->isOriginalFounder() || pIndividual->isOrdinaryFounder()){
		// For individuals who have already been drawn -- display only the id 
		y+= DrawingMetrics::getIconRadius()  +
		    DrawingMetrics::getLabelMargin() + 
		    DrawingMetrics::getYMaximum()    +
		    DrawingMetrics::getLineHeight();
		
		drawText(x,y, _labelManager.fitStringToLabelWidth(pIndividual->getId().get()) );
		
	}
	
	
	//
	// Mark as drawn:
	//
	pIndividual->setHasBeenDrawn();
	
}


//
// drawLabelSet:
//
void DrawingCanvas::drawLabelSet(Individual* pIndividual){
	
	double x = pIndividual->getX();
	//
	// Set Y start position as Y + radius + margin + yMaximum + lineHeight:
	// ==> Note Bene: We leave one extra line so we have room to mark individuals
	//     with tags such as "A", "B", "C", etc.
	//
	double y = pIndividual->getY() + 
	           DrawingMetrics::getIconRadius()  +
	           DrawingMetrics::getLabelMargin() + 
	           DrawingMetrics::getYMaximum()    +
	           DrawingMetrics::getLineHeight();
	           
	//
	// By default just the individualId is displayed:
	//
	if(_labelSet->getNumberOfLabels() == 0){
		
		//
		// Print just the ID:
		// drawText(x,y, pIndividual->getId().get());
		//
		drawText(x,y, _labelManager.fitStringToLabelWidth(pIndividual->getId().get()) );
		return;
	}
	
	double lineHeight = DrawingMetrics::getLineHeight();
	
	if(DrawingMetrics::getDisplayBirthOrder()){
		unsigned order=pIndividual->getBirthOrder();
		if(order != 0){
			std::stringstream birthOrder;
			birthOrder << order;
			if(order >= 10){
				drawEncircledText(birthOrder.str(),x,y,2*DrawingMetrics::getFontSize()/3,DrawingMetrics::getFontSize()/2,"birthOrder");
			}else   drawEncircledText(birthOrder.str(),x,y,DrawingMetrics::getFontSize()/2,DrawingMetrics::getFontSize()/2,"birthOrder");
		}
		y+= lineHeight+DrawingMetrics::getYMaximum();
		
	}
	//
	// Get here if there are labels to process:
	//
	std::vector<UTF8String> labels = _labelSet->getLabelSet(pIndividual);
	for(unsigned i=0;i<labels.size();i++){
		
		drawText(x,y, _labelManager.fitStringToLabelWidth( labels[i] ) );
		y+=lineHeight;
	}
	
}

//
// show:
//
void DrawingCanvas::show(const char* filename){
	
	
	// DEBUG:
	//std::cout << _header << std::endl;
	// Show the layers before the body
	//for(unsigned i=0;i<_layers.size();i++)
	//	std::cout << _layers[i] << std::endl;
	//std::cout << _body << std::endl;
	//std::cout << _footer << std::endl;
	// Write to file with name : pedigree_id+".xml"
	
	// If label and/or icon legends are present
	// readjust the x and y extents:
	if(_labelSet->getNumberOfLabels() != 0 || _iconLegendFlag){ 
		_xMinimum -= _labelLegend.getWidth();
		double height = _labelLegend.getHeight();
		if(_iconLegendFlag) height += _iconLegend.getHeight();
		if(height > getYRange()) {
			_yMaximum += (height - getYRange());
		}
	}
	//
	// Only set the header now because at this point the
	// true x and y extents (width and height) are known:
	_setHeader();
	
	//
	// Add the legends:
	//
	_drawLegends();
	
	std::ofstream svgfile(filename);
	if(svgfile.is_open()){
		svgfile << _header.str() << std::endl;
		
		for(unsigned i=0;i<_layers.size();i++){
			
			svgfile << _layers[i] << std::endl;
			
		}
		
		svgfile << _body.str()   << std::endl;
		svgfile << _footer.str() << std::endl;
		svgfile.close();
		
	}else{
		
		std::cout << "Unable to open file " << filename << std::endl;
		
	}
}

//
// getXMinimum
//
double DrawingCanvas::getXMinimum(){
	
	return _xMinimum - DrawingMetrics::getHorizontalInterval() - DrawingMetrics::getLabelMargin();
	
}

//
// getYMinimum
//
double DrawingCanvas::getYMinimum(){
	
	// In this here, "VerticalDrop2" is an arbitrary margin distance:
	return _yMinimum - DrawingMetrics::getIconRadius() - DrawingMetrics::getVerticalDrop2();
	
}

//
// getXMaximum
//
double DrawingCanvas::getXMaximum(){
	
	return _xMaximum + DrawingMetrics::getHorizontalInterval() + DrawingMetrics::getLabelMargin();
	
}

//
// getYMaximum
//
double DrawingCanvas::getYMaximum(){
	
	
	return _yMaximum + DrawingMetrics::getVerticalDrop1() + DrawingMetrics::getVerticalDrop2() + DrawingMetrics::getIconRadius();
	
}

//
// getXRange
//
double DrawingCanvas::getXRange(){
	
	return getXMaximum() - getXMinimum();
	
}

//
// getYRange
//
double DrawingCanvas::getYRange(){
	
	return getYMaximum() - getYMinimum();
	
}

//
// drawRectangle:
//
void DrawingCanvas::drawRectangle(double x,double y,double width,double height){
	
	if(_layerFlag) _layer << "  <rect class=\"keyBox\" x=\"" << x << "\" y=\"" << y << "\" width=\"" << width << "\" height=\"" << height << "\" />\n";
	else           _body  << "  <rect class=\"keyBox\" x=\"" << x << "\" y=\"" << y << "\" width=\"" << width << "\" height=\"" << height << "\" />\n";
	
}


//
// drawEncircledText:
//
void DrawingCanvas::drawEncircledText(std::string text,double x,double y,double xradius,double yradius,const std::string& myclass){
	
	_body << "  <ellipse cx=\"" << x << "\" cy=\"" << y << "\" rx=\"" << xradius << "\" ry=\"" << yradius << "\" />\n"; 
	drawText(x,y-DrawingMetrics::getYMinimum(), text, myclass);
	
}


//
// arc
//
void DrawingCanvas::arc( double x, double y, double r, double startAngle, double endAngle,const std::string &color ,const std::string &arcLabel,const std::string &arcClass,bool isMale){
	
	double delta   = endAngle - startAngle;
	double modulus = fmod(delta,Number::TWO_PI);
	
	// Used to calculate baseline adjustment for label:
	double lineSpacing,xAdvance,yMinimum,yMaximum;
	
	if(modulus<Number::MINIMUM_DIFFERENCE){
		//
		// Case of a full circle:
		//
		_body << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << r << "\"";
		_body << " style=\"fill:" << color << ";stroke:none;\"/>\n";
		// Draw the label:
		if(arcLabel != std::string(".")){
			// y += DrawingMetrics::getYMaximum()/2;
			_doc.get_dimensions(arcLabel,&lineSpacing,&xAdvance,&yMinimum,&yMaximum);
			 y+= 0.5*(yMaximum-yMinimum);
		}
		drawText(x,y ,arcLabel,arcClass);
		return;
	}
	
	//
	// Get here if arc is only a partial circle:
	//
	
	double x1,y1,x2,y2;
	
	x1=x2=x;
	y1=y2=y;
	
	x1 -= cos(startAngle)*r;
	y1 -= sin(startAngle)*r;
	x2 -= cos(endAngle  )*r;
	y2 -= sin(endAngle  )*r;
	
	// Move to center point:
	_body << "<path d=\"M " << x << " " << y << " ";
	
	// Line from center to edge of circle:
	_body << "L " << x1 << " " << y1 << " ";
	// Arc to second angle:
	_body << "A " << r << " " << r << " 0 0 1 " << x2 << " " << y2 << " ";
	// Line back to center:
	_body << "z\" style=\"fill:" << color << ";stroke:none;\"/>\n";
	
	//
	// Draw the label:
	//
	double x3 = x;
	double y3 = y;
	if(isMale){
		//x3 = x3 + 0.1445*r*cos(startAngle+delta/2);
		//y3 = y3 + 0.1445*r*sin(startAngle+delta/2);
		x3 -= cos(startAngle+delta/2) * 0.4 * r;
		y3 -= sin(startAngle+delta/2) * 0.4 * r;
		
	}else {
		x3 -= cos(startAngle+delta/2) * r * 0.5;
		y3 -= sin(startAngle+delta/2) * r * 0.5;
	}
	
	//
	// if(arcLabel != std::string(".")) y3 += DrawingMetrics::getYMaximum()*0.5;
	// *DrawingMetrics::getScalingFactor();
	//
	if(arcLabel != std::string(".")){
		_doc.get_dimensions(arcLabel,&lineSpacing,&xAdvance,&yMinimum,&yMaximum);
		 y3+= 0.5*(yMaximum-yMinimum);
	}
	drawText(x3,y3,arcLabel,arcClass);
	
}

//
// iconPie
//
void DrawingCanvas::iconPie( double x, double y, Individual *pIndividual ){
	
	//
	// Handle virtual individuals:
	//
	if(pIndividual->isVirtual()){
		
		//_body << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << 0.5*DrawingMetrics::getScalingFactor() << "\"";
		//_body << " class=\"thinLine\"";
		//_body << "/>\n";
		return;
		
	}
	//
	// Get here if non-virtual:
	//
	
	//
	// How many sections are there?
	//
	unsigned sections = pIndividual->getDataTable()->getIconColumnCount();
	if(!sections){
		//
		// No affected fields : Draw a small circle in the middle:
		// (Nice for debugging -- otherwise comment out )
		//
		//_body << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << 0.5*DrawingMetrics::getScalingFactor() << "\"";
		//_body << " class=\"thinLine\"";
		//_body << "/>\n";
		return;
	}
	
	//
	// Setup clipping Id, if needed:
	//
	std::string clipId = pIndividual->getId().get() + "_clipPath";
	double radius=DrawingMetrics::getIconRadius();
	bool isMale = false;
	if( pIndividual->getGender().getEnum()==Gender::MALE ){
		
		setClipPath(x,y,clipId);
		//
		// Increase radius for clipping:
		//
		radius*=Number::SQRT_TWO;
		isMale = true;
		_body << "<g clip-path=\"url(#" << clipId << ")\">\n";
		
	}else{
		//
		// Empty g with no clipping for female:
		//
		_body << "<g>\n";
		
	}
	
	//
	// Iterate over _iconColumns:
	//
	const DataTable *pDT = pIndividual->getDataTable();
	double arcAngle = 2.0*Number::PI/sections;
	double startAngle = 0.5*Number::PI - arcAngle;
	double endAngle   = startAngle+arcAngle;
	for(unsigned i=0;i<sections;i++){
		//
		// Get the data column of the ith icon column:
		//
		DataColumn * pDC = pDT->getColumn( pDT->getIconColumnIndex(i) );
		//
		// Get the UniqueList for this column:
		//
		const UniqueList * pUL = pDC->getUniqueList();
		
		//
		// Only process if there are some non-missing levels
		// present:
		//
		if(!pUL->getLevels()) continue;
		
		//
		// What is the level and label in the UniqueList corresponding to the data value
		// for this individual?
		//
		unsigned level;
		std::string label;
		pUL->getOrdinalAndLabelForKey( pDC->getDataAtIndex( pIndividual->getRowIndex() ),label,level );
		//
		// UniqueList ordinals are 1-offset, so we should only get zero back
		// if the key was not found, which should never happen:
		//
		if(level==0) throw Exception("DrawingCanvas::iconPie()","UniqueList returned ordinal 0.");
		//
		// Level is 1-offset, so subtract:
		//
		level--;
		
		//
		// Get the color series corresponding to this icon column:
		//
		// NOTA BENE: (1) IF there is only one section, we use the 
		//                blackAndWhite series
		//                UNLESS there is a color override.
		//
		//            (2) OTHERWISE if there are more than one section, 
		//                we use the color series
		//                UNLESS there is a black-and-white override.
		//
		ColorSeries *pCS;
		if(sections==1){
			if(DrawingMetrics::getColor()) pCS = pDT->getColorSeriesFromStack(i);
			else                           pCS = pDT->getBlackAndWhiteSeriesFromStack(i);
		}else{
			if(DrawingMetrics::getBlackAndWhite()) pCS = pDT->getBlackAndWhiteSeriesFromStack(i);
			else                                   pCS = pDT->getColorSeriesFromStack(i);
		}
		//
		// Assume reversed for now:
		//
		bool reversed=true;
		std::string arcClass="blackInkLetter";
		if(!pCS->reversedSeriesUseBlackInkAtLevel(level)) arcClass="whiteInkLetter";
		
		//
		// What is the point of "arcClass_n" ? I don't get it ...
		//
		//if(sections == 1)      arcClass += "_1";
		//else if(sections == 2) arcClass += "_2";
		//else if(sections == 3) arcClass += "_3";
		
		arc(x,y,radius,startAngle,endAngle,(reversed?pCS->reversedSeriesGetColorAtLevel(level):pCS->getColorAtLevel(level)),label,arcClass,isMale);
		startAngle+=arcAngle;
		endAngle+=arcAngle;
		
		// For the first time draw the icon legend too:
		if(!_iconLegendFlag){
			_iconLegendFlag = true;
			_iconLegend.setPedigreeTable(pDT);
		}
		
	}
	_body << "</g>\n";
	return;
	
}


//
// setClipPath() : Sets the clipping path for a MALE icon
//
void DrawingCanvas::setClipPath(double x, double y, const std::string &id){
	
	double d = DrawingMetrics::getIconDiameter();
	double r = 0.5 * d;
	x -= r;
	y -= r;
	_body << "  <clipPath id=\"" << id << "\">\n";
	_body << "   <rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << d << "\" height=\"" << d << "\" />\n";;
	_body << "  </clipPath>\n";
	
}

//
// _drawSampledIndicator()
//
void DrawingCanvas:: _drawSampledIndicator(double x, double y){
	
	double offset = DrawingMetrics::getIconRadius();
	double padding = 1.0*DrawingMetrics::getScalingFactor();
	double symbolDiameter = 2.5*DrawingMetrics::getScalingFactor();
	double symbolRadius   = 0.5*symbolDiameter;
	
	double ax = x - offset - padding - symbolRadius; 
	double ay = y - offset + symbolRadius;
	
	drawHorizontalLine (ay, ax - symbolRadius , ax + symbolRadius );
	drawVerticalLine   (ax, ay - symbolRadius , ay + symbolRadius );
	
}

