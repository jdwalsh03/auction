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

#ifndef __SOPBID_HPP_INCLUDED
#define __SOPBID_HPP_INCLUDED

#include <algorithm>   // std::nth_element, std::sort
#include <cmath>       // std::isinf
#include "glob.hpp"    // mfloat, muint
#include "object.hpp"  // objlist

class SOPbid {
public:
  SOPbid() : pCRWT (0), pEPS (1.0), pID (0), pMXWT (0) { };
  SOPbid (const muint inid, const muint wt, const objlist& A)
    : pARC (A), pCRWT (0), pEPS (1.0), pID (inid), pMXWT (wt) { };
  bool Active() const { return (pMXWT > pCRWT); };
  void MakeBid (const objlist& PR, objlist& D);
  void Pop() { pCRWT -= 1; return; };
  void Push() { pCRWT += 1; return; };
  void Refresh (const mfloat ep) { pEPS = ep; pCRWT = 0; return; };
private:
  objlist pARC;   // arc costs
  muint   pCRWT;  // total weight claimed by bidder
  mfloat  pEPS;   // epsilon
  muint   pID;    // bidder id number
  muint   pMXWT;  // total weight needed by bidder
};

typedef std::vector<SOPbid> SOPbidlist;

#endif // __SOPBID_HPP_INCLUDED
