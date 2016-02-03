/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager and Ritu Khanna
// Copyright (c) 2005, 2016 by the
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
// Last updated: 2016.02.01 by ET
//

#include "Pedigree.h"
#include "Date.h"
#include "String.h"
#include "Twin.h"
#include "Number.h"
#include "DrawingCanvas.h"
#include "Grid.h"
#include "HexavigesimalConverter.h"
#include "RandomId.h"

#include <algorithm>
#include <map>
#include <vector>
#include <deque>
#include <set>

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
	}*/
	
}

//
// _addDescentTree
//
void Pedigree::_addDescentTree(unsigned id) {
	
	_descentTrees.push_back(new DescentTree(id));
	
}

///
/// _checkMarkedTwinsDOB: Issue warning messages if the DOB of twins in a twin group do not match.
///
void Pedigree::_checkMarkedTwinsDOB(){
	
	// get all the Individuals with the same marker from the map
	std::map<std::string,std::vector<Individual*> >::iterator mit = _twinMarkers.begin();
	while(mit != _twinMarkers.end()){
		// return if there is no dob field present
		if(mit->second[0]->getDOB() == 0) return;
		std::vector<Data*> dates;
		unsigned missingCount=0;
		for(unsigned i=0;i<mit->second.size();i++){
			if(mit->second[i]->getDOB()->isMissing()){
				missingCount++;
			}else{
				dates.push_back(mit->second[i]->getDOB());
			}
		}
		
		if(missingCount == mit->second.size());
		else if(missingCount && missingCount < mit->second.size()){
			Warning("Pedigree::_checkMarkedTwinsDOB()","DOB is missing for some twins in the twin group with marker '%s'.",mit->first.substr(0,1).c_str());
		}else{
			bool unique=true;
			Data* initial = dates[0];
			for(unsigned i=1;i<dates.size();i++){
				if(*initial == *dates[i]);
				else { unique = false; break; }
			}
			if(!unique) Warning("Pedigree::_checkMarkedTwinsDOB","Dates of birth of twins in group '%s' are not identical.",mit->first.substr(0,1).c_str());
			
		}
		++mit;
	}
	
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
		
		
		(*pit.first)->findTwinsByDOB();
		
		// Add the nuclearFamily to the individual
		mother->addNuclearFamily(*pit.first);
		father->addNuclearFamily(*pit.first);
	}
	return;
	
}


///
/// _checkParentsGender: Checks the consistency of the pedigree input data table.
///
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
			std::set<Individual*,Individual::compareIndividual>::const_iterator it = (*pspouses).begin();
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
			std::set<Individual*,Individual::compareIndividual>::const_iterator it = (*pspouses).begin();
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
	std::set<Individual*,Individual::compareIndividual>::const_iterator it = (*pspouses).begin();
	
	static unsigned _recursionLevel=0;
	
	while(it != (*pspouses).end()){
		std::pair<std::set<Individual*,compareIndividual>::const_iterator,bool> pit = foundingGroup.insert(*it);
		std::cout << "+++ >>> At recursion level " << _recursionLevel << " adding " << (*it)->getId() << " to founding group " << static_cast<void *>(&foundingGroup) << " ." << std::endl;
		if(pit.second) _getSpouses(foundingGroup,*it);
		++it;
	}
	
	_recursionLevel++;
	
}

///
/// _markLeftLoopFlags: Recursively traverses up the descent tree from a consanguinous individual
/// and marks the left consanguinous flags
///
void Pedigree::_markLeftLoopFlags(Individual* individual,unsigned loopNumber){
	
	// Assign the loopNumber to the Individual
	individual->setLeftSideOfLoop(loopNumber);
	// get parents of the Individual
	Individual* father = individual->getFather();
	if(father->isOriginalFounder()) {
		father->setLeftSideOfLoop(loopNumber);
		Individual* mother = individual->getMother();
		mother->setLeftSideOfLoop(loopNumber);
		return;
	}
	// NOTE: Not required to set the flags for ordinaryFounders.
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

///
/// _markRightLoopFlags: Recursively traverses up the descent tree from a consanguinous individual
/// and marks the right consanguinous flags
///
void Pedigree::_markRightLoopFlags(Individual* individual,unsigned loopNumber){
	
	// Assign the loopNumber to the Individual
	individual->setRightSideOfLoop(loopNumber);
	// get parents of the Individual
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


///
/// _markLeftExternalConnectionFlags:Recursively traverses up the descent tree from an individual with an external connection
/// and marks the left external connection flags
///
void Pedigree::_markLeftExternalConnectionFlags(Individual* individual,unsigned connectionNumber){
	
	// Assign the connectionNumber to the Individual
	individual->setLeftSideOfExternalConnection(connectionNumber);
	
	// get parents of the Individual
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
		if(father->getNumberOfSpouses() > 1){
			Individual* mother = individual->getMother();
			mother->setLeftSideOfExternalConnection(connectionNumber);
		}
		// recurse up the tree
		_markLeftExternalConnectionFlags(father,connectionNumber);
	}
	Individual* mother = individual->getMother();
	if(!mother->isOrdinaryFounder()){
		mother->setLeftSideOfExternalConnection(connectionNumber);
		if(mother->getNumberOfSpouses() > 1){
			Individual* father = individual->getFather();
			father->setLeftSideOfExternalConnection(connectionNumber);
		}
		_markLeftExternalConnectionFlags(mother,connectionNumber);
	}
	
}

///
/// _markRightExternalConnectionFlags:Recursively traverses up the descent tree from an individual with external connection
/// and marks the right external connection flags
///
void Pedigree::_markRightExternalConnectionFlags(Individual* individual,unsigned connectionNumber){
	
	// Assign the connectionNumber to the Individual
	individual->setRightSideOfExternalConnection(connectionNumber);
	// get parents of the Individual
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
		if(father->getNumberOfSpouses() > 1){
			Individual* mother = individual->getMother();
			mother->setRightSideOfExternalConnection(connectionNumber);
		}
		// Recurse up the tree
		_markRightExternalConnectionFlags(father,connectionNumber);
	}
	Individual* mother = individual->getMother();
	if(!mother->isOrdinaryFounder()){
		mother->setRightSideOfExternalConnection(connectionNumber);
		if(mother->getNumberOfSpouses() > 1){
			Individual* father = individual->getFather();
			father->setRightSideOfExternalConnection(connectionNumber);
		}
		_markRightExternalConnectionFlags(mother,connectionNumber);
	}
	
}


///
/// _markExternalConnectionFlags:
///
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
// (1) Make sure that the start individual
//     and all of his or her originalFounder spouses
//     are marked with their descentTreeId.
//
// (2) Make sure all the children in the descent tree
//     are likewise marked with their descentTreeId(s).
//
//     NOTA BENE: ordinaryFounders below the originalFounders
//     in a given descent tree do not need to be considered
//     part of the descent tree and so are not marked
//
void Pedigree::_assignDescentTrees(){
	
	unsigned cnt,descentTreeId,cnt1;
	
	for(unsigned i =0;i < _descentTrees.size();i++){
		
		descentTreeId = _descentTrees[i]->getId();
		
		Individual* startIndividual = _descentTrees[i]->getStartIndividual();
		// DEBUG: std::cout << ">>> _assignDescentTrees(): startIndividual is " << startIndividual->getId() << std::endl;
		startIndividual->addDescentTree(descentTreeId);
		// This marks the direct spouses of the startIndividual
		// (who are also originalFounders):
		_assignSpousesToDescentTree(startIndividual,descentTreeId);
		// The following is recursive, so all direct lineage children
		// get marked:
		_assignChildrenToDescentTree(startIndividual,descentTreeId);
		
	}
	
}

//
// _assignChildrenToDescentTree:
//
// Assigns (flags) all direct lineage children in a descent tree
// with the descentTreeId so that this information can later be
// used to find marriages across descent trees, etc.
//
void Pedigree::_assignChildrenToDescentTree(Individual* individual,unsigned descentTreeId){
	
	const std::set<Individual*,Individual::compareIndividual> *pChildren = individual->getChildren();
	std::set<Individual*,Individual::compareIndividual>::const_iterator it;
	
	for(it=pChildren->begin();it!=pChildren->end();++it){
		// DEBUG: std::cout << ">>> _assignChildrenToDescentTree: Individual " << (*it)->getId()
		//          << " assigned to descent tree " << descentTreeId << std::endl;
		
		//
		// Assign the children in this sibship:
		//
		(*it)->addDescentTree(descentTreeId);
		//
		// Recursively assign the children of these children 
		// to the descentTree too:
		//
		_assignChildrenToDescentTree(*it,descentTreeId);
	}
	
}

//
// _assignSpousesToDescentTree:
//
// Assigns (flags) spouses of the starting individual
// with a descentTreeId
//
void Pedigree::_assignSpousesToDescentTree(Individual* individual,unsigned descentTreeId){
	
	const std::set<Individual*,Individual::compareIndividual> *pSpouses = individual->getSpouses();
	std::set<Individual*,Individual::compareIndividual>::const_iterator it;
	
	for(it=pSpouses->begin();it!=pSpouses->end();++it){
		// DEBUG: std::cout << ">>> _assignSpousesToDescentTree: Individual " << (*it)->getId()
		//          << " assigned to descent tree " << descentTreeId << std::endl;
		(*it)->addDescentTree(descentTreeId);
	}
	
}

