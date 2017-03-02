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

#ifndef __GALOT_HPP_INCLUDED
#define __GALOT_HPP_INCLUDED

#include <vector>      // std::vector
#include "glob.hpp"    // mfloat, muint
#include "claim.hpp"   // Claim, cllist, clptr
#include "object.hpp"  // Object
#include "gabid.hpp"   // GAbdlst

class GAlot {
public:
  GAlot();
  GAlot (const muint id, const mfloat wt)
    : pCLMS (new cllist), pFRWT (wt), pID (id), pMXWT (wt) { };
  bool Active() const { return (pFRWT > gEPS); };
  void Export (cllist& CV) const { CV = *pCLMS; return; }; 
  void UpdateClaim (Claim& C, Object& P, GAbdlst& B);
  void Refresh() { pCLMS->clear (); pFRWT = pMXWT; return; };
private:
  clptr  pCLMS;  // pointer to claim list
  mfloat pFRWT;  // total weight claimed in lot
  muint  pID;    // lot id number
  mfloat pMXWT;  // total weight available in lot
};

typedef std::vector <GAlot> GAltlst;

#endif // __GALOT_HPP_INCLUDED
