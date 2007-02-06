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
// Pedigree.cpp
//

#include "Pedigree.h"
#include "Date.h"
#include "String.h"
#include "Twin.h"
#include "Number.h"
#include "DrawingCanvas.h"
#include "Grid.h"

#include <algorithm>
#include <map>
#include <vector>
#include <deque>

/////////////////////////////////////
//
// Destructor:
//
/////////////////////////////////////

Pedigree::~Pedigree(){
	
	
	std::set<Individual*,compareIndividual>::iterator it = _individuals.begin();
	while(it != _individuals.end()){
		delete *it;
		++it;
	}
	_individuals.clear();
	std::set<NuclearFamily*,compareNuclearFamily>::iterator nit = _nuclearFamilies.begin();
	while(nit != _nuclearFamilies.end()){
		delete *nit;
		++nit;
	}
	_nuclearFamilies.clear();
	for(std::vector<DescentTree*>::iterator dit =_descentTrees.begin();dit != _descentTrees.end();++dit)
		delete *dit;
	_descentTrees.clear();
	
}

/////////////////////////////////////
//
// Private methods:
//
/////////////////////////////////////


//
// _setIndividualTwinField:
//
void Pedigree::_setIndividualTwinField(const DataColumn* twinColumn,char type){
	
	// for warnings:
	const char *methodName="Pedigree::_setIndividualTwinField()";
	
	unsigned rowIndex;
	std::string marker;
	std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	while(individualIt != _individuals.end()){
		rowIndex = (*individualIt)->getRowIndex();
		marker = twinColumn->get(rowIndex);
		if(type == 'M'){
			(*individualIt)->setTwin(marker,Twin::MONOZYGOTIC_TWIN);
		}else if(type == 'D'){
			if((*individualIt)->getTwinMarker().isMissing())
			(*individualIt)->setTwin(marker,Twin::DIZYGOTIC_TWIN);
			else{
				Twin tempTwinMarker(marker,Twin::DIZYGOTIC_TWIN);
				if(tempTwinMarker.isMissing());
				else{
					Warning(methodName,"Individual %s has more than one twin marker set.",(*individualIt)->getId().get().c_str());
					Warning(methodName,"Twin Marker %s for Individual %s has been ignored.",marker.c_str(),(*individualIt)->getId().get().c_str());
				}
			}
		}
		// setTwin resets an invalid marker to "." 
		marker = (*individualIt)->getTwinMarker().getMarker();
		if(marker != "." && (*individualIt)->getTwinMarker().getTwinType()[0]==type){
			std::map<std::string,std::vector<Individual*> >::iterator mit = _twinMarkers.find(marker);
			if(mit != _twinMarkers.end()){
				mit->second.push_back(*individualIt);
			}else{
				std::vector<Individual*> tempvec;
				tempvec.push_back(*individualIt);
				_twinMarkers.insert(std::map<std::string,std::vector<Individual*> >::value_type(marker,tempvec));
			}
		}
		++individualIt;
	}
	if(_twinMarkers.empty()) return;
	std::map<std::string,std::vector<Individual*> >::iterator mit = _twinMarkers.begin();
	std::vector<std::map<std::string,std::vector<Individual*> >::iterator> twinIterators;
	while(mit != _twinMarkers.end()){
		if(mit->second.size() < 2){
			std::string twin = mit->second.front()->getId().get();
			Warning(methodName,"Only one twin %s found with marker %s.",twin.c_str(),mit->first.c_str());
			Warning(methodName,"Reset the twin marker for %s.",twin.c_str());
			mit->second.front()->setTwin(".",Twin::MISSING_TWIN);
			twinIterators.push_back(mit);
		}
		// NOTE: This number has been arbitrarily set to 12 for a twinGroup
		if(mit->second.size() > 12){
			throw Exception("setIndividualTwinField()","Check the Twin Marker field. There are more than 12 people with the same marker %s",mit->first.c_str());
		}
		++mit;
	}
	while(twinIterators.size()){
		mit = twinIterators.back();
		_twinMarkers.erase(mit);
		twinIterators.pop_back();
	}
	
	// DEBUG: Print out all the twins:
	/*if(_twinMarkers.size() >= 1){
		std::cout << "The twins are " << std::endl;
		mit = _twinMarkers.begin();
		while(mit != _twinMarkers.end()){
			for(unsigned i=0;i<mit->second.size();i++)
			std::cout << "Twin:    " << mit->second[i]->getId() << " Marker :" << mit->second[i]->getTwinMarker().get() << " Type: " << mit->second[i]->getTwinMarker().getTwinType() << std::endl;
			++mit;
		}
	}
	*/
}

//
// _addDescentTree
//
void Pedigree::_addDescentTree(unsigned id) {
	
	_descentTrees.push_back(new DescentTree(id));
	
}

//
// _addNuclearFamily:
//
void Pedigree::_addNuclearFamily(Individual* mother, Individual* father){
	
	unsigned twinGroupCount = 0;
	NuclearFamily* tempNF = new NuclearFamily(mother,father);
	
	// Check if the NF already exists
	std::set<NuclearFamily*,compareNuclearFamily>::iterator nfit = _nuclearFamilies.find(tempNF);
	if(nfit != _nuclearFamilies.end()){ 
		delete tempNF;
		return;
	}
	delete tempNF;
	
	std::pair<std::set<NuclearFamily*,compareNuclearFamily>::iterator,bool> pit = _nuclearFamilies.insert(new NuclearFamily(mother,father));
	if(pit.second){
		// The nuclear family was just added
		// Add all the children that belong to that nuclear family
		std::vector<Individual*> children;
		if(!_dtsHaveConsanguinity) mother->getChildrenWithSpouse(father,children);
		else                       mother->getChildrenSortedByExternalConnections(father,children);
		unsigned j=0;
		while(j < children.size()){
			
			if(!(*pit.first)->hasChild(children[j])){
				(*pit.first)->addChild(children[j]);
				//
				// If the NF has a twinGroup the children default ordering by id changes
				// as the twins have to be adjacent
				//
				if((children[j])->getTwinMarker().get() != "."){
					std::string marker = (children[j])->getTwinMarker().getMarker();
					// get all the Individuals with the same marker from the map
					std::map<std::string,std::vector<Individual*> >::iterator mit = _twinMarkers.find(marker);
					if(mit != _twinMarkers.end()){
						twinGroupCount++;
						if(mit->second.size() > children.size()){
							throw Exception("addNuclearFamily()","More than one Nuclear Family has the same Twin marker %s",mit->first.c_str());
						}
						for(unsigned i=0;i<mit->second.size();i++){
							if(!(*pit.first)->hasChild(mit->second[i])){
								(*pit.first)->addChild(mit->second[i]);
							}
						}
					}
				}
			}
			j++;
		}
		
		if(twinGroupCount > 0){
			(*pit.first)->setTwinGroupCount(twinGroupCount);
		}
		// Add the nuclearFamily to the individual
		mother->addNuclearFamily(*pit.first);
		father->addNuclearFamily(*pit.first);
	}
	return;
	
}


//
// _checkParentsGender: checks the consistency of the pedigree input data table
//
void Pedigree::_checkParentsGender(Individual* individual){
	
	// for warnings:
	const char *methodName="Pedigree::_checkParentsGender()";
	
	Individual* mother = individual->getMother();
	Individual* father = individual->getFather();
	if(mother->getGender().getEnum() == Gender::FEMALE){
		if(father->getGender().getEnum() == Gender::MALE){
		 return;
		}else{
			const std::set<Individual*,Individual::compareIndividual> * pspouses = father->getSpouses();
			std::set<Individual*,Individual::compareIndividual>::iterator it = (*pspouses).begin();
			unsigned cnt = 0;
			unsigned numberOfSpouses = father->getNumberOfSpouses();
			while(cnt < numberOfSpouses){
				if((*it)->getGender().getEnum() == Gender::FEMALE){
					++it; cnt++; continue;
				}else{
					throw Exception("Pedigree::_checkParentsGender()","Conflicting gender for Individual %s.",father->getId().get().c_str());
					//Warning(methodName,"Pedigree %s will be ignored for further queries.",_id.c_str());
					return;
				}
			}
			if(cnt == numberOfSpouses){
				// the father gender is corrected.
				father->setGender("M");
				Warning(methodName,"Changed Gender of Individual %s to Male.",father->getId().get().c_str());
			}
		}
	}else { 
		if((father && father->isVirtual() == true) || (father && father->getGender().getEnum() != Gender::MALE)){
			// interchange gender of father
			if(father->isVirtual() == true) father->setGender("F");
			// update pointers
			individual->setMotherId(father->getId().get());
			individual->setFatherId(mother->getId().get());
			Individual* temp = father;
			individual->setFather(mother);
			individual->setMother(temp);
		}else{
			//
			// Check all the spouses to see if they are males:
			//
			const std::set<Individual*,Individual::compareIndividual> * pspouses = mother->getSpouses();
			std::set<Individual*,compareIndividual>::iterator it = (*pspouses).begin();
			unsigned cnt = 0;
			unsigned numberOfSpouses = mother->getNumberOfSpouses();
			while(cnt < numberOfSpouses){
				if((*it)->getGender().getEnum() == Gender::MALE){
					++it; cnt++; continue;
				}else{
					throw Exception("Pedigree::_checkParentsGender()","Conflicting gender for Individual %s.",mother->getId().get().c_str());
					return;
				}
			}
			if(cnt == numberOfSpouses){
				// the mother gender is corrected
				mother->setGender("F");
				Warning(methodName,"Changed Gender of Individual %s to Female.",mother->getId().get().c_str());
			}
		}
	}
	
}

//
// _getSpouses: insert all the spouses of the founding group individual into the foundingGroup set
//
void Pedigree::_getSpouses(std::set<Individual*,compareIndividual>& foundingGroup, Individual* individual) {
	
	const std::set<Individual*,Individual::compareIndividual> * pspouses = individual->getSpouses();
	std::set<Individual*,compareIndividual>::iterator it = (*pspouses).begin();
	
	while(it != (*pspouses).end()){
		std::pair<std::set<Individual*,compareIndividual>::iterator,bool> pit = foundingGroup.insert(*it);
		if(pit.second) _getSpouses(foundingGroup,*it);
		++it;
	}
	
}

//
// _markLeftLoopFlags:
//
void Pedigree::_markLeftLoopFlags(Individual* individual,unsigned loopNumber){
	
	// Assign the loopNumber to the Individual
	// get parents of the Individual
	// NOTE: Not required to set the flags for ordinaryFounders.
	individual->setLeftSideOfLoop(loopNumber);
	//std::cout << "Individual: " << individual->getId() << "LEFT LOOP: " << individual->getLeftSideOfLoop() << std::endl;
	Individual* father = individual->getFather();
	if(father->isOriginalFounder()) {
		father->setLeftSideOfLoop(loopNumber);
		Individual* mother = individual->getMother();
		mother->setLeftSideOfLoop(loopNumber);
		return;
	}
	if(!father->isOrdinaryFounder()){
		// assign the loopNumber to the Father
		father->setLeftSideOfLoop(loopNumber);
		if(father->getNumberOfSpouses() > 1){
			Individual* mother = individual->getMother();
			mother->setLeftSideOfLoop(loopNumber);
		}
		// recurse up the tree
		_markLeftLoopFlags(father,loopNumber);
	}
	Individual* mother = individual->getMother();
	if(!mother->isOrdinaryFounder()){
		mother->setLeftSideOfLoop(loopNumber);
		if(mother->getNumberOfSpouses() > 1){
			Individual* father = individual->getFather();
			father->setLeftSideOfLoop(loopNumber);
		}
		_markLeftLoopFlags(mother,loopNumber);
	}
	
}

//
// _markRightLoopFlags:
//
void Pedigree::_markRightLoopFlags(Individual* individual,unsigned loopNumber){
	
	// Assign the loopNumber to the Individual
	// get parents of the Individual
	// NOTE: Not required to set the flags for ordinaryFounders.
	individual->setRightSideOfLoop(loopNumber);
	//std::cout << "Individual: " << individual->getId() << "RIGHT LOOP: " << individual->getRightSideOfLoop() << std::endl;
	Individual* father = individual->getFather();
	if(father->isOriginalFounder()){
		father->setRightSideOfLoop(loopNumber);
		Individual* mother = individual->getMother();
		mother->setRightSideOfLoop(loopNumber);
		return;
	}
	if(!father->isOrdinaryFounder()){
		// Assign the loopNumber to the Father
		father->setRightSideOfLoop(loopNumber);
		if(father->getNumberOfSpouses() > 1){
			Individual* mother = individual->getMother();
			mother->setRightSideOfLoop(loopNumber);
		}

		// Recurse up the tree
		_markRightLoopFlags(father,loopNumber);
	}
	Individual* mother = individual->getMother();
	if(!mother->isOrdinaryFounder()){
		mother->setRightSideOfLoop(loopNumber);
		if(mother->getNumberOfSpouses() > 1){
			Individual* father = individual->getFather();
			father->setRightSideOfLoop(loopNumber);
		}
		_markRightLoopFlags(mother,loopNumber);
	}
	
}


