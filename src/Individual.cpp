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
// 2005.08.31.ET
//

//
// Individual.cpp
//

#include "Individual.h"
#include "NuclearFamily.h"
#include "Debug.h"
#include <cstdlib>
#include <ctime>

bool Individual::_sampledColumnPresent = false;

//////////////////////////////////////
//
// CONSTRUCTORS
//
//////////////////////////////////////

//
// _init() : Set private members
//           common to all of the constructors
//
inline void Individual::_init(void){
	
	_isOriginalFounder = false;
	_isOrdinaryFounder = false;
	_isVirtual = false;
	_isConsanguinous = false;
	_isUnconnected = false;
	_hasBeenVisited = false;
	_hasBeenDrawn = false;
	_hasExternalConnection = false;
	_dob = 0;
	_deceased = 0;
	_affected = 0;
	_proband = 0;
	_sampled = 0;
	_birthOrder=0;
	
	_leftWidth = _rightWidth = 1;
	_totalWidth= _leftWidth + _rightWidth;
	
	_leftSideOfLoop=0;
	_rightSideOfLoop=0;
	_leftSideOfExternalConnection=0;
	_rightSideOfExternalConnection = 0;
	_leftSpouseConnector = false;
	
	_individualIndicatingNoOffspring=false;
	_individualIndicatingNonFertility=false;
	_individualIndicatingTerminatedPregnancy=false;
	_isSpecial=false;
	
}

//
// _checkForSpecialIndividualTypes
//
void Individual::_checkForSpecialIndividualTypes(void){
	
	if( _id.isMissing() ) return;
	switch(_id[0]){
	case '!':
		// VIRTUAL: FOR TESTING PURPOSES:
		_isVirtual = true;
		break;
	case '^':
		// MARRIAGE WITH NO OFFSPRING:
		_individualIndicatingNoOffspring=true;
		_isSpecial=true;
		break;
	case '&':
		// NON-FERTILITY:
		_individualIndicatingNonFertility=true;
		_isSpecial=true;
		break;
	case '@':
		// TERMINATED PREGNANCY: Spontaneous or other abortion
		// or ectopic pregnancy
		//
		_individualIndicatingTerminatedPregnancy=true;
		_isSpecial=true;
		break;
	}
	
}

//
// Individual()
//
Individual::Individual(){
	
	_init();
	
}

//
// Individual()
//
Individual::Individual(const std::string & id){ 
	
	_init();
	_id.set(id);
	_checkForSpecialIndividualTypes();
}

//
// Individual()
//
Individual::Individual(const std::string & id, int rowIndex, int tableIndex){
	
	_init();
	_id.set(id);
	_checkForSpecialIndividualTypes();
	_rowIndex   = rowIndex;
	_tableIndex = tableIndex;
	
}

//
// Individual()
//
Individual::Individual(const std::string &id, const std::string &motherId, const std::string &fatherId, const std::string &gender){
	
	_init();
	_id.set(id);
	_checkForSpecialIndividualTypes();
	_motherId.set(motherId);
	_fatherId.set(fatherId);
	_gender.set(gender);
	
}

//
// Individual()
//
Individual::Individual(const std::string &id,const std::string &motherId, const std::string &fatherId, const std::string &gender,int rowIndex,int tableIndex){
	
	_init();
	_id.set(id);
	_checkForSpecialIndividualTypes();
	_motherId.set(motherId);
	_fatherId.set(fatherId);
	_gender.set(gender);
	_rowIndex   = rowIndex;
	_tableIndex = tableIndex;
	
}

//////////////////////////////////////
//
// PUBLIC METHODS
//
//////////////////////////////////////

//
// getNuclearFamily:
//
NuclearFamily* Individual::getNuclearFamily(unsigned index){
	
	return _nuclearFamilies[index];
	
}

//
// getRandomId: for virtual Individuals
//
std::string Individual::getRandomId() {
	
	srand((unsigned)time(0));
	char ch;
	std::string temp;
	temp = "!";
	for(int index=1; index<8; index++){
		while((ch='0'+rand()%43)>=':' && ch<='@');
		temp.append(1,ch);
	} 
	temp.append("!");
	return temp;
	
}

//
// getSpouseIds:
//
std::vector<std::string> Individual::getSpouseIds(){
	
	std::vector<std::string> temp;
	std::set<Individual*,compareIndividual>::iterator spouseIt = _spouses.begin();
	while(spouseIt != _spouses.end()){
		temp.push_back((*spouseIt)->getId().get());
		++spouseIt;
	}
	return temp;
}

