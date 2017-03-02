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

#ifndef __APMAP_HPP_INCLUDED
#define __APMAP_HPP_INCLUDED

#include <algorithm>   // std::sort
#include <cmath>       // std::floor
#include <iostream>    // std::cout, std::endl, std::sprintf
#include <vector>      // std::vector
#include "glob.hpp"    // mfloat, mfvec, mint, muint
#include "object.hpp"  // objlist, voblist
#include "apbid.hpp"   // APbidlist

class APmap {
public:
  APmap() : pEPS (1.0), pGCD (1), pMN (1.0), pSTP (0.25) { };
  APmap (const mfvec& DWT, const mfvec& SWT, const voblist& A,
         const mfloat MX, const mfloat MN, const mfloat ST);
  void Solve (objlist& T, mfvec& PR);
private:
  APbidlist  pBDR;  // vector of bidders
  objlist    pBID;  // vector of current bids
  mfloat     pEPS;  // current epsilon value
  mint       pGCD;  // gcd of weights
  mfloat     pMN;   // minimum epsilon value
  objlist    pPR;   // vector of prices
  mfloat     pSTP;  // epsilon step size
  void Auction();
  mint gcd (const mint a, const mint b) const;
  void Round();
  void UpdateClaims();
};

#endif // __APMAP_HPP_INCLUDED
