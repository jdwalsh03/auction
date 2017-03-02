/** ----------------------------------------------------------------------------
Copyright (C) 2016 Joseph D Walsh III <math@jdwalsh03.com>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.

This file is part of the "AUCTION ALGORITHMS IN C++" software project. See the
document <filelist.txt> for a full list of the project files. If
<filelist.txt> or any other file is missing, go to <http://www.jdwalsh03.com/>
to download the complete project.

This material is based upon work supported by the National Science Foundation
Graduate Research Fellowship under Grant No. DGE-1148903. Any opinion,
findings, and conclusions or recommendations expressed in this material are
those of the author and do not necessarily reflect the views of the National
Science Foundation.
---------------------------------------------------------------------------- **/

#ifndef __OBJECT_HPP_INCLUDED
#define __OBJECT_HPP_INCLUDED

#include <vector>    // std::vector
#include "glob.hpp"  // mfloat, mint

struct Object {
  Object () : c (0.0), i (0), j (0) { }
  Object (mfloat ci, mint ii, mint ji) : c (ci), i (ii), j (ji) { }
  mfloat c;  // cost, expense, flow, price (depending on context)
  mint   i;  // bidder node id number
  mint   j;  // destination/lot node id number
  bool operator< (const Object& o) const { return (c < o.c); };
  bool operator> (const Object& o) const { return (c > o.c); };
  bool operator<= (const Object& o) const { return (c <= o.c); };
  bool operator>= (const Object& o) const { return (c >= o.c); };
};

typedef std::vector<Object> objlist;
typedef std::vector<objlist> voblist;

#endif // __OBJECT_HPP_INCLUDED