//
// getSpouses:
//
const std::set<Individual*,Individual::compareIndividual> *const Individual::getSpouses(void) const{
	
	return &_spouses;
	
}

//
// getChildrenIds:
//
std::vector<std::string> Individual::getChildrenIds(){
	
	std::vector<std::string> temp;
	std::set<Individual*,compareIndividual>::iterator childrenIt = _children.begin();
	while(childrenIt != _children.end()){
		temp.push_back((*childrenIt)->getId().get());
		++childrenIt;
	}
	return temp;
}

//
// getChildren:
//
const std::set<Individual*,Individual::compareIndividual> *const Individual::getChildren(void) const{
	
	return &_children;
	
}


//
// getChildrenIds: with a given spouse
//
std::vector<std::string> Individual::getChildrenIds(Individual* spouse){
	
	// Return the children Ids based on default ordering
	std::vector<std::string> temp;
	std::set<Individual*,compareIndividual>::iterator childrenIt = _children.begin();
	while(childrenIt != _children.end()){
		if(_gender.get() == "M"){
			if((*childrenIt)->getMother() == spouse && (*childrenIt)->getFather() == this)
				temp.push_back((*childrenIt)->getId().get());
			
		}else if((*childrenIt)->getFather() == spouse && (*childrenIt)->getMother() == this)
			temp.push_back((*childrenIt)->getId().get());
		++childrenIt;
	}
	return temp;
	
}

void Individual::getChildrenWithSpouse(Individual* spouse,std::vector<Individual*>& children){
	
	std::set<Individual*,compareIndividual>::iterator childrenIt = _children.begin();
	while(childrenIt != _children.end()){
		if(_gender.get() == "M"){
			if((*childrenIt)->getMother()->getId() == spouse->getId() && (*childrenIt)->getFather()->getId() == _id)
				children.push_back((*childrenIt));
			
		}else if((*childrenIt)->getFather()->getId() == spouse->getId() && (*childrenIt)->getMother()->getId() == _id)
			children.push_back((*childrenIt));
		++childrenIt;
	}
	
}


void Individual::getChildrenSortedByExternalConnections(Individual* spouse,std::vector<Individual*>& children){
	
	// If there are multiple DTs the default ordering of the children is altered based on the external flags
	if(_childrenIdsSortedByExternalConnections.size()){
		std::string parentPair;
		if(_gender.getEnum() == Gender::MALE) parentPair = _id.get()+spouse->getId().get();
		else parentPair = spouse->getId().get()+_id.get();
		std::map<std::string,std::vector<Individual*> >::iterator mit = _childrenIdsSortedByExternalConnections.find(parentPair);
		if(mit != _childrenIdsSortedByExternalConnections.end()){
			std::vector<Individual*>::iterator it = (mit->second).begin();
			while(it != (mit->second).end()){
				children.push_back((*it));
				++it;
			}
		}
		return;
	}
	getChildrenWithSpouse(spouse,children);
	
}


//
// getChildrenIdsSortedByExternalConnections: with a given spouse
//
std::vector<std::string> Individual::getChildrenIdsSortedByExternalConnections(Individual* spouse){
	
	// If there are multiple DTs the default ordering of the children is altered based on the external flags
	if(_childrenIdsSortedByExternalConnections.size()){
		
		std::string parentPair;
		if(_gender.getEnum() == Gender::MALE) parentPair = _id.get()+spouse->getId().get();
		else parentPair = spouse->getId().get()+_id.get();
		std::map<std::string,std::vector<Individual*> >::iterator mit = _childrenIdsSortedByExternalConnections.find(parentPair);
		if(mit != _childrenIdsSortedByExternalConnections.end()){
			std::vector<std::string> temp;
			std::vector<Individual*>::iterator it = (mit->second).begin();
			while(it != (mit->second).end()){
				temp.push_back((*it)->getId().get());
				++it;
			}
			//return mit->second;
			return temp;
		}
	}
	return getChildrenIds(spouse);
	
}

//
// getFirstSpouse: for cases with missing mother or father id in the input pedigree data table
//
Individual* Individual::getFirstSpouse(){
	
	std::set<Individual*,compareIndividual>::iterator spouseIt = _spouses.begin();
	return (*spouseIt);
	
}


