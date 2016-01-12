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
// Individual.h
//
#ifndef Individual_INCLUDED
#define Individual_INCLUDED

#include "String.h"
#include "Gender.h"
#include "Date.h"
#include "Affected.h"
#include "Twin.h"
#include "LivingDead.h"
#include "Proband.h"
#include "DataTable.h"
#include "Sampled.h"
#include "Consultand.h"
#include "Carrier.h"
#include "ObligateCarrier.h"
#include "RelationshipEnded.h"
#include "Infertility.h"
#include "Sterility.h"
#include "Pregnancy.h"
#include "Collapsed.h"
#include "SampleQuantity.h"
#include "Donor.h"

#include <set>
#include <vector>
#include <map>
#include <deque>


class NuclearFamily;

class Individual{
	
private:
	
	inline void _init(void);
	void _checkForSpecialIndividualTypes(void);
	void _rearrangeMultipleSpouses(std::deque<Individual*>& initial,std::deque<Individual*>& left,std::deque<Individual*>& right,std::deque<Individual*>& result);
	
public:
	//
	// "less" function for comparing Individuals based on id:
	//
	struct compareIndividual
	{
		bool operator()(Individual* i1, Individual* i2) const
		{
			return i1->getId().get().compare(i2->getId().get()) < 0;
		}
	};
	
	
private:
	
	//
	// Inherent attributes -- These are normally derived directly from
	// reading a database table:
	// 
	String   _id;         // individual's ID - never missing
	String   _motherId;   // mother's ID - can be missing if father is also missing
	String   _fatherId;   // father's ID - can be missing if mother is also missing
	Gender   _gender;     // male, female, or missing
	
	// Attributes not present in FieldLabels:
	Date*              _dob;               // date of birth
	// <currently not used> Date*              _dod;               // date of death
	Affected*          _affected;          // affection status
	LivingDead*        _deceased;          // deceased or alive - defaults to missing
	Proband*           _proband;           // true for the proband only, else false
	Consultand*        _consultand;        // true if a consultand, else false.
	Pregnancy*         _pregnancy;         // true if this is a pregnancy
	Carrier*           _carrier;           // true if a carrier, else false.
	ObligateCarrier*   _obligateCarrier;   // true if an obligate carrier, else false.
	RelationshipEnded* _relationshipEnded; // true if relationship ended (divorced,separated,reason unknown) - requires that a mated pair are *both* so marked for display ...
	Sterility*         _sterility;         // 2009.05.19.ET ADDENDUM
	Infertility*       _infertility;       // 2009.05.19.ET ADDENDUM
	Twin               _twinMarker;        // stores the marker of monozygotic or dizygotic twin 
	Sampled*           _sampled;           // true if individual has been sampled
	Collapsed*         _collapsed;         // TRUE if individual represents a collapsed group of individuals
	SampleQuantity*    _sampleQuantity;    // Holds sample quantity values
	Donor*             _donor;             // Holds donor status (missing, sperm, ovum, sperm+ovum)
	
	                                       // — 2015.08.21.ET ADDENDUM
	Individual *_father;  // Pointer to the father -- defaults to NULL (0)
	Individual *_mother;  // Pointer to the mother -- defaults to NULL (0)
	
	Individual *_adoptedComplement; // Pointer to adopted complement -- defaults to NULL (0);
	
	//
	// Constructed attributes:
	//
	bool     _isVirtual;             // true only for "virtual" individuals created by Madeline
	bool     _isUnconnected;         // true if the person remains unattached
	bool     _isConsanguinous;       // true if consanguinously mated
	bool     _hasExternalConnection; // true if the individual has a spouse from a different descent tree	
	unsigned _birthOrder;            // set when dob is present
	
	// Determined attributes:
	bool     _isOriginalFounder;
	bool     _isOrdinaryFounder;
	
