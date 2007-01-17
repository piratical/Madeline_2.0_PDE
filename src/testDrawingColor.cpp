//
// testDrawingColor.cpp
//
// 2006.11.16.ET
//
// This program creates an SVG document consisting of 11 color discs
// varying in saturation from s=0.0 to s=1.0 in increments of 0.10.
// Each color disc is divided into segments with hue, h, varying from
// h=0 (red) to green at 120 degrees to blue at 240 degrees.  A new
// segment is created every 10 degrees.  The lightness or value, v, 
// varies from 0.30 at the inner edge of the disc, to 1.0 at the outer
// edge of the disc, in increments of 0.10. The darkest segments from 
// v=0.0 to v=0.20 are not drawn, thus producing discs with holes in the
// middle.
//
// A single-letter label is printed in the center of each segment
// in either black or white.  This tests the DrawingColor class' useBlackInk()
// method.  While it is trivial to decide whether to print labels in black or
// white for the achromatic grayscale case, choosing the right label color
// when colored backgrounds are present is a bit more complicated due to the
// human eye's non-uniform sensitivity to different frequencies of light.
// Since human color sensitivity peaks in the yellow range between green and red,
// yellow colors appear lightest and blue colors darkest.
//
// View the resulting SVG in Firefox, Opera, Inkscape, or another SVG
// viewing application.
//

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include "DrawingColor.h"
using namespace std;

int main(int argc, char *argv[]);
std::string arc(double x, double y, double r, double startAngle, double endAngle,std::string color);
std::string pie( double x, double y, double r, int sections );
std::string curvedSegment( double x, double y, double r1,double r2, double startAngle, double endAngle,std::string color,std::string labelColor);

double PI=M_PI;

int main(int argc, char *argv[]){
	
	std::string svgStart="<?xml version=\"1.0\" standalone=\"no\"?>\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n<svg width=\"2000\" height=\"1500\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n";
	std::string svgEnd  ="</svg>\n";
	
	std::string clipPath="<clipPath id=\"squareClipper\">\n<rect x=\"200\" y=\"200\" width=\"100\" height=\"100\" />\n</clipPath>\n";
	
	
	cout << svgStart;
	
	DrawingColor segmentColor("segmentColor","#000");
	DrawingColor black("black","#000");
	DrawingColor white("white","#fff");
	
	// v is value, h is hue:
	double scale=2.0;
	
	int xc = 250;
	int yc = 250;
	
	for(int s=0;s<=10;s++){
		for(int v=30;v<=100;v+=10){
			for(int h=0;h<360;h+=10){
				
				segmentColor.setFromHSV(h , s/10.0 , v/100.0 );
				cout << curvedSegment(xc,yc, scale*v, scale*(v+10) , h/180.0*PI , (h+10)/180.0*PI , segmentColor.get(), segmentColor.useBlackInk()?black.get():white.get() );
				
			}
		}
		xc+=500;
		if(xc>1750){ 
			xc=250; 
			yc+=500;
		}
	}
	
	
	cout << svgEnd;
	
	return 0;
}

//
// arc
//
std::string arc( double x, double y, double r, double startAngle, double endAngle,std::string color){
	
	std::ostringstream arc;
	double x1,y1,x2,y2;
	
	x1=x2=x;
	y1=y2=y;
	
	x1 -= cos(startAngle)*r;
	y1 -= sin(startAngle)*r;
	x2 -= cos(endAngle  )*r;
	y2 -= sin(endAngle  )*r;
	
	// Move to center point:
	arc << "<path d=\"M " << x << " " << y << " ";
	
	// Line from center to edge of circle:
	arc << "L " << x1 << " " << y1 << " ";
	// Arc to second angle:
	arc << "A " << r << " " << r << " 0 0 1 " << x2 << " " << y2 << " ";
	// Line back to center:
	//arc << "Z\" style=\"fill:" << color << ";stroke:black;stroke-width:1.0mm;\"/>\n";
	arc << "Z\" style=\"fill:" << color << ";stroke:none;\"/>\n";
	
	return arc.str();
	
}


//
// pie
//
std::string pie( double x, double y, double r, int sections ){
	
	std::ostringstream pie;
	double arcAngle = 2.0*PI/sections;
	double startAngle = 0.5*PI - arcAngle;
	double endAngle   = startAngle+arcAngle;
	
	std::string colorArray[6]={"black","white","blue","orange","purple","magenta" };
	
	for(int i=0;i<sections;i++){
		
		pie << arc(x,y,r,startAngle,endAngle,colorArray[i]);
		startAngle+=arcAngle;
		endAngle+=arcAngle;
		
	}
	return pie.str();
}

//
// curvedSegment
//
std::string curvedSegment( double x, double y, double r1,double r2, double startAngle, double endAngle,std::string color,std::string labelColor){
	
	std::ostringstream arcc;
	double x1,x2,x3,x4,y1,y2,y3,y4;
	
	x1=x2=x3=x4=x;
	y1=y2=y3=y4=y;
	
	x1 -= cos(startAngle)*r2;
	y1 -= sin(startAngle)*r2;
	x2 -= cos(endAngle  )*r2;
	y2 -= sin(endAngle  )*r2;
	
	x4 -= cos(startAngle)*r1;
	y4 -= sin(startAngle)*r1;
	x3 -= cos(endAngle  )*r1;
	y3 -= sin(endAngle  )*r1;
	
	// COLOR TESTING:
	double xc,yc;
	xc = 0.5*(x2+x4);
	yc = 0.5*(y2+y4);
	std::string black="black";
	std::string white="white";
	
	
	// Move to starting point:
	arcc << "<path d=\"M " << x4 << " " << y4 << " ";
	
	// Line from starting point on edge of inner
	// circle out to edge of outer circle:
	arcc << "L " << x1 << " " << y1 << " ";
	
	// Arc to second angle:
	arcc << "A " << r2 << " " << r2 << " 0 0 1 " << x2 << " " << y2 << " ";
	//arc << "L " << x2 << " " << y2 << " ";
	
	// Line back to inner circle:
	arcc << "L " << x3 << " " << y3 << " ";
	
	// Arc back to starting point:
	arcc << "A " << r1 << " " << r1 << " 0 0 0 " << x4 << " " << y4 << " ";
	//arc << "L " << x4 << " " << y4 << " ";
	
	// Finish up:
	arcc << "Z\" style=\"fill:" << color << ";stroke:black;stroke-width:1px;\"/>\n";
	
	// COLOR TESTING:
	//arc << pie(xc,yc,5,2);
	
	//arcc << arc(xc,yc,5,-90/180.0*PI,90/180.0*PI, labelColor );
	arcc << "<text x=\"" << (xc-5) << "\" y=\"" << (yc+5) << "\" style=\"font-family:Verdana;font-size:14;font-weight:bold;\" fill=\"" << labelColor << "\" >A</text>";
	return arcc.str();
	
}


