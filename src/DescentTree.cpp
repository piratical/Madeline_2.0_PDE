/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna, and Adrian Marrs
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
// 2005.12.01
//

//
// DescentTree.cpp
//

#include "DescentTree.h"

DescentTree::~DescentTree(){
	
	
	
}

//////////////////////////////////////
//
// PUBLIC METHODS
//
//////////////////////////////////////


//
// addExternalConnectorPair():
//
void DescentTree::addExternalConnectorPair(Individual* individual,Individual* spouse){
	
	_externalConnectorPair.push_back(individual);
	_externalConnectorPair.push_back(spouse);
	
}

//
// getNumberOfConnectionsWithDT():
//
unsigned DescentTree::getNumberOfConnectionsWithDT(unsigned dtId){
	
	std::map<unsigned,unsigned>::iterator mit = _connectionsWithDT.find(dtId);
	if(mit != _connectionsWithDT.end()) return mit->second;
	return 0;
	
}

///
/// getStartIndividual(): Returns start individual of a founding group.
///
Individual* DescentTree::getStartIndividual(void){
	
	if(_startIndividual) { return _startIndividual; }
	
	// Determine the start individual for future use:
	Individual* startIndividual = _foundingGroup.front();
	for(unsigned i=1;i<_foundingGroup.size();i++){
		
		if(_foundingGroup[i]->getNumberOfSpouses() > startIndividual->getNumberOfSpouses()){
			startIndividual = _foundingGroup[i];
		}
	}
	_startIndividual = startIndividual;
	return _startIndividual;
	
}

//
// incrementConnectionsWithDT():
//
void DescentTree::incrementConnectionsWithDT(unsigned dtId){
	
	++_connectionsWithDT[dtId];
	
}