//
// sortSpouses: If the DT has consanguinity find the optimal ordering of the spouses
//
void Individual::sortSpouses(){
	
	std::deque<Individual*> leftLoopIndividuals;
	std::deque<Individual*> rightLoopIndividuals;
	std::deque<Individual*> initial;
	std::deque<Individual*> result;
	std::vector<Individual*> spouses;
	
	std::vector<NuclearFamily*> temp;
	
	// Read all the spouses into a temporary vector
	std::set<Individual*,compareIndividual>::iterator spouseIt = _spouses.begin();
	
	// Unused variables:
	// unsigned left,right;
	
	while(spouseIt != _spouses.end()){
		spouses.push_back((*spouseIt));
		++spouseIt;
	}
	
	bool consanguinousLoop = true;
	
	// classify the spouses based on the consanguinous flags
	groupIndividualsBasedOn(consanguinousLoop,spouses,initial,leftLoopIndividuals,rightLoopIndividuals,true);
	
	// DEBUG: Display the vectors
	/*std::cout << "MULTIPLE SPOUSE LEFT" << std::endl;
	for(unsigned i=0;i<leftLoopIndividuals.size();i++)
		std::cout << leftLoopIndividuals[i]->getId() << " : " << leftLoopIndividuals[i]->getLeftSideOfLoop() << std::endl;
	std::cout << "MULTIPLE SPOUSE RIGHT " << std::endl;
	for(unsigned i=0;i<rightLoopIndividuals.size();i++)
		std::cout << rightLoopIndividuals[i]->getId() << " : " << rightLoopIndividuals[i]->getRightSideOfLoop() << std::endl;
	std::cout << "MULTIPLE SPOUSE INITIAL" << std::endl;
	for(unsigned i=0;i<initial.size();i++)
		std::cout << initial[i]->getId() << std::endl;
	*/
	
	// Check if the individual is a male and consanguinous
	// In such cases the first spouse should be one that has no loop flags set
	// *** ADDED NOW
	if(_gender.get() == "M" && _isConsanguinous && _spouses.size() > 2){
		if(initial.size()){
			result.push_back(initial.front());
			initial.pop_front();
		}else Warning("Individual:sortSpouses(): A spouse with none of the loop flags set could not be found for %s",_id.get().c_str());
	}
	
	// If there exists a left spouse and a right spouse with the same flag merge them into the result vector
	if(leftLoopIndividuals.size() > 0 && rightLoopIndividuals.size() > 0)
	if(leftLoopIndividuals.back()->getLeftFlag(consanguinousLoop) == rightLoopIndividuals.front()->getRightFlag(consanguinousLoop)){
		if(leftLoopIndividuals.back()->getId() != rightLoopIndividuals.front()->getId()){
			
			// NOTE: Added the below if on 2006-11-14 to enable left and right loop flag individuals to be drawn on the same side of the multiple spouse individual
			if(!initial.empty()){
				
				result.push_back(leftLoopIndividuals.back());
				result.push_back(initial.front());
				initial.pop_front();
				result.push_back(rightLoopIndividuals.front());
				
			}else{
				result.push_back(rightLoopIndividuals.front());
				result.push_back(leftLoopIndividuals.back());
			}
			// delete these two individuals from both the queues
			leftLoopIndividuals.pop_back();
			rightLoopIndividuals.pop_front();
			
		}
	}
	
	// Push all individuals with no flags set into result
	while(!initial.empty()){
		if((leftLoopIndividuals.empty() && !rightLoopIndividuals.empty()) || (!leftLoopIndividuals.empty() && rightLoopIndividuals.empty())){
			if(result.size() % 2 == 0){
				if(!rightLoopIndividuals.empty()){
					result.push_back(rightLoopIndividuals.back());
					rightLoopIndividuals.pop_back();
				}
			}else{
				if(!leftLoopIndividuals.empty()){
					result.push_back(leftLoopIndividuals.back());
					leftLoopIndividuals.pop_back();
				}
			}
		}
		result.push_back(initial.front());
		initial.pop_front();
	}
	// Push individuals in the left and right qs alternately:
	while(leftLoopIndividuals.size() > 0 && rightLoopIndividuals.size() > 0){
		if(result.size() % 2 == 0){
			result.push_back(rightLoopIndividuals.back());
			result.push_back(leftLoopIndividuals.front());
			leftLoopIndividuals.pop_front();
			rightLoopIndividuals.pop_back();
		}else{
			result.push_back(rightLoopIndividuals.back());
			result.push_back(leftLoopIndividuals.front());
			leftLoopIndividuals.pop_front();
			rightLoopIndividuals.pop_back();
		}
	}
	bool even = (result.size() % 2 == 0);
	// Push remaining individuals in the left q
	while(!leftLoopIndividuals.empty()){
		if(even){
			result.push_back(leftLoopIndividuals.front());
			leftLoopIndividuals.pop_front();
		}else{
			result.push_back(leftLoopIndividuals.back());
			leftLoopIndividuals.pop_back();
		}
	}
	// Push remaining individuals in the right q
	
	while(!rightLoopIndividuals.empty()){
		if(even){
			result.push_back(rightLoopIndividuals.back());
			rightLoopIndividuals.pop_back();
		}else{
			result.push_back(rightLoopIndividuals.front());
			rightLoopIndividuals.pop_front();
		}
	}
	
	// DEBUG:
	/*std::cout << " RESULT MUL SPOUSE VECTOR IS " << std::endl;
	for(unsigned j=0;j<result.size();j++)
		std::cout << result[j]->getId() << std::endl;
	*/
	//
	// The reordered spouses are now in the result q.
	// Sort the NFs of the individual based on this new spouse ordering
	// as the NFs ordering will be used to later calculate widths and draw individuals 
	// 
	NuclearFamily* nf;
	for(unsigned j=0;j<result.size();j++){
		// Get the first NF of the spouse
		nf = result[j]->getNuclearFamily((unsigned)0);
		if(_gender.getEnum() == Gender::MALE){
			if(nf->getFather()->getId() == _id) temp.push_back(nf);
			else{
				// This spouse has > 1 NF
				for(unsigned i=1;i<result[j]->getNumberOfNuclearFamilies();i++){
					if(result[j]->getNuclearFamily(i)->getFather()->getId() == _id){
						temp.push_back(result[j]->getNuclearFamily(i));
						break;
					}
				}
			}
		}else{
			if(nf->getMother()->getId() == _id) temp.push_back(nf);
			else{
				// This spouse has > 1 NF 
				for(unsigned i=1;i<result[j]->getNumberOfNuclearFamilies();i++){
					if(result[j]->getNuclearFamily(i)->getMother()->getId() == _id){
						temp.push_back(result[j]->getNuclearFamily(i));
						break;
					}
				}
			}
		}
	}
	// Exchange the two NF vectors:
	_nuclearFamilies.swap(temp);
	
}

