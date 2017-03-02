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

#include "apmap.hpp"

/** --- Standard Constructor ---------------------------------------------------
 **/
APmap::APmap (const mfvec& DWT, const mfvec& SWT, const voblist& A,
              const mfloat MX, const mfloat MN, const mfloat ST)
  : pEPS (MX), pGCD (1), pMN (MN), pSTP (ST)
{
  pBDR.clear();
  pBID.clear();
  pPR.clear();
  if ((!DWT.empty()) && (!SWT.empty())) {
    bool isint = true;
    muint i = 0;
    while ((isint) && (i < DWT.size())) {
      if (!equal(DWT[i], std::floor (DWT[i]))) {
        isint = false;
      }
      ++i;
    }
    i = 0;
    while ((isint) && (i < SWT.size())) {
      if (!equal(SWT[i], std::floor (SWT[i]))) {
        isint = false;
      }
      ++i;
    }
    if (!isint) {
      std::cout << "  WARNING: AUCTION requires integer weights."
        << std::endl;
    } else {
      pGCD = mint (DWT[0]);
      i = 1;
      while ((i < DWT.size()) && (pGCD > 1)) {
        pGCD = gcd (pGCD, mint (DWT[i]));
        i += 1;
      }
      i = 0;
      while ((i < SWT.size()) && (pGCD > 1)) {
        pGCD = gcd (pGCD, mint (SWT[i]));
        i += 1;
      }
      muvec pos (1, 0);
      muint s = 0;
      for (muint it = 0; it < SWT.size(); it++) {
        s += muint (SWT[it]) / muint (pGCD);
        pos.emplace_back (s);
        for (muint n = 0; n < muint (SWT[it]) / muint (pGCD); n++) {
          pPR.emplace_back (0.0, -1, it);
        }
      }
      pBID = objlist (pPR.size(), Object (0.0, -1, -1));
      pBDR.reserve (pPR.size());
      objlist tA;
      i = 0;
      for (muint it = 0; it < DWT.size(); it++) {
        tA.clear();
        for (muint m = 0; m < A[it].size(); m++) {
          for (muint n = 0;
               n < muint (SWT[muint (A[it][m].j)]) / muint (pGCD); n++) {
            tA.emplace_back (A[it][m].c, A[it][m].j,
                             pos[muint (A[it][m].j)] + n);
          }
        }
        for (muint n = 0; n < muint (DWT[it]) / muint (pGCD); n++) {
          pBDR.emplace_back (i, it, 1, tA);
          pBDR.back().Refresh (pEPS);
          i++;
        }
      }
      if (gVBS > 0) {
        char str[255];
        std::sprintf (str,
                      "  - Assignment auction size : %lu bidders, %lu lots",
                      pBDR.size(), s);
        std::cout << str << std::endl;
      }
    }
  }
}

/** --- Solve ------------------------------------------------------------------
 **/
void APmap::Solve (objlist& T, mfvec& PR) {
  T.clear();
  PR.clear();
  if (!pBDR.empty()) {
    Auction();
    muint k;
    muint n = 0;
    std::sort (pPR.begin(), pPR.end(),
               [](const Object& a, const Object& b) -> bool {
                 return ((a.j < b.j) || ((a.j == b.j) && (a.i < b.i))); });
    PR = mfvec (pPR.size(), 0.0);
    for (muint it = 0; it < pPR.size(); it++) {
      PR[muint (pPR[it].j)] = pPR[it].c;
    }
    while (n < pPR.size()) {
      T.emplace_back (pGCD, pBDR[muint (pPR[n].i)].Class(), pPR[n].j);
      k = n + 1;
      while ((k < pPR.size()) && (pPR[k].j == pPR[n].j)) {
        if (pBDR[muint (pPR[k].i)].Class() != pBDR[muint (pPR[n].i)].Class()) {
          k += 1;
        } else {
          T.back().c += mfloat (pGCD);
          pPR.erase (pPR.begin() + mint (k));
        }
      }
      n += 1;
    }
  }
  return;
}

/** --- Auction ----------------------------------------------------------------
 **/
void APmap::Auction() {
  do {
    pEPS *= pSTP;
    for (muint it = 0; it < pBDR.size(); it++) {
      pBDR[it].Refresh (pEPS);
    }
    for (auto it = pPR.begin(); it < pPR.end(); it++) {
      it->i = -1;
    }
    Round();
  } while (pEPS >= pMN);
  return;
}

/** --- GCD --------------------------------------------------------------------
 **/
mint APmap::gcd (const mint a, const mint b) const {
  return (b == 0) ? a : gcd (b, a % b);
}

/** --- Round ------------------------------------------------------------------
 **/
void APmap::Round() {
  bool avail = true;
  muint n;
  while (avail) {
    // get new bids
    for (muint it = 0; it < pBDR.size(); it++) {
      pBDR[it].MakeBid (pPR, pBID[it]);
    }
    // select winners from bids
    UpdateClaims();
    avail = false;
    n = pBDR.size();
    while ((!avail) && (n > 0)) { avail = pBDR[--n].Active(); }
  }
  return;
}

/** --- UpdateClaims -----------------------------------------------------------
 **/
void APmap::UpdateClaims() {
  std::vector <bool> ch (pPR.size(), false);
  for (muint it = 0; it < pBID.size(); it++) {
    if ((pBID[it].j > -1) && (pPR[muint (pBID[it].j)] < pBID[it])) {
      if (!ch[muint (pBID[it].j)]) {
        ch[muint (pBID[it].j)] = true;
        if (pPR[muint (pBID[it].j)].i != -1) {
          pBDR[muint (pPR[muint (pBID[it].j)].i)].Pop();
        }
      }
      pPR[muint (pBID[it].j)].c = pBID[it].c;
      pPR[muint (pBID[it].j)].i = pBID[it].i;
    }
  }
  // apply results to bidders
  for (muint k = 0; k < ch.size(); k++) {
    if (ch[k]) {
      pBDR[muint (pPR[k].i)].Push();
    }
  }
  return;
}

/// ----------------------------------------------------------------------------