//
// _markLeftExternalConnectionFlags:
//
void Pedigree::_markLeftExternalConnectionFlags(Individual* individual,unsigned connectionNumber){
	
	// Assign the connectionNumber to the Individual
	// get parents of the Individual
	// NOTE: Not required to set the flags for ordinaryFounders.
	individual->setLeftSideOfExternalConnection(connectionNumber);
	//std::cout << "Individual: " << individual->getId() << "LEFT EXT #: " << individual->getLeftSideOfExternalConnection() << std::endl;
	Individual* father = individual->getFather();
	if(father->isOriginalFounder()) {
		father->setLeftSideOfExternalConnection(connectionNumber);
		Individual* mother = individual->getMother();
		mother->setLeftSideOfExternalConnection(connectionNumber);
		return;
	}
	if(!father->isOrdinaryFounder()){
		// assign the connectionNumber to the Father
		father->setLeftSideOfExternalConnection(connectionNumber);
		// recurse up the tree
		_markLeftExternalConnectionFlags(father,connectionNumber);
	}
	Individual* mother = individual->getMother();
	if(!mother->isOrdinaryFounder()){
		mother->setLeftSideOfExternalConnection(connectionNumber);
		_markLeftExternalConnectionFlags(mother,connectionNumber);
	}
	
}

//
// _markRightExternalConnectionFlags:
//
void Pedigree::_markRightExternalConnectionFlags(Individual* individual,unsigned connectionNumber){
	
	// assign the connectionNumber to the Individual
	// get parents of the Individual
	// NOTE: Not required to set the flags for ordinaryFounders.
	individual->setRightSideOfExternalConnection(connectionNumber);
	//std::cout << "Individual: " << individual->getId() << " RIGHT EXT #: " << individual->getRightSideOfExternalConnection() << std::endl;
	Individual* father = individual->getFather();
	if(father->isOriginalFounder()) {
		father->setRightSideOfExternalConnection(connectionNumber);
		Individual* mother = individual->getMother();
		mother->setRightSideOfExternalConnection(connectionNumber);
		return;
	}
	if(!father->isOrdinaryFounder()){
		// Assign the connectionNumber to the Father
		father->setRightSideOfExternalConnection(connectionNumber);
		// Recurse up the tree
		_markRightExternalConnectionFlags(father,connectionNumber);
	}
	Individual* mother = individual->getMother();
	if(!mother->isOrdinaryFounder()){
		mother->setRightSideOfExternalConnection(connectionNumber);
		_markRightExternalConnectionFlags(mother,connectionNumber);
	}
	
}


//
// _markExternalConnectionFlags:
//
void Pedigree::_markExternalConnectionFlags(){
	
	unsigned index=0;
	unsigned connectionNumber=1;
	Individual* individual,*spouse;
	
	for(unsigned i=0;i<_descentTrees.size();i++){
		index=0;
		for(unsigned j=0;j<_descentTrees[i]->getNumberOfExternalConnections();j++){
			individual = _descentTrees[i]->getExternalConnector(index);
			spouse = _descentTrees[i]->getExternalConnector(index+1);
			if(individual->hasExternalConnection() == false || spouse->hasExternalConnection() == false){
				_markLeftExternalConnectionFlags(individual,connectionNumber);
				_markRightExternalConnectionFlags(spouse,connectionNumber);
				individual->setExternalConnection(true);
				spouse->setExternalConnection(true);
				connectionNumber++;
			}
			index+=2;
		}
	}
	
}


//
// _assignDescentTrees:
//
void Pedigree::_assignDescentTrees(){
	
	unsigned cnt,descentTreeId,cnt1;
	
	for(unsigned i =0;i < _descentTrees.size();i++){
		// Get number of individuals in the founding group
		cnt = _descentTrees[i]->getNumberOfFoundingGroupIndividuals();
		cnt1 = cnt;
		descentTreeId = _descentTrees[i]->getId();
		// Determine the start Individual for each descent tree
		Individual* startIndividual = _descentTrees[i]->getStartIndividual();
		while(cnt > 0){
			startIndividual->addDescentTree(descentTreeId);
			cnt--;
			if(cnt > 0){
				// Assign descentTreeId to all the spouses of the startIndividual:
				const std::set<Individual*,Individual::compareIndividual> * pspouses = startIndividual->getSpouses();
				std::set<Individual*,Individual::compareIndividual>::iterator it = (*pspouses).begin();
				while(it != (*pspouses).end()){
					(*it)->addDescentTree(descentTreeId);
					cnt--;
					++it;
				}
			}
			_assignChildrenDescentTree(startIndividual,descentTreeId);
			//
			// If any of the founding group members have not been assigned their descentTree
			// make them the startIndividual
			//
			for(unsigned j=0;j<cnt1;j++){
				Individual* individual = _descentTrees[i]->getFoundingGroupIndividual(j);
				if(individual->getNumberOfDescentTrees()) continue;
				startIndividual = individual;
				break;
			}
		}
	}
	
}

//
// _assignChildrenDescentTree:
//
void Pedigree::_assignChildrenDescentTree(Individual* individual,unsigned descentTreeId){
	
	const std::set<Individual*,Individual::compareIndividual> * pchildren = individual->getChildren();
	std::set<Individual*,Individual::compareIndividual>::iterator it = (*pchildren).begin();
	while(it != (*pchildren).end()){
		(*it)->addDescentTree(descentTreeId);
		_assignChildrenDescentTree(*it,descentTreeId);
		++it;
	}
	
}


//
// _getPrimaryDescentTreeIndex: Get the DT index of the individual.
// For cases where the individual belongs to more than 1 DT trace his parents and their ancestors till one DT index is obtained
//
unsigned Pedigree::_getPrimaryDescentTreeIndex(std::set<unsigned>& dt,Individual* individual,bool increment){ 
	
	std::set<unsigned>::iterator sit;
	unsigned dtIndex=0;
	if(dt.size() == 1){
		sit = dt.begin();
	}else{
		// Try to find the single DT to which the individual's parent or his/her ancestors belong:
		Individual* ancestorIndividual = individual->getMother();
		// If the mother is an ordinary founder the DT is not set on her:
		if(ancestorIndividual->getNumberOfDescentTrees() == 0) ancestorIndividual = individual->getFather();
		std::set<unsigned> ancestorDt;
		while(ancestorIndividual->getNumberOfDescentTrees() != 1){
			if(ancestorIndividual->isOriginalFounder()){
				break;
			}
			if(ancestorIndividual->getMother()->getNumberOfDescentTrees() == 0)
			ancestorIndividual = ancestorIndividual->getFather();
			else ancestorIndividual = ancestorIndividual->getMother();
		}
		ancestorDt = ancestorIndividual->getDescentTrees();
		sit = ancestorDt.begin();
		
	}
	for(dtIndex=0;dtIndex<_descentTrees.size();dtIndex++){
		if(_descentTrees[dtIndex]->getId() == (*sit)){
			if(increment)
			_descentTrees[dtIndex]->incrementNumberOfExternalConnections();
			break;
		}
	}
	return dtIndex;
	
}

//
// _addDescentTreesConnectedTo: In cases with > 1 DT to find the most complex tree it is essential to know
// for each DT which are the other DTs that have connectors to it.
//
void Pedigree::_addDescentTreesConnectedTo(unsigned dtIndex,std::deque<DescentTree*>& orderedDescentTrees,bool left){
	
	unsigned max=0;
	bool flag=true;
	for(unsigned j=0;j<_descentTrees.size();j++){
		if(j==dtIndex) continue;
		max = _descentTrees[dtIndex]->getNumberOfConnectionsWithDT(_descentTrees[j]->getId());
		if(max > 0){
			// Check if the DT is not already in the deque
			for(unsigned i=0;i<orderedDescentTrees.size();i++)
				if(orderedDescentTrees[i]->getId() == _descentTrees[j]->getId()) {
					
					flag = false; break; 
				}
			if(flag){
				if(left) orderedDescentTrees.push_front(_descentTrees[j]);
				else   orderedDescentTrees.push_back(_descentTrees[j]);
			}
			flag=true;
			if(orderedDescentTrees.size() == _descentTrees.size()) break;
		}
	}
	
}

//
// _determineConnectorIndividuals(): This is a rewrite of determineConnectorIndividualsOld
//    Instead of looping through the list of individuals a depth-first search approach is used
//    to mark the connector flags. 
//
void Pedigree::_determineConnectorIndividuals(){
	
	// Determine external and internal connectors:
	unsigned loopNumber = 0;
	if(_descentTrees.size() > 1)
		// Assign descent trees to individuals only if there is more than one descent tree
		// If there is only one descent tree the connectors can only be consanguinous.
		_assignDescentTrees();
	unsigned cnt, cnt1;
	for(unsigned i=0;i<_descentTrees.size();i++){
		// Get number of individuals in the founding group
		cnt = _descentTrees[i]->getNumberOfFoundingGroupIndividuals();
		cnt1=cnt;
		Individual* startIndividual = _descentTrees[i]->getStartIndividual();
		while(cnt > 0){
			cnt--;
			if(cnt > 0){
				/*const std::set<Individual*,Individual::compareIndividual> * pspouses = startIndividual->getSpouses();
				std::set<Individual*,compareIndividual>::iterator it = (*pspouses).begin();
				while(it != (*pspouses).end()){
					(*it)->setVisited(true);
					const std::set<Individual*,Individual::compareIndividual> * pchildren = startIndividual->getChildren();
					std::set<Individual*,compareIndividual>::iterator childIt = (*pchildren).begin();
					while(childIt != (*pchildren).end()){
						std::cout << " Child is  " << (*childIt)->getId() << std::endl;
						_markConnectorIndividuals((*childIt),loopNumber);
						++childIt;
					}
					cnt--;
					++it;
				}*/

				const std::set<Individual*,Individual::compareIndividual> * pspouses = startIndividual->getSpouses();
				std::set<Individual*,compareIndividual>::iterator it = (*pspouses).begin();
				while(it != (*pspouses).end()){
					(*it)->setVisited(true);
					std::vector<Individual*> children;
					startIndividual->getChildrenWithSpouse((*it),children);
					unsigned childCnt = 0;
					while(childCnt < children.size()){
						//std::cout << " New child is " << children[childCnt]->getId() << std::endl;
						_markConnectorIndividuals(children[childCnt++],loopNumber);
					}
					cnt--;
					++it;
				}
			}
			// For any remaining founding group member who has not been visited yet
			for(unsigned j=0;j<cnt1;j++){
				Individual* individual = _descentTrees[i]->getFoundingGroupIndividual(j);
				if(individual->hasBeenVisited()) continue;
				startIndividual = individual;
				break;
			}
			
		}
	}
	
	// Reset all the hasBeenVisited flags to false 
	std::set<Individual*,compareIndividual>::iterator it;
	it = _individuals.begin();
	while(it != _individuals.end()){
		(*it)->setVisited(false);
		++it;
	}
	
	// DEBUG:
	//for(unsigned i=0;i<_descentTrees.size();i++){
	//	std::cout << "For Descent TREE with Id" << _descentTrees[i]->getId() << std::endl;
	//	for(unsigned j=0;j<_descentTrees.size();j++){
	//		if(j==i) continue;
	//		std::cout << " # of connections with " << _descentTrees[j]->getId() << " is " << _descentTrees[i]->getNumberOfConnectionsWithDT(_descentTrees[j]->getId()) << std::endl;
	//	}
	//}
	//std::cout << "End of determine Connector Ind :" << _descentTrees.size() << std::endl;
	
	if(_descentTrees.size() == 1) return;
	// Return when there is only one UNCONNECTED individual in a pedigree
	if(_descentTrees.size() == 0) return;
	
	// If there is more than 1 DT in the pedigree find the optimal ordering of the DTs
	_reorderDescentTreesBasedOnExternalConnections();
	_markExternalConnectionFlags();
	
	if(_dtsHaveConsanguinity){
		_markConsanguinousIndividuals();
	}
	
	
}