//
// getLeftFlag: Get the consanguinity or external left flag 
//
unsigned Individual::getLeftFlag(bool consanguinousLoop){
	
	if(consanguinousLoop) return _leftSideOfLoop;
	else return _leftSideOfExternalConnection;
	
}

//
// getRightFlag: Get the consanguinity or external right flag
//
unsigned Individual::getRightFlag(bool consanguinousLoop){
	
	if(consanguinousLoop) return _rightSideOfLoop;
	else return _rightSideOfExternalConnection;
	
}

//
// setChildrenIdsSortedByExternalConnections: assign children Ids vector which is reordered based on external flags to a parentpair
//
//void Individual::setChildrenIdsSortedByExternalConnections(std::string parentPair,std::vector<std::string>  children){
void Individual::setChildrenIdsSortedByExternalConnections(std::string parentPair,std::vector<Individual*>  children){
	std::map<std::string, std::vector<Individual*> >::iterator mit = _childrenIdsSortedByExternalConnections.find(parentPair);
	if(mit != _childrenIdsSortedByExternalConnections.end()){
		//std::cout << "ERROR: setChildrenIdsSortedByExternalConnections: There already exists such a parent pair " << parentPair << std::endl;
	}else{
		_childrenIdsSortedByExternalConnections.insert(std::map<std::string,std::vector<Individual*> >::value_type(parentPair,children));
	}
	
}

void Individual::setSampledColumnPresent(){
	_sampledColumnPresent = true;
}

bool Individual::getSampledColumnPresent(){
	return _sampledColumnPresent;
}

