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
// Last modified: 2006.10.11.ET
// 
/////////////////////////////////////////////////////////

//
// Grid.h
//
#ifndef GRID_INCLUDED
#define GRID_INCLUDED

#include <map>
#include <algorithm>

// FOR DEBUGGING ONLY: #include <iostream>

// FOR DEBUGGING ONLY: using namespace std;

//
// template class Grid : Implements a sparse two-dimensional array
//                       as an STL "map of maps".  The vector of vertical
//                       (Y) indices stores the vectors of horizontal
//                       (X) positions.
//                       
// class N is a number class (i.e., int, double, etc.)
// class pT must be of type "pointer to object of type T"
// 
// Pointers are used: (1) to avoid expensive object copies
//               and: (2) to allow returning NULL (0) instead of a sentinel object 
//                        for empty nodes in the sparse array
template <class N,class pT> class Grid{
	private: 
		typedef std::map<N,pT> horizontalVector;
		typedef std::map<N,horizontalVector> verticalVector;
		
		verticalVector _grid;
		
	public: 
		void insert(N xPosition,N yPosition,pT pointerToObjectOfTypeT);
		pT   find  (N xPosition,N yPosition);
		
		// FOR DEBUGGING ONLY: 
		 void printNames();
};

//
// Grid::insert() method: Insert a pointer to object of type T
//                        at coordinates xPosition, yPosition
//
template<class N,class pT> void Grid<N,pT>::insert(N xPosition,N yPosition,pT pointerToObjectOfTypeT){
	
	// Get an iterator to the horizontalVector map in row Y of
	// the grid:
	typename verticalVector::iterator tempY = _grid.find(yPosition);
	// If a horizontalVector map is not yet present, then add one:
	if(tempY == _grid.end()){
		horizontalVector hV;
		tempY = _grid.insert( std::pair <N,horizontalVector>(yPosition,hV)).first;
	}
	// Insert the objectOfTypeT into the horizontalVector:
	tempY->second.insert(std::pair<N,pT>(xPosition,pointerToObjectOfTypeT));
	
}

//
// Grid::find() method: Find an object at coordinates xPosition,yPosition
//
// Returns an object of type pT if found.  
// Returns 0 (NULL) if not found.
//
// NOTE BENE: It is the user's responsibility to check if the 
//            returned pointer is NULL before dereferencing!
//            
template<class N,class pT> pT Grid<N,pT>::find(N xPosition,N yPosition){
	
	// Get an iterator to the horizontalVector map in row Y of
	// the grid:
	typename verticalVector::iterator tempY = _grid.find(yPosition);
	//
	// If a horizontalVector map is not yet present, then return
	// 
	if(tempY == _grid.end()){
		
		return (pT) 0;
		
	}
	//
	// Get here if row Y has entries: look for x:
	//
	typename horizontalVector::iterator tempX = tempY->second.find(xPosition);
	if(tempX != tempY->second.end()){
		return tempX->second;
	}
	return (pT) 0;
	
}

//
// printNames() is to be used for debugging only:
//
template<class N,class pT> void Grid<N,pT>::printNames(){
	
	typename verticalVector::const_iterator y;
	typename horizontalVector::const_iterator x;
	
	
	std::cout << "Y\tX\tObject" << "\n";
	
	for(y = _grid.begin(); y != _grid.end() ; y++ ){
		
		for(x = y->second.begin() ; x != y->second.end() ; x++ ){
			std::cout << y->first << "\t" << x->first << "\t";
			// Since we are using a grid of pointers:
			x->second->display();
			std::cout << "\n";
		}
	}
}
#endif