//
// _reorderDescentTreesBasedOnExternalConnections:
//
void Pedigree::_reorderDescentTreesBasedOnExternalConnections(){
	
	// The most complex tree is in the center; 2 of the remaining trees which have max connections with it flank it on either side 
	unsigned max= 0,complexDTIndex=0;
	
	for(unsigned i=0;i<_descentTrees.size();i++){
		if(_descentTrees[i]->getNumberOfExternalConnections() >= max){
			max = _descentTrees[i]->getNumberOfExternalConnections();
			if(complexDTIndex  == 0 || _descentTrees[i]->hasConsanguinity()){
				complexDTIndex = i;
			}else if(!_dtsHaveConsanguinity && _descentTrees[complexDTIndex]->getNumberOfExternalConnections() < max){
				complexDTIndex = i;
			}
		}
	}
	std::deque<DescentTree*> orderedDescentTrees;
	orderedDescentTrees.push_back(_descentTrees[complexDTIndex]);
	
	// Find the two DTs which have most number of connections with this tree:
	max=0;
	
	unsigned rightDTIndex=0;
	unsigned leftDTIndex=0;
	for(unsigned j=0;j<_descentTrees.size();j++){
		
		if(j==complexDTIndex) continue;
		if(_descentTrees[complexDTIndex]->getNumberOfConnectionsWithDT(_descentTrees[j]->getId()) >= max){
			rightDTIndex = leftDTIndex;
			max = _descentTrees[complexDTIndex]->getNumberOfConnectionsWithDT(_descentTrees[j]->getId());
			leftDTIndex = j;
		}
	}
	
	if(leftDTIndex != rightDTIndex && rightDTIndex != complexDTIndex) orderedDescentTrees.push_back(_descentTrees[rightDTIndex]);
	orderedDescentTrees.push_front(_descentTrees[leftDTIndex]);
	
	/* unsigned rightDTIndex=0,leftDTIndex=0;
	for(unsigned j=0;j<_descentTrees.size();j++){
		if(j==complexDTIndex) continue;
		if(_descentTrees[complexDTIndex]->getNumberOfConnectionsWithDT(_descentTrees[j]->getId()) >= max){
			leftDTIndex = rightDTIndex;
			max = _descentTrees[complexDTIndex]->getNumberOfConnectionsWithDT(_descentTrees[j]->getId());
			rightDTIndex = j;
		}
	}
	if(leftDTIndex != rightDTIndex && leftDTIndex != complexDTIndex) orderedDescentTrees.push_front(_descentTrees[leftDTIndex]);
	orderedDescentTrees.push_back(_descentTrees[rightDTIndex]);
	*/
	// Find all other DTs not included in the deque yet
	
	// First find all the DTs related to the left DT tree
	if(orderedDescentTrees.size() < _descentTrees.size())
	_addDescentTreesConnectedTo(leftDTIndex,orderedDescentTrees,true);
	if(orderedDescentTrees.size() < _descentTrees.size())
	_addDescentTreesConnectedTo(rightDTIndex,orderedDescentTrees,false);
	if(orderedDescentTrees.size() < _descentTrees.size())
	_addDescentTreesConnectedTo(complexDTIndex,orderedDescentTrees,false);
	
	std::vector<DescentTree*>::iterator vit;
	for(unsigned i=0;i<orderedDescentTrees.size();i++){
		vit = find(_descentTrees.begin(),_descentTrees.end(),orderedDescentTrees[i]);
		if(vit != _descentTrees.end()) _descentTrees.erase(vit);
	}
	// There might be DTs which are not connected to any other DT
	// The options are to ignore such DT or just draw them unconnected
	// Currently these dts are pushed to the end of the ordered DT vector
	if(_descentTrees.size() != 0){
		for(unsigned i=0;i<_descentTrees.size();i++)
			orderedDescentTrees.push_back(_descentTrees[i]);
	}
	// DEBUG:
	//std::cout << " THE REORDERED DT vector IS " << std::endl;
	
	_descentTrees.clear();
	for(unsigned i=0;i<orderedDescentTrees.size();i++){
		_descentTrees.push_back(orderedDescentTrees[i]);
		//std::cout << " DT SI " << orderedDescentTrees[i]->getStartIndividual()->getId() << std::endl;
	}
	
	
}

//
// _markConnectorIndividuals: This function marks all the individuals that are consanguinous or have an external connection.
// If there is a single DT, all the consanguinous pair flags are propagated upwards in the descent tree
//
void Pedigree::_markConnectorIndividuals(Individual* individual,unsigned& loopNumber){
	
	// Mark consanguinous individuals and individuals that have external connections:
	if(individual->isOrdinaryFounder() == true || individual->isOriginalFounder() == true) { return;}
	
	if(individual->getNumberOfSpouses() > 0){
		
		const std::set<Individual*,Individual::compareIndividual> * pspouses = individual->getSpouses();
		std::set<Individual*,compareIndividual>::iterator spouseIt = (*pspouses).begin();
		while(spouseIt != (*pspouses).end()){
			if((*spouseIt)->isOriginalFounder() == true || (*spouseIt)->isOrdinaryFounder() == true){
			
			}else
			if(_descentTrees.size() == 1){
				// Assign the flags:
				if(individual->isConsanguinous() == false || (*spouseIt)->isConsanguinous() == false){
					loopNumber++;
					_descentTrees[0]->setConsanguinity();
					// DEBUG:
					//std::cout << "CONSANGUINITY FOUND: Internal connection between " << (*spouseIt)->getId() << " and " << individual->getId() << " LOOP " << loopNumber << std::endl;
					//std::cout << "RIGHT LOOP " << loopNumber << " : " << (*spouseIt)->getId() << std::endl;
					//std::cout << "LEFT LOOP " << loopNumber << " : " << individual->getId() << std::endl;
					_markLeftLoopFlags(individual,loopNumber);
					_markRightLoopFlags(*spouseIt,loopNumber);
					individual->setIsConsanguinous(true);
					(*spouseIt)->setIsConsanguinous(true);
				}
			}else{
				// There are multiple DTs; determine if the pairing is external or consanguinous
				std::set<unsigned> dt1 = individual->getDescentTrees(); 
				std::set<unsigned> dt2 = (*spouseIt)->getDescentTrees();
				std::set<unsigned> result;
				// Create an insert_iterator for result
				std::insert_iterator<std::set<unsigned> > res_ins(result, result.begin());
				set_intersection(dt1.begin(),dt1.end(),dt2.begin(),dt2.end(),res_ins);
				if(result.size() > 0){
					if(individual->isConsanguinous() == false || (*spouseIt)->isConsanguinous() == false){
						loopNumber++;
						//_markLeftLoopFlags(individual,loopNumber);
						//_markRightLoopFlags(*spouseIt,loopNumber);
						(*spouseIt)->setIsConsanguinous(true);
						individual->setIsConsanguinous(true);
						std::string pairIds;
						if(individual->getGender().getEnum() == Gender::MALE){
							pairIds = individual->getId().get()+(*spouseIt)->getId().get();
							_consangPairIds.insert(pairIds);
						}else{
							pairIds = (*spouseIt)->getId().get()+individual->getId().get();
							_consangPairIds.insert(pairIds);
						}
						// set DT that has consanguinity
						// This can be done using the _getPrimaryDescentTree below:
						unsigned dtIndex = _getPrimaryDescentTreeIndex(dt1,individual,false);
						_descentTrees[dtIndex]->setConsanguinity();
						_dtsHaveConsanguinity=true;
					}
				}else{
					//  To sort the descent trees based on complexity
					//  we need to know how many external connections
					// are there is each DT and with which DT they have maximum connections:
					unsigned dtIndex = _getPrimaryDescentTreeIndex(dt1,individual,true);
					unsigned spouseDTIndex = _getPrimaryDescentTreeIndex(dt2,(*spouseIt),false);
					_descentTrees[dtIndex]->addExternalConnectorPair(individual,(*spouseIt));
					_descentTrees[dtIndex]->incrementConnectionsWithDT(_descentTrees[spouseDTIndex]->getId());
					
				}
			}
			//************
			// Get the children of this pair and recurse:
			//************
			
			const std::set<Individual*,Individual::compareIndividual> * pchildren = individual->getChildren();
			std::set<Individual*,Individual::compareIndividual>::iterator childIt = (*pchildren).begin();
			while(childIt != (*pchildren).end()){
				_markConnectorIndividuals((*childIt),loopNumber);
				++childIt;
			}
			++spouseIt;
		}
	}
	
}

//
// _markConsanguinousIndividuals: This function is called only when there are multiple DTs with consanguinity.
// This function marks all the individuals that are consanguinous.
//
void Pedigree::_markConsanguinousIndividuals(){
	
	unsigned cnt, cnt1;
	unsigned loopNumber=0;
	for(unsigned i=0;i<_descentTrees.size();i++){
		// Get number of individuals in the founding group
		cnt = _descentTrees[i]->getNumberOfFoundingGroupIndividuals();
		cnt1=cnt;
		Individual* startIndividual = _descentTrees[i]->getStartIndividual();
		
		while(cnt > 0){
			cnt--;
			if(cnt > 0){
				const std::set<Individual*,Individual::compareIndividual> * pspouses = startIndividual->getSpouses();
				std::set<Individual*,compareIndividual>::iterator it = (*pspouses).begin();
				while(it != (*pspouses).end()){
					(*it)->setVisited(true);
					std::vector<Individual*> children;
					std::vector<Individual*> sortedChildren;
					startIndividual->getChildrenWithSpouse((*it),children);
					_sortSibsBasedOnExternalConnections(children,sortedChildren);
					unsigned childCnt = 0;
					while(childCnt < sortedChildren.size()){
						if(sortedChildren[childCnt]->getNumberOfSpouses() > 0){
							_markConsanguinousFlags(sortedChildren[childCnt],loopNumber);
						}
						childCnt++;
					}
					cnt--;
					++it;
				}
				startIndividual->setVisited(true);
			}
			// For any remaining founding group member who has not been visited yet
			for(unsigned j=0;j<cnt1;j++){
				Individual* individual = _descentTrees[i]->getFoundingGroupIndividual(j);
				if(individual->hasBeenVisited()) continue;
				startIndividual = individual;
				break;
			}
		}
	}
	// Reset all the hasBeenVisited flags to false 
	std::set<Individual*,compareIndividual>::iterator it;
	it = _individuals.begin();
	while(it != _individuals.end()){
		(*it)->setVisited(false);
		++it;
	}
	
}

//
// _markConsanguinousFlags: This function is called only when there are multiple DTs with consanguinity.
// This function gets all the individuals with consanguinity and propagates the flags up in the descent tree
// These flags determine the sibling sorting order of the NF
//
void Pedigree::_markConsanguinousFlags(Individual* individual,unsigned& loopNumber){
	
	const std::set<Individual*,Individual::compareIndividual> * pspouses = individual->getSpouses();
	std::set<Individual*,compareIndividual>::iterator spouseIt = (*pspouses).begin();
	while(spouseIt != (*pspouses).end()){
		if(individual->hasBeenVisited() && (*spouseIt)->hasBeenVisited()){
			++spouseIt;
			continue;
		}
		if((*spouseIt)->isConsanguinous()){
			//std::cout << " LSL " << individual->getId() << " RSL " << (*spouseIt)->getId() << std::endl;
			loopNumber++;
			// Check if this pair is consanguinous
			std::string pairId;
			if((*spouseIt)->getGender().getEnum() == Gender::MALE) pairId = (*spouseIt)->getId().get()+individual->getId().get();
			else pairId = individual->getId().get()+(*spouseIt)->getId().get();
			std::set<std::string>::iterator pairIt = _consangPairIds.find(pairId);
			if(pairIt != _consangPairIds.end()){
				_markLeftLoopFlags(individual,loopNumber);
				_markRightLoopFlags(*spouseIt,loopNumber);
			}
		}
		(*spouseIt)->setVisited(true);
		
		//************
		// Get the children of this pair and recurse:
		//************
		
		std::vector<Individual*> children;
		std::vector<Individual*> sortedChildren;
		individual->getChildrenWithSpouse((*spouseIt),children);
		_sortSibsBasedOnExternalConnections(children,sortedChildren);
		unsigned childCnt = 0;
		while(childCnt < sortedChildren.size()){
			if(sortedChildren[childCnt]->getNumberOfSpouses() > 0){
				_markConsanguinousFlags(sortedChildren[childCnt],loopNumber);
				
			}
			childCnt++;
		}
		++spouseIt;
	}
	individual->setVisited(true);
	
}

