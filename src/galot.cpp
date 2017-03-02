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

#include "galot.hpp"

/** --- UpdateClaims -----------------------------------------------------------
 **/
void GAlot::UpdateClaim (Claim& C, Object& P, GAbdlst& B) {
  Claim tmp;
  mfloat df;
  if (pMXWT < C.wt) {
    C.wt = pMXWT;
  }
  df = C.wt - pFRWT;
  while ((df > gEPS) && (C.pr > P.c)) {
    // free up weight
    auto ri = pCLMS->begin ();
    if (ri->bdr == C.bdr) {  // Hungry Cannibals rule
      C.wt += ri->wt;
      if (pMXWT < C.wt) {
        C.wt = pMXWT;
      }
      df = C.wt - pFRWT;
    }
    if (ri->wt - df > gEPS) {
      tmp = *ri;
      pFRWT = C.wt;
      if (ri->bdr > -1) {
        B[muint (ri->bdr)].Pop (df);
      }
      tmp.wt -= df;
      df = 0.0;
      pCLMS->erase (ri);
      pCLMS->emplace_hint (pCLMS->cbegin(), tmp);
    } else {
      pFRWT += ri->wt;
      if (ri->bdr > -1) {
        B[muint (ri->bdr)].Pop (ri->wt);
      }
      df -= ri->wt;
      pCLMS->erase (ri);
      if (!pCLMS->empty ()) {
        P.c = pCLMS->begin()->pr;
      }
    }
  }
  if (pFRWT < C.wt) {
    C.wt = pFRWT;
  }
  if (C.wt > 0) {
    P.i = C.bdr;
    // add claim to dst list
    pCLMS->emplace_hint (pCLMS->cbegin(), C);
    pFRWT -= C.wt;
    B[muint (C.bdr)].Push (C.wt);
    if (pFRWT < gEPS) {
      P.c = pCLMS->begin()->pr;
    }
  }
  return;
}

/// ----------------------------------------------------------------------------
