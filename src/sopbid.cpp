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

#include "sopbid.hpp"

/** --- MakeBid ----------------------------------------------------------------
 **/
void SOPbid::MakeBid (const objlist& PR, objlist& D) {
  muint ct = muint (pMXWT - pCRWT);
  if (ct > 0) {
    mfloat W;
    objlist Mo, Mn;
    Mo.reserve(pARC.size());
    Mn.reserve(pARC.size());
    bool bc = true;
    mint id = -1;
    for (muint it = 0; it < pARC.size(); it++) {
      if (mint (pID) == PR[muint (pARC[it].j)].i) {
        Mo.emplace_back(pARC[it].c, pARC[it].i, pARC[it].j);
        if (bc) {
          if (id == -1) {
            id = Mo.back().i;
          } else if (id != Mo.back().i) {
            bc = false;
          }
        }
      } else {
        Mn.emplace_back(pARC[it].c - PR[muint (pARC[it].j)].c, pARC[it].i, pARC[it].j);
      }
    }
    if (ct >= Mn.size()) {
      std::sort (Mn.begin(), Mn.end(),
        [](const Object& a, const Object& b) -> bool { return (a.c > b.c); });
      W  = -gINF;
      ct = Mn.size();
    } else {
      std::nth_element (Mn.begin(), Mn.begin() + mint (ct), Mn.end(),
        [](const Object& a, const Object& b) -> bool { return (a.c > b.c); });
      W = Mn[ct].c;
      if ((bc) && (ct+1 < Mn.size())) {
        if (id == -1) {
          id = Mn[0].i;
        }
        if (Mn[ct].i == id) {
          muint ti = 0;
          while ((bc) && (ti < ct)) {
            if (Mn[ti++].i != id) { bc = false; }
          }
          if (bc) { // W is unsuitable; need 1st not-ts element
            std::sort (Mn.begin(), Mn.end(),
              [](const Object& a, const Object& b) -> bool {
                return (a.c > b.c); });
            muint tt = ct + 1;
            while ((tt < Mn.size()) && (Mn[tt].i == id)) {
              ++tt;
            }
            if (tt < Mn.size()) {
              W = Mn[tt].c;
            } else {
              W = Mn[ct].c;
            }
          }
        }
      }
    }
    Object bd;
    bd.i = mint (pID);
    for (muint n = 0; n < Mo.size(); n++) {
      bd.c = Mo[n].c + pEPS - W;
      bd.j = Mo[n].j;
      D[n] = bd;
    }
    for (muint n = 0; n < ct; n++) {
      if (Mn[n].c <= -gINF) {
        bd.c = gINF;
      } else {
        bd.c = Mn[n].c + PR[muint (Mn[n].j)].c + pEPS - W;
      }
      bd.j = Mn[n].j;
      D[n + Mo.size()] = bd;
    }
  }
  return;
}

/// ----------------------------------------------------------------------------
