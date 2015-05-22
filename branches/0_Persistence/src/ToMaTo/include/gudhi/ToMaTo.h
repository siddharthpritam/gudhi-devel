/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Vincent Rouvreau
 *
 *    Copyright (C) 2015  INRIA Saclay (France)
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

#ifndef SRC_TOMATO_INCLUDE_GUDHI_TOMATO_H_
#define SRC_TOMATO_INCLUDE_GUDHI_TOMATO_H_

#include "gudhi/ToMaTo/Cluster.h"
#include "gudhi/ToMaTo/Graph.h"
#include "gudhi/ToMaTo/Interval.h"
#include "gudhi/ToMaTo/Vertex.h"

namespace Gudhi {
namespace ToMaTo {
/** \defgroup ToMaTo ToMaTo 

\author Vincent Rouvreau

\section Introduction
ToMaTo proposes a zero persistence computation, and a hard-clustering method.

\section Definitions

For instance, on a point of clouds:
*\image html "2pointscloud.png" "2 clouds of points" width=20cm

The density can be represented as follow:
*\image html "2pointscloudwithdensity.png" "2 clouds of points and the coloured density" width=20cm

The 0-persistence of the density is:
*\image html "0-persistenceofthedensity.png" "0-persistence of the density" width=20cm

The persistence threshold will be called \f$ \tau \f$.

\section API

\subsection Overview

\subsection Visitor

\section Example

 
\subsection 0-persistence of the density on 2 clouds of points
 

\include density_then_0_pers.cpp

\include density_then_0_pers.result

\section Acknowledgements
The author wishes to thank Steve Oudot and Primoz Skraba for 
their collaboration to write the documentation. 

\copyright GNU General Public License v3.                         
\verbatim  Contact: gudhi-users@lists.gforge.inria.fr \endverbatim
*/
/** @} */  // end defgroup
}  // namespace ToMaTo
}  // namespace Gudhi


#endif  // SRC_TOMATO_INCLUDE_GUDHI_TOMATO_H_
