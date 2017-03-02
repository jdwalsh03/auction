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

#include "sobid.hpp"

/** --- MakeBid ----------------------------------------------------------------
 **/
void SObid::MakeBid (const objlist& PR, Object& D) {
  D.j = -1;
  if (pMXWT > pCRWT) {
    D.i = mint (pID);
    if (pARC.size() > 1) {
      objlist M = pARC;
      for (muint it = 0; it < M.size(); it++) {
        M[it].c -= PR[muint (M[it].j)].c;
      }
      std::nth_element (M.begin(), M.begin() + 1, M.end(),
        [](const Object& a, const Object& b) -> bool { return (a.c > b.c); });
      mfloat W;
      if (M[0].i != M[1].i) {
        W = M[1].c;
      } else {
        std::sort (M.begin(), M.end(),
          [](const Object& a, const Object& b) -> bool { return (a.c > b.c); });
        muint ct = 2;
        while ((ct < M.size()) && (M[ct].i == M[0].i)) {
          ++ct;
        }
        if (ct < M.size()) {
          W = M[ct].c;
        } else {
          W = M[1].c;
        }
      }
      D.j = M[0].j;
      D.c = M[0].c + PR[muint (M[0].j)].c + pEPS - W;
    } else {
      D.j = pARC[0].j;
      D.c = gINF;
    }
  }
  return;
}

/// ----------------------------------------------------------------------------
