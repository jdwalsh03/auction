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

#include "gamap.hpp"

/** --- Standard Constructor ---------------------------------------------------
 **/
GAmap::GAmap (const mfvec& DWT, const mfvec& SWT, const voblist& A,
              const mfloat MX, const mfloat MN, const mfloat ST)
  : pEPS (MX), pMN (MN), pSTP (ST)
{
  pBDR.clear();
  pCLM.clear();
  pLTS.clear();
  pPR.clear();
  if ((!DWT.empty()) && (!SWT.empty())) {
    pLTS.reserve (SWT.size());
    pPR.reserve (SWT.size());
    for (muint it = 0; it < SWT.size(); it++) {
      pLTS.emplace_back (it, SWT[it]);
      pPR.emplace_back (0.0, -1, it);
    }
    pCLM.reserve(DWT.size());
    for (muint it = 0; it < DWT.size(); it++) {
      pCLM.emplace_back ();
    } 
    pBDR.reserve (DWT.size());
    for(muint it = 0; it < DWT.size(); it++) {
      pBDR.emplace_back (it, DWT[it], A[it]);
      pBDR.back().Refresh (pEPS);
    }
    if (gVBS > 0) {
      char str[255];
      std::sprintf (str,
                    "  - General auction size    : %lu bidders, %lu lots",
                    pBDR.size(), pLTS.size());
      std::cout << str << std::endl;
    }
  }
}

/** --- Solve ------------------------------------------------------------------
 **/
void GAmap::Solve (objlist& T, mfvec& PR) {
  T.clear();
  PR.clear();
  if (pBDR.size() > 0) {
    Auction ();
    PR = mfvec (pPR.size(), 0.0);
    for (muint it = 0; it < pPR.size(); it++) {
      PR[muint (pPR[it].j)] = pPR[it].c;
    }
    Object O;
    muint st;
    bool dup;
    cllist CL;
    for (muint j = 0; j < pLTS.size(); j++) {
      st = T.size();
      O.j = mint (j);
      pLTS[j].Export (CL);
      if (CL.size() == 0) {
        T.emplace_back (0.0, -1, j);
      } else {
        for (auto it = CL.begin(); it != CL.end(); it++) {
          O.i = it->bdr;
          O.c = it->wt;
          dup = false;
          // test solution for duplicate arc
          for (muint i = st; i < T.size(); i++) {
            if (T[i].i == O.i) {
              dup = true;
              T[i].c += O.c;
                i = T.size();
            }
          }
          if (!dup) {
            T.push_back(O);
          }
        }
      }
    }
  }
  return;
}

/** --- Auction ----------------------------------------------------------------
 **/
void GAmap::Auction() {
  do {
    for (auto it = pPR.begin(); it < pPR.end(); it++) {
      it->i  = -1;
    }
    pEPS *= pSTP;
    for (muint it = 0; it < pBDR.size(); it++) {
      pBDR[it].Refresh (pEPS);
    }
    for (muint it = 0; it < pLTS.size(); it++) {
      pLTS[it].Refresh ();
    }
    Round ();
  } while (pEPS >= pMN);
  return;
}

/** --- GCD --------------------------------------------------------------------
 **/
mint GAmap::gcd (const mint a, const mint b) const {
  return (b == 0) ? a : gcd (b, a % b);
}

/** --- Round ------------------------------------------------------------------
 **/
void GAmap::Round() {
  bool avail = true;
  muint n;
  while (avail) {
    // get new bids
    for (muint it = 0; it < pBDR.size(); it++) {
      pBDR[it].MakeBid(pPR, pCLM[it]);
    }
    // select winners from bids
    for (muint it = 0; it < pCLM.size(); it++) {
      if (pCLM[it].dst != -1) {
        pLTS[muint (pCLM[it].dst)].UpdateClaim
          (pCLM[it], pPR[muint (pCLM[it].dst)], pBDR);
      }
    }
    // decide if round is over
    avail = false;
    n = pBDR.size();
    while ((!avail) && (n > 0)) { avail = pBDR[--n].Active(); }
  }
  return;
}

/// ----------------------------------------------------------------------------