//
// _establishNuclearFamilies:  Determine the nuclear families of the pedigree and store them in a set.
//
void Pedigree::_establishNuclearFamilies(){
	
	std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	while(individualIt != _individuals.end()){
		if(!(*individualIt)->getNumberOfSpouses()) { ++individualIt; continue; }
		// Get the spouses
		const std::set<Individual*,Individual::compareIndividual> * pspouses = (*individualIt)->getSpouses();
		std::set<Individual*,Individual::compareIndividual>::iterator it = (*pspouses).begin();
		while(it != (*pspouses).end()){ 
			if((*individualIt)->getGender().getEnum() == Gender::FEMALE){
				_addNuclearFamily(*individualIt,*it);
			}else{
				_addNuclearFamily(*it,*individualIt);
			}
			++it;
		}
		++individualIt;
	}
	// DEBUG: Display all the nuclear families
	//	(*nfIt)->sortChildren();
	//	++nfIt;
	//}
	
}

// 
// _sortIndividualNuclearFamilies: sort individuals with multiple spouses for cases with consanguinity/external connections:
//
void Pedigree::_sortIndividualNuclearFamilies(){
	
	std::set<Individual*,compareIndividual>::const_iterator it = _individuals.begin();
	while(it != _individuals.end()){
		if((*it)->isOrdinaryFounder()) { ++it; continue; }
		if((*it)->getNumberOfSpouses() > 1){ 
			(*it)->sortSpouses();
		}
		++it;
	}
	
}

//
// _sortNuclearFamilies:
//
void Pedigree::_sortNuclearFamilies(bool consanguinousFlag){
	
	// Sort all the nuclear families based on optimal/classical ordering of children
	// when consanguinity or external connection is found in the descent tree
	
	std::set<NuclearFamily*,compareNuclearFamily>::iterator nfIt = _nuclearFamilies.begin();
	
	while(nfIt != _nuclearFamilies.end()){
		if(_dtsHaveConsanguinity){
			(*nfIt)->sortChildrenInClassicalOrder(true,_descentTrees.size());
		}else{
			(*nfIt)->sortChildrenInClassicalOrder(consanguinousFlag);
		}
		
		++nfIt;
	}
	 
}

//
// _sortNuclearFamiliesBasedOnDataField:
//
void Pedigree::_sortNuclearFamiliesBasedOnDataField(const std::string& name,bool dobSortOrder){
	
	std::set<NuclearFamily*,compareNuclearFamily>::iterator nfIt = _nuclearFamilies.begin();
	
	while(nfIt != _nuclearFamilies.end()){
		(*nfIt)->sortChildrenBasedOnDataField(name,dobSortOrder);
		++nfIt;
	}
	
	
}

//
// _calculateDescentTreeWidth:
//
void Pedigree::_calculateDescentTreeWidth(){
	
	bool classicalOrdering = false;
	// 
	// If the descent tree has a consanguinious loop or an external connection
	// find the optimal sibling ordering for each nuclear family
	//
	if(_descentTrees.size() == 1 && _descentTrees[0]->hasConsanguinity()){
		// After the loop flags are marked and the nuclear families are determined 
		// it might be required that the nuclear families be sorted on individuals
		// with multiple spouses
		
		
		_sortIndividualNuclearFamilies();
		
		// Sort the siblings within each nuclear family based on the loop flags set
		_sortNuclearFamilies(true);
		
		classicalOrdering = true;
	}else{
		if(_descentTrees.size() > 1){
			_sortIndividualNuclearFamilies();
			_sortNuclearFamilies(false);
			classicalOrdering = true;
		}
	}
	
	// Calculate width:
	for(unsigned cnt=0;cnt < _descentTrees.size();cnt++){
		
		Individual* startIndividual = _descentTrees[cnt]->getStartIndividual();
		_calculateWidth(startIndividual,classicalOrdering,cnt);
		
		// Check if there exists any FG member who has > 1 NF 
		for(unsigned i=0;i<_descentTrees[cnt]->getNumberOfFoundingGroupIndividuals();i++){
			Individual* fgIndividual = _descentTrees[cnt]->getFoundingGroupIndividual(i);
			if(fgIndividual->getId() == startIndividual->getId()) continue;
			if(fgIndividual->getNumberOfNuclearFamilies() > 1){
				// Get each NF and check if it has been processed
				for(unsigned j=0;j<fgIndividual->getNumberOfNuclearFamilies();j++){
					if(fgIndividual->getNuclearFamily(j)->getTotalWidth() == 0){
						std::cout << " This FG member " << fgIndividual->getId() << " needs to be processed further" << std::endl;
						fgIndividual->getNuclearFamily(j)->calculateWidth(classicalOrdering);
						_nfOfOrdinaryFounders.push_back(fgIndividual->getNuclearFamily(j));
					}
				}
			}
		}
	}
	
}

//
// _setLeftShifConnectionFlags():
//
void Pedigree::_setLeftShiftConnectionFlags(){
	
	// Need to confirm the Left Connectors and change left and right widths of their parent NFs
	std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	while(individualIt != _individuals.end()){
		if((*individualIt)->getLeftSpouseConnector()){
			//std::cout << " This individual has left spouse connector " << (*individualIt)->getId() << std::endl;
			NuclearFamily* lcnf = (*individualIt)->getNuclearFamily((unsigned)0);
			Individual* spouse = lcnf->getFather();
			Individual* spouseFather = lcnf->getFather()->getFather();
			Individual* spouseMother = lcnf->getFather()->getMother();
			NuclearFamily *spouseParentNF;
			// Check if any one of the parent has multiple mates:
			// Reset the left spouse connector flag in such cases
			if((!spouseFather->isOrdinaryFounder() && spouseFather->getNumberOfSpouses() > 1) || (!spouseMother->isOrdinaryFounder() && spouseMother->getNumberOfSpouses() > 1)){
				(*individualIt)->setLeftSpouseConnector(false);
				++individualIt;
				continue;
			}
			unsigned j=0;
			while(j < spouseFather->getNumberOfSpouses()){
				spouseParentNF = spouseFather->getNuclearFamily(j);
				Individual* mother = spouseParentNF->getMother();
				if(spouseMother->getId() == mother->getId()) break;
				j++;
			}
			NuclearFamily* parentNF;
			// Determine if the father is the last child in his parent's NF
			if(spouseParentNF->getChildInClassicalOrder(spouseParentNF->getNumberOfChildren()-1)->getId() == spouse->getId()){
				// Father is the last child in his parent's NF
				//unsigned leftWidth,rightWidth;
				Individual* father;
				if(lcnf->getTotalWidth() == 4 || lcnf->getNumberOfChildren() == 1){
					// set the left connection shift flag on the parents NF
					father = (*individualIt)->getFather();
					j = 0;
					while(j < father->getNumberOfSpouses()){
						parentNF = father->getNuclearFamily(j);
						Individual* mother = parentNF->getMother();
						if(mother->getId() == (*individualIt)->getMother()->getId()) break;
						j++;
					}
					if(parentNF->getNumberOfChildren() > 1){
						parentNF->setLeftConnectionShiftFlag(true);
					}
				}
			}else{
				// 
				// Father is not the last child in his parent's NF. 
				// The left connector flag is not reset now but will be checked again by NF
				// If the individual's parentNF is consanguinous there might be a need for shifting.
				// Same case holds when the individual's NF itself is consanguinous
				// INCOMPLETE: This has only been tested for c2.txt. It has to be thoroughly tested.
				Individual* father = (*individualIt)->getFather();
				Individual* mother;
				j = 0;
				while(j < father->getNumberOfSpouses()){
					parentNF = father->getNuclearFamily(j);
					mother = parentNF->getMother();
					if(mother->getId() == (*individualIt)->getMother()->getId()) break;
					j++;
				}
				if(parentNF->getNumberOfChildren() > 1 && (parentNF->isConsanguinous() || parentNF->hasExternalConnection())){
					parentNF->setLeftConnectionShiftFlag(true);
				}else{
					if(spouseParentNF->hasChild(father) || spouseParentNF->hasChild(mother) && parentNF->getNumberOfChildren() <= 2){
						parentNF->setLeftConnectionShiftFlag(true);
					}
				}
			}
		}
		++individualIt;
	}
	
}

//
// _calculateWidth:
//
void Pedigree::_calculateWidth(Individual* individual,bool classicalOrdering,unsigned descentTreeIndex){
	
	unsigned sumLeftWidth = 0,sumRightWidth=0;
	// For each nuclear family of the start individual calculate width
	for(unsigned j=0;j<individual->getNumberOfNuclearFamilies();j++){
		individual->getNuclearFamily(j)->calculateWidth(classicalOrdering);
		if(j % 2 == 0){
			sumRightWidth += individual->getNuclearFamily(j)->getTotalWidth();
			if(j == 0){
				unsigned offset = individual->getNuclearFamily(j)->getLeftWidth();
				sumRightWidth -= offset;
				sumLeftWidth += offset;
			}
		}else{
			sumLeftWidth += individual->getNuclearFamily(j)->getTotalWidth();
		}
	}
	//std::cout << "LEFT Width of the START Individual is " << sumLeftWidth << std::endl;
	//std::cout << "RIGHT Width of the START Individual is " << sumRightWidth << std::endl;
	
	//
	// The total ,left and right widths have to be stored in the start Individual
	// only if the startIndividual has more than one NF
	// Assign the widths to the descent trees
	//
	_descentTrees[descentTreeIndex]->setLeftWidth(sumLeftWidth);
	_descentTrees[descentTreeIndex]->setRightWidth(sumRightWidth);
	_descentTrees[descentTreeIndex]->setTotalWidth(sumLeftWidth+sumRightWidth);
	
	//std::cout << "****************************" << std::endl;
	//std::cout << "END OF WIDTH CALCULATION" << std::endl;
	//std::cout << "****************************" << std::endl;
	return;
	
}

//
// _populateIndividualGrid
//
void Pedigree::_populateIndividualGrid(){
	
	std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	
	individualIt = _individuals.begin();
	while(individualIt != _individuals.end()){
		_individualGrid.insert(int((*individualIt)->getX()),int((*individualIt)->getY()),(*individualIt));
		++individualIt;
	}
	
	
}

//
// _hasIndividualAtPosition
//
bool Pedigree::_hasIndividualAtPosition(Individual* start,Individual* end){
	
	double xstart = start->getX();
	double ystart = start->getY();
	double xend   = end->getX();
	double yend   = end->getY();
	double horizontalInterval = DrawingMetrics::getHorizontalInterval();
	if(ystart != yend) { 
		std::cout << " Trying a new approach " << start->getId() << " and " << end->getId() << std::endl;
                xstart += horizontalInterval;
                int ystarti = int(ystart);
                xend -= horizontalInterval;
                if(xstart >= xend) return true;
                while(xstart < xend){
                        Individual* found = _individualGrid.find(int(xstart),ystarti);
                        if(found){ return true; }
                        xstart+= horizontalInterval;
                }
		return false;
		//return true; 
		
	}
	xstart += horizontalInterval;
	int ystarti=int(ystart);
	if(xstart >= xend) return true;
	while(xstart < xend){
		Individual* found = _individualGrid.find(int(xstart),ystarti);
		if(found){ return true; }
		xstart+= horizontalInterval;
		
	}
	return false;
	
}

//
// _sortSibsBasedOnExternalConnections:
//
void Pedigree::_sortSibsBasedOnExternalConnections(const std::vector<Individual*>& sibs,std::vector<Individual*>& sortedSibs){
	
	std::deque<Individual*> left;
	std::deque<Individual*> right;
	std::deque<Individual*> initial;
	
	// If there is only one sib return:
	if(sibs.size() == 1){
		sortedSibs.push_back(sibs.front());
		return;
	}
	
	// Since we are checking for EXT connections consang flag is false
	Individual::groupIndividualsBasedOn(false,sibs,initial,left,right);
	// Merge the Qs
	// push all the left loop individuals to the right of result
	while(!left.empty()){
		initial.push_back(left.front());
		left.pop_front();
	}
	// merge the remaining individuals on the right loop to the left of result
	bool flag;
	while(!right.empty()){
		flag = true;
		for(unsigned j=0;j<initial.size();j++){
			
			if(initial[j]->getId() == right.back()->getId()){
				right.pop_back();
				flag=false;
				break;
			}
		}
		if(flag){ initial.push_front(right.back()); right.pop_back(); }
	}
	std::vector<std::string> idsSortedByExternalConnection;
	while(!initial.empty()){
		sortedSibs.push_back(initial.front());
		idsSortedByExternalConnection.push_back(initial.front()->getId().get());
		initial.pop_front();
	}
	// Set this sorting on the parents:
	Individual* father = sortedSibs.front()->getFather();
	Individual* mother = sortedSibs.front()->getMother();
	std::string parentPair = father->getId().get()+mother->getId().get();
	//mother->setChildrenIdsSortedByExternalConnections(parentPair,idsSortedByExternalConnection);
	mother->setChildrenIdsSortedByExternalConnections(parentPair,sortedSibs);
	
}