	//
	// Drawing attributes:
	//
	unsigned _leftWidth;
	unsigned _rightWidth;
	unsigned _totalWidth;
	double   _x;              // drawing x position 
	double   _y;              // drawing y position
	bool     _hasBeenVisited; // true if the individual has been visited
	int      _visitCount;     // counter for times visited
	bool     _hasBeenDrawn;   // true if the individual has been drawn
	
	// Loop flags:
	unsigned _leftSideOfLoop;
	unsigned _rightSideOfLoop;
	
	// ExternalConnection flags:
	unsigned _leftSideOfExternalConnection;
	unsigned _rightSideOfExternalConnection;
	
	// Spouse Connector flag:
	bool _leftSpouseConnector;
	
	struct compareData{
		bool operator()(Data* a, Data* b) const{
			return *a < *b;
		}
	};
	
	// Containers that establish individual connections:
	std::set<Individual *,compareIndividual> _spouses;
	std::set<Individual *,compareIndividual> _children;
	
	std::map<std::string, std::vector<Individual*> > _childrenIdsSortedByExternalConnections;
	std::set<unsigned> _descentTrees;
	std::vector<NuclearFamily*> _nuclearFamilies;
	
	// Link with the DataTable
	int _rowIndex;
	int _tableIndex;
	const DataTable *_pedigreeTable;
	
	static bool _sampledColumnPresent;
	
	//
	// Flags for special types of individuals:
	//
	bool _individualIndicatingNoOffspring;
	bool _individualIndicatingNonFertility;
	bool _individualIndicatingNoOffspringWithNoSymbol;
	bool _individualIndicatingNoOffspringWithEntwinedRingsSymbol;
	bool _individualIndicatingTerminatedPregnancy;
	bool _individualAdoptedOut;
	bool _individualAdoptedIn;
	bool _isSpecial;
	
	unsigned _collapsedCount;
	
public:
	
	//
	// Constructors:
	//
	Individual();
	Individual(const std::string & id);
	Individual(const std::string & id, int rowIndex, int tableIndex);
	Individual(const std::string & id, const std::string & motherId, const std::string & fatherId, const std::string &gender);
	Individual(const std::string & id, const std::string & motherId, const std::string & fatherId, const std::string & gender,int rowIndex,int tableIndex);
	
	//
	// Static Methods:
	//
	static std::string getRandomId();
	static void groupIndividualsBasedOn(bool consanguinousLoop,const std::vector<Individual*>& individuals,std::deque<Individual*>& initial,std::deque<Individual*>& left,std::deque<Individual*>& right,bool unique=false);
	static std::vector<Individual*> sortIndividualsBasedOnDataField(const std::string& name,const std::vector<Individual*>& individuals,bool dobSortOrder);
	static void setSampledColumnPresent();
	static bool getSampledColumnPresent(); 
	   
	//
	// Setters:
	//
	void setOrdinaryFounder(bool value) { _isOrdinaryFounder = value; }
	void setOriginalFounder(bool value) { _isOriginalFounder = value; }
	void setExternalConnection(bool value) { _hasExternalConnection = value; }
	void setIsUnconnected(bool value) { _isUnconnected = value; }
	void setIsConsanguinous(bool value) { _isConsanguinous = value; }
	void setMother(Individual* mother) { _mother = mother; }
	void setFather(Individual* father) { _father = father; }
	void setAdoptedComplement(Individual* adoptedComplement) { _adoptedComplement = adoptedComplement; }
	void setDOB(Date* dob) { _dob = dob; }
	void setAffectionStatus(Affected* affected) { _affected = affected; }
	void setProbandStatus(Proband* proband) { _proband = proband; }
	void setDeceasedStatus(LivingDead* deceased) { _deceased = deceased; }
	void setVirtualIndividual(bool value){ _isVirtual = value; }
	void setMotherId(std::string motherId){ _motherId = motherId; }
	void setFatherId(std::string fatherId){ _fatherId = fatherId; }
	void setTwin(std::string marker,Twin::TWIN_TYPE type) { _twinMarker.set(marker,type); }
	void setGender(std::string gender) { _gender.set(gender); }
	void setSampled(Sampled* sampled){ _sampled=sampled; }
	void setConsultandStatus(Consultand* consultand){ _consultand=consultand; }
	void setCarrierStatus(Carrier* carrier){ _carrier=carrier; }
	void setObligateCarrierStatus(ObligateCarrier* obligateCarrier){ _obligateCarrier = obligateCarrier; }
	void setDonorStatus(Donor* donor){ _donor = donor; }
	// 2009.05....ET Newly added columns:
	void setRelationshipEndedStatus(RelationshipEnded* relationshipEnded){ _relationshipEnded=relationshipEnded; }
	void setPregnancyStatus(Pregnancy* pregnancy){ _pregnancy=pregnancy; }
	void setInfertilityStatus      (Infertility* infertility            ){ _infertility      =infertility;       }
	void setSterilityStatus        (Sterility* sterility                ){ _sterility        =sterility;         }
	void setCollapsedStatus        (Collapsed* collapsed                ){ _collapsed        =collapsed;         }
	void setSampleQuantityStatus   (SampleQuantity* sampleQuantity      ){ _sampleQuantity   =sampleQuantity;    }
	