//
// groupIndividualsBasedOn:
//
void Individual::groupIndividualsBasedOn(bool consanguinousLoop,const std::vector<Individual*>& individuals,std::deque<Individual*>& initial,std::deque<Individual*>& left,std::deque<Individual*>& right,bool unique){
	
	unsigned count = individuals.size();
	for(unsigned i=0;i<count;i++){
		
		//
		// First step in the sorting process is to construct 3 queues
		// The left queue has individuals with leftLoopFlag set (front of the Q with the largest loop #)
		// The right queue has individuals with rightLoopFlag set
		// The remaining individuals are in the initial queue 
		//
		if(individuals[i]->getLeftFlag(consanguinousLoop) == 0 && individuals[i]->getRightFlag(consanguinousLoop) == 0){ 
			initial.push_back(individuals[i]);
			continue; 
		}
		
		if(individuals[i]->getLeftFlag(consanguinousLoop) > 0){
			if(!unique || (unique && individuals[i]->getLeftFlag(consanguinousLoop) >= individuals[i]->getRightFlag(consanguinousLoop))){
				// insert into the left deque
				if(left.size() > 0){
					if(left.front()->getLeftFlag(consanguinousLoop) > individuals[i]->getLeftFlag(consanguinousLoop)){
						left.push_front(individuals[i]);
					}else{
						// *** ADDED NOW
						// Make additional check for twin cases:
						if(left.back()->getLeftFlag(consanguinousLoop) > individuals[i]->getLeftFlag(consanguinousLoop) && left.back()->getTwinMarker().get() != ".")
							left.push_front(individuals[i]);
						else left.push_back(individuals[i]);
					}
				}else{
					left.push_back(individuals[i]);
				}
			}
		} 
		if(individuals[i]->getRightFlag(consanguinousLoop) > 0){
			
			if(!unique || (unique && individuals[i]->getRightFlag(consanguinousLoop) > individuals[i]->getLeftFlag(consanguinousLoop))){
				// insert into the right deque
				if(right.size() > 0){
					if(right.front()->getRightFlag(consanguinousLoop) > individuals[i]->getRightFlag(consanguinousLoop)){
						right.push_back(individuals[i]);
					}else{
						right.push_front(individuals[i]);
					}
				}else{
					right.push_back(individuals[i]);
				}
			}
		}
	}
	
	// DEBUG:
	
	/*std::cout << "Sorting the  Inds" << std::endl;
	std::cout << " %%%%%%%%%%%%%START " << std::endl;
	std::cout << " INITIAL INITIAL " << std::endl;
	for(unsigned i=0;i<initial.size();i++){
		std::cout << initial[i]->getId() << std::endl;
	}
	
	std::cout << "INITIAL LEFT" << std::endl;
	for(unsigned i=0;i<left.size();i++){
		std::cout << left[i]->getId() << " and " << left[i]->getLeftFlag(consanguinousLoop) << std::endl;
	}
	std::cout << "INITIAL RIGHT " << std::endl;
	for(unsigned i=0;i<right.size();i++){
		std::cout << right[i]->getId() << " and " << right[i]->getRightFlag(consanguinousLoop) << std::endl;
	}
	std::cout << "%%%%%%%%%%%% END " << std::endl;
	*/
	
	
}

//
// sortIndividualsBasedOnDataField:
//
std::vector<Individual*> Individual::sortIndividualsBasedOnDataField(const std::string& name,const std::vector<Individual*>& individuals,bool dobSortOrder){
	
	std::map<Data*,std::vector<Individual*>,compareData> dataMap;
	
	unsigned count = individuals.size();
	unsigned index = 0;
	std::vector<Individual*> individualsWithMissingData;
	std::vector<Individual*> individualsSorted;
	
	// Insert all the individuals sorted by the data value of the desired column
	while(index < count){
		Data* data = individuals[index]->getData(name); 
		if(data->isMissing()){
			individualsWithMissingData.push_back(individuals[index]);
			index++;
			continue;
		}
		std::map<Data*,std::vector<Individual*>,compareData>::iterator dit = dataMap.find(data);
		if(dit != dataMap.end()){
			dit->second.push_back(individuals[index]);
		}else{
				std::vector<Individual*> tempvec;
				tempvec.push_back(individuals[index]);
				dataMap.insert(std::map<Data*,std::vector<Individual*> >::value_type(data,tempvec));
		}
		index++;
	}
	
	unsigned birthOrder=0;
	std::map<Data*,std::vector<Individual*>,compareData>::iterator mit = dataMap.begin();
	while(mit != dataMap.end()){
		for(unsigned i=0;i<mit->second.size();i++){
			//std::cout << "Ind:    " << mit->second[i]->getId() << " Data :" << mit->first->get() <<  std::endl;
			
			individualsSorted.push_back(mit->second[i]);
			// If the sort field is DOB store the birthOrder on each individual:
			if(dobSortOrder){
				birthOrder++;
				mit->second[i]->setBirthOrder(birthOrder);
			}
		}
		++mit;
	}
	
	
	// Merge the vectors:
	for(unsigned i = 0;i<individualsWithMissingData.size();i++)
		individualsSorted.push_back(individualsWithMissingData[i]);
	
	// DEBUG:
	//std::vector<Individual*>::iterator it;
	//for(it=individualsSorted.begin();it<individualsSorted.end();it++) std::cout <<  (*it)->getId() << std::endl;
	
	return individualsSorted;
	
}