//
// _drawConsanguinityLetter: For dashed individuals drawn for consanguinous and external connections
// assign a unique label to each unique individual
//
void Pedigree::_drawConsanguinityLetter(Individual* mother,Individual* father,char& consanguinityLetter,double iconInterval,double iconDiameter, std::map<std::string,std::string>& individualConsanguinityLetter,DrawingCanvas& dc,double multipleSpouseOffset,bool leftConnector){
	
	double yOffset = DrawingMetrics::getIconRadius()  +
	                 DrawingMetrics::getLabelMargin() + 
	                 DrawingMetrics::getYMaximum();
	double offset = iconInterval;
	if(leftConnector) offset *= -1;
	if(multipleSpouseOffset != 0) offset = multipleSpouseOffset;
	std::map<std::string,std::string>::iterator it = individualConsanguinityLetter.find(father->getId().get());
	if(it != individualConsanguinityLetter.end())
		dc.drawText(mother->getX()+offset,mother->getY()+yOffset,it->second);
	else{
		std::stringstream buf;
		buf << consanguinityLetter;
		individualConsanguinityLetter.insert(std::map<std::string,std::string>::value_type(father->getId().get(),buf.str()));
		dc.drawText(mother->getX()+offset,mother->getY()+yOffset,buf.str());
		dc.drawText(father->getX(),father->getY()+yOffset,buf.str());
		consanguinityLetter++;
	}
	
}

//
// _drawHorizontalConnectorLine:
//
void Pedigree::_drawHorizontalConnectorLine(double y,double x1,double x2,bool isConsanguinous,DrawingCanvas& dc){
	
	double verticalTick = DrawingMetrics::getVerticalTick();
	if(isConsanguinous) y += verticalTick/2;
	dc.drawHorizontalLine(y,x1,x2);
	if(isConsanguinous)
		dc.drawHorizontalLine(y-DrawingMetrics::getVerticalTick(),x1,x2);
	
}

//
// _drawVerticalConnectorLine:
//
void Pedigree::_drawVerticalConnectorLine(double motherY,double fatherY,double motherX,double fatherX,bool isConsanguinous,DrawingCanvas& dc,double multipleSpouseOffset,bool singleChild){
	
	double verticalTick = DrawingMetrics::getVerticalTick();
	double radius = DrawingMetrics::getIconRadius();
	double motherOffset,fatherOffset;
	motherOffset = fatherOffset = -1*verticalTick;
	double extension;

	if(isConsanguinous){
		motherY += verticalTick/2;
		fatherY += verticalTick/2;
	}
	if(fatherX > motherX){ 
		//extension = DrawingMetrics::getHorizontalInterval()*-1;
		//extension = (DrawingMetrics::getHorizontalInterval()+DrawingMetrics::getHorizontalInterval()/4)*-1;
		extension = (DrawingMetrics::getHorizontalInterval() - radius - verticalTick) * -1;
		if(fatherY > motherY){ extension += -verticalTick; motherOffset*=-1;fatherOffset*=-1;} 
		if(multipleSpouseOffset) multipleSpouseOffset = multipleSpouseOffset + extension; 
	}else{
		//extension = DrawingMetrics::getHorizontalInterval() - verticalTick;
		//extension = DrawingMetrics::getHorizontalInterval()+DrawingMetrics::getHorizontalInterval()/4;
		extension = DrawingMetrics::getHorizontalInterval() - radius - verticalTick; 
		if(singleChild){
			extension += DrawingMetrics::getHorizontalInterval();
		}
		if(fatherY > motherY){ 
			extension += verticalTick; 
		}else { 
			fatherOffset *= -1; motherOffset *= -1;
		}
		if(multipleSpouseOffset) multipleSpouseOffset = multipleSpouseOffset + extension; 
	}
	
	dc.drawHorizontalLine(motherY,motherX,fatherX+extension-multipleSpouseOffset);
	dc.drawHorizontalLine(fatherY,fatherX,fatherX+extension-multipleSpouseOffset);
	dc.drawVerticalLine(fatherX+extension-multipleSpouseOffset,motherY,fatherY);
	if(isConsanguinous){
		dc.drawHorizontalLine(motherY-verticalTick,motherX,fatherX+extension-multipleSpouseOffset+motherOffset);
		dc.drawHorizontalLine(fatherY-verticalTick,fatherX,fatherX+extension-multipleSpouseOffset+fatherOffset);
		dc.drawVerticalLine(fatherX+extension-multipleSpouseOffset+fatherOffset,motherY-verticalTick,fatherY-verticalTick);
	}
	
}

//
// _drawConsanguinousConnectors:
//
void Pedigree::_drawConsanguinousConnectors(DrawingCanvas& dc){
	
	std::set<NuclearFamily*,compareNuclearFamily>::iterator nfIt = _nuclearFamilies.begin();
	//double fatherX,motherX;
	Individual* mother,*father;
	// Note: All the connectors are drawn on a different layer
	// However all the dashed individuals go into the body
	char consanguinityLetter='A';
	// For each individual with consanguinity and multiple spouses we assign
	// a unique letter which is displayed below each dashed icon of the individual
	// This map stores the individualId and the letter associated 
	std::map<std::string,std::string> individualConsanguinityLetter;
	
	double horizontalInterval = DrawingMetrics::getHorizontalInterval();
	double iconInterval = DrawingMetrics::getIconInterval();
	double iconDiameter = DrawingMetrics::getIconDiameter();
	double radius = (iconDiameter/2);
	
	while(nfIt != _nuclearFamilies.end()){
		if((*nfIt)->isConsanguinous() || (*nfIt)->hasExternalConnection()){
			// Check for different conditions
			father = (*nfIt)->getFather();
			mother = (*nfIt)->getMother();
			
			//DEBUG:
			//std::cout << "FOUND A CONSANG NF" << std::endl;
			//std::cout << "Mother " << mother->getId() << " x " << mother->getX() << std::endl;
			//std::cout << "Father " << father->getId() << " x " << father->getX() << std::endl;
			//std::cout << "NF LCSF " << (*nfIt)->getLeftConnectionShiftFlag() << std::endl;
			if((*nfIt)->getMother()->getNumberOfSpouses() == 1){
				// Father is to the Left of Mother
				if(mother->getLeftSpouseConnector()){
					if(fabs((father->getX()+(*nfIt)->getLeftWidth()*horizontalInterval+iconInterval)-mother->getX()) < Number::MINIMUM_DIFFERENCE || (fabs((father->getX()+(*nfIt)->getLeftWidth()*horizontalInterval+iconInterval+horizontalInterval)-mother->getX()) < Number::MINIMUM_DIFFERENCE)){
						// Check the vertical coordinate
						if(mother->getY() == father->getY()){
							_drawHorizontalConnectorLine(mother->getY(),mother->getX()-iconInterval+radius,father->getX()+radius,(*nfIt)->isConsanguinous(),dc);
						}else{
							// Check if father is the only child
							bool singleChild=false;
							std::vector<std::string> temp = father->getMother()->getChildrenIds(father->getFather());
							if(temp.size() == 1) singleChild = true;
							// Draw a vertical line
							_drawVerticalConnectorLine(mother->getY(),father->getY(),mother->getX()-iconInterval+radius,father->getX()+radius,(*nfIt)->isConsanguinous(),dc,0.0,singleChild);
						}
					}else{
						// std::cout << "LEFT connector: Draw dashed individual" << std::endl;
						if(mother->getY() != father->getY() || _hasIndividualAtPosition(father,mother)){
							//dc.drawDashedIndividual(father,mother->getX()-iconInterval,mother->getY(),iconDiameter);
							dc.drawIndividual(father,mother->getX()-iconInterval,mother->getY(),true);
							_drawConsanguinityLetter(mother,father,consanguinityLetter,iconInterval,iconDiameter,individualConsanguinityLetter,dc,0,true);
						}else{
							_drawHorizontalConnectorLine(mother->getY(),father->getX()+radius,mother->getX()-radius,(*nfIt)->isConsanguinous(),dc);
						}
					}
				}else
				if(father->getX() < mother->getX()){
					//dc.drawDashedIndividual(father,mother->getX()+iconInterval,mother->getY(),iconDiameter);
					dc.drawIndividual(father,mother->getX()+iconInterval,mother->getY(),true);
					_drawConsanguinityLetter(mother,father,consanguinityLetter,iconInterval,iconDiameter,individualConsanguinityLetter,dc);
				}else
				if((fabs((mother->getX()+(*nfIt)->getRightWidth()*horizontalInterval+iconInterval)-father->getX()) < Number::MINIMUM_DIFFERENCE) || 
				   ((*nfIt)->getLeftConnectionShiftFlag() && fabs((mother->getX()+(*nfIt)->getRightWidth()*horizontalInterval+horizontalInterval)-father->getX()) < Number::MINIMUM_DIFFERENCE)){
					// Check the vertical coordinate
					if(mother->getY() == father->getY()){
						_drawHorizontalConnectorLine(mother->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc);
					}else{
						// Draw a vertical line
						double multipleSpouseOffset = 0.0;
						if(father->getNumberOfNuclearFamilies() > 1)
							multipleSpouseOffset = father->getLeftWidth()*horizontalInterval - horizontalInterval;
						_drawVerticalConnectorLine(mother->getY(),father->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc,multipleSpouseOffset);
					}
				}else{
					// Check if the father is the only child
					std::vector<std::string> temp = father->getMother()->getChildrenIds(father->getFather());
					if(temp.size() == 1 && fabs((mother->getX()+(*nfIt)->getRightWidth()*horizontalInterval+iconInterval)+horizontalInterval-father->getX()) < Number::MINIMUM_DIFFERENCE){
						// Check the vertical coordinate
						if(mother->getY() == father->getY()){
							_drawHorizontalConnectorLine(mother->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc);
						}else{
							// Draw a vertical line
							double multipleSpouseOffset = 0.0;
							if(father->getNumberOfNuclearFamilies() > 1){
								multipleSpouseOffset = father->getLeftWidth()*horizontalInterval - horizontalInterval;
								if(multipleSpouseOffset == 0) multipleSpouseOffset = iconInterval;
							}
							_drawVerticalConnectorLine(mother->getY(),father->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc,multipleSpouseOffset);
						}
					}else
					// Cases in which the father has <= 3NFs and the first NF has a totalWidth > 4 is not detected by the previous case;
					if(father->getNumberOfNuclearFamilies() > 1 && father->getNumberOfNuclearFamilies() <= 3 &&  fabs((mother->getX()+((*nfIt)->getRightWidth()+father->getNuclearFamily(unsigned(0))->getLeftWidth())*horizontalInterval) - father->getX()) < Number::MINIMUM_DIFFERENCE){
						// If there is more than 1 NF on the left side of the father it is possible
						// that the line connecting the spouse may go through the children with other spouses
						// To avoid this we have a limit of 1 NF on the LHS ie total of 3 NFs 
						if(mother->getY() == father->getY()){
							_drawHorizontalConnectorLine(mother->getY(),mother->getX()+(*nfIt)->getRightWidth()*horizontalInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc);
						}else{
							double multipleSpouseOffset = father->getLeftWidth()*horizontalInterval-horizontalInterval;
							_drawVerticalConnectorLine(mother->getY(),father->getY(),mother->getX()+horizontalInterval,father->getX()-radius,(*nfIt)->isConsanguinous(),dc,multipleSpouseOffset);
						}
					}else{
						if(_hasIndividualAtPosition(mother,father)){
							//dc.drawDashedIndividual(father,mother->getX()+iconInterval,mother->getY(),iconDiameter);
							dc.drawIndividual(father,mother->getX()+iconInterval,mother->getY(),true);
							_drawConsanguinityLetter(mother,father,consanguinityLetter,iconInterval,iconDiameter,individualConsanguinityLetter,dc);
						}else{
							if(mother->getY() == father->getY()){
								_drawHorizontalConnectorLine(mother->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc); 
							}else{
								_drawVerticalConnectorLine(mother->getY(),father->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc);
							}
						}
					}
				}
			}else{
				//
				// The female has multiple spouses
				//
				// Get the widths of the individual NF for the spouse
				unsigned lw=0,rw=0,lastlw=0,lastrw=0,i;
				for(i=0;i<mother->getNumberOfNuclearFamilies();i++){
					NuclearFamily* nf = mother->getNuclearFamily(i);
					if(nf == *nfIt){
						if(i % 2 == 0){ rw+= nf->getLeftWidth(); if(i != 0) lastrw = nf->getRightWidth(); }
						else{ lw += nf->getRightWidth(); lastlw = nf->getLeftWidth(); }
						break;
					}
					if(i % 2 == 0) rw+= nf->getTotalWidth();
					else lw += nf->getTotalWidth();
					
				}
				if(i > 0){
					NuclearFamily* nf = mother->getNuclearFamily((unsigned)0);
					lw += nf->getLeftWidth();
					rw -= nf->getRightWidth();
				}
				
				if(i % 2 == 0){
					if(fabs((mother->getX()+(rw+lastrw)*horizontalInterval+iconInterval)-father->getX()) < Number::MINIMUM_DIFFERENCE){
						if(mother->getY() == father->getY()){
							// Horizontal line
							if(i == 0){
								_drawHorizontalConnectorLine(mother->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc);
							}else{
								_drawHorizontalConnectorLine(mother->getY(),mother->getX()+rw*horizontalInterval+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc);
							}
						}else{
							// Vertical line
							if(i==0){
								_drawVerticalConnectorLine(mother->getY(),father->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc);
							}else{
								_drawVerticalConnectorLine(mother->getY(),father->getY(),mother->getX()+rw*horizontalInterval+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc);
							}
						}
					}else{
						if(i==0){
							//dc.drawDashedIndividual(father,mother->getX()+iconInterval,mother->getY(),iconDiameter);
							dc.drawIndividual(father,mother->getX()+iconInterval,mother->getY(),true);
							_drawConsanguinityLetter(mother,father,consanguinityLetter,iconInterval,iconDiameter,individualConsanguinityLetter,dc);
						}else{
							//dc.drawDashedIndividual(father,mother->getX()+rw*horizontalInterval+iconInterval,mother->getY(),iconDiameter);
							dc.drawIndividual(father,mother->getX()+rw*horizontalInterval+iconInterval,mother->getY(),true);
							_drawConsanguinityLetter(mother,father,consanguinityLetter,iconInterval,iconDiameter,individualConsanguinityLetter,dc,rw*horizontalInterval+iconInterval);
						}
					}
				}else{
					if(fabs((mother->getX()-(lw+lastlw)*horizontalInterval)-father->getX()) < Number::MINIMUM_DIFFERENCE){
						
						if(mother->getY() == father->getY()){
							// Horizontal line
							_drawHorizontalConnectorLine(mother->getY(),father->getX()+radius,mother->getX()-(lw)*horizontalInterval+radius,(*nfIt)->isConsanguinous(),dc);
						}else{
							// Vertical line
							_drawVerticalConnectorLine(mother->getY(),father->getY(),father->getX()+iconInterval+radius,father->getX()+radius,(*nfIt)->isConsanguinous(),dc);
						}
					
					}else{
						if(mother->getY() != father->getY() || _hasIndividualAtPosition(father,mother)){
							//dc.drawDashedIndividual(father,mother->getX()-lw*horizontalInterval,mother->getY(),iconDiameter);
							dc.drawIndividual(father,mother->getX()-lw*horizontalInterval,mother->getY(),true);
							_drawConsanguinityLetter(mother,father,consanguinityLetter,iconInterval,iconDiameter,individualConsanguinityLetter,dc,lw*horizontalInterval*(-1.0));
						}else{
							_drawHorizontalConnectorLine(mother->getY(),mother->getX()-(lw+lastlw)*horizontalInterval,mother->getX()-(lw+lastlw-2)*horizontalInterval+radius,(*nfIt)->isConsanguinous(),dc);
						}
					}
				}
			}
		}
		++nfIt;
	}
	
}