//
// _assignMultipleDescentTreeJoinerSpouses
//
// Mark individuals who are multiply mated to individuals across
// different descent trees. There are two cases which are 
// treated here:
//
// (1) An originalFounder with multiple spouses could be
//     an original founder in more than one descentTree.
//
// (2) An originalFounder or ordinaryFounder could join
//     together two different descent trees (in which case
//     the intersection set of the descent trees of the 
//     founder and one of his or her spouses is expected to 
//     be an empty set).
//
void Pedigree::_assignMultipleDescentTreeJoinerSpouses(){
	
	std::set<Individual*,compareIndividual>::iterator it;
	for(it=_individuals.begin();it!=_individuals.end();++it){
		
		//
		// CASE 1: An original founder of more than one descent tree:
		//
		if( (*it)->isOriginalFounder() && (*it)->getNumberOfDescentTrees()>1 ){
			(*it)->setMultipleDescentTreeJoinerSpouse(true);
			continue;
		}
		
		//
		// CASE 2: A founder who does not share descent trees
		//         with one or more of his or her spouses:
		//         (NOTA BENE: This depends on Madeline's current
		//         behavior of not marking married-in ordinary
		//         founders as being part of their spouse's descent tree.
		//         However, note that the code is careful to cover 
		//         the case of individuals who might an originalFounder
		//         in one descentTree and an ordinaryFounder in "another"
		//         descentTree.
		//
		std::set<unsigned> dt1 = (*it)->getDescentTrees();
		if((*it)->getNumberOfSpouses()>1){
			// DEBUG: std::cout << ">>> DMDTJS: Individual " << (*it)->getId() 
			//          << " has " << (*it)->getNumberOfSpouses() << " spouses"
			//          << " and belongs to " << (*it)->getNumberOfDescentTrees() << " descent tree(s)"
			//          << std::endl;
			const std::set<Individual*,Individual::compareIndividual> *pSpouses = (*it)->getSpouses();
			std::set<Individual*,Individual::compareIndividual>::const_iterator spi;
			for(spi=pSpouses->begin();spi!=pSpouses->end();++spi){
				std::set<unsigned> dt2 = (*spi)->getDescentTrees();
				std::vector<unsigned> vIntersect;
				std::set_intersection(dt1.begin(),dt1.end(),
				                      dt2.begin(),dt2.end(),
				                      std::back_inserter(vIntersect));
				//
				// DEBUG: std::cout << (*it)->getId() << " and " << (*spi)->getId() << " have " << vIntersect.size() << " elements in common" << std::endl;
				//
				if(vIntersect.size()==0){
					(*it)->setMultipleDescentTreeJoinerSpouse(true);
					continue;
				}
			}
		}
	}
	
}