//
// DEBUG: displayChildren:
//
void Individual::displayChildren() {
	
	std::cout << "Children of :" << _id << std::endl;
	std::set<Individual*,compareIndividual>::const_iterator it = _children.begin();
	while(it != _children.end()){
		std::cout << (*it)->getId() << "\t";
		++it;
	}
	std::cout << std::endl;
	
} 

//
// displaySpouses:
//
void Individual::displaySpouses() {
	
	std::cout << "Spouses of :" << _id << std::endl;
	std::set<Individual*,compareIndividual>::const_iterator it = _spouses.begin();
	while(it != _spouses.end()){
		std::cout << (*it)->getId() << "\t";
		++it;
	}
	std::cout << std::endl;
	
}

//
// displayNuclearFamilies:
//
void Individual::displayNuclearFamilies(){
	
	std::cout << "Nuclear Families of : " << _id << std::endl;
	std::vector<NuclearFamily*>::const_iterator it = _nuclearFamilies.begin();
	while(it != _nuclearFamilies.end()){
		std::cout << (*it)->getFather()->getId() << "\t" << (*it)->getMother()->getId() << std::endl;
		++it;
	}
	std::cout << std::endl;
	
	
}

//
// display:
//
void Individual::display(){
	
	std::cout << " IND ID is" << _id << std::endl;
	//std::cout << " DOB" << getData("dob")->get() << std::endl; 
}