	void setBirthOrder(unsigned birthOrder){ _birthOrder = birthOrder; }
	
	// Set drawing attributes:
	void setLeftWidth(unsigned leftWidth){ _leftWidth = leftWidth; }
	void setRightWidth(unsigned rightWidth){ _rightWidth = rightWidth; }
	void setTotalWidth(unsigned totalWidth) { _totalWidth = totalWidth; } 
	void setX(double x) { _x = x; }
	void setY(double y) { _y = y; }
	void setLeftSideOfLoop(unsigned loopNumber){ _leftSideOfLoop = loopNumber; }
	void setRightSideOfLoop(unsigned loopNumber){ _rightSideOfLoop = loopNumber; }
	void setLeftSideOfExternalConnection(unsigned connectionNumber) { _leftSideOfExternalConnection = connectionNumber; }
	void setRightSideOfExternalConnection(unsigned connectionNumber) { _rightSideOfExternalConnection = connectionNumber; }
	void setHasBeenDrawn()             { _hasBeenDrawn = true; }
	void setVisited(bool value)        { _hasBeenVisited = value; }
	
	// By default the spouse connector is drawn on the right
	// In exceptional cases like consanguinity if the male of the pair
	// is on the left then the connector on the female is drawn on the left
	void setLeftSpouseConnector(bool value){ _leftSpouseConnector = value; }
	
	
	void incrementCollapsedCount(void){ _collapsedCount++; }
	//
	// Getters:
	//
	unsigned getCollapsedCount(void)   { return _collapsedCount; }
	String   getId( void )             { return _id;       }
	String   getMotherId( void )       { return _motherId; }
	String   getFatherId( void )       { return _fatherId; }
	Gender   getGender(void)           { return _gender;   }
	Date*    getDOB(void)              { return _dob;      }
	
