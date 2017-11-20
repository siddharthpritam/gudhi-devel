/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Pawel Dlotko
 *
 *    Copyright (C) 2017 Swansea University UK
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>



#ifndef HASSE_DIAGRAM_CELL_H
#define HASSE_DIAGRAM_CELL_H


namespace Gudhi {

namespace Hasse_diagram {


template < typename Cell_type > class Hasse_diagram;

template <typename Incidence_type_, typename Filtration_type_ , typename Additional_information_ = void>
class Hasse_Diagram_Cell
{
public:
	typedef Incidence_type_ Incidence_type;
	typedef Filtration_type_ Filtration_type;
	typedef Additional_information_ Additional_information;

    /**
     * Default constructor.
    **/
	Hasse_Diagram_Cell():dimension(0),position(0),deleted_(false){}

	/**
     * Constructor of a cell of dimension dim.s
    **/
	Hasse_Diagram_Cell( unsigned dim ):dimension(dim),position(0),deleted_(false){}

	/**
     * Constructor of a cell of dimension dim with a given boundary.
    **/
	Hasse_Diagram_Cell( const std::vector< std::pair<Hasse_Diagram_Cell*,int> >& boundary_ , unsigned dim ):
	dimension(dim),boundary(boundary_),position(0),deleted_(false){}

	/**
     * Constructor of a cell of dimension dim with a given boundary and coboundary.
    **/
	Hasse_Diagram_Cell( const std::vector< std::pair<Hasse_Diagram_Cell*,int> >& boundary_ , const std::vector< std::pair<Hasse_Diagram_Cell*,int> >& coboundary_,
		 unsigned dim ):dimension(dim),boundary(boundary_),coBoundary(coboundary_),
		 position(0),deleted_(false){}

	/**
     * Constructor of a cell of dimension dim with a given boundary, coboundary and
     * additional information.
    **/
	Hasse_Diagram_Cell( const std::vector< std::pair<Hasse_Diagram_Cell*,int> >& boundary_ , const std::vector< std::pair<Hasse_Diagram_Cell*,int> >& coboundary_,
	Additional_information ai, unsigned dim ):
	dimension(dim),boundary(boundary_),coBoundary(coboundary_),additional_info(ai),
	position(0),deleted_(false){}

	/**
     * Construcor of a cell of dimension dim having given additional information.
    **/
	Hasse_Diagram_Cell(Additional_information ai, unsigned dim ):
	dimension(dim),additional_info(ai),position(0),deleted_(false){}

	/**
     * Procedure to get the boundary of a fiven cell. The output format
     * is a vector of pairs of pointers to boundary elements and incidence
     * coefficients.
    **/
	inline std::vector< std::pair<Hasse_Diagram_Cell*,int> >& get_boundary(){return this->boundary;}

	/**
     * Procedure to get the coboundary of a fiven cell. The output format
     * is a vector of pairs of pointers to coboundary elements and incidence
     * coefficients.
    **/
	inline std::vector< std::pair<Hasse_Diagram_Cell*,int> >& get_coBoundary(){return this->coBoundary;}

	/**
     * Procedure to get the dimension of a cell.
    **/
	inline unsigned& get_dimension(){return this->dimension;}

	/**
     * Procedure to get additional information about the cell.s
    **/
	inline Additional_information& get_additional_information(){return this->additional_info;}

	/**
	 * Procedure to retrive position of the cell in the structure. It is used in
	 * the implementation of Hasse diagram and set by it. Note that removal of
	 * cell and subsequent call of clean_up_the_structure will change those
	 * positions.
	**/
	inline size_t& get_position(){return this->position;}

	/**
	 * A procedure used to check if the cell is deleted. It is used by the
	 * subsequent implementation of Hasse diagram that is absed on lazy
	 * delete.
	**/
	bool deleted(){return this->deleted_;}

	template < typename Cell_type >
	friend class Hasse_diagram;

	/**
	 * Procedure to remove deleted boundary and coboundary elements from the
	 * vectors of boundary and coboundary elements of this cell.
	**/
	void remove_deleted_elements_from_boundary_and_coboundary()
	{
		std::vector< std::pair<Hasse_Diagram_Cell*,int> > new_boundary;
		new_boundary.reserve( this->boundary.size() );
		for ( size_t bd = 0 ; bd != this->boundary.size() ; ++bd )
		{
			if ( !this->boundary[bd]->deleted() )
			{
				new_boundary.push_back( this->boundary[bd] );
			}
		}
		this->boundary.swap( new_boundary );

		std::vector< std::pair<Hasse_Diagram_Cell*,int> > new_coBoundary;
		new_coBoundary.reserve( this->coBoundary.size() );
		for ( size_t cbd = 0 ; cbd != this->coBoundary.size() ; ++cbd )
		{
			if ( !this->coBoundary[cbd]->deleted() )
			{
				new_coBoundary.push_back( this->coBoundary[cbd] );
			}
		}
		this->coBoundary.swap( new_coBoundary );
	}

	/**
	 * Writing to a stream operator.
	**/
	friend std::ostream operator<<( std::ostream& out, Hasse_Diagram_Cell<Incidence_type,Filtration_type,Additional_information> const& c )
	{
		 out << c.position << " " << c.dimension << " " << c.filtration << std::endl;
		 for ( size_t bd = 0 ; bd != c.boundary.size() ; ++bd )
	     {
			 //do not write out the cells that has been deleted
			 if ( c.boundary[bd].first->deleted() )continue;
			 out << c.boundary[bd].first->position << " " << c.boundary[bd].second << " ";
		 }
		 out << std::endl;
		return out;
	}
private:
	std::vector< std::pair<Hasse_Diagram_Cell*,int> > boundary;
	std::vector< std::pair<Hasse_Diagram_Cell*,int> > coBoundary;
	unsigned dimension;
	Additional_information additional_info;
	size_t position;
	bool deleted_;
	Filtration_type filtration;

	/**
	 * A procedure to delete a cell. It is a private function of the Hasse_Diagram_Cell
	 * class, since in the Hasse_diagram class I want to have a control
	 * of removal of cells. Therefore, to remove cell please use
	 * remove_cell in the Hasse_diagram structure.
	**/
	void delete_cell(){ this->deleted_ = true; }
};//Hasse_Diagram_Cell



}//namespace Hasse_diagram
}//namespace Gudhi

#endif //HASSE_DIAGRAM_CELL_H
