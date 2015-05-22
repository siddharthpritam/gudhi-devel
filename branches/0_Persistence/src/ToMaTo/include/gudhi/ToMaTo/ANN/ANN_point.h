/*    This file is part of the Gudhi Library. The Gudhi library
 *    (Geometric Understanding in Higher Dimensions) is a generic C++
 *    library for computational topology.
 *
 *    Author(s):       Primoz Skraba
 *
 *    Copyright (C) 2009 Primoz Skraba.  All Rights Reserved.
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

#ifndef SRC_TOMATO_INCLUDE_GUDHI_TOMATO_ANN_ANN_POINT_H_
#define SRC_TOMATO_INCLUDE_GUDHI_TOMATO_ANN_ANN_POINT_H_

#include <iostream>

class ANN_point {
 public:
  typedef struct {
    bool operator()(const ANN_point a, const ANN_point b) const {
      for (int i = 0; i < a.dim; i++) {
        if (a.coord[i] < b.coord[i]) return true;
        else if (a.coord[i] > b.coord[i]) return false;
      }
      return false;
    }
  } less_than;

  double *coord;
  int dim;

  ANN_point() { }

  ANN_point(int d) {
    dim = d;
  }

  //------------------------------------------------------------------------
  // I/O functions
  //------------------------------------------------------------------------

  friend std::istream& operator>>(std::istream &in, ANN_point &out) {
    int i;
    out.coord = new double[out.dim];
    for (i = 0; i < out.dim; i++) {
      in >> out.coord[i];
    }
    return in;
  }

  friend std::ostream& operator<<(std::ostream &out, const ANN_point &in) {
    int i;
    for (i = 0; i < in.dim; i++) {
      out << in.coord[i] << " ";
    }
    return out;
  }
};

#endif  // SRC_TOMATO_INCLUDE_GUDHI_TOMATO_ANN_ANN_POINT_H_
