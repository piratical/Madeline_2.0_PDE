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
// 2005.11.29.RK
//

//
// DescentTree.h
//
#ifndef DescentTree_INCLUDED
#define DescentTree_INCLUDED

#include "Individual.h"
#include "Width.h"
#include <vector>
#include <map>
#include <iostream>

//! Each founding group forms a Descent Tree.
/*! Descent Tree keeps track of its width, external and internal connections.
*/
class DescentTree
{
private:
	unsigned _order;
	unsigned _id;
	unsigned _externalConnections;
	bool _hasConsanguinity;
	Individual* _startIndividual;
	std::vector<Individual*> _foundingGroup;
	std::map<unsigned,unsigned> _connectionsWithDT;
	std::vector<Individual*> _externalConnectorPair; // list of individuals (and their spouses) who make external connections with other DTs. 
	Width _width;
	
public:
	
	//
	// Constructors/Destructor:
	// 
	DescentTree(unsigned id){
		_id = id;
		_startIndividual = NULL;
		_hasConsanguinity = false;
		_externalConnections = 0;
	}
	DescentTree(unsigned id,std::vector<Individual*> foundingGroup) : _id(id),_startIndividual(NULL) { _foundingGroup = foundingGroup; _hasConsanguinity = false; }
	~DescentTree();
	
	//
	// Setters:
	//
	void setConsanguinity() { _hasConsanguinity = true; }
	void setOrder(unsigned order) { _order = order; }
	void setStartIndividual(Individual* startIndividual){ _startIndividual = startIndividual; }
	void setLeftWidth(unsigned left) { _width.setLeft(left); }
	void setRightWidth(unsigned right) { _width.setRight(right);  }
	void setTotalWidth(unsigned total) { _width.setTotal(total); }
	void addIndividualToFoundingGroup(Individual* individual){ _foundingGroup.push_back(individual); }
	void setNumberOfExternalConnections(unsigned count) { _externalConnections = count; }
	
	//
	// Getters:
	//
	unsigned getNumberOfFoundingGroupIndividuals() const { return _foundingGroup.size(); }
	unsigned getId( void ){ return _id; }
	Individual* getStartIndividual(void);
	Individual* getFoundingGroupIndividual(unsigned i){ return _foundingGroup[i]; }
	unsigned getTotalWidth() { return _width.getTotal(); }
	unsigned getLeftWidth()  { return _width.getLeft(); }
	unsigned getRightWidth() { return _width.getRight(); }
	unsigned getNumberOfExternalConnections() { return _externalConnections; }
	Individual* getExternalConnector(unsigned index){ return _externalConnectorPair[index]; }
	unsigned getNumberOfConnectionsWithDT(unsigned dtId);
	bool hasConsanguinity(void) const { return _hasConsanguinity; }
	
	void addExternalConnectorPair(Individual* individual,Individual* spouse);
	void incrementConnectionsWithDT(unsigned dtId);
	void incrementNumberOfExternalConnections(){ _externalConnections++; }
	
	//
	// DEBUG:
	//
	void displayFoundingGroup(){
		for(unsigned i=0;i<_foundingGroup.size();i++)
			std::cout << "FG " << _foundingGroup[i]->getId() << std::endl;
	}
	
};

#endif