//********************************
//
// Public methods:
//
//********************************

//
// addIndividual:
//
void Pedigree::addIndividual(const std::string ind,std::string mother,std::string father, std::string gender, int rowIndex, int tableIndex ,const DataTable& pedigreeTable) {
	
	// for warnings:
	const char *methodName="Pedigree::addIndiidual()";
	if(ind == "."){
		Warning(methodName,"Id is missing for individual with father %s and mother %s. This individual will be ignored.",father.c_str(),mother.c_str());
		return;
	}
	if(mother != "." && mother != ind && mother == father){
		//
		// check if the mother/father is already in the pedigree
		// If yes, check for gender and set the other parent to missing
		// else change the gender of one of the parent to missing
		//
		Individual* tempIndividual = new Individual(mother);
		std::set<Individual*,compareIndividual>::const_iterator it = _individuals.find(tempIndividual);
		delete tempIndividual;
		if(it != _individuals.end()){
			if((*it)->getGender().getEnum() == Gender::MALE){
				mother = ".";
				Warning(methodName,"Both mother and father have the same id. Mother of %s set to Missing.",ind.c_str());
			}else{
				father = ".";
				Warning(methodName,"Both mother and father have the same id. Father of %s set to Missing.",ind.c_str());
			}
		}else{
			throw Exception("addIndividual()","Both mother and father of %s have the same id. For founders use '.' as the id.",ind.c_str());
		}
	}else if(ind == mother || ind == father){
		// If the mother/father id is the same as the individual
		// set the conflicting parent to missing
		if(ind == mother){
			Warning(methodName,"Mother and Individual Ids for %s are the same. Set Mother to MISSING.",mother.c_str());
			mother = ".";
		}
		if(ind == father){
			Warning(methodName,"Father and Individual Ids for %s are the same. Set Father to MISSING.",father.c_str());
			father = ".";
		}
	}
	std::pair<std::set<Individual*,compareIndividual>::iterator,bool> iit;
	iit =  _individuals.insert(new Individual(ind,mother,father,gender,rowIndex,tableIndex)); 
	if(iit.second)
		(*iit.first)->setPedigreeDataTable(&pedigreeTable);
	
}

//
// setCoreOptionalFields:
//
void Pedigree::setCoreOptionalFields(const DataTable* pedigreeTable){
	
	bool hasDOBColumn = false;
	bool hasDeceasedColumn = false;
	bool hasSampledColumn = false;
	bool hasProbandColumn = false;
	bool hasAffectedColumn = false;
	
	DataColumn *dobColumn;
	DataColumn *deceasedColumn;
	DataColumn *sampledColumn;
	DataColumn *probandColumn;
	DataColumn *affectedColumn;
	
	// Check if MZTwin column exists in the datatable
	if(pedigreeTable->getMZTwinColumnIndex() != DataTable::COLUMN_IS_MISSING){
		DataColumn* mzTwinColumn = pedigreeTable->getColumn(pedigreeTable->getMZTwinColumnIndex());
		_setIndividualTwinField(mzTwinColumn,'M');
	}
	
	// Check if DZTwin column exists in the datatable
	if(pedigreeTable->getDZTwinColumnIndex() != DataTable::COLUMN_IS_MISSING){
		DataColumn* dzTwinColumn = pedigreeTable->getColumn(pedigreeTable->getDZTwinColumnIndex());
		_setIndividualTwinField(dzTwinColumn,'D');
	}
	
	// Check if DOB column exists in the datatable
	if(pedigreeTable->getDOBColumnIndex() != DataTable::COLUMN_IS_MISSING){
		dobColumn = pedigreeTable->getColumn(pedigreeTable->getDOBColumnIndex());
		hasDOBColumn = true;
	}
	
	// Check if Deceased column exists in the datatable
	if(pedigreeTable->getDeceasedColumnIndex() != DataTable::COLUMN_IS_MISSING){
		deceasedColumn = pedigreeTable->getColumn(pedigreeTable->getDeceasedColumnIndex());
		hasDeceasedColumn = true;
	}
	
	// Check if Sampled column exists in the datatable
	if(pedigreeTable->getSampledColumnIndex() != DataTable::COLUMN_IS_MISSING){
		sampledColumn = pedigreeTable->getColumn(pedigreeTable->getSampledColumnIndex());
		hasSampledColumn = true;
		Individual::setSampledColumnPresent();
	}
	
	// Check if Proband column exists in the datatable
	if(pedigreeTable->getProbandColumnIndex() != DataTable::COLUMN_IS_MISSING){
		probandColumn = pedigreeTable->getColumn(pedigreeTable->getProbandColumnIndex());
		hasProbandColumn = true;
	}
	
	// Check if Affected column exists in the datatable
	if(pedigreeTable->getAffectedColumnIndex() != DataTable::COLUMN_IS_MISSING){
		affectedColumn = pedigreeTable->getColumn(pedigreeTable->getAffectedColumnIndex());
		hasAffectedColumn = true;
	}
	
	unsigned rowIndex;
	std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	
	while(individualIt != _individuals.end()){
		rowIndex = (*individualIt)->getRowIndex();
		if(hasDOBColumn) (*individualIt)->setDOB(dynamic_cast<Date*>(const_cast<Data* const>(dobColumn->getDataAtIndex(rowIndex))));
		if(hasDeceasedColumn) (*individualIt)->setDeceasedStatus(dynamic_cast<LivingDead*>(const_cast<Data* const>(deceasedColumn->getDataAtIndex(rowIndex))));
		if(hasSampledColumn) (*individualIt)->setSampled(dynamic_cast<Sampled*>(const_cast<Data* const>(sampledColumn->getDataAtIndex(rowIndex))));
		if(hasProbandColumn) (*individualIt)->setProbandStatus(dynamic_cast<Proband*>(const_cast<Data* const>(probandColumn->getDataAtIndex(rowIndex))));
		if(hasAffectedColumn) (*individualIt)->setAffectionStatus(dynamic_cast<Affected*>(const_cast<Data* const>(affectedColumn->getDataAtIndex(rowIndex))));
		++individualIt;
	}
	
	
}

//
// checkParentChildDOB:
//
void Pedigree::checkParentChildDOB(){
	
	// for warnings:
	const char *methodName="Pedigree::checkParentChildDOB()";
	
	Individual* mother;
	Individual* father;
	std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	while(individualIt != _individuals.end()){
		if((*individualIt)->isOrdinaryFounder() == true || (*individualIt)->isVirtual() || (*individualIt)->getDOB()->isMissing()) { ++individualIt; continue; }
		mother = (*individualIt)->getMother();
		if(mother->getDOB() == 0){ ++individualIt; continue; }
		if(mother){
			if(mother->getDOB()->isMissing()) { ++individualIt; continue; }
			Number ageDifference((*(*individualIt)->getDOB() - *mother->getDOB())/Date::getDaysInYear()); 
			if(ageDifference < Number(15))
				Warning(methodName,"Age difference between the mother %s and child %s is less than 15 years.",mother->getId().get().c_str(),(*individualIt)->getId().get().c_str());
		}
		father = (*individualIt)->getFather();
		if(father->getDOB() == 0){ ++individualIt; continue; }
		if(father){
			if(father->getDOB()->isMissing()) { ++individualIt; continue; }
			Number ageDifference((*(*individualIt)->getDOB() - *father->getDOB())/Date::getDaysInYear()); 
			if(ageDifference < Number(15))
				Warning(methodName,"Age difference between the father %s and child %s is less than 15 years.",father->getId().get().c_str(),(*individualIt)->getId().get().c_str());
		}
		++individualIt;
	}
	
}

