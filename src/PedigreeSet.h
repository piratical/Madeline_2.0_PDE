#include <iostream>
#include "DataTable.h"
#include "Data.h"
#include "Pedigree.h"
#include "LabelSet.h"

/*!
 * The PedigreeSet class 
 */

struct ltstr
{
        bool operator()(Data *c1,Data *c2) const{
                return (*c1 < *c2);
        }
};

struct eqstr
{
        bool operator()(Data *c1,Data *c2) const{
                return (*c1 == *c2);
        }
};

struct comparePedigrees{
        bool operator()(Pedigree *p1,Pedigree *p2) const{
                if(p1->getId() == p2->getId()) return p1->getTableId() < p2->getTableId();
                return p1->getId() < p2->getId();
        }
};


class PedigreeSet{
public:
	
	PedigreeSet(){ }
	~PedigreeSet();
	void addPedigreesFromDataTable(const DataTable * p_pedigreeTable,unsigned tableIndex,const std::string& sortField);
	void draw(const DataTable *const pedigreeTable);
	// Debug:
	void display();                          // Prints out all pedigrees that have been read in

private:
	std::set<Pedigree*,comparePedigrees> _pedigrees;          // Set of pedigrees
	
	void _checkParentChildDOB();
	void _checkPregnancyStateValidity();
	void _computeWidths(const std::string& sortField,bool dobSortOrder=false);
	void _determineFoundingGroups();
	void _establishConnections();
	void _setCoreOptionalFields(const DataTable* pedigreeTable);
	
};