//
// sortSpouses: Old method of sorting spouses:
//
void Individual::sortSpousesOld(void){
	
	std::set<Individual*,compareIndividual>::iterator spouseIt = _spouses.begin();
	std::vector<Individual*> leftLoopIndividuals;
	std::vector<Individual*> rightLoopIndividuals;
	std::vector<Individual*> initial;
	std::vector<Individual*> result;
	
	std::vector<NuclearFamily*> temp;
	
	unsigned left,right;
	//
	// Determine if any loop flags are set on the spouses
	//
	while(spouseIt != _spouses.end()){
		
		left = (*spouseIt)->getLeftSideOfLoop();
		right = (*spouseIt)->getRightSideOfLoop();
		if(left == 0 && right == 0) { initial.push_back(*spouseIt); ++spouseIt; continue; } 
		if(left && right){
			if(left > right){
				if(leftLoopIndividuals.size() > 0){
					if(leftLoopIndividuals.front()->getLeftSideOfLoop() > left){
						leftLoopIndividuals.insert(leftLoopIndividuals.begin(),*spouseIt);
					}else{
						leftLoopIndividuals.push_back(*spouseIt);
					}
				}else{
					leftLoopIndividuals.push_back(*spouseIt);
				}
			}else{
				if(rightLoopIndividuals.size() > 0){
					if(rightLoopIndividuals.front()->getRightSideOfLoop() > right){
						rightLoopIndividuals.insert(rightLoopIndividuals.begin(),*spouseIt);
					}else{
						rightLoopIndividuals.push_back(*spouseIt);
					}
				}else{
					rightLoopIndividuals.push_back(*spouseIt);
				}
			}
		}else
		if(left){
			if(leftLoopIndividuals.size() > 0){
				if(leftLoopIndividuals.front()->getLeftSideOfLoop() > left){
					leftLoopIndividuals.insert(leftLoopIndividuals.begin(),*spouseIt);
				}else{
					leftLoopIndividuals.push_back(*spouseIt);
				}
			}else{
				leftLoopIndividuals.push_back(*spouseIt);
			}
		}else
		if(right){
			if(rightLoopIndividuals.size() > 0){
				if(rightLoopIndividuals.front()->getRightSideOfLoop() > right){
					rightLoopIndividuals.insert(rightLoopIndividuals.begin(),*spouseIt);
				}else{
					rightLoopIndividuals.push_back(*spouseIt);
				}
			}else{
				rightLoopIndividuals.push_back(*spouseIt);
			}
		}
		++spouseIt;
	}
	
	// DEBUG: Display the vectors
	/*std::cout << "MULTIPLE SPOUSE LEFT" << std::endl;
	for(unsigned i=0;i<leftLoopIndividuals.size();i++)
		std::cout << leftLoopIndividuals[i]->getId() << " : " << leftLoopIndividuals[i]->getLeftSideOfLoop() << std::endl;
	std::cout << "MULTIPLE SPOUSE RIGHT " << std::endl;
	for(unsigned i=0;i<rightLoopIndividuals.size();i++)
		std::cout << rightLoopIndividuals[i]->getId() << " : " << rightLoopIndividuals[i]->getRightSideOfLoop() << std::endl;
	std::cout << "MULTIPLE SPOUSE INITIAL" << std::endl;
	for(unsigned i=0;i<initial.size();i++)
		std::cout << initial[i]->getId() << std::endl;
	*/
	
	//
	// Merge the left and right vectors if they exist
	//
	unsigned j;
	// Only left vector exists
	if(!rightLoopIndividuals.size()){
		// NOTE: ADDED the below if on 2006-04-27 for poag465
		// hasnt been tested to see if this placement is optimal
		if(!leftLoopIndividuals.empty() && leftLoopIndividuals.size() < 2){
			// If the spouse's left loop = individual's right loop, push the spouse towards the left
			if(leftLoopIndividuals[0]->getLeftSideOfLoop() == _rightSideOfLoop){
				unsigned limit = (initial.size() % 2 == 0)? initial.size()-1:initial.size();
				for(j=0;j<limit;j++) result.push_back(initial[j]);
				result.push_back(leftLoopIndividuals[0]);
				if(limit < initial.size()) result.push_back(initial[limit]); 
			}else{
				
				// Push all the ones in initial to the result
				// Push the leftLoopIndividual as the last even spouse so that it is drawn the right most
				unsigned limit = (initial.size() % 2 == 0)? initial.size():initial.size()-1;
				for(j=0;j<limit;j++) result.push_back(initial[j]);
				result.push_back(leftLoopIndividuals[0]);
				if(limit < initial.size()) result.push_back(initial[limit]); 
			}
		}else{
			// Merge initial to the right of left Loop Individuals
			for(j=0;j<initial.size();j++) leftLoopIndividuals.push_back(initial[j]);
			//  Determine the resulting order
			unsigned mid = leftLoopIndividuals.size()/2;
			result.push_back(leftLoopIndividuals[mid]);
			unsigned leftMid, rightMid;
			leftMid = mid - 1; rightMid = mid+1;
			while(result.size() < leftLoopIndividuals.size()){
				if(leftMid < 0 && rightMid >= leftLoopIndividuals.size()) break;
				if(leftMid >= 0) result.push_back(leftLoopIndividuals[leftMid]);
				leftMid--;
				if(rightMid < leftLoopIndividuals.size()) result.push_back(leftLoopIndividuals[rightMid]);
				rightMid++;
			}
		}
	}else
	if(!leftLoopIndividuals.size()){ // Only right vector exists
		if(!rightLoopIndividuals.empty() && rightLoopIndividuals.size() < 2){
			// If the spouse's rigth loop = individual's left loop push the spouse towards the right
			if(rightLoopIndividuals[0]->getRightSideOfLoop() == _leftSideOfLoop){
				unsigned limit = (initial.size() % 2 == 0)? initial.size():initial.size()-1;
				for(j=0;j<limit;j++) result.push_back(initial[j]);
				result.push_back(rightLoopIndividuals[0]);
				if(limit < initial.size()) result.push_back(initial[limit]); 
			}else{
				// Push all the ones in initial to the result
				// Push the rightLoopIndividual as the last odd spouse so that it is drawn the left most
				unsigned limit = (initial.size() % 2 == 0)? initial.size()-1:initial.size();
				for(j=0;j<limit;j++) result.push_back(initial[j]);
				result.push_back(rightLoopIndividuals[0]);
				if(limit < initial.size()) result.push_back(initial[limit]); 
			}
		}else{
			// Merge initial to the left of right Loop Individuals
			for(j=0;j<rightLoopIndividuals.size();j++) initial.push_back(rightLoopIndividuals[j]);
			//  Determine the resulting order
			unsigned mid = initial.size()/2;
			result.push_back(initial[mid]);
			unsigned leftMid, rightMid;
			leftMid = mid - 1; rightMid = mid+1;
			while(result.size() < initial.size()){
				if(leftMid < 0 && rightMid >= initial.size()) break;
				if(rightMid < initial.size()) result.push_back(initial[rightMid]);
				rightMid++;
				if(leftMid >= 0) result.push_back(initial[leftMid]);
				leftMid--;
			}
		}
	}else{
		// Both left and right exist merge the two
		// Merge the left and right Loop Individuals 
		for(j=0;j<rightLoopIndividuals.size();j++) leftLoopIndividuals.push_back(rightLoopIndividuals[j]);
		//  Determine the resulting order
		unsigned mid = leftLoopIndividuals.size()/2;
		result.push_back(leftLoopIndividuals[mid]);
		
		unsigned leftMid, rightMid;
		leftMid = mid - 1; rightMid = mid+1;
		while(result.size() < leftLoopIndividuals.size()){
			if(leftMid < 0 && rightMid >= leftLoopIndividuals.size()) break;
			if(leftMid >= 0){ result.push_back(leftLoopIndividuals[leftMid]); }
			leftMid--;
			if(rightMid < leftLoopIndividuals.size()) result.push_back(leftLoopIndividuals[rightMid]);
			rightMid++;
		}
		// Then add initial 
		for(j=0;j<initial.size();j++){ result.push_back(initial[j]);  }
		
		// NOTE: ALTERNATIVE  on 2006-04-03 just to see the results of an alternative approach
		// This approach seems to work best when the CONSANG loops connect to external NFs
		// and not the NFs belonging to the multiple spouses
		// Merge the initial to the left
		//for(j=0;j<initial.size();j++) leftLoopIndividuals.push_back(initial[j]);
		// Merge the right to the left
		//for(j=0;j<rightLoopIndividuals.size();j++) leftLoopIndividuals.push_back(rightLoopIndividuals[j]);
		//unsigned mid = leftLoopIndividuals.size()/2;
		//result.push_back(leftLoopIndividuals[mid]);
		//unsigned leftMid, rightMid;
		//leftMid = mid - 1; rightMid = mid+1;
		//while(result.size() < leftLoopIndividuals.size()){
		//	if(leftMid < 0 && rightMid >= leftLoopIndividuals.size()) break;
		//	if(leftMid >= 0){ std::cout << "leftMid " << leftMid << std::endl; result.push_back(leftLoopIndividuals[leftMid]); }
		//	leftMid--;
		//	std::cout << "rightMid " << rightMid <<   std::endl;
		//	if(rightMid < leftLoopIndividuals.size()) result.push_back(leftLoopIndividuals[rightMid]);
		//	rightMid++;
		//}
		//std::cout << "NEW RESULT is" << std::endl;
		//for(j=0;j<result.size();j++) std::cout << result[j]->getId() << std::endl; 
	}
	
	if(Debug::DEBUG){
		std::cout << "SPOUSE SORTING NEW RESULT is" << std::endl;
		for(j=0;j<result.size();j++) std::cout << result[j]->getId() << std::endl; 
	}
	
	//
	// Sort the NFs of the individual based on the spouse ordering
	//
	NuclearFamily* nf;
	for(j=0;j<result.size();j++){
		// Get the first NF of the spouse
		nf = result[j]->getNuclearFamily((unsigned)0);
		if(_gender.getEnum() == Gender::MALE){
			if(nf->getFather()->getId() == _id) temp.push_back(nf);
			else{
				//std::cout << "This spouse has more than 1 NF" << std::endl;
				for(unsigned i=1;i<result[j]->getNumberOfNuclearFamilies();i++){
					if(result[j]->getNuclearFamily(i)->getFather()->getId() == _id){
						temp.push_back(result[j]->getNuclearFamily(i));
						break;
					}
				}
			}
		}else{
			if(nf->getMother()->getId() == _id) temp.push_back(nf);
			else{
				//std::cout << "This spouse has > 1 NF " << std::endl;
				for(unsigned i=1;i<result[j]->getNumberOfNuclearFamilies();i++){
					if(result[j]->getNuclearFamily(i)->getMother()->getId() == _id){
						temp.push_back(result[j]->getNuclearFamily(i));
						break;
					}
				}
			}
		}
	}
	// Exchange the two vectors:
	_nuclearFamilies.swap(temp);
	
}
