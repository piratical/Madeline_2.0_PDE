#ifndef Pedigree_INCLUDED
#define Pedigree_INCLUDED

#include <iostream>
#include "Individual.h"
#include "NuclearFamily.h"
#include "DataTable.h"
#include "DataColumn.h"
#include "Data.h"
#include "DescentTree.h"
#include "LabelSet.h"
#include "Exception.h"
#include "utility.h"
#include "Grid.h"


/*!
 * The Pedigree class 
 */
	// Order Individuals by their id:
	struct compareIndividual{
		bool operator()(Individual* i1,Individual* i2) const{
			return i1->getId() < i2->getId();
		}
	};
	// Order NuclearFamilies:
	struct compareNuclearFamily{
		bool operator()(NuclearFamily* n1,NuclearFamily* n2) const{
			if(n1->getFather()->getId() != n2->getFather()->getId())
				return n1->getFather()->getId() < n2->getFather()->getId();
			return n1->getMother()->getId() < n2->getMother()->getId();
		}
	};

class Pedigree{
public:
	
	// Constructors:
	Pedigree(){ _dtsHaveConsanguinity=false; }
	Pedigree(const std::string id, const unsigned tableId) : _id(id),_tableId(tableId),_dtsHaveConsanguinity(false) { }
	~Pedigree();
	
	void set(const std::string id, const unsigned tableId) { _id = id; _tableId = tableId; }
	std::string getId() const { return _id; }
	unsigned getTableId() const { return _tableId; }
	void addIndividual(const std::string ind,std::string mother,std::string father, std::string gender, int rowIndex, int tableIndex,const DataTable& pedigreeTable );
	
	void setCoreOptionalFields(const DataTable* pedigreeTable);
	void determineFoundingGroups();
	void establishIndividualConnections();
	
	// 2011.03.17.ET Addenda:
	void checkForAncestorDescendantAnomoly(Individual *ancestor);
	void clearVisitCounts();
	
	void checkParentChildDOB();
	void computePedigreeWidth(const std::string& sortField,bool dobSortOrder);
	void draw(const LabelSet* labelSet);
	// Debug:
	void display() const;                          // Prints out all pedigrees that have been read in
	static void setDrawingFileExtension(const std::string& ext);

private:
	
	std::string _id;    // Pedigree Id
	unsigned _tableId;  // Table Id
	bool _dtsHaveConsanguinity; // flag that gets set when there are multiple DTs with consanguinity
	
	std::set<Individual*,compareIndividual> _individuals; // Set of Individuals
	std::set<NuclearFamily*,compareNuclearFamily> _nuclearFamilies; // Set of NuclearFamilies
	std::vector<NuclearFamily*> _nfOfOrdinaryFounders; // keeps a list of all the NFs or ordinary/original founders
	std::vector<DescentTree*> _descentTrees; // list of DescentTrees of the pedigree
	std::map<std::string, std::vector<Individual*> > _twinMarkers; // twin groups with their marker
	std::set<std::string> _consangPairIds; //  used with multiple DTs ; a list of all the consanguinous pairs 
	Grid<int,Individual* > _individualGrid; // Grid that keeps track of the individuals and their (x,y) positions
	
	// 2011.03.17.ET addendum:
	std::vector<Individual *> _anomolous;
	
	void _setIndividualTwinField(const DataColumn * twinDataColumn,char type);
	void _assignChildrenDescentTree(Individual* individual,unsigned descentTreeId);	
	void _assignDescentTrees();
	void _addDescentTree(unsigned id) ;
	void _addDescentTreesConnectedTo(unsigned dtIndex,std::deque<DescentTree*>& orderedDescentTrees,bool left);
	void _sortAndCalculateDescentTreeWidth();
	void _checkParentsGender(Individual* individual);
	void _checkMarkedTwinsDOB();
	void _calculateWidth(Individual* individual,bool classicalOrder,unsigned descentTreeIndex);
	void _populateIndividualGrid();
	void _sortNuclearFamilies(bool consanguinousFlag);
	void _sortIndividualNuclearFamilies();
	void _sortNuclearFamiliesBasedOnDataField(const std::string& name,bool dobSortOrder);
	void _addNuclearFamily(Individual* mother,Individual* father);
	void _establishNuclearFamilies();
	void _determineConnectorIndividuals();
	void _reorderDescentTreesBasedOnExternalConnections();
	void determineConnectorIndividualsOld();
	void _setLeftShiftConnectionFlags();
	
	
	// Methods that initialize various drawing flags on individuals
	void _markConnectorIndividuals(Individual* individual,unsigned& loopNumber);
	void _markLeftLoopFlags(Individual* individual,unsigned loopNumber);
	void _markRightLoopFlags(Individual* individual,unsigned loopNumber);
	void _markLeftExternalConnectionFlags(Individual* individual,unsigned connectionNumber);
	void _markRightExternalConnectionFlags(Individual* individual,unsigned connectionNumber);
	void _markExternalConnectionFlags();
	void _markConsanguinousIndividuals();
	void _markConsanguinousFlags(Individual* individual,unsigned& loopNumber);
	
	// Drawing Methods:
	void _drawConsanguinousConnectors(DrawingCanvas& dc);
	void _drawConsanguinityLetter(Individual* mother,Individual* father,unsigned int &uniqueId,double iconInterval,double iconDiameter, std::map<std::string,std::string>& individualConsanguinityLetter,DrawingCanvas& dc,double multipleSpouseOffset=0,bool leftConnector=false);
	void _drawHorizontalConnectorLine(double y,double x1,double x2,bool isConsanguinous,DrawingCanvas& dc);
	void _drawVerticalConnectorLine(double startY,double endY,double startX,double endX,bool isConsanguinous,DrawingCanvas& dc,double multipleSpouseOffset=0.0,bool singleChild=false);
	void _getSpouses(std::set<Individual*,compareIndividual>& foundingGroup,Individual* individual) ;
	
	bool _hasIndividualAtPosition(Individual* start,Individual* end);
	void _sortSibsBasedOnExternalConnections(const std::vector<Individual*>& sibs,std::vector<Individual*>& sortedSibs);

	void _sortSibsBasedOnConsanguinousConnections(const std::vector<Individual*>& sibs,std::vector<Individual*>& sortedSibs);
	
	unsigned _getPrimaryDescentTreeIndex(std::set<unsigned>& dt,Individual* individual,bool increment);
	
};

#endif // Pedigree_INCLUDED