	//
	// Getters that return Boolean:
	//
	bool     isOrdinaryFounder(void)   { return _isOrdinaryFounder; }
	bool     isOriginalFounder(void)   { return _isOriginalFounder; } 
	bool     isVirtual(void)           { return _isVirtual; }
	bool     hasExternalConnection(void) { return _hasExternalConnection; }
	bool     hasBeenVisited(void)      { return _hasBeenVisited; }
	bool     hasBeenDrawn(void)        { return _hasBeenDrawn; }
	bool     isConsanguinous(void)     { return _isConsanguinous; }
	//
	// 2014.07.09.ET ADDENDA: hasChildren() and hasSpouses()
	inline bool     hasChildren(void)          { return !_children.empty(); } // returns true if individual has one or more child
	inline bool     hasSpouses(void)           { return !_spouses.empty();  } // returns true if individual has one or more spouses
	inline bool     pregnancyStateInvalid(void){ return isPregnancy() && (hasChildren() || hasSpouses()); } // returns true if the individual cannot represent a pregnancy
	void            removePregnancyFlag(void)  { if(_pregnancy) _pregnancy->set(false); }
	// For these, if the pointer is NULL, then we have no information, so return false
	// Else, if the pointer is not NULL, then getBoolean() returns the state information:
	bool     isAffected(void)          { if(_affected   == 0) return false; return _affected->getBoolean(); }
	bool     isSampled(void)           { if(_sampled    == 0) return false; return _sampled->getBoolean(); }
	bool     isProband(void)           { if(_proband    == 0) return false; return _proband->getBoolean();  }
	bool     isConsultand(void)        { if(_consultand == 0) return false; return _consultand->getBoolean(); }
	bool     isPregnancy(void)         { if(_pregnancy  == 0) return false; return _pregnancy->getBoolean(); }
	bool     isCarrier(void)           { if(_carrier    == 0) return false; return _carrier->getBoolean(); }
	// 2015.12.07.ET addenda:
	bool     isObligateCarrier(void)   { if(_obligateCarrier == 0) return false; return _obligateCarrier->getBoolean(); }
	bool     hasSampleQuantity(void)   { if(_sampleQuantity  == 0) return false; return _sampleQuantity->getBoolean(); }
	double   getSampleQuantity(void)   { if(_sampleQuantity  == 0) return 0; return _sampleQuantity->getValue(); }
	// 2016.01.12.ET addendum:
	Donor::DONOR_TYPE getDonorType(void){ if(_donor  == 0) return Donor::MISSING_DONOR; return _donor->getDonorType(); }
	// 2009.05.11.ET Addendum:
	bool     relationshipHasEnded(void){ if(_relationshipEnded == 0) return false; return _relationshipEnded->getBoolean(); }
	// 2009.05.19.ET Addenda:
	bool     isInfertile(void)         { if(_infertility == 0) return false; return _infertility->getBoolean(); }
	bool     isSterile(void)           { if(_sterility   == 0) return false; return _sterility->getBoolean(); }
	
	bool     isDeceased(void)          { if(_deceased   == 0) return false; return _deceased->getBoolean(); }
	
	// 2009.05.11.ET Addendum:
	// get RelationshipEnded type:
	//
	RelationshipEnded::TYPE getRelationshipEndedType(void) const {if(_relationshipEnded == 0) return RelationshipEnded::RELATIONSHIP_ENDED_MISSING_TYPE; return _relationshipEnded->getType(); }
	// 2009.05.19.ET Addenda:
	Infertility::TYPE getInfertilityType(void) const {if(_infertility == 0) return Infertility::UNKNOWN_INFERTILITY_TYPE; return _infertility->getType(); }
	Sterility::TYPE getSterilityType(void)     const {if(_sterility   == 0) return Sterility::UNKNOWN_STERILITY_TYPE; return _sterility->getType(); }
	std::string getInfertilityLabel(void)      const {if(_infertility == 0) return std::string("."); return _infertility->get(); }
	std::string getSterilityLabel(void)        const {if(_sterility   == 0) return std::string("."); return _sterility->get(); }
	
	
	//
	// More getters:
	//
	unsigned getBirthOrder(void)       { return _birthOrder; }
	unsigned getRowIndex(void)         { return _rowIndex; }
	Individual* getFather(void)        { return _father; }
	Individual* getMother(void)        { return _mother; }
	Individual* getAdoptedComplement(void) { return _adoptedComplement; }
	Individual* getFirstSpouse(void);
	Twin getTwinMarker(void)           { return _twinMarker; }
	unsigned getNumberOfSpouses(void)  { return _spouses.size(); }
	unsigned getNumberOfChildren(void) { return _children.size(); }
	unsigned getNumberOfDescentTrees(void) { return _descentTrees.size(); }
	unsigned getNumberOfNuclearFamilies(void) { return _nuclearFamilies.size(); }
	NuclearFamily* getNuclearFamily(unsigned index);
	std::set<unsigned> getDescentTrees(void) { return _descentTrees; }
	