///
/// _getPrimaryDescentTreeIndex: Get the descent tree index of the individual.
///
unsigned Pedigree::_getPrimaryDescentTreeIndex(std::set<unsigned>& dt,Individual* individual,bool increment){ 
	
	std::set<unsigned>::iterator sit;
	unsigned dtIndex=0;
	if(dt.size() == 1){
		sit = dt.begin();
	}else{
		// Try to find the single DT to which the individual's parent or his/her ancestors belong:
		// For cases where the individual belongs to more than 1 descent tree trace his parents and 
		// their ancestors till one descent tree index is obtained.
		Individual* ancestorIndividual = individual->getMother();
		// If the mother is an ordinary founder the DT is not set on her:
		if(ancestorIndividual->getNumberOfDescentTrees() == 0) ancestorIndividual = individual->getFather();
		std::set<unsigned> ancestorDt;
		while(ancestorIndividual->getNumberOfDescentTrees() != 1){
			if(ancestorIndividual->isOriginalFounder()){
				break;
			}
			if(ancestorIndividual->getMother()->getNumberOfDescentTrees() == 0){
				ancestorIndividual = ancestorIndividual->getFather();
			}else{
				ancestorIndividual = ancestorIndividual->getMother();
			}
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

///
/// _addDescentTreesConnectedTo: For each descent tree determine which are the other descent trees that have connectors to it.
///
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
// _determineConnectorIndividuals(): 
//  
//  A depth-first search approach is used to mark individuals who are consanguinous 
//  or have mates who are part of a different descent tree.
//
void Pedigree::_determineConnectorIndividuals(){
	
	// Determine external and internal connectors:
	unsigned loopNumber = 0;
	if(_descentTrees.size() > 1)
		// Assign descent trees to individuals only if there is more than one descent tree
		// If there is only one descent tree the connectors can only be consanguinous.
		_assignDescentTrees();
		_assignMultipleDescentTreeJoinerSpouses();
	unsigned cnt, cnt1;
	for(unsigned i=0;i<_descentTrees.size();i++){
		// Get number of individuals in the founding group
		cnt = _descentTrees[i]->getNumberOfFoundingGroupIndividuals();
		cnt1=cnt;
		Individual* startIndividual = _descentTrees[i]->getStartIndividual();
		while(cnt > 0){
			cnt--;
			if(cnt > 0){
				const std::set<Individual*,Individual::compareIndividual> * pspouses = startIndividual->getSpouses();
				std::set<Individual*,Individual::compareIndividual>::const_iterator it = (*pspouses).begin();
				while(it != (*pspouses).end()){
					(*it)->setVisited(true);
					std::vector<Individual*> children;
					startIndividual->getChildrenWithSpouse((*it),children);
					unsigned childCnt = 0;
					while(childCnt < children.size()){
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
	//	for(unsigned j=0;j<_descentTrees.size();j++){
	//		if(j==i) continue;
	//		std::cout << " # of connections with " << _descentTrees[j]->getId() << " is " << _descentTrees[i]->getNumberOfConnectionsWithDT(_descentTrees[j]->getId()) << std::endl;
	//	}
	//}
	
	if(_descentTrees.size() == 1){
		_markConsanguinousIndividuals();
		return;
	}
	
	// Return when there is only one UNCONNECTED individual in a pedigree
	if(_descentTrees.size() == 0) return;
	
	// If there is more than 1 DT in the pedigree find the optimal ordering of the DTs
	_reorderDescentTreesBasedOnExternalConnections();
	_markExternalConnectionFlags();
	
	if(_dtsHaveConsanguinity){
		_markConsanguinousIndividuals();
	}
	
	
}

///
/// _reorderDescentTreesBasedOnExternalConnections: Rearrange the descent trees based on the external connections.
///
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
	//std::cout << " The Reordered Descent Tree vector: " << std::endl;
	_descentTrees.clear();
	for(unsigned i=0;i<orderedDescentTrees.size();i++){
		_descentTrees.push_back(orderedDescentTrees[i]);
		//std::cout << " Descent Tress Start Individual: " << orderedDescentTrees[i]->getStartIndividual()->getId() << std::endl;
	}
	
	
}

///
/// _markConnectorIndividuals: This function marks all the individuals that are consanguinous or have an external connection.
///
void Pedigree::_markConnectorIndividuals(Individual* individual,unsigned& loopNumber){
	
	// Mark consanguinous individuals and individuals that have external connections:
	if(individual->isOrdinaryFounder() == true || individual->isOriginalFounder() == true){ 
		return;
	}
	
	if(individual->getNumberOfSpouses() > 0){
		
		const std::set<Individual*,Individual::compareIndividual> * pspouses = individual->getSpouses();
		std::set<Individual*,Individual::compareIndividual>::const_iterator spouseIt = (*pspouses).begin();
		while(spouseIt != (*pspouses).end()){
			if((*spouseIt)->isOriginalFounder() == true || (*spouseIt)->isOrdinaryFounder() == true){
				// Don't do anything
			}else
			if(_descentTrees.size() == 1){
				// Mark the consanguinous individuals:
				if(individual->isConsanguinous() == false || (*spouseIt)->isConsanguinous() == false){
					loopNumber++;
					_descentTrees[0]->setConsanguinity();
					// DEBUG:
					//std::cout << "CONSANGUINITY FOUND: Internal connection between " << (*spouseIt)->getId() << " and " << individual->getId() << " LOOP " << loopNumber << std::endl;
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
						// Note: Consang pairs are inserted in a set as it is possible that with 
						// multiple descent trees both the spouses with multiple mates could have
						// consanguinous flags set but the relationship with a specific mate may be external
						// This scenario cannot occur with single descent trees
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
					// are there in each DT and with which DT they have maximum connections:
					unsigned dtIndex = _getPrimaryDescentTreeIndex(dt1,individual,true);
					unsigned spouseDTIndex = _getPrimaryDescentTreeIndex(dt2,(*spouseIt),false);
					_descentTrees[dtIndex]->addExternalConnectorPair(individual,(*spouseIt));
					_descentTrees[dtIndex]->incrementConnectionsWithDT(_descentTrees[spouseDTIndex]->getId());
					
				}
			}
			//
			// Get the children of this pair and recurse:
			//
			const std::set<Individual*,Individual::compareIndividual> * pchildren = individual->getChildren();
			std::set<Individual*,Individual::compareIndividual>::const_iterator childIt = (*pchildren).begin();
			while(childIt != (*pchildren).end()){
				_markConnectorIndividuals((*childIt),loopNumber);
				++childIt;
			}
			++spouseIt;
		}
	}
	
}

///
/// _markConsanguinousIndividuals: This function marks all the individuals that are consanguinous.
///
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
				std::set<Individual*,Individual::compareIndividual>::const_iterator it = (*pspouses).begin();
				while(it != (*pspouses).end()){
					(*it)->setVisited(true);
					std::vector<Individual*> children;
					std::vector<Individual*> sortedChildren;
					startIndividual->getChildrenWithSpouse((*it),children);
					_sortSibsBasedOnConsanguinousConnections(children,sortedChildren);
					if(_descentTrees.size() > 1){
						children.swap(sortedChildren);
						sortedChildren.clear();
						_sortSibsBasedOnExternalConnections(children,sortedChildren);
					}
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

///
/// _markConsanguinousFlags: This function gets all the individuals with 
/// consanguinity and propagates the flags up in the descent tree
/// These flags determine the sibling sorting order of the Nuclear Family.
///
void Pedigree::_markConsanguinousFlags(Individual* individual,unsigned& loopNumber){
	
	const std::set<Individual*,Individual::compareIndividual> * pspouses = individual->getSpouses();
	std::set<Individual*,Individual::compareIndividual>::const_iterator spouseIt = (*pspouses).begin();
	while(spouseIt != (*pspouses).end()){
		if(individual->hasBeenVisited() && (*spouseIt)->hasBeenVisited()){
			++spouseIt;
			continue;
		}
		if((*spouseIt)->isConsanguinous()){
			loopNumber++;
			if(_descentTrees.size() == 1 && individual->isConsanguinous()){
				//std::cout << " Left loop individual: " << individual->getId() << " Right loop individual: " << (*spouseIt)->getId() << std::endl;
				_markLeftLoopFlags(individual,loopNumber);
				_markRightLoopFlags(*spouseIt,loopNumber);
			}else{
				// Check if this pair is consanguinous
				std::string pairId;
				if((*spouseIt)->getGender().getEnum() == Gender::MALE) pairId = (*spouseIt)->getId().get()+individual->getId().get();
				else pairId = individual->getId().get()+(*spouseIt)->getId().get();
				std::set<std::string>::iterator pairIt = _consangPairIds.find(pairId);
				if(pairIt != _consangPairIds.end()){
					//std::cout << " Left loop individual: " << individual->getId() << " Right loop individual: " << (*spouseIt)->getId() << std::endl;
					_markLeftLoopFlags(individual,loopNumber);
					_markRightLoopFlags(*spouseIt,loopNumber);
				}
			}
		}
		(*spouseIt)->setVisited(true);
		
		// ************
		// Get the children of this pair and recurse:
		// ************
		
		std::vector<Individual*> children;
		std::vector<Individual*> sortedChildren;
		individual->getChildrenWithSpouse((*spouseIt),children);
		_sortSibsBasedOnConsanguinousConnections(children,sortedChildren);
		if(_descentTrees.size() > 1){
			children.swap(sortedChildren);
			sortedChildren.clear();
			_sortSibsBasedOnExternalConnections(children,sortedChildren);
		}
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

///
/// _establishNuclearFamilies:  Determine the nuclear families of a pedigree.
///
void Pedigree::_establishNuclearFamilies(){
	
	std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	while(individualIt != _individuals.end()){
		if(!(*individualIt)->getNumberOfSpouses()) { ++individualIt; continue; }
		// Get the spouses
		const std::set<Individual*,Individual::compareIndividual> * pspouses = (*individualIt)->getSpouses();
		std::set<Individual*,Individual::compareIndividual>::const_iterator it = (*pspouses).begin();
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

/// 
/// _sortIndividualNuclearFamilies: Sort individuals with multiple spouses for cases with consanguinity/external connections:
///
void Pedigree::_sortIndividualNuclearFamilies(){
	
	std::set<Individual*,compareIndividual>::iterator it = _individuals.begin();
	while(it != _individuals.end()){
		if((*it)->isOrdinaryFounder()) { ++it; continue; }
		if((*it)->getNumberOfSpouses() > 1){ 
			if(_descentTrees.size() > 1 && !_dtsHaveConsanguinity) (*it)->sortSpouses(true);
			else (*it)->sortSpouses();
		}
		++it;
	}
	
}

//
// _sortNuclearFamilies:
//
void Pedigree::_sortNuclearFamilies(bool consanguinousFlag){
	
	/// Sort all the nuclear families based on optimal/classical ordering of children
	/// when consanguinity or external connection is found in the descent tree
	
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

///
/// _sortNuclearFamiliesBasedOnDataField: If the user specifies a sorting field, 
/// the siblings are sorted based on that data field.
///
void Pedigree::_sortNuclearFamiliesBasedOnDataField(const std::string& name,bool dobSortOrder){
	
	std::set<NuclearFamily*,compareNuclearFamily>::iterator nfIt = _nuclearFamilies.begin();
	
	while(nfIt != _nuclearFamilies.end()){
		(*nfIt)->sortChildrenBasedOnDataField(name,dobSortOrder);
		++nfIt;
	}
	
	
}

///
/// _sortAndCalculateDescentTreeWidth: Sorts the nuclear families based on consanguinous connections 
/// and calculates the descent tree width.
///
void Pedigree::_sortAndCalculateDescentTreeWidth(){
	
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
		
		// Check if there exists any Founding Group member who has > 1 NF 
		for(unsigned i=0;i<_descentTrees[cnt]->getNumberOfFoundingGroupIndividuals();i++){
			Individual* fgIndividual = _descentTrees[cnt]->getFoundingGroupIndividual(i);
			if(fgIndividual->getId() == startIndividual->getId()) continue;
			if(fgIndividual->getNumberOfNuclearFamilies() > 1){
				// Get each NF and check if it has been processed
				for(unsigned j=0;j<fgIndividual->getNumberOfNuclearFamilies();j++){
					if(fgIndividual->getNuclearFamily(j)->getTotalWidth() == 0){
						// THIS MESSAGE IS NOT INFORMATIVE: std::cout << " This Founding Group member " << fgIndividual->getId() << " needs to be processed further" << std::endl;
						fgIndividual->getNuclearFamily(j)->calculateWidth(classicalOrdering);
						_nfOfOrdinaryFounders.push_back(fgIndividual->getNuclearFamily(j));
					}
				}
			}
		}
	}
	
}

//
// _setLeftShifConnectionFlags(): Confirms the Left Connectors and changes left and right widths of their 
// parent NFs if required.
//
void Pedigree::_setLeftShiftConnectionFlags(){
	
	// Sometimes when a child in a NF has a left connector set it may be required to add an extra
	// width of horizontalInterval on the NF which is done by the draw method of NF.
	// This method sets the left shift connection flag for such cases.
	
	std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	while(individualIt != _individuals.end()){
		if((*individualIt)->getLeftSpouseConnector()){
			
			NuclearFamily* lcnf = (*individualIt)->getNuclearFamily((unsigned)0);
			Individual* father = (*individualIt)->getFather();
			Individual* mother = (*individualIt)->getMother();
			// Check if any one of the parent has multiple mates:
			// Reset the left spouse connector flag in such cases
			if((!father->isOrdinaryFounder() && father->getNumberOfSpouses() > 1) || (!mother->isOrdinaryFounder() && mother->getNumberOfSpouses() > 1)){
				(*individualIt)->setLeftSpouseConnector(false);
				++individualIt;
				continue;
			}
			
			Individual* spouse = lcnf->getFather();
			Individual* spouseFather = lcnf->getFather()->getFather();
			Individual* spouseMother = lcnf->getFather()->getMother();
			NuclearFamily *spouseParentNF;
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
				// The left connector flag is not reset but will be checked again by Nuclear Family's draw method
				// If the individual's parentNF is consanguinous there might be a need for shifting.
				//
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
					if( (
					      spouseParentNF->hasChild(father) 
					      || 
					      spouseParentNF->hasChild(mother)
					    )
					    && parentNF->getNumberOfChildren() <= 2
					  ){
						parentNF->setLeftConnectionShiftFlag(true);
					}
				}
			}
		}
		++individualIt;
	}
	
}

///
/// _calculateWidth: Calculates the width of a descent tree.
///
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
	//
	// The total ,left and right widths have to be stored in the start Individual
	// only if the startIndividual has more than one NF
	// Assign the widths to the descent trees
	//
	_descentTrees[descentTreeIndex]->setLeftWidth(sumLeftWidth);
	_descentTrees[descentTreeIndex]->setRightWidth(sumRightWidth);
	_descentTrees[descentTreeIndex]->setTotalWidth(sumLeftWidth+sumRightWidth);
	
	return;
	
}

///
/// _populateIndividualGrid: Initialize the grid with the x and y positions of the individuals on the canvas.
///
void Pedigree::_populateIndividualGrid(){
	
	std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	
	individualIt = _individuals.begin();
	while(individualIt != _individuals.end()){
		_individualGrid.insert(int((*individualIt)->getX()),int((*individualIt)->getY()),(*individualIt));
		++individualIt;
	}
	
	
}

///
/// _hasIndividualAtPosition: Checks if an individual exists between two given individuals on the grid.
///
bool Pedigree::_hasIndividualAtPosition(Individual* start,Individual* end){
	
	double xstart = start->getX();
	double ystart = start->getY();
	double xend   = end->getX();
	double yend   = end->getY();
	double horizontalInterval = DrawingMetrics::getHorizontalInterval();
	xstart += horizontalInterval;
	int ystarti = int(ystart);
	if(ystart != yend) { 
		// if female with a left spouse connector is on the right, we need to subtract iconinterval
		if(end->getLeftSpouseConnector()){ xend -= horizontalInterval; }
		xend -= horizontalInterval;
		if(xstart >= xend) return true;
		while(xstart < xend){
			Individual* found = _individualGrid.find(int(xstart),ystarti);
			if(found){ return true; }
			xstart+= horizontalInterval;
		}
		return false;
	}
	if(xstart >= xend){ return true; }
	while(xstart < xend){
		Individual* found = _individualGrid.find(int(xstart),ystarti);
		if(found){ if(found->getId() == end->getId()) return false; return true; }
		xstart+= horizontalInterval;
		
	}
	return false;
	
}

//
// _sortSibsBasedOnConsanguinousConnections:
//
void Pedigree::_sortSibsBasedOnConsanguinousConnections(const std::vector<Individual*>& sibs,std::vector<Individual*>& sortedSibs){
	
	std::deque<Individual*> consang;
	std::deque<Individual*> initial;
	
	// If there is only one sib return:
	if(sibs.size() == 1){
		sortedSibs.push_back(sibs.front());
		return;
	}
	// push all the consanguinous individuals in the consang queue
	for(unsigned i=0;i<sibs.size();i++){
		if(sibs[i]->isConsanguinous()) consang.push_back(sibs[i]);
		else initial.push_back(sibs[i]);
	}
	if(!consang.size()|| consang.size() == sibs.size()){
		sortedSibs.insert(sortedSibs.begin(),sibs.begin(),sibs.end());
		return;
	}
	// Alternately, push the consanguinous to the front and back of the remaining individuals
	for(unsigned j=0;j<consang.size();j++){
		if(j%2 == 0) initial.push_front(consang[j]);
		else initial.push_back(consang[j]);
	}
	// Copy the result into sortedChildren vector:
	while(!initial.empty()){
		sortedSibs.push_back(initial.front());
		initial.pop_front();
	}
	
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
	while(!initial.empty()){
		sortedSibs.push_back(initial.front());
		initial.pop_front();
	}
	// Set this sorting on the parents:
	Individual* father = sortedSibs.front()->getFather();
	Individual* mother = sortedSibs.front()->getMother();
	std::string parentPair = father->getId().get()+mother->getId().get();
	mother->setChildrenIdsSortedByExternalConnections(parentPair,sortedSibs);
	
}

///
/// _drawConsanguinityLetter: Assign a unique label to each unique individual who is repeated for consanguinity and external connections.
///
void Pedigree::_drawConsanguinityLetter(Individual* mother,Individual* father,unsigned int &uniqueId,double iconInterval,double iconDiameter, std::map<std::string,std::string>& individualConsanguinityLetter,DrawingCanvas& dc,double multipleSpouseOffset,bool leftConnector){
	
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
		HexavigesimalConverter hxc;
		buf << hxc.itoa(uniqueId);
		individualConsanguinityLetter.insert(std::map<std::string,std::string>::value_type(father->getId().get(),buf.str()));
		dc.drawText(mother->getX()+offset,mother->getY()+yOffset,buf.str());
		dc.drawText(father->getX(),father->getY()+yOffset,buf.str());
		
		uniqueId++;
	}
	
}

///
/// _drawHorizontalConnectorLine: Draws horizontal consanguinous and external connections.
///
void Pedigree::_drawHorizontalConnectorLine(double y,double x1,double x2,bool isConsanguinous,DrawingCanvas& dc){
	
	double verticalTick = DrawingMetrics::getVerticalTick();
	if(isConsanguinous) y += verticalTick/2;
	dc.drawHorizontalLine(y,x1,x2);
	if(isConsanguinous)
		dc.drawHorizontalLine(y-DrawingMetrics::getVerticalTick(),x1,x2);
	
}

//
// _drawSteppedConnectorLine: 
//
// Draws a stepped connecting line 
// between consanguinous or externally-connected
// mated pairs.
// 
// Stepped connecting lines look like this:
//    ___       ___
// __|      or     |___
//
// "stepUp" or "stepDown"
//
void Pedigree::_drawSteppedConnectorLine(double startY,double endY,double startX,double endX,bool isConsanguinous,DrawingCanvas& dc,double multipleSpouseOffset,bool singleChild){
	
	/////////////////////////////////////////////////////
	//
	// DEBUG ONLY: Draw a direct diagonal line just to
	//             see where the connections are going:
	//
	//dc.drawLine(startX,startY,endX,endY);
	//
	/////////////////////////////////////////////////////
	
	//
	// This is calculation for the
	// x midpoint is too simplistic
	// but let it go for now:
	//
	double midX = 0.5*(startX+endX);
	//
	// Either we are stepping up or stepping down:
	//
	bool stepDown=false;
	//
	// There are four cases but reducible to
	// just two if we swap points in the latter
	// two cases:
	//
	if(startX<endX && startY<endY){
		// left-to-right step down, no swap:
		stepDown=true;
	}else if(startX<endX && startY>endY){
		// left-to-right step up, no swap:
		stepDown=false;
	}else if(startX>endX && startY>endY){
		// This is a left-to-right step down after swapping points:
		double temp;
		temp=endX; endX=startX; startX=temp;
		temp=endY; endY=startY; startY=temp;
		stepDown=true;
	}else if(startX>endX && startY<endY){
		// This is a left-to-right step up after swapping points
		double temp;
		temp=endX; endX=startX; startX=temp;
		temp=endY; endY=startY; startY=temp;
		stepDown=false;
	}
	
	if(isConsanguinous){
		if(stepDown){
			// left to right step down:
			dc.drawHorizontalDoubleLine(startY,startX,midX,'|','/');
			dc.drawVerticalDoubleLine(midX,startY,endY,'/','/');
			dc.drawHorizontalDoubleLine(endY,midX,endX,'/','|');
		}else{
			// left to right step up:
			dc.drawHorizontalDoubleLine(startY,startX,midX,'|','\\');
			dc.drawVerticalDoubleLine(midX,startY,endY,'\\','\\');
			dc.drawHorizontalDoubleLine(endY,midX,endX,'\\','|');
		}
	}else{
		if(stepDown){
			// left to right step down:
			dc.drawHorizontalLine(startY,startX,midX);
			dc.drawVerticalLine(midX,startY,endY);
			dc.drawHorizontalLine(endY,midX,endX);
		}else{
			// left to right step up:
			dc.drawHorizontalLine(startY,startX,midX);
			dc.drawVerticalLine(midX,startY,endY);
			dc.drawHorizontalLine(endY,midX,endX);
		}
	}
	
	//
	// TEMPORARY RETURN POSITION:
	//
	return;
	
	double verticalTick = DrawingMetrics::getVerticalTick();
	double radius = DrawingMetrics::getIconRadius();
	double startOffset,endOffset;
	startOffset = endOffset = -1*verticalTick;
	double extension;

	if(isConsanguinous){
		startY += verticalTick/2;
		endY += verticalTick/2;
	}
	if(endX > startX){ 
		extension = (DrawingMetrics::getHorizontalInterval() - radius - verticalTick) * -1;
		if(endY > startY){ extension += -verticalTick; startOffset*=-1;endOffset*=-1;} 
		if(multipleSpouseOffset) multipleSpouseOffset = multipleSpouseOffset + extension; 
	}else{
		extension = DrawingMetrics::getHorizontalInterval() - radius - verticalTick; 
		if(singleChild){
			extension += DrawingMetrics::getHorizontalInterval();
		}
		if(endY > startY){ 
			extension += verticalTick; 
		}else { 
			endOffset *= -1; startOffset *= -1;
		}
		if(multipleSpouseOffset) multipleSpouseOffset = multipleSpouseOffset + extension; 
	}
	
	dc.drawHorizontalLine(startY,startX,endX+extension-multipleSpouseOffset);
	dc.drawHorizontalLine(endY,endX,endX+extension-multipleSpouseOffset);
	dc.drawVerticalLine(endX+extension-multipleSpouseOffset,startY,endY);
	if(isConsanguinous){
		dc.drawHorizontalLine(startY-verticalTick,startX,endX+extension-multipleSpouseOffset+startOffset);
		dc.drawHorizontalLine(endY-verticalTick,endX,endX+extension-multipleSpouseOffset+endOffset);
		dc.drawVerticalLine(endX+extension-multipleSpouseOffset+endOffset,startY-verticalTick,endY-verticalTick);
	}
	
}

//
// _drawConsanguinousAndExternalConnectors:
//
void Pedigree::_drawConsanguinousAndExternalConnectors(DrawingCanvas& dc){
	
	std::set<NuclearFamily*,compareNuclearFamily>::iterator nfIt = _nuclearFamilies.begin();
	Individual* mother,*father;
	// NOTE: All the connectors are drawn on a different layer
	// However all the dashed individuals go into the body
	
	//
	// This 'uniqueId' will be displayed as a 
	// hexavigesimal bijective number : 
	// A-Z,AA-ZZ,AAA-ZZZ, etc.
	//
	unsigned int uniqueId=1; 
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
							// Draw a stepped connector line
							_drawSteppedConnectorLine(mother->getY(),father->getY(),mother->getX()-iconInterval+radius,father->getX()+radius,(*nfIt)->isConsanguinous(),dc,0.0,singleChild);
						}
					}else{
						if(_hasIndividualAtPosition(father,mother)){
							dc.drawIndividual(father,mother->getX()-iconInterval,mother->getY(),true);
							_drawConsanguinityLetter(mother,father,uniqueId,iconInterval,iconDiameter,individualConsanguinityLetter,dc,0,true);
						}else{
							if(mother->getY() == father->getY()){
								_drawHorizontalConnectorLine(mother->getY(),father->getX()+radius,mother->getX()-radius,(*nfIt)->isConsanguinous(),dc);
							}else{
								// Check if father is the only child
								bool singleChild=false;
								std::vector<std::string> temp = father->getMother()->getChildrenIds(father->getFather());
								if(temp.size() == 1) singleChild = true;
								_drawSteppedConnectorLine(father->getY(),mother->getY(),father->getX()+radius,mother->getX()-iconInterval+radius,(*nfIt)->isConsanguinous(),dc,0.0,singleChild);
							}
						}
					}
				}else
				if(father->getX() < mother->getX()){
					dc.drawIndividual(father,mother->getX()+iconInterval,mother->getY(),true);
					_drawConsanguinityLetter(mother,father,uniqueId,iconInterval,iconDiameter,individualConsanguinityLetter,dc);
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
						_drawSteppedConnectorLine(mother->getY(),father->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc,multipleSpouseOffset);
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
							_drawSteppedConnectorLine(mother->getY(),father->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc,multipleSpouseOffset);
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
							_drawSteppedConnectorLine(mother->getY(),father->getY(),mother->getX()+horizontalInterval,father->getX()-radius,(*nfIt)->isConsanguinous(),dc,multipleSpouseOffset);
						}
					}else{
						if(_hasIndividualAtPosition(mother,father)){
							dc.drawIndividual(father,mother->getX()+iconInterval,mother->getY(),true);
							_drawConsanguinityLetter(mother,father,uniqueId,iconInterval,iconDiameter,individualConsanguinityLetter,dc);
						}else{
							if(mother->getY() == father->getY()){
								_drawHorizontalConnectorLine(mother->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc); 
							}else{
								_drawSteppedConnectorLine(mother->getY(),father->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc);
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
							// Stepped connector line:
							if(i==0){
								_drawSteppedConnectorLine(mother->getY(),father->getY(),mother->getX()+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc);
							}else{
								_drawSteppedConnectorLine(mother->getY(),father->getY(),mother->getX()+rw*horizontalInterval+iconInterval-radius,father->getX()-radius,(*nfIt)->isConsanguinous(),dc);
							}
						}
					}else{
						if(i==0){
							dc.drawIndividual(father,mother->getX()+iconInterval,mother->getY(),true);
							_drawConsanguinityLetter(mother,father,uniqueId,iconInterval,iconDiameter,individualConsanguinityLetter,dc);
						}else{
							dc.drawIndividual(father,mother->getX()+rw*horizontalInterval+iconInterval,mother->getY(),true);
							_drawConsanguinityLetter(mother,father,uniqueId,iconInterval,iconDiameter,individualConsanguinityLetter,dc,rw*horizontalInterval+iconInterval);
						}
					}
				}else{
					if(fabs((mother->getX()-(lw+lastlw)*horizontalInterval)-father->getX()) < Number::MINIMUM_DIFFERENCE){
						
						if(mother->getY() == father->getY()){
							// Horizontal line
							_drawHorizontalConnectorLine(mother->getY(),father->getX()+radius,mother->getX()-(lw)*horizontalInterval+radius,(*nfIt)->isConsanguinous(),dc);
						}else{
							// Stepped connector line
							_drawSteppedConnectorLine(mother->getY(),father->getY(),father->getX()+iconInterval+radius,father->getX()+radius,(*nfIt)->isConsanguinous(),dc);
						}
					
					}else{
						if(mother->getY() != father->getY() || _hasIndividualAtPosition(father,mother)){
							dc.drawIndividual(father,mother->getX()-lw*horizontalInterval,mother->getY(),true);
							_drawConsanguinityLetter(mother,father,uniqueId,iconInterval,iconDiameter,individualConsanguinityLetter,dc,lw*horizontalInterval*(-1.0));
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


// ********************************
//
// Public methods:
//
// ********************************

//
// addIndividual:
//
Individual * Pedigree::addIndividual(const std::string &ind,std::string mother,std::string father,std::string gender,int rowIndex,int tableIndex ,const DataTable& pedigreeTable) {
	//
	// for warnings:
	//
	const char *methodName="Pedigree::addIndividual()";
	if(ind == "."){
		Warning(methodName,"Id is missing for individual with father %s and mother %s. This individual will be ignored.",father.c_str(),mother.c_str());
		return 0;
	}
	//
	// Check for mother/father IDs being identical:
	//
	if(mother != "." && mother != ind && mother == father){
		//
		// check if the mother/father is already in the pedigree
		// If yes, check for gender and set the other parent to missing
		// else change the gender of one of the parents to missing
		//
		Individual tempIndividual(mother);
		std::set<Individual*,compareIndividual>::iterator it = _individuals.find(&tempIndividual);
		
		if(it != _individuals.end()){
			if((*it)->getGender().getEnum() == Gender::MALE){
				mother = ".";
				Warning(methodName,"Both mother and father have the same id. Mother of %s set to MISSING.",ind.c_str());
			}else{
				father = ".";
				Warning(methodName,"Both mother and father have the same id. Father of %s set to MISSING.",ind.c_str());
			}
		}else{
			throw Exception("addIndividual()","Both mother and father of %s have the same id. For founders use '.' as the id.",ind.c_str());
		}
	}else if(ind == mother || ind == father){
		//
		// If the mother/father id is the same as the individual
		// set the conflicting parent to missing
		//
		if(ind == mother){
			Warning(methodName,"Mother and Individual Ids for %s are the same. Set Mother to MISSING.",mother.c_str());
			mother = ".";
		}
		if(ind == father){
			Warning(methodName,"Father and Individual Ids for %s are the same. Set Father to MISSING.",father.c_str());
			father = ".";
		}
	}
	//
	// new candidate individual:
	//
	Individual *newCandidateIndividual = new Individual(ind,mother,father,gender,rowIndex,tableIndex);
	//
	// Check whether individual already exists in pedigree:
	//
	std::set<Individual*,compareIndividual>::iterator it = _individuals.find(newCandidateIndividual);
	if(it==_individuals.end()){
		//
		// Add new individual to pedigree
		//
		std::pair<std::set<Individual*,compareIndividual>::iterator,bool> iit;
		iit =  _individuals.insert(newCandidateIndividual); 
		if(iit.second){
			(*iit.first)->setPedigreeDataTable(&pedigreeTable);
			return *iit.first;
		}else{
			throw Exception(methodName,"Unable to add individual %1$s to pedigree %2$s",ind.c_str(),_id.c_str());
		}
	}else{
		//
		// Report replicated individual:
		//
		Warning(methodName,"Individual %1$s already exists in pedigree %2$s: Ignoring replicated individual from data file.",ind.c_str(),_id.c_str());
		delete newCandidateIndividual;
		return *it;
	}
}

///
/// setCoreOptionalFields: Sets core optional fields (if present) like deceased, proband, affected on the individual.
///
void Pedigree::setCoreOptionalFields(const DataTable* pedigreeTable){
	
	bool hasAffectedColumn          = false;
	bool hasCarrierColumn           = false;
	bool hasCollapsedColumn         = false; // 2015.08.21.ET
	bool hasConsultandColumn        = false;
	bool hasDeceasedColumn          = false;
	bool hasDOBColumn               = false;
	bool hasDonorColumn             = false; // 2016.01.12.ET
	bool hasInfertilityColumn       = false; // 2009.05.19.ET
	bool hasPregnancyColumn         = false;
	bool hasProbandColumn           = false;
	bool hasObligateCarrierColumn   = false;
	bool hasRelationshipEndedColumn = false; // 2009.05.11.ET
	bool hasSampledColumn           = false;
	bool hasSampleQuantityColumn    = false; // 2015.08.21.ET
	bool hasSterilityColumn         = false; // 2009.05.19.ET
	
	DataColumn *affectedColumn;
	DataColumn *carrierColumn;
	DataColumn *collapsedColumn;
	DataColumn *consultandColumn;
	DataColumn *deceasedColumn;
	DataColumn *dobColumn;
	DataColumn *donorColumn;
	DataColumn *infertilityColumn;
	DataColumn *pregnancyColumn;
	DataColumn *probandColumn;
	DataColumn *obligateCarrierColumn;
	DataColumn *relationshipEndedColumn;
	DataColumn *sampledColumn;
	DataColumn *sampleQuantityColumn;
	DataColumn *sterilityColumn;
	
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
	
	// Check if Consultand column exists in the datatable
	if(pedigreeTable->getConsultandColumnIndex() != DataTable::COLUMN_IS_MISSING){
		consultandColumn = pedigreeTable->getColumn(pedigreeTable->getConsultandColumnIndex());
		hasConsultandColumn = true;
	}
	
	// Check if Pregnancy column exists in the datatable
	if(pedigreeTable->getPregnancyColumnIndex() != DataTable::COLUMN_IS_MISSING){
		pregnancyColumn = pedigreeTable->getColumn(pedigreeTable->getPregnancyColumnIndex());
		hasPregnancyColumn = true;
	}
	
	// Check if Carrier column exists in the datatable
	if(pedigreeTable->getCarrierColumnIndex() != DataTable::COLUMN_IS_MISSING){
		carrierColumn = pedigreeTable->getColumn(pedigreeTable->getCarrierColumnIndex());
		hasCarrierColumn = true;
	}
	// Check if ObligateCarrier column exists in the datatable
	if(pedigreeTable->getObligateCarrierColumnIndex() != DataTable::COLUMN_IS_MISSING){
		obligateCarrierColumn = pedigreeTable->getColumn(pedigreeTable->getObligateCarrierColumnIndex());
		hasObligateCarrierColumn = true;
	}
	
	// Check if RelationshipEnded column exists in the datatable:
	if(pedigreeTable->getRelationshipEndedColumnIndex() != DataTable::COLUMN_IS_MISSING){
		relationshipEndedColumn = pedigreeTable->getColumn(pedigreeTable->getRelationshipEndedColumnIndex());
		hasRelationshipEndedColumn = true;
	}
	
	// 2009.05.19.ET Addendum:
	// Check if Infertility column exists in the datatable:
	if(pedigreeTable->getInfertilityColumnIndex() != DataTable::COLUMN_IS_MISSING){
		infertilityColumn    = pedigreeTable->getColumn(pedigreeTable->getInfertilityColumnIndex());
		hasInfertilityColumn = true;
	}
	
	// 2009.05.19.ET Addendum:
	// Check if Sterility column exists in the datatable:
	if(pedigreeTable->getSterilityColumnIndex() != DataTable::COLUMN_IS_MISSING){
		sterilityColumn    = pedigreeTable->getColumn(pedigreeTable->getSterilityColumnIndex());
		hasSterilityColumn = true;
	}
	
	// Check if Affected column exists in the datatable
	if(pedigreeTable->getAffectedColumnIndex() != DataTable::COLUMN_IS_MISSING){
		affectedColumn = pedigreeTable->getColumn(pedigreeTable->getAffectedColumnIndex());
		hasAffectedColumn = true;
	}

	// Check if Collapsed column exists in the datatable
	if(pedigreeTable->getCollapsedColumnIndex() != DataTable::COLUMN_IS_MISSING){
		collapsedColumn = pedigreeTable->getColumn(pedigreeTable->getCollapsedColumnIndex());
		hasCollapsedColumn = true;
	}
	
	// Check if SampleQuantity column exists in the datatable
	if(pedigreeTable->getSampleQuantityColumnIndex() != DataTable::COLUMN_IS_MISSING){
		sampleQuantityColumn = pedigreeTable->getColumn(pedigreeTable->getSampleQuantityColumnIndex());
		hasSampleQuantityColumn = true;
	}
	
	// Check if Donor column exists in the datatable
	if(pedigreeTable->getDonorColumnIndex() != DataTable::COLUMN_IS_MISSING){
		donorColumn = pedigreeTable->getColumn(pedigreeTable->getDonorColumnIndex());
		hasDonorColumn = true;
	}
	
	unsigned rowIndex;
	std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	
	while(individualIt != _individuals.end()){
		
		rowIndex = (*individualIt)->getRowIndex();
		
		if(hasAffectedColumn)   (*individualIt)->setAffectionStatus(dynamic_cast<Affected*>(const_cast<Data* const>(affectedColumn->getDataAtIndex(rowIndex))));
		if(hasCarrierColumn)    (*individualIt)->setCarrierStatus(dynamic_cast<Carrier*>(const_cast<Data* const>(carrierColumn->getDataAtIndex(rowIndex))));
		if(hasCollapsedColumn)  (*individualIt)->setCollapsedStatus(dynamic_cast<Collapsed*>(const_cast<Data* const>(collapsedColumn->getDataAtIndex(rowIndex))));
		if(hasConsultandColumn) (*individualIt)->setConsultandStatus(dynamic_cast<Consultand*>(const_cast<Data* const>(consultandColumn->getDataAtIndex(rowIndex))));
		if(hasDeceasedColumn)   (*individualIt)->setDeceasedStatus(dynamic_cast<LivingDead*>(const_cast<Data* const>(deceasedColumn->getDataAtIndex(rowIndex))));
		if(hasDOBColumn)        (*individualIt)->setDOB(dynamic_cast<Date*>(const_cast<Data* const>(dobColumn->getDataAtIndex(rowIndex))));
		if(hasDonorColumn)      (*individualIt)->setDonorStatus(dynamic_cast<Donor*>(const_cast<Data* const>(donorColumn->getDataAtIndex(rowIndex))));
		if(hasInfertilityColumn) (*individualIt)->setInfertilityStatus(dynamic_cast<Infertility*>(const_cast<Data* const>(infertilityColumn->getDataAtIndex(rowIndex))));
		if(hasObligateCarrierColumn) (*individualIt)->setObligateCarrierStatus(dynamic_cast<ObligateCarrier*>(const_cast<Data* const>(obligateCarrierColumn->getDataAtIndex(rowIndex))));
		if(hasProbandColumn)    (*individualIt)->setProbandStatus(dynamic_cast<Proband*>(const_cast<Data* const>(probandColumn->getDataAtIndex(rowIndex))));
		if(hasPregnancyColumn)  (*individualIt)->setPregnancyStatus(dynamic_cast<Pregnancy*>(const_cast<Data* const>(pregnancyColumn->getDataAtIndex(rowIndex))));
		if(hasRelationshipEndedColumn) (*individualIt)->setRelationshipEndedStatus(dynamic_cast<RelationshipEnded*>(const_cast<Data* const>(relationshipEndedColumn->getDataAtIndex(rowIndex))));
		if(hasSampledColumn)    (*individualIt)->setSampled(dynamic_cast<Sampled*>(const_cast<Data* const>(sampledColumn->getDataAtIndex(rowIndex))));
		if(hasSampleQuantityColumn) (*individualIt)->setSampleQuantityStatus(dynamic_cast<SampleQuantity*>(const_cast<Data* const>(sampleQuantityColumn->getDataAtIndex(rowIndex))));
		if(hasSterilityColumn)   (*individualIt)->setSterilityStatus(dynamic_cast<Sterility*>(const_cast<Data* const>(sterilityColumn->getDataAtIndex(rowIndex))));
		
		++individualIt;
		
	}
	
}

///
/// checkParentChildDOB: Checks if the parents age is comparable to the ages of their children.
///
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
// checkPregnancyStateValidity()
//
void Pedigree::checkPregnancyStateValidity(){
	
	// for warnings:
	const char *methodName="Pedigree::checkPregnancyStateValidity()";
	
	std::set<Individual*,compareIndividual>::iterator individualIt = _individuals.begin();
	
	while(individualIt != _individuals.end()){
		if( (*individualIt)->pregnancyStateInvalid() ){
			(*individualIt)->removePregnancyFlag();
			Warning(methodName,"Pregnancy flag removed from individual %s because of the presence of a spouse or children.",(*individualIt)->getId().get().c_str() );
		}
		++individualIt;
	}
}

//
// determineFoundingGroups(): Determine the original founding groups in a pedigree
//
// NOTA BENE: This method has been completely rewritten as a complete replacement for the
//            original determineFoundingGroups() method.
//
void Pedigree::determineFoundingGroups(){
	
	// for warnings:
	const char *methodName="Pedigree::determineFoundingGroups2()";
	// counter and sequential ID for descent trees:
	unsigned descentTreeCount = 0;
	// vector of ordinary founders:
	std::vector<Individual*> ordinaryFounders;
	// vector of original founders:
	std::vector<Individual*> originalFounders;
	// Iterator over the _individuals set:
	std::set<Individual*,compareIndividual>::iterator iit;
	//
	// Loop over the individuals and check to see if the
	// ordinary founders are original founders:
	//
	// *=> "Ordinary founders" are individuals who have neither 
	//     a father nor a mother in the data table.
	//
	// *=> "Original founders" are "ordinary founders" whose spouse
	//     is also an ordinary founder. In other words, original
	//     founders are the parents of children who have no 
	//     grandparents in the pedigree. Because there are no
	//     grandparents, the parents form the root or start of 
	//     a descent tree.
	//
	for(iit=_individuals.begin();iit!=_individuals.end();++iit){
		if((*iit)->isOrdinaryFounder()){
			//
			// Loop over this person's spouses:
			// If the spouse is also an ordinary
			// founder, make the pair original founders.
			//
			std::set<Individual*,compareIndividual>::iterator spi;
			for(spi=(*iit)->getSpouses()->begin();spi!=(*iit)->getSpouses()->end();++spi){
				
				if((*spi)->isOrdinaryFounder()){
					if(!(*iit)->isOriginalFounder()) (*iit)->setOriginalFounder(true);
					if(!(*spi)->isOriginalFounder()) (*spi)->setOriginalFounder(true);
					// DEBUG std::cout << ">>> Setting " << (*iit)->getId() << " and " << (*spi)->getId() << " as original founders." << std::endl;
				}
			}
		}
	}
	//
	// Original founders are all now flagged.
	// Now remove their setting as "ordinary founders"
	// (making them exclusively "original founders")
	// and place them into a multiset where they will be:
	// sorted based on number of spouses descending:
	//
	
	std::multiset<Individual *,Individual::compareSpousalCount> allOriginalFounders;
	
	for(iit=_individuals.begin();iit!=_individuals.end();++iit){
		if((*iit)->isOriginalFounder()){
			// DEBUG
			//std::cout << ">>> Setting " << (*iit)->getId() << " as no longer an ordinary founder." << std::endl;
			(*iit)->setOrdinaryFounder(false);
			allOriginalFounders.insert((*iit));
		}
	}
	//
	// Create the vector of descent trees. Individuals with the most
	// spouses are the first to be added (because of the multiset's
	// ordering via the Individual::compareSpousalCount functor):
	//
	unsigned descentTreeId=0;
	std::multiset<Individual *,Individual::compareSpousalCount>::iterator it;
	for(it=allOriginalFounders.begin();it!=allOriginalFounders.end();it++){
		
		// DEBUG
		//std::cout << ">>> " << (*it)->getId() 
		//          << " has spouse count = " << (*it)->getSpouses()->size() 
		//          << " and " << ((*it)->hasBeenVisited()?"has been":"has not been") << " visited."
		//          << std::endl;
		
		if( ! (*it)->hasBeenVisited() ){
			(*it)->setVisited(true);
			descentTreeId++;
			DescentTree *newDescentTree = new DescentTree( descentTreeId );
			newDescentTree->addIndividualToFoundingGroup((*it));
			newDescentTree->setStartIndividual((*it));
			//
			// (1) Set all of the start individual's direct spouses as visited
			//     (so that they won't become startIndividuals to another 
			//     descent tree)
			//
			// (2) Then add them as members of this descentTree founders' group:
			//
			std::set<Individual*,compareIndividual>::iterator spi;
			for(spi=(*it)->getSpouses()->begin();spi!=(*it)->getSpouses()->end();++spi){
				
				(*spi)->setVisited(true);
				newDescentTree->addIndividualToFoundingGroup((*spi));
			}
			//
			// Push the newDescentTree onto the pedigree:
			//
			// DEBUG: std::cout << ">>> DescentTree #" << descentTreeId 
			//             << " with start individual " << (*it)->getId() << " added." << std::endl;
			_descentTrees.push_back( newDescentTree );
		}
	}
	//
	// Prophylactically reset the visit flags for use by other algorithms:
	//
	for(it=allOriginalFounders.begin();it!=allOriginalFounders.end();it++){
		(*it)->setVisited(false);
	}
	//
	// DEBUG: Print all the descent tree ids:
	//
	//std::cout << "# of Descent Trees " << _descentTrees.size() << std::endl;
	//for(unsigned cnt =0;cnt < _descentTrees.size();cnt++){
	//	std::cout << "id: " << _descentTrees[cnt]->getId() << std::endl;
	//	_descentTrees[cnt]->displayFoundingGroup();
	//}
	
	return;
	
}

//
// computePedigreeWidth:
//
void Pedigree::computePedigreeWidth(const std::string& sortField,bool dobSortOrder){
	
	// Determine external and internal connectors:
	_determineConnectorIndividuals();
	// Establish nuclear families for width calculation:
	_establishNuclearFamilies();
	_checkMarkedTwinsDOB();
	if(sortField != std::string("")){
		_sortNuclearFamiliesBasedOnDataField(sortField,dobSortOrder);
	}
	// Calculate descent tree width
	_sortAndCalculateDescentTreeWidth();
	// set left spouse connections flags
	_setLeftShiftConnectionFlags();
	
}

///
/// draw: Draws a pedigree for each family on a separate SVG canvas and outputs them to separate files.
/// The output file names correspond to the family ids.
///
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
	_drawConsanguinousAndExternalConnectors(dc);
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
		if(drawingFileName.empty()) drawingFileName = _id+"_pedigree";
		
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
	drawingFileName += DrawingMetrics::getDrawingFileNameExtension();
	
	//
	// Print out the drawing:
	//
	dc.show(drawingFileName.c_str());
	std::cout << "Pedigree output file is “" << drawingFileName << "”" << std::endl; 
	
}


//
// addIdentifiedMissingParents() 
//
//            Before establishing connections, add any parents
//            who are identified by an ID in the FATHER or MOTHER 
//            column but who don't have rows in the data table.
//            These are what Madeline calls "virtual parents".
//
// NOTA BENE: This was previously done in establishIndividualConnections()
//            but it is better to do it here as a prerequisite step so that
//            all parental connections can subsequently be done in a single 
//            pass. 
//
void Pedigree::addIdentifiedMissingParents(){
	
	// for warnings:
	const char *methodName="Pedigree::addIdentifiedMissingParents()";
	
	std::set<Individual*,Individual::compareIndividual> missingParents;
	std::set<Individual*,Individual::compareIndividual>::iterator iit;
	
	for(iit=_individuals.begin();iit != _individuals.end();++iit){
		//
		// Find identified missing fathers:
		//
		if(!(*iit)->getFatherId().isMissing()){
			Individual findFather((*iit)->getFatherId().get());
			std::set<Individual*,Individual::compareIndividual>::iterator fatherIt = _individuals.find(&findFather);
			if(fatherIt==_individuals.end()){
				// Check if we have already added this virtual father into the missingParents set:
				fatherIt = missingParents.find(&findFather);
				if(fatherIt==missingParents.end()){
					Warning(methodName,"Adding virtual father %s who is not present in the input data file.",(*iit)->getFatherId().get().c_str());
					Individual *newVirtualFather = new Individual((*iit)->getFatherId().get(),".",".","M",-1,-1);
					newVirtualFather->setOrdinaryFounder(true);
					newVirtualFather->setVirtualIndividual(true);
					missingParents.insert(newVirtualFather);
				}
			}
		}
		//
		// Find identified missing mothers:
		//
		if(!(*iit)->getMotherId().isMissing()){
			Individual findMother((*iit)->getMotherId().get());
			std::set<Individual*,Individual::compareIndividual>::iterator motherIt = _individuals.find(&findMother);
			if(motherIt==_individuals.end()){
				// Check if we have already added this virtual mother into the missingParents set:
				motherIt = missingParents.find(&findMother);
				if(motherIt==missingParents.end()){
					Warning(methodName,"Adding virtual mother %s who is not present in the input data file.",(*iit)->getMotherId().get().c_str());
					Individual *newVirtualMother = new Individual((*iit)->getMotherId().get(),".",".","F",-1,-1);
					newVirtualMother->setOrdinaryFounder(true);
					newVirtualMother->setVirtualIndividual(true);
					missingParents.insert(newVirtualMother);
				}
			}
		}
	}
	//
	// Now append the missingParents directly to the _individuals set:
	//
	std::set<Individual*,compareIndividual>::iterator mpi;
	for(mpi=missingParents.begin();mpi!=missingParents.end();++mpi){
		_individuals.insert((*mpi));
	}
}

///
/// establishIndividualConnections: Establish parent/child and spouse connections:
///
void Pedigree::establishIndividualConnections(){
	
	// for warnings:
	const char *methodName="Pedigree::establishIndividualConnections()";
	
	std::vector<Individual*> individualsMissingParentInformation;
	std::set<Individual*,compareIndividual>::iterator individualIt;
	
	for(individualIt=_individuals.begin();individualIt != _individuals.end();++individualIt){
		if((*individualIt)->getFatherId().isMissing() && (*individualIt)->getMotherId().isMissing()){
			//
			// Case where both parents are missing: individual is an ordinary founder
			//
			(*individualIt)->setOrdinaryFounder(true);
		}else if((*individualIt)->getMotherId().isMissing()){
			//
			// Case where the mother is missing
			//
			individualsMissingParentInformation.push_back(*individualIt);
			//
			// Add the father link
			//
			Individual findFather((*individualIt)->getFatherId().get());
			std::set<Individual*,compareIndividual>::iterator fatherIt = _individuals.find(&findFather);
			if(fatherIt != _individuals.end()){
				(*individualIt)->setFather(*fatherIt);
				(*fatherIt)->addChild(*individualIt);
			}else{
				throw Exception(methodName,"Virtual father %s is not present in the set of individuals.",(*individualIt)->getFatherId().get().c_str());
				//Warning(methodName,"Adding virtual father %s who is not present in the input data file.",(*individualIt)->getFatherId().get().c_str());
				//Individual *newVirtualFather = new Individual((*individualIt)->getFatherId().get(),".",".","M",-1,-1);
				//(*individualIt)->setFather(newVirtualFather);
				//newVirtualFather->addChild(*individualIt);
				//newVirtualFather->setOrdinaryFounder(true);
				//newVirtualFather->setVirtualIndividual(true);
			}
		}else if((*individualIt)->getFatherId().isMissing()){
			//
			// Case where the father is missing
			//
			individualsMissingParentInformation.push_back(*individualIt);
			//
			// Add the mother link
			//
			Individual findMother((*individualIt)->getMotherId().get());
			std::set<Individual*,compareIndividual>::iterator motherIt = _individuals.find(&findMother);
			if(motherIt != _individuals.end()){
				(*individualIt)->setMother(*motherIt);
				(*motherIt)->addChild(*individualIt);
			}else{
				throw Exception(methodName,"Virtual mother %s is not present in the set of individuals.",(*individualIt)->getMotherId().get().c_str());
				//Warning(methodName,"Adding virtual mother %s who is not present in the input data file.",(*individualIt)->getMotherId().get().c_str());
				//Individual *newVirtualMother = new Individual((*individualIt)->getMotherId().get(),".",".","F",-1,-1);
				//(*individualIt)->setMother(newVirtualMother);
				//newVirtualMother->addChild(*individualIt);
				//newVirtualMother->setOrdinaryFounder(true);
				//newVirtualMother->setVirtualIndividual(true);
			}
		}else{
			//
			// Case where both parents are present: establish mother and father connections:
			//
			Individual findFather((*individualIt)->getFatherId().get());
			Individual findMother((*individualIt)->getMotherId().get());
			std::set<Individual*,compareIndividual>::iterator fatherIt = _individuals.find(&findFather);
			std::set<Individual*,compareIndividual>::iterator motherIt = _individuals.find(&findMother);
			if(fatherIt != _individuals.end()){
				(*individualIt)->setFather(*fatherIt);
				(*fatherIt)->addChild(*individualIt);
			}else{
				Warning(methodName,"Father %s row not present in the input data file.",(*individualIt)->getFatherId().get().c_str());
				//
				// Add a virtual father with id from the father column
				//
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
				//
				// Add a virtual mother with id from the mother column
				//
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
		}
		
		///////////////////////////////////////////////////////////
		//
		// 2009.05.22.ET
		//
		// Code to connect adopted in/out complement individuals:
		//
		///////////////////////////////////////////////////////////
		
		std::string id        = (*individualIt)->getId().get();
		const char first_char = id[0];
		if(first_char=='+' || first_char=='-'){
			
			//std::cerr << ">>>>> START SEARCH FOR ADOPTED COMPLEMENT OF " << id << "\n";
			
			//
			// Flip the adopted "in" and "out" flags 
			// so we can search for the complement:
			//
			if     (id[0]=='+') id[0]='-';
			else if(id[0]=='-') id[0]='+';
			Individual* complementIndividual = new Individual( id );
			std::set<Individual*,compareIndividual>::iterator adoptedComplementIt = _individuals.find(complementIndividual);
			if(adoptedComplementIt != _individuals.end()){
				
				(*individualIt)->setAdoptedComplement( (*adoptedComplementIt) );
				
				//std::cerr << ">>>>> FOUND : COMPLEMENT OF " << (*individualIt)->getId().get() << " IS " << (*adoptedComplementIt)->getId().get() << "\n";
				
			}
		}
	}
	
	/////////////////////////////////////////////////////
	//
	// Fix up cases where a single parent is missing:
	//
	/////////////////////////////////////////////////////
	if(individualsMissingParentInformation.size()){
		Warning(methodName,"One or more children in the pedigree have only one parent: Consider augmenting your data set.");
	}
	for(unsigned cnt=0;cnt<individualsMissingParentInformation.size();cnt++){
		RandomId randomIdGenerator;
		Individual *individual = individualsMissingParentInformation[cnt];
		if(individual->getMotherId().isMissing()){
			//
			// Case where mother is missing:
			//
			Individual* father = individual->getFather();
			if(father){
				Individual* spouse = father->getFirstSpouse();
				if(spouse){
					individual->setMotherId(spouse->getId().get());
					individual->setMother(spouse);
					spouse->addChild(individual);
				}else{
					//
					// Id of the missing mother cannot be determined.
					// Add a virtual mother with a random id:
					//
					std::string randomId = randomIdGenerator.get();
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
			//
			// Case where father is missing:
			//
			Individual* mother = individual->getMother();
			if(mother){
				Individual* spouse = mother->getFirstSpouse();
				if(spouse){
					individual->setFatherId(spouse->getId().get());
					individual->setFather(spouse);
					spouse->addChild(individual);
				}else{
					//
					// Id of the missing father cannot be determined.
					// Add a virtual father with a random id
					//
					std::string randomId = randomIdGenerator.get();
					std::pair<std::set<Individual*,compareIndividual>::iterator,bool> p;
					p = _individuals.insert(new Individual(randomId,".",".","M",-1,-1));
					(*p.first)->addChild(individual);
					individual->setFatherId(randomId);
					individual->setFather(*p.first);
					mother->addSpouse(*p.first);
					(*p.first)->addSpouse(mother);
					(*p.first)->setOrdinaryFounder(true);
					(*p.first)->setVirtualIndividual(true);
					Warning(methodName,"Father of %s could not be determined from the input data file. %s has been set as the father.",individual->getId().get().c_str(),randomId.c_str());
				}
			_checkParentsGender(individual);
			}
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////
	//
	// Prophylactically check for individuals who are there own ancestors - Yikes!
	//
	// => This is an insidious type of data error that can send the program into an
	//    infinite loop: not good!
	//
	///////////////////////////////////////////////////////////////////////////////
	for(individualIt = _individuals.begin();individualIt != _individuals.end();individualIt++){
		
		_anomolous.clear(); // clear list of visited individuals who might be problematic
		clearVisitCounts();
		
		//std::cerr << "checking ancestors of " << (*individualIt)->getId() << std::endl;
		if((*individualIt)->getFather()){
			//std::cerr << "calling check on FATHER..." << std::endl;
			checkForAncestorDescendantAnomoly((*individualIt)->getFather());
		}
		if((*individualIt)->getMother()){
			//std::cerr << "calling check on MOTHER..." << std::endl;
			checkForAncestorDescendantAnomoly((*individualIt)->getMother());
		}
		
	}
}


//
// Debug Methods:
//
void Pedigree::display() const{
	
	std::cout << "Number of Individuals in Pedigree :" << _id << " = " << _individuals.size() << std::endl;
	std::set<Individual*,compareIndividual>::const_iterator it = _individuals.begin();
	while(it != _individuals.end()){
		std::cout << (*it)->getId().get() << std::endl;
		//(*it)->display();
		//(*it)->displaySpouses();
		//(*it)->displayChildren();
		it++;
	}
	
}

//
// clearVisitCounts
//
void Pedigree::clearVisitCounts(){
	
	std::set<Individual*,compareIndividual>::iterator it;
	
	for(it=_individuals.begin(); it != _individuals.end(); it++ ){
		(*it)->resetVisitCount();
	}
	
}

//
// checkForAncestorDescendantAnomoly
//
void Pedigree::checkForAncestorDescendantAnomoly(Individual *ancestor){
	
	//
	// Return if NULL pointer - This case is prophylactic and should not occur:
	//
	if(!ancestor){
		return;
	}
	
	// 
	// Return if individual is a founder because only individuals with both ancestors
	// and descendants can be part of such anomolous ancestor-is-also-descendant error cases:
	//
	if( !(ancestor->getFather() && ancestor->getMother()) ){
		return;
	}
	
	//std::cerr << "checking FATHER " << ancestor->getFatherId() << " ptr is " << ancestor->getFather() << std::endl;
	//std::cerr << "checking MOTHER " << ancestor->getMotherId() << " ptr is " << ancestor->getMother() << std::endl;
	
	//
	// If the ancestor has already been counted at least once, then
	// we know we have either:
	// 
	//    (1) Entered an anomolous ancestor-is-also-descendant loop.
	// or (2) Passed an ancestor in a consanguineous loop.
	//
	if( ancestor->getVisitCount() ){
		if( ancestor->getVisitCount()>1 ){
			//
			// Stop if this is the second time around the loop:
			//
			std::vector<Individual *>::const_iterator an_it;
			
			for(an_it = _anomolous.begin();an_it!=_anomolous.end();an_it++){
				Warning("Pedigree::checkForAncestorDescendantAnomoly()","Individual %s is part of an impossible ancestor-descendant loop!",(*an_it)->getId().get().c_str() );
			}
			throw Exception("Pedigree::checkForAncestorDescendantAnomoly()","An impossible ancestor-is-also-descendant loop exists in the data file.");
		}else{
			//
			// Keep tabs on all participants who may be in an anomalous ancestor-is-also-descendant loop.
			// However, because it is possible to visit an ancestor more than once when a valid consanguineous
			// loop exists, we don't issue immediate warnings; but rather, as shown here, we
			// save the list and, if it turns out to truly be an error loop, then we issue the warnings
			// immediately before throwing an exception,
			//
			_anomolous.push_back(ancestor);
			
		}
	}
	
	//
	// Here is the tracking key: incrementing the visit counter
	//
	ancestor->incrementVisitCount();
	
	//
	// Recurse up ancestors on both dad's and mom's sides:
	//
	if(ancestor->getFather()){
		checkForAncestorDescendantAnomoly(ancestor->getFather());
	}
	if(ancestor->getMother()){
		checkForAncestorDescendantAnomoly(ancestor->getMother());
	}
	
}