//
// determineFoundingGroups:
//
void Pedigree::determineFoundingGroups(){
	
	// for warnings:
	const char *methodName="Pedigree::determineFoundingGroups()";
	
std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	
	// Determine the number of Descent Trees and founding groups:
	std::vector<Individual*> ordinaryFounders;
	unsigned descentTreeCount = 0;
	//
	// Push all the ordinary founders in a vector
	//
	individualIt = _individuals.begin();
	while(individualIt != _individuals.end()){
		if((*individualIt)->isOrdinaryFounder() == true){ 
			//std::cout << "ORD " << (*individualIt)->getId() << std::endl;
			ordinaryFounders.push_back((*individualIt));
			// Check for unconnected people:
			if((*individualIt)->getNumberOfSpouses() == 0 && (*individualIt)->getNumberOfChildren() == 0){
				(*individualIt)->setIsUnconnected(true);
				std::string indid = (*individualIt)->getId().get();
				Warning(methodName,"Individual %s is Unconnected.",indid.c_str());
				(*individualIt)->setOrdinaryFounder(false);
				ordinaryFounders.pop_back();
				if(_individuals.size() == 1){
					Warning(methodName,"There is only 1 Unconnected individual %s in Pedigree %s.",indid.c_str(),_id.c_str()); 
					Warning(methodName,"Pedigree %s will not be drawn.",_id.c_str());
				}
			}
		}
		++individualIt;
	}
	
	while(ordinaryFounders.size() != 0){
		Individual* temp = ordinaryFounders[0];
		std::set<Individual*,compareIndividual> foundingGroup;
		foundingGroup.insert(temp);
		_getSpouses(foundingGroup,temp);
		bool originalFounderGroup = true;
		std::set<Individual*,compareIndividual>::iterator it = foundingGroup.begin();
		while(it != foundingGroup.end()){
			if((*it)->isOrdinaryFounder() == true){
				++it; continue;
			}
			originalFounderGroup = false;
			break;
		}
		if(originalFounderGroup){
			// Set the originalFounder flag
			descentTreeCount++;
			// add to the list of Descent Trees:
			_addDescentTree(descentTreeCount);
			DescentTree* dt = _descentTrees.back();
			it = foundingGroup.begin();
			while(it != foundingGroup.end()){
				(*it)->setOrdinaryFounder(false);
				(*it)->setOriginalFounder(true);
				// add the individual to the descent Tree founding group:
				dt->addIndividualToFoundingGroup(*it);
				++it;
			}
		}
		std::vector<Individual*>::iterator vit;
		it = foundingGroup.begin();
		while(it != foundingGroup.end()){
			// Delete these individuals from the ordinaryFounders vector
			vit = find(ordinaryFounders.begin(),ordinaryFounders.end(),*it);
			if (vit != ordinaryFounders.end()) ordinaryFounders.erase(vit);
			++it;
		}
	}
	
	// DEBUG: Print the original and ordinary founders:
	/*individualIt = _individuals.begin();
	while(individualIt != _individuals.end()){
		if((*individualIt)->isOrdinaryFounder() == true){ 
			std::cout << "ORD " << (*individualIt)->getId() << std::endl;
		}
		if((*individualIt)->isOriginalFounder() == true){ 
			std::cout << "ORG*** " << (*individualIt)->getId() << std::endl;
		}
		++individualIt;
	}
	
	// DEBUG: Print all the descent tree ids:
	std::cout << "DESCENT TREES " << _descentTrees.size() << std::endl;
	
	for(unsigned cnt =0;cnt < _descentTrees.size();cnt++){
		std::cout << _descentTrees[cnt]->getId() << std::endl;
		_descentTrees[cnt]->displayFoundingGroup();
	}
	*/
	return;
	
}

//
// computePedigreeWidth:
//
void Pedigree::computePedigreeWidth(const std::string& sortField,bool dobSortOrder){
	
	// Determine external and internal connectors:
	_determineConnectorIndividuals();
	
	//determineConnectorIndividualsOld();
	// Establish nuclear families for width calculation:
	_establishNuclearFamilies();
	
	if(sortField != std::string("")){
		_sortNuclearFamiliesBasedOnDataField(sortField,dobSortOrder);
	}
	
	// Calculate descent tree width
	_calculateDescentTreeWidth();
	// set left spouse connections flags
	_setLeftShiftConnectionFlags();
	
}

//
// draw:
//
void Pedigree::draw(const LabelSet* labelSet){
	
	
	// Instantiate a drawing canvas:
	DrawingCanvas dc(labelSet,"XGA"); 
	
	double horizontalInterval = DrawingMetrics::getHorizontalInterval();
	bool classicalOrdering = false;
	double startX,startY=50.0;
	
	
	// If there is only one individual in a Pedigree and is UNCONNECTED return:
	if(_descentTrees.size() == 0){ return; }
	
	
	// Start the drawing with the group tag
	dc.startGroup("drawing");
	if(_descentTrees.size() == 1){
		startX = _descentTrees[0]->getLeftWidth()*horizontalInterval;
		if(startX < dc.getWidth()) startX = dc.getWidth()/2;
		else startX -= dc.getWidth()/2;
		if(_descentTrees[0]->hasConsanguinity()) classicalOrdering = true;
		dc.drawText(startX+horizontalInterval,startY-20,_id,"header");
	}else{
		// get the LW of the first ordered descent tree
		startX = _descentTrees[0]->getLeftWidth()* horizontalInterval;
		classicalOrdering = true;
		dc.drawText(dc.getWidth()/2,startY-20,_id,"header");
	}
	
	
	for(unsigned cnt=0;cnt < _descentTrees.size();cnt++){
		
		Individual* startIndividual = _descentTrees[cnt]->getStartIndividual();
		
		if(startIndividual->getNumberOfNuclearFamilies() > 1){
			double xl = startX - (startIndividual->getNuclearFamily((unsigned)0)->getLeftWidth()) * horizontalInterval;
			double xr = startX;
			Individual* spouse;
			for(unsigned j=0;j<startIndividual->getNumberOfNuclearFamilies();j++){
				if(j % 2 == 0){
					if(j != 0) xr += startIndividual->getNuclearFamily(j)->getLeftWidth() * horizontalInterval;
					startIndividual->getNuclearFamily(j)->draw(startIndividual,dc,xr,startY,classicalOrdering);
					xr += startIndividual->getNuclearFamily(j)->getRightWidth() * horizontalInterval;
				}else{
					xl -= (startIndividual->getNuclearFamily(j)->getRightWidth()) * horizontalInterval;
					
					if(startIndividual->getId() != startIndividual->getNuclearFamily(j)->getMother()->getId())
						spouse = startIndividual->getNuclearFamily(j)->getMother();
					else 
						spouse = startIndividual->getNuclearFamily(j)->getFather();
					startIndividual->getNuclearFamily(j)->draw(spouse,dc,xl,startY,classicalOrdering);
					xl -= (startIndividual->getNuclearFamily(j)->getLeftWidth()) * horizontalInterval;
				}
			}
			if(startIndividual->getNumberOfNuclearFamilies() >= 2){
				double iconDiameter = DrawingMetrics::getIconDiameter();
				double iconInterval = DrawingMetrics::getIconInterval();
				startIndividual->getNuclearFamily((unsigned)0)->drawSpouseConnectors(startIndividual,horizontalInterval,iconInterval,iconDiameter,dc);
			}
			if(cnt+1 < _descentTrees.size()){
				startX += (_descentTrees[cnt]->getRightWidth() + _descentTrees[cnt+1]->getLeftWidth()) * horizontalInterval;
			}
		}else{
			startIndividual->getNuclearFamily((unsigned)0)->draw(startIndividual,dc,startX,startY,classicalOrdering);
			if(cnt+1 < _descentTrees.size()){
				startX += (_descentTrees[cnt]->getRightWidth() + _descentTrees[cnt+1]->getLeftWidth()) * horizontalInterval;
			}
		}
	}
	
	_populateIndividualGrid();
	
	// NOTE: It is possible for original/ordinary founders to have multiple spouse who themselves are ordinary founders.
	// Such cases are currently handled only for original founders wherein the nuclear families formed by these additional ordinary founders
	// are displayed adjacent to the main founding group pedigree
	// INCOMPLETE: Ordinary founders having multiple ordinary founder spouses is currently not handled
	// Such nuclear families are not displayed on the pedigree.
	if(_nfOfOrdinaryFounders.size() != 0){
		if(_descentTrees.size() == 1){
			unsigned cnt=0;
			startX += (_descentTrees[0]->getRightWidth()+_nfOfOrdinaryFounders[cnt]->getLeftWidth())*horizontalInterval;
			while(cnt < _nfOfOrdinaryFounders.size()){
				Individual* tempOrgIndividual;
				tempOrgIndividual = _nfOfOrdinaryFounders[cnt]->getFather();
				_nfOfOrdinaryFounders[cnt]->draw(tempOrgIndividual,dc,startX,startY,classicalOrdering,true);
				if(cnt+1 < _nfOfOrdinaryFounders.size()){
					startX += (_nfOfOrdinaryFounders[cnt+1]->getLeftWidth() + _nfOfOrdinaryFounders[cnt]->getRightWidth())*horizontalInterval;
				}
				cnt++;
			}
		}
	}
	// End the drawing with the group tag
	dc.endGroup();
	
	// Start the connectors on a new layer
	dc.startLayer();
	_drawConsanguinousConnectors(dc);
	// End the connectors layer
	dc.endLayer();
	
	
	/////////////////////////////////
	//
	// Set up file name for drawing:
	//
	/////////////////////////////////
	std::string drawingFileName = DrawingMetrics::getDrawingFileNamePrefix();
	
	if(DrawingMetrics::getHasOnlyOnePedigreeState()==true){
		
		//
		// Just use the _id if no file name prefix was provided:
		//
		if(drawingFileName.empty()) drawingFileName = _id+"ped";
		
	}else{
		//
		// If a file name prefix was provided, add an underscore
		// before adding the _id :
		//
		if(!drawingFileName.empty()) drawingFileName.push_back('_');
		drawingFileName += _id;
		
	}
	//
	// Add file name extension:
	//
	drawingFileName += ".xml";
	
	//
	// Print out the drawing:
	//
	dc.show(drawingFileName.c_str());
	std::cout << "Pedigree output file is \"" << drawingFileName << "\"" << std::endl; 
	
}