	// Get drawing attributes:
	unsigned getLeftWidth(void)        { return _leftWidth; }
	unsigned getRightWidth(void)       { return _rightWidth; }
	unsigned getTotalWidth(void)       { return _totalWidth; }
	double getX(void)                  { return _x; }
	double getY(void)                  { return _y; }
	unsigned getLeftSideOfLoop(void)   { return _leftSideOfLoop; }
	unsigned getRightSideOfLoop(void)  { return _rightSideOfLoop; }
	unsigned getLeftSideOfExternalConnection(void) { return _leftSideOfExternalConnection; }
	unsigned getRightSideOfExternalConnection(void) { return _rightSideOfExternalConnection; }
	unsigned getLeftFlag(bool consanguinousLoop);
	unsigned getRightFlag(bool consanguinousLoop);
	bool getLeftSpouseConnector(void) { return _leftSpouseConnector; }
	
	// Getters of Individual connections:
	std::vector<std::string> getChildrenIds(Individual* spouse);
	
	const std::set<Individual*,compareIndividual> *const getSpouses(void) const;
	const std::set<Individual*,compareIndividual> *const getChildren(void) const;
	
	void getChildrenWithSpouse(Individual* spouse,std::vector<Individual*>& children);
	void getChildrenSortedByExternalConnections(Individual* spouse,std::vector<Individual*>& children);
	
	// Methods that access the underlying pedigree Data Table:
	void setPedigreeDataTable(const DataTable *pedigreeTable){
		_pedigreeTable = pedigreeTable;
	}
	
	Data* getData(const std::string label){
		if(_tableIndex != -1 && _rowIndex != -1){
			if(_pedigreeTable->columnExists(label.c_str())){
				return _pedigreeTable->getDataAtIndex(label,_rowIndex);
			}
		}
		
		Data* temp = new String(".");
		return temp;
	}
	
	// Other Methods:
	void sortSpouses(bool externalFlag=false);
	void addSpouse(Individual* spouse);
	void addChild(Individual* child) { _children.insert(child); }
	void addDescentTree(unsigned descentTreeId) { _descentTrees.insert(descentTreeId); }
	void addNuclearFamily(NuclearFamily* nuclearFamily){ _nuclearFamilies.push_back(nuclearFamily); }
	void setChildrenIdsSortedByExternalConnections(std::string parentPair,std::vector<Individual*>  children);
	
	// Debug:
	void displayChildren();
	void displaySpouses();
	void displayNuclearFamilies();
	void display();
	
	//
	// "get" functions for determining special categories of individuals:
	//
	inline bool isIndividualIndicatingNoOffspring(void){         return _individualIndicatingNoOffspring;         }
	inline bool isIndividualIndicatingNonFertility(void){        return _individualIndicatingNonFertility;        }
	inline bool isIndividualIndicatingNoOffspringWithNoSymbol(void){ return _individualIndicatingNoOffspringWithNoSymbol; }
	inline bool isIndividualIndicatingNoOffspringWithEntwinedRingsSymbol(void){ return _individualIndicatingNoOffspringWithEntwinedRingsSymbol; }
	inline bool isIndividualIndicatingTerminatedPregnancy(void){ return _individualIndicatingTerminatedPregnancy; }
	inline bool isIndividualAdoptedIn(void){                     return _individualAdoptedIn;                     }
	inline bool isIndividualAdoptedOut(void){                    return _individualAdoptedOut;                    }
	inline bool isSpecial(void){                                 return _isSpecial;                               }
	
	//
	// getDataTable:
	//
	const DataTable * getDataTable(void) const { return _pedigreeTable; }
	
	//
	// 2011.03.17.ET Addendum:
	//
	int getVisitCount()          { return _visitCount; }
	void resetVisitCount()       { _visitCount = 0;    }
	void incrementVisitCount()   { _visitCount++;      }
	
};

#endif
