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
// 2006.02.03.RK
//

//
// NuclearFamily.h
//
#ifndef NuclearFamily_INCLUDED
#define NuclearFamily_INCLUDED

#include "Width.h"
#include "utility.h"
#include "Date.h"
#include <deque>

class DrawingCanvas;
class Individual;
class NuclearFamily{
private:
	Individual* _mother;
	Individual* _father;
	Width _width;
	bool _leftConnectionShiftFlag;
	unsigned _twinGroupCount; // # of Twin groups in the NF
	std::vector<Individual*> _childrenInClassicalOrder; // empty if there is no consanguinity or external connections
	std::vector<Individual*> _sortedChildren; // the current default is order by id
	
	void _rearrangeIndividualsBasedOnTwins(const bool consanguinousLoop,std::deque<Individual*>& initial,std::deque<Individual*>& leftLoopIndividuals,std::deque<Individual*>& rightLoopIndividuals);
	void _orderTwins(Individual* pivotIndividual,std::deque<Individual*>& srcLoopIndividuals, std::deque<Individual*>& dstLoopIndividuals,const std::deque<unsigned>& indices,bool front=false );
	void _drawTwinConnectors(DrawingCanvas& dc,bool classicalOrder,double verticalDropY);
	inline bool _isMaleWithLoopFlags(Individual* individual,unsigned nuclearFamilyIndex);
	bool _hasIndividualInDeque(Individual* individual,const std::deque<Individual*>& individualQ);	
public:
	//
	// Constructors:
	//
	NuclearFamily() { _twinGroupCount=0; _leftConnectionShiftFlag=false; }
	NuclearFamily(Individual* mother,Individual* father);
	//
	// Methods:
	//
	void addChild(Individual* child){ _sortedChildren.push_back(child); }
	bool hasChild(Individual* child){ 
		std::vector<Individual*>::iterator it = _sortedChildren.begin();
		while(it != _sortedChildren.end()){
			if((*it)->getId() == child->getId()){
			 return true; 
			}
			++it;
		}
		return false; 
	}
	void calculateWidth(bool classicalOrder);
	void drawVerticalDropToIndividual(DrawingCanvas & dc, Individual *pChild, double x,double y);
	void draw(Individual* startIndividual,DrawingCanvas& dc,double startX,double startY, bool classicalOrder,bool dashedOrg=false);
	void drawSpouseConnectors(Individual* individual,const double horizontalInterval,const double iconInterval,const double iconDiameter,DrawingCanvas& dc);
	void sortChildrenInClassicalOrder(bool consanguinousFlag,bool mutipleDT=false);
	void sortChildrenBasedOnDataField(const std::string& name,bool dobSortOrder);
	void findTwinsByDOB();	
	//
	// Setters:
	//
	void setTotalWidth(unsigned total){ _width.setTotal(total); }
	void setLeftWidth(unsigned left){ _width.setLeft(left); }
	void setRightWidth(unsigned right){ _width.setRight(right); }
	void setTwinGroupCount(unsigned twinGroupCount) { _twinGroupCount=twinGroupCount; }
	void setLeftConnectionShiftFlag(bool value) { _leftConnectionShiftFlag = true; }
	
	//
	// Getters:
	//
	Individual* getMother(){ return _mother; }
	Individual* getFather(){ return _father; }
	unsigned getTotalWidth() { return _width.getTotal(); }
	unsigned getLeftWidth()  { return _width.getLeft(); }
	unsigned getRightWidth() { return _width.getRight(); }
	unsigned getNumberOfChildren() { return _sortedChildren.size(); }
	bool getLeftConnectionShiftFlag() { return _leftConnectionShiftFlag; }
	Individual* getChildInClassicalOrder(unsigned index);
	
	bool isConsanguinous(void){ 
		// True if both the mother and father are consanguinous:
		return _father->isConsanguinous() && _mother->isConsanguinous(); 
	}
	
	bool hasExternalConnection(void) { 
		// True if both the mother and father have an external connection:
		return _father->hasExternalConnection() && _mother->hasExternalConnection(); 
	}
	
	unsigned getTwinGroupCount(void) { return _twinGroupCount; }
	
	// Debug:
	void display();
	
};

#endif