//
// establishIndividualConnections: establish parent/child and spouse connections:
//
void Pedigree::establishIndividualConnections(){
	
	// for warnings:
	const char *methodName="Pedigree::establishIndividualConnections()";
	
	std::vector<Individual*> individualsMissingParentInformation;
	std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	while(individualIt != _individuals.end()){
		
		if((*individualIt)->getFatherId().isMissing() && (*individualIt)->getMotherId().isMissing()){
			(*individualIt)->setOrdinaryFounder(true);
		}else if((*individualIt)->getMotherId().isMissing()){
			individualsMissingParentInformation.push_back(*individualIt);
			// Add the father link
			Individual* tempIndividual = new Individual((*individualIt)->getFatherId().get());
			std::set<Individual*,compareIndividual>::iterator fatherIt = _individuals.find(tempIndividual);
			if(fatherIt != _individuals.end()){
				(*individualIt)->setFather(*fatherIt);
				(*fatherIt)->addChild(*individualIt);
			}
			delete tempIndividual;
		}else if((*individualIt)->getFatherId().isMissing()){
			individualsMissingParentInformation.push_back(*individualIt);
			// Add the mother link
			Individual* tempIndividual = new Individual((*individualIt)->getMotherId().get());
			std::set<Individual*,compareIndividual>::iterator motherIt = _individuals.find(tempIndividual);
			if(motherIt != _individuals.end()){
				(*individualIt)->setMother(*motherIt);
				(*motherIt)->addChild(*individualIt);
			}
			delete tempIndividual;
		}else{
			// Establish mother and father connections:
			Individual* tempIndividual = new Individual((*individualIt)->getFatherId().get());
			Individual* tempIndividual1 = new Individual((*individualIt)->getMotherId().get());
			std::set<Individual*,compareIndividual>::iterator fatherIt = _individuals.find(tempIndividual);
			std::set<Individual*,compareIndividual>::iterator motherIt = _individuals.find(tempIndividual1);
			if(fatherIt != _individuals.end()){
				(*individualIt)->setFather(*fatherIt);
				(*fatherIt)->addChild(*individualIt);
			}else{
				Warning(methodName,"Father %s row not present in the input data file.",(*individualIt)->getFatherId().get().c_str());
				std::pair<std::set<Individual*,compareIndividual>::iterator,bool> p;
				p = _individuals.insert(new Individual((*individualIt)->getFatherId().get(),".",".","M",-1,-1));
				(*individualIt)->setFather(*p.first);
				(*p.first)->addChild(*individualIt);
				(*p.first)->setOrdinaryFounder(true);
				(*p.first)->setVirtualIndividual(true);
				fatherIt = p.first;
			}
			if(motherIt != _individuals.end()){
				(*individualIt)->setMother(*motherIt);
				(*motherIt)->addChild(*individualIt);
			}else{
				Warning(methodName,"Mother %s row not present in the input data file.",(*individualIt)->getMotherId().get().c_str());
				// Add a virtual individual with a pre-determined id
				std::pair<std::set<Individual*,compareIndividual>::iterator,bool> p;
				p = _individuals.insert(new Individual((*individualIt)->getMotherId().get(),".",".","F",-1,-1));
				(*individualIt)->setMother(*p.first);
				(*p.first)->addChild(*individualIt);
				(*p.first)->setOrdinaryFounder(true);
				(*p.first)->setVirtualIndividual(true);
				motherIt = p.first;
			}
			(*fatherIt)->addSpouse(*motherIt);
			(*motherIt)->addSpouse(*fatherIt);
			_checkParentsGender((*individualIt));
			delete tempIndividual;
			delete tempIndividual1;
		}
		++individualIt;
	}
	
	unsigned cnt =0;
	Individual* individual;
	while(cnt < individualsMissingParentInformation.size()){
		individual = individualsMissingParentInformation[cnt];
		if(individual->getMotherId().isMissing()){
			
			Individual* father = individual->getFather();
			if(father){
				Individual* spouse = father->getFirstSpouse();
				if(spouse){
					individual->setMotherId(spouse->getId().get());
					individual->setMother(spouse);
					spouse->addChild(individual);
				}else{
					// Id of the missing parent cannot be determined.
					// Add a virtual individual with a random id
					std::string randomId = Individual::getRandomId();
					std::pair<std::set<Individual*,compareIndividual>::iterator,bool> p;
					p = _individuals.insert(new Individual(randomId,".",".","F",-1,-1));
					(*p.first)->addChild(individual);
					individual->setMotherId(randomId);
					individual->setMother(*p.first);
					father->addSpouse(*p.first);
					(*p.first)->addSpouse(father);
					(*p.first)->setOrdinaryFounder(true);
					(*p.first)->setVirtualIndividual(true);
					Warning(methodName,"Mother of %s could not be determined from the input data file. %s has been set as the mother.",individual->getId().get().c_str(),randomId.c_str());
				}
			_checkParentsGender(individual);
			}
		}else if(individual->getFatherId().isMissing()){
			
			Individual* mother = individual->getMother();
			if(mother){
				Individual* spouse = mother->getFirstSpouse();
				if(spouse){
					individual->setFatherId(spouse->getId().get());
					individual->setFather(spouse);
					spouse->addChild(individual);
				}else{
					// Id of the missing parent cant be determined.
					// Add a virtual individual with a random id
					std::string randomId = Individual::getRandomId();
					std::pair<std::set<Individual*,compareIndividual>::iterator,bool> p;
					p = _individuals.insert(new Individual(randomId,".",".","M",-1,-1));
					(*p.first)->addChild(individual);
					individual->setFatherId(randomId);
					individual->setFather(*p.first);
					mother->addSpouse(*p.first);
					(*p.first)->addSpouse(mother);
					(*p.first)->setOrdinaryFounder(true);
					(*p.first)->setVirtualIndividual(true);
					Warning(methodName,"Father of %s could not be determined from the input data file. %s has been set as the mother.",individual->getId().get().c_str(),randomId.c_str());
				}
			_checkParentsGender(individual);
			}
		}
		cnt++;
	}
	
}


//
// Debug Methods:
//
void Pedigree::display() const{
	
	std::cout << "Number of Individuals in Pedigree :" << _id << " = " << _individuals.size() << std::endl;
	std::set<Individual*>::const_iterator it = _individuals.begin();
	while(it != _individuals.end()){
		std::cout << (*it)->getId().get() << std::endl;
		//(*it)->display();
		//(*it)->displaySpouses();
		//(*it)->displayChildren();
		it++;
	}
	
}











//
// determineConnectorIndividualsOld: Original Version
//
void Pedigree::determineConnectorIndividualsOld(){
	
	// Determine external and internal connectors:
	unsigned loopNumber = 0;
	std::set<Individual*,compareIndividual>::iterator it;
	if(_descentTrees.size() > 1)
		// Assign descent trees to individuals only if there is more than one descent tree
		// If there is only one descent tree the connectors can only be consanguinous.
		_assignDescentTrees();
	// Mark consanguinous individuals and individuals that have external connections:
	it = _individuals.begin();
	while(it != _individuals.end()){
		if((*it)->isOriginalFounder()== true || (*it)->isOrdinaryFounder()== true) { ++it; continue;}
		if((*it)->getNumberOfSpouses() > 0){
			std::vector<std::string> spouses = (*it)->getSpouseIds();
			std::set<Individual*,compareIndividual>::iterator spouseIt;
			unsigned i=0;
			while(i < spouses.size()){
				
				Individual* tempIndividual = new Individual(spouses[i]);
				//spouseIt = _individuals.find(new Individual(spouses[i]));
				spouseIt = _individuals.find(tempIndividual);
				delete tempIndividual;
				if(spouseIt != _individuals.end()){
					if((*spouseIt)->isOriginalFounder() == true || (*spouseIt)->isOrdinaryFounder() == true) { i++; continue; }
					if(_descentTrees.size() == 1){
						// NOTE: INCOMPLETE TEST to see if the below condition is enough to determine all unique set of consanguinous pairs.
						if((*it)->isConsanguinous() == false || (*spouseIt)->isConsanguinous() == false){
							loopNumber++;
							_descentTrees[0]->setConsanguinity();
							//std::cout << "CONSANGUINITY FOUND: Internal connection between " << (*spouseIt)->getId() << " and " << (*it)->getId() << " LOOP " << loopNumber << std::endl;
							//std::cout << "LEFT LOOP " << loopNumber << std::endl;
							_markLeftLoopFlags(*it,loopNumber);
							//std::cout << "RIGHT LOOP " << loopNumber << std::endl;
							_markRightLoopFlags(*spouseIt,loopNumber);
							(*it)->setIsConsanguinous(true);
							(*spouseIt)->setIsConsanguinous(true);
						}
					}else{
						//std::cout << "Find intersection of " << (*spouseIt)->getId() << " and " << (*it)->getId() << std::endl;
						std::set<unsigned> dt1 = (*it)->getDescentTrees(); 
						std::set<unsigned> dt2 = (*spouseIt)->getDescentTrees();
						std::set<unsigned> result;
						//Create an insert_iterator for result
						std::insert_iterator<std::set<unsigned> > res_ins(result, result.begin());
						set_intersection(dt1.begin(),dt1.end(),dt2.begin(),dt2.end(),res_ins);
						//std::cout << "Result size is " << result.size() << std::endl;
						if(result.size() > 0){
							if((*it)->isConsanguinous() == false || (*spouseIt)->isConsanguinous() == false){
								loopNumber++;
								//
								//std::cout << "CONSANGUINITY found between :" << (*spouseIt)->getId() << " and " << (*it)->getId() << std::endl;
								// NOTE: NEED TO SET WHICH DT has consanguinity
								// This can be done using the _getPrimaryDescentTree below:
								loopNumber++;
								_markLeftLoopFlags(*it,loopNumber);
								//std::cout << "RIGHT LOOP " << loopNumber << std::endl;
								_markRightLoopFlags(*spouseIt,loopNumber);
								(*spouseIt)->setIsConsanguinous(true);
								(*it)->setIsConsanguinous(true);
								unsigned dtIndex = _getPrimaryDescentTreeIndex(dt1,(*it),false);
								//std::cout << " The DTS CONSANG are " << _descentTrees[dtIndex]->getId() << std::endl;
								_descentTrees[dtIndex]->setConsanguinity();
							}
						}else{
							//std::cout << "EXTERNAL connection between: " << (*spouseIt)->getId() << " and " << (*it)->getId() << std::endl;
							//(*spouseIt)->setExternalConnection(true);
							//(*it)->setExternalConnection(true);
							
							// NOTE: ADDED on 2006-04-20 to sort the descent trees based on
							// complexity. For this we need to know how many external connections
							// are there is each DT and with which DT they have maximum connections:
							unsigned dtIndex = _getPrimaryDescentTreeIndex(dt1,(*it),true);
							unsigned spouseDTIndex = _getPrimaryDescentTreeIndex(dt2,(*spouseIt),false);
							_descentTrees[dtIndex]->addExternalConnectorPair((*it),(*spouseIt));
							_descentTrees[dtIndex]->incrementConnectionsWithDT(_descentTrees[spouseDTIndex]->getId());
							
						}
					}
				}
				i++;
			}
		}
		++it;
	}
	
	// DEBUG:
	//for(unsigned i=0;i<_descentTrees.size();i++){
	//	std::cout << "For Descent TREE with Id" << _descentTrees[i]->getId() << std::endl;
	//	for(unsigned j=0;j<_descentTrees.size();j++){
	//		if(j==i) continue;
	//		std::cout << " # of connections with " << _descentTrees[j]->getId() << " is " << _descentTrees[i]->getNumberOfConnectionsWithDT(_descentTrees[j]->getId()) << std::endl;
	//	}
	//}
	//std::cout << "End of determine Connector Ind :" << _descentTrees.size() << std::endl;
	if(_descentTrees.size() == 1) return;
	// NOTE: Added on 2006-07-01 This case could occur when there is only 1 individual
	// in a pedigree and is UNCONNECTED
	if(_descentTrees.size() == 0) return;
	// If there is more than 1 DT in the pedigree find the optimal ordering of the DTs
	// The most complex tree is in the center; 2 of the remaining trees which have max connections with it flank it on either side 
	unsigned max= 0,complexDTIndex=0;
	for(unsigned i=0;i<_descentTrees.size();i++){
		if(_descentTrees[i]->getNumberOfExternalConnections() > max){
			max = _descentTrees[i]->getNumberOfExternalConnections();
			complexDTIndex = i;
		}
	}
	std::cout << " Most complex descent Tree is " << _descentTrees[complexDTIndex]->getId() << std::endl;
	std::deque<DescentTree*> orderedDescentTrees;
	orderedDescentTrees.push_back(_descentTrees[complexDTIndex]);
	
	// Find the two DTs which have most number of connections with this tree:
	max=0;
	
	unsigned rightDTIndex=0,leftDTIndex=0;
	for(unsigned j=0;j<_descentTrees.size();j++){
		if(j==complexDTIndex) continue;
		if(_descentTrees[complexDTIndex]->getNumberOfConnectionsWithDT(_descentTrees[j]->getId()) >= max){
			leftDTIndex = rightDTIndex;
			max = _descentTrees[complexDTIndex]->getNumberOfConnectionsWithDT(_descentTrees[j]->getId());
			rightDTIndex = j;
		}
	}
	if(leftDTIndex != rightDTIndex && leftDTIndex != complexDTIndex) orderedDescentTrees.push_front(_descentTrees[leftDTIndex]);
	orderedDescentTrees.push_back(_descentTrees[rightDTIndex]);
	//std::cout << " THE DESCENT TREE DEQUE IS " << std::endl;
	//for(unsigned i=0;i<orderedDescentTrees.size();i++)
	//std::cout << orderedDescentTrees[i]->getId() << std::endl;
	
	
	// Find all other DTs not included in the deque yet
	
	// First find all the DTs related to the left DT tree
	if(orderedDescentTrees.size() < _descentTrees.size())
	_addDescentTreesConnectedTo(leftDTIndex,orderedDescentTrees,true);
	if(orderedDescentTrees.size() < _descentTrees.size())
	_addDescentTreesConnectedTo(rightDTIndex,orderedDescentTrees,false);
	if(orderedDescentTrees.size() < _descentTrees.size())
	_addDescentTreesConnectedTo(complexDTIndex,orderedDescentTrees,false);
	
	std::vector<DescentTree*>::iterator vit;
	for(unsigned i=0;i<orderedDescentTrees.size();i++){
		vit = find(_descentTrees.begin(),_descentTrees.end(),orderedDescentTrees[i]);
		if(vit != _descentTrees.end()) _descentTrees.erase(vit);
	}
	//std::cout << " DT SIZE NOW IS " << _descentTrees.size() << std::endl;
	
	// There might be DTs which are not connected to any other DT
	// The options are to ignore such DT or just draw them unconnected
	// The second option is the default
	if(_descentTrees.size() != 0){
		//std::cout << " OOPS ! missed out on some tree";
		for(unsigned i=0;i<_descentTrees.size();i++)
			orderedDescentTrees.push_back(_descentTrees[i]);
	}
	
	//std::cout << " THE REORDERED DT vector IS " << std::endl;
	_descentTrees.clear();
	for(unsigned i=0;i<orderedDescentTrees.size();i++){
		_descentTrees.push_back(orderedDescentTrees[i]);
	}
	
	_markExternalConnectionFlags();
	
}


