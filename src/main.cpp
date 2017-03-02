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

#include <algorithm>   // std::find
#include <chrono>      // std::chrono::duration, std::chrono::duration_cast,
                       //   std::chrono::high_resolution_clock::now,
                       //   std::chrono::high_resolution_clock::time_point
#include <cmath>       // std::abs, std::sqrt
#include <cstdlib>     // exit, EXIT_FAILURE
#include <cstdio>      // std::sprintf
#include <fstream>     // std::ifstream, std::ofstream
#include <iostream>    // std::cout, std::endl, std::ios::out, std::ios::trunc
#include <limits>      // std::numeric_limits
#include <random>      // std::mt19937_64, std::uniform_int_distribution
#include <sstream>     // std::stringstream
#include <string>      // std::string, std::getline
#include <vector>      // std::vector
#include "glob.hpp"    // mfloat, mfvec, mint, muint
#include "object.hpp"  // objlist, voblist
#include "apmap.hpp"   // APmap
#include "gamap.hpp"   // GAmap
#include "sopmap.hpp"  // SOPmap
#include "somap.hpp"   // SOmap

mfloat gEPS = std::sqrt(std::numeric_limits<mfloat>::epsilon());
mfloat gINF = std::numeric_limits<mfloat>::infinity();
muint  gVBS = 0;  // verbosity (0, 1, or 2)

void APrun (const mfvec& DWT, const mfvec& SWT, const voblist& A,
            const mfloat MX, const mfloat MN, const mfloat ST,
            objlist& T, mfvec& PR);
void GArun (const mfvec& DWT, const mfvec& SWT, const voblist& A,
            const mfloat MX, const mfloat MN, const mfloat ST,
            objlist& T, mfvec& PR);
void SOPrun (const mfvec& DWT, const mfvec& SWT, const voblist& A,
             const mfloat MX, const mfloat MN, const mfloat ST,
             objlist& T, mfvec& PR);
void SOrun (const mfvec& DWT, const mfvec& SWT, const voblist& A,
            const mfloat MX, const mfloat MN, const mfloat ST,
            objlist& T, mfvec& PR);
mfloat Dual (const mfvec& DWT, const mfvec& SWT, const voblist& ARX,
             const mfvec& PR);
mfloat Primal (const char* aname, const voblist& ARX, const objlist& T);
char* GetOption (char ** begin, char ** end, const std::string & option);
char** GetOptionList (char ** begin, char ** end, const std::string & option,
                      const muint cnt);
bool OptionExists (char** begin, char** end, const std::string& option);
muint ReadFile (const char* fname, mfvec& DWT, mfvec& SWT,
                voblist& ARX, voblist& RARX);
void Split (const std::string &s, std::vector<std::string> &elms);
void Split (const std::string &s, char dlm, std::vector<std::string> &elms);
int  WriteFile (const char* fname, const muint seed, const mfvec& DWT,
                const mfvec& SWT, const voblist& ARX, const objlist& T);

/** --- main -------------------------------------------------------------------
 * program flags:
 *   -a   : perform assignment auction
 *   -g   : perform general auction
 *   -l S : load and process graph file S
 *   -m # : maximum auction step size [requires # > 0]
 *   -n # : minimum auction step size [requires # > 0]
 *   -o   : perform extended (SO) auction
 *   -p   : perform extended (SOP) auction
 *   -r # : step size scaling rate [requires value 0 < # < 1]
 *   -s S : save graph results file S
 *   -v   : verbose output
 *   -vv  : very verbose output
 **/

int main (int argc, char *argv[])
{
  char    str[255];
  bool    ap = false;
  mfloat eps = -1.0;
  int    err = 0;
  bool    ga = false;
  mfloat min = -1.0;
  muint seed = 0;
  mfloat stp = 0.25;
  bool    wr = false;
  bool    xo = false;
  bool    xp = false;
  mfvec DWT, SWT;
  voblist ARX, RARX;
  // Implement option flags
  if (OptionExists(argv, argv+argc, "-a")) {
    ap = true;
  }
  if (OptionExists(argv, argv+argc, "-g")) {
    ga = true;
  }
  if (OptionExists(argv, argv+argc, "-o")) {
    xo = true;
  }
  if (OptionExists(argv, argv+argc, "-p")) {
    xp = true;
  }
  if (OptionExists(argv, argv+argc, "-vv")) {
    gVBS = 2;
  } else if (OptionExists(argv, argv+argc, "-v")) {
    gVBS = 1;
  }
  char* fname = GetOption(argv, argv + argc, "-l");
  if (fname != 0) {
    seed = ReadFile (fname, DWT, SWT, ARX, RARX);
  }
  if (seed == 0) {
    std::cout << "Error opening file" << std::endl;
    exit (EXIT_FAILURE);
  }
  char* sname = GetOption(argv, argv + argc, "-s");
  if (sname != 0) {
    wr = true;
  }
  char* mxstr = GetOption(argv, argv + argc, "-m");
  if (mxstr != 0) {
    if (std::strtod (mxstr, nullptr) > 0.0) {
      eps = std::strtod (mxstr, nullptr);
    }
  }
  char* mnstr = GetOption(argv, argv + argc, "-n");
  if (mnstr != 0) {
    if (std::strtod (mnstr, nullptr) > 0.0) {
      min = std::strtod (mnstr, nullptr);
    }
  }
  char* rpstr = GetOption(argv, argv + argc, "-r");
  if (rpstr != 0) {
    if (   (std::strtod (rpstr, nullptr) > 0.0)
        && (std::strtod (rpstr, nullptr) < 1.0)) {
      stp = std::strtod (rpstr, nullptr);
    }
  }
  if ((!ap) && (!ga) && (!xo) && (!xp)) {
    ga = true;
  }
  mfloat C = 0;
  muint  ar = 0;
  for (muint i = 0; i < ARX.size(); i++) {
    for (muint j = 0; j < ARX[i].size(); j++) {
      ar++;
      if (C < std::abs(ARX[i][j].c)) {
        C = std::abs(ARX[i][j].c);
      }
    }
  }
  if (eps < gEPS) {
    eps = C / 5.0;
  }
  if (min < gEPS) {
    min = 1.0 / mfloat (SWT.size());
  }
  objlist T;     // transport plan
  mfvec   PR;    // price vector
  mfloat  pcst;  // primal cost
  mfloat  dcst;  // dual cost
  std::chrono::high_resolution_clock::time_point t1, t2;
  std::chrono::duration <mfloat> dur;
  std::sprintf (str, "  GRAPH: %lu sinks, %lu sources, %lu arcs", DWT.size(),
                SWT.size(), ar);
  std::cout << str << std::endl;
  std::sprintf (str, "  EPS  : %f starting, %e minimum", eps, min);
  std::cout << str << std::endl;
  std::cout << "  ----------------------------------------------------------"
            << std::endl;
  if (ga) {
    t1 = std::chrono::high_resolution_clock::now();
    GArun (DWT, SWT, ARX, eps, min, stp, T, PR);
    t2 = std::chrono::high_resolution_clock::now();
    dur = std::chrono::duration_cast <std::chrono::duration <mfloat> >
      (t2 - t1);
    if (T.empty()) {
      std::sprintf (str, "  General auction NOT SOLVED");
      std::cout << str << std::endl;
    } else {
      pcst = Primal ("GA", ARX, T);
      std::sprintf (str, "  General auction primal cost    : %25.15f", pcst);
      std::cout <<  str << std::endl;
      dcst = Dual (DWT, SWT, ARX, PR);
      std::sprintf (str, "  General auction dual cost      : %25.15f", dcst);
      std::cout <<  str << std::endl;
      std::sprintf (str, "  General auction diff           : %25.15f",
                  dcst-pcst);
      std::cout <<  str << std::endl;
    }
    std::sprintf (str, "  General auction time           : %13.3f sec",
                  dur.count() );
    std::cout << str << std::endl;
    std::cout << "  ----------------------------------------------------------"
              << std::endl;
    if (wr) {
      err = WriteFile (sname, seed, DWT, SWT, ARX, T);
      wr = false;
    }
  }
  if (ap) {
    t1 = std::chrono::high_resolution_clock::now();
    APrun (DWT, SWT, ARX, eps, min, stp, T, PR);
    t2 = std::chrono::high_resolution_clock::now();
    dur = std::chrono::duration_cast <std::chrono::duration <mfloat> >
      (t2 - t1);
    if (T.empty()) {
      std::sprintf (str, "  Assignment auction NOT SOLVED");
      std::cout << str << std::endl;
    } else {
      pcst = Primal ("AP", ARX, T);
      std::sprintf (str, "  Assignment auction primal cost : %25.15f", pcst);
      std::cout <<  str << std::endl;
      dcst = Dual (DWT, SWT, ARX, PR);
      std::sprintf (str, "  Assignment auction dual cost   : %25.15f", dcst);
      std::cout <<  str << std::endl;
      std::sprintf (str, "  Assignment auction diff        : %25.15f",
                    dcst-pcst);
      std::cout <<  str << std::endl;
      std::sprintf (str, "  Assignment auction time        : %13.3f sec",
                    dur.count() );
      std::cout << str << std::endl;
    }
    std::cout << "  ----------------------------------------------------------"
              << std::endl;
    if (wr) {
      err = WriteFile (sname, seed, DWT, SWT, ARX, T);
      wr = false;
    }
  }
  if (xo) {
    t1 = std::chrono::high_resolution_clock::now();
    SOrun (DWT, SWT, ARX, eps, min, stp, T, PR);
    t2 = std::chrono::high_resolution_clock::now();
    dur = std::chrono::duration_cast <std::chrono::duration <mfloat> >
      (t2 - t1);
    if (T.empty()) {
      std::sprintf (str, "  AUCTION-SO NOT SOLVED");
      std::cout << str << std::endl;
    } else {
      pcst = Primal ("SO", ARX, T);
      std::sprintf (str, "  AUCTION-SO primal cost         : %25.15f", pcst);
      std::cout <<  str << std::endl;
      dcst = Dual (DWT, SWT, ARX, PR);
      std::sprintf (str, "  AUCTION-SO dual cost           : %25.15f", dcst);
      std::cout <<  str << std::endl;
      std::sprintf (str, "  AUCTION-SO diff                : %25.15f",
                    dcst-pcst);
      std::cout <<  str << std::endl;
      std::sprintf (str, "  AUCTION-SO time                : %13.3f sec",
                    dur.count() );
      std::cout << str << std::endl;
    }
    std::cout << "  ----------------------------------------------------------"
              << std::endl;
    if (wr) {
      err = WriteFile (sname, seed, DWT, SWT, ARX, T);
      wr = false;
    }
  }
  if (xp) {
    objlist S;
    t1 = std::chrono::high_resolution_clock::now();
    SOPrun (SWT, DWT, RARX, eps, min, stp, S, PR);
    t2 = std::chrono::high_resolution_clock::now();
    dur = std::chrono::duration_cast <std::chrono::duration <mfloat> >
      (t2 - t1);
    if (S.empty()) {
      std::sprintf (str, "  AUCTION-SOP NOT SOLVED");
      std::cout << str << std::endl;
    } else {
      T.clear();
      for (muint it = 0; it < S.size(); it++) {
        T.emplace_back (S[it].c, S[it].j, S[it].i);
      }
      pcst = Primal ("SOP", ARX, T);
      std::sprintf (str, "  AUCTION-SOP primal cost        : %25.15f", pcst);
      std::cout <<  str << std::endl;
      dcst = Dual (DWT, SWT, ARX, PR);
      std::sprintf (str, "  AUCTION-SOP dual cost          : %25.15f", dcst);
      std::cout <<  str << std::endl;
      std::sprintf (str, "  AUCTION-SOP diff               : %25.15f",
                    dcst-pcst);
      std::cout <<  str << std::endl;
      std::sprintf (str, "  AUCTION-SOP time               : %13.3f sec",
                    dur.count() );
      std::cout << str << std::endl;
    }
    std::cout << "  ----------------------------------------------------------"
              << std::endl;
    if (wr) {
      err = WriteFile (sname, seed, DWT, SWT, ARX, T);
      wr = false;
    }
  }
  return err;
}

/** --- APrun ------------------------------------------------------------------
 **/
void APrun (const mfvec& DWT, const mfvec& SWT, const voblist& A,
            const mfloat MX, const mfloat MN, const mfloat ST,
            objlist& T, mfvec& PR) {
  APmap apslv (DWT, SWT, A, MX, MN, ST);
  apslv.Solve (T, PR);
  return;
}

/** --- GArun ------------------------------------------------------------------
 **/
void GArun (const mfvec& DWT, const mfvec& SWT, const voblist& A,
            const mfloat MX, const mfloat MN, const mfloat ST,
            objlist& T, mfvec& PR) {
  GAmap gslv (DWT, SWT, A, MX, MN, ST);
  gslv.Solve (T, PR);
  return;
}

/** --- SOPrun -----------------------------------------------------------------
 **/
void SOPrun (const mfvec& DWT, const mfvec& SWT, const voblist& A,
             const mfloat MX, const mfloat MN, const mfloat ST,
             objlist& T, mfvec& PR) {
  SOPmap sopslv (DWT, SWT, A, MX, MN, ST);
  sopslv.Solve (T, PR);
  return;
}

/** --- SOrun -----------------------------------------------------------------
 **/
void SOrun (const mfvec& DWT, const mfvec& SWT, const voblist& A,
            const mfloat MX, const mfloat MN, const mfloat ST,
            objlist& T, mfvec& PR) {
  SOmap soslv (DWT, SWT, A, MX, MN, ST);
  soslv.Solve (T, PR);
  return;
}

/** --- Dual -------------------------------------------------------------------
 **/
mfloat Dual (const mfvec& DWT, const mfvec& SWT, const voblist& ARX,
             const mfvec& PR) {
  mfloat dcst = 0.0;
  mfvec  M;
  char   str[255];
  for (muint i = 0; i < DWT.size(); i++) {
    M.clear();
    for (muint j = 0; j < ARX[i].size(); j++) {
      M.push_back (ARX[i][j].c - PR[muint (ARX[i][j].j)]);
      if (gVBS > 1) {
        std::sprintf (str, " Exp %lu -> %lu : %f - %f = %f", i, j,
                      ARX[i][j].c, PR[muint (ARX[i][j].j)], M.back());
        std::cout << str << std::endl;
      }
    }
    dcst += DWT[i] * *std::max_element (M.begin(), M.end());
  }
  if (gVBS > 1) {
    std::cout << "---------------" << std::endl;
  }
  for (muint it = 0; it < SWT.size(); it++) {
    dcst += SWT[it] * PR[it];
    if (gVBS > 1) {
      std::sprintf (str, " Price %lu : %f", it, PR[it]);
      std::cout << str << std::endl;
    }
  }
  if (gVBS > 1) {
    std::cout << "---------------" << std::endl;
  }
  return dcst;
}

/** --- Primal -----------------------------------------------------------------
 **/
mfloat Primal (const char* aname, const voblist& ARX, const objlist& T) {
  mfloat pcst = 0.0;
  muint  tst;
  char   str[255];
  for (muint it = 0; it < T.size(); it++) {
    tst = 0;
    while (   (tst < ARX[muint (T[it].i)].size())
           && (ARX[muint (T[it].i)][tst].j != T[it].j)) {
      tst++;
    }
    if (tst < ARX[muint (T[it].i)].size()) {
      pcst += ARX[muint (T[it].i)][tst].c * T[it].c;
      if (gVBS > 1) {
        std::sprintf (str, "    %3ld -> %3ld : %f @ %f ", T[it].j, T[it].i,
                      T[it].c, ARX[muint (T[it].i)][tst].c);
        std::cout << str << std::endl;
      }
    } else {
      std::cout << "Overflow in " << aname << " cost calculation" << std::endl;
    }
  }
  return pcst;
}

/** --- GetOption --------------------------------------------------------------
 **/
char* GetOption (char ** begin, char ** end, const std::string & option) {
  char ** itr = std::find(begin, end, option);
  if (itr != end && ++itr != end) {
    return *itr;
  }
  return 0;
}

/** --- GetOptionList ----------------------------------------------------------
 **/
char** GetOptionList (char ** begin, char ** end, const std::string & option,
                     const muint cnt) {
  char ** itr = std::find(begin, end, option);
  if ((itr == end) || (++itr == end)) {
    return 0;
  }
  muint ctr = 0;
  while (++ctr < cnt) {
    if (itr+ctr == end) {
      return 0;
    }
  }
  return itr;
}

/** --- OptionExists -----------------------------------------------------------
 **/
bool OptionExists (char** begin, char** end, const std::string& option) {
  return std::find(begin, end, option) != end;
}

/** --- Split ------------------------------------------------------------------
 **/
void Split (const std::string &s, std::vector<std::string> &elms) {
  Split (s, ' ', elms);
  return;
}

/** --- Split ------------------------------------------------------------------
 **/
void Split (const std::string &s, char dlm, std::vector<std::string> &elms) {
  elms.clear();
  std::stringstream ss(s);
  std::string itm;
  while (std::getline(ss, itm, dlm)) {
    if (!itm.empty()) {
      elms.push_back(itm);
    }
  }
  return;
}

/** --- ReadFile ---------------------------------------------------------------
 **/
muint ReadFile (const char* fname, mfvec& DWT, mfvec& SWT, voblist& ARX,
                voblist& RARX) {
  mfloat AMT;    // individual node weight
  mfloat CST;    // individual arc cost
//   mfloat FLW;    // individual arc flow
  muint  I;      // individual node id
//   muint  IARCS;  // number of arcs
  muint  IH;     // individual arc head
  muint  ISEED;  // random number seed
  muint  IT;     // individual arc tail
//   mfloat ITSUP;  // total supply
  muint  NSINK;  // total number of sink nodes
  muint  NSORC;  // total number of source nodes
  bool good = false;
  bool rev  = false;
  ARX.clear ();
  SWT.clear ();
  DWT.clear ();
  std::ifstream f (fname);
  if (f.is_open()) {
    std::string line;
    if (std::getline (f, line)) {
      std::vector<std::string> elms;  
      Split (line, elms);
      if (!elms.empty()) {
        bool  cont = true;
        muint ctr  = 1;
        muint pos  = 0;
        if (elms.size() == 1) {
          if (elms[0].compare ("BEGIN") == 0) {
            while ((cont) && (std::getline (f, line))) {
              cont = false;
              Split (line, elms);
              switch (pos) {
                case 0:
                  if ((elms.size() == 8) &&
                      (elms[0].compare ("GRAPH") == 0)) {
                    ISEED  = std::stoul (elms[1], nullptr, 10);
                    if (elms[2].compare ("WITH") == 0) {
                      NSINK  = std::stoul (elms[3], nullptr, 10);
                      if (   (elms[4].compare ("SINKS") == 0)
                          && (elms[5].compare ("AND") == 0)) {
                        NSORC  = std::stoul (elms[6], nullptr, 10);
                        if (elms[7].compare ("SOURCES") == 0) {
                          cont = true;
                          pos = 1;
                          if (NSORC > NSINK) {
                            rev = true;
                            DWT.reserve (NSORC);
                            SWT.reserve (NSINK);
                            ARX.resize (NSORC);
                            RARX.resize (NSINK);
                          } else {
                            DWT.reserve (NSINK);
                            SWT.reserve (NSORC);
                            ARX.resize (NSINK);
                            RARX.resize (NSORC);
                          }
                        }
                      }
                    }
                  }
                  break;
                case 1:
                  if (elms.size() == 6) {
                    // IARCS = std::stoul (elms[0], nullptr, 10); // UNUSED
                    if (   (elms[1].compare ("ARCS") == 0)
                        && (elms[2].compare ("AND") == 0)) {
                      // ITSUP  = std::stod  (elms[3], nullptr); // UNUSED
                      if ((elms[4].compare ("TOTAL") == 0)
                          && (elms[5].compare ("WEIGHT") == 0)) {
                        cont = true;
                        pos = 2;
                      }
                    }
                  }
                case 2:
                  if ((elms.size() == 1) && (elms[0].compare ("DEMAND") == 0)) {
                    cont = true;
                    pos = 3;
                  }
                  break;
                case 3:
                  if (elms.size() == 1) {
                    if (elms[0].compare ("ARCS") == 0) {
                      cont = true;
                      pos = 4;
                    }
                  } else if (elms.size() == 2) {
                    I = std::stoul (elms[0], nullptr, 10);
                    if (I == ctr) {
                      AMT = std::stod (elms[1], nullptr);
                      if (rev) {
                        SWT.push_back(AMT);
                      } else {
                        DWT.push_back(AMT);
                      }
                      cont = true;
                      ctr++;
                    }
                  }
                  break;
                case 4:
                  if (elms.size() == 1) {
                    if (elms[0].compare ("SUPPLY") == 0) {
                      cont = true;
                      pos = 5;
                    }
                  } else if (elms.size() == 4) {
                    IT  = std::stoul (elms[0], nullptr, 10);
                    IH  = std::stoul (elms[1], nullptr, 10);
                    CST = std::stod  (elms[2], nullptr);
                    // FLW = std::stod  (elms[3], nullptr); // UNUSED
                    if (rev) {
                      ARX[IH-NSINK-1].emplace_back (CST, IH-NSINK-1, IT-1);
                      RARX[IT-1].emplace_back (CST, IT-1, IH-NSINK-1);
                    } else {
                      ARX[IT-1].emplace_back (CST, IT-1, IH-NSINK-1);
                      RARX[IH-NSINK-1].emplace_back (CST, IH-NSINK-1, IT-1);
                    }
                    cont = true;
                  }
                  break;
                case 5:
                  if (elms.size() == 1) {
                    if (elms[0].compare ("END") == 0) {
                      good = true;
                    }
                  } else if (elms.size() == 2) {
                    I = std::stoul (elms[0], nullptr, 10);
                    if (I == ctr) {
                      AMT = std::stod (elms[1], nullptr);
                      if (rev) {
                        DWT.push_back(AMT);
                      } else {
                        SWT.push_back(AMT);
                      }
                      cont = true;
                      ctr++;
                    }
                  }
                  break;
                default:
                  break;
              }
            }
          }
        }
      }
    }
    f.close();
  }
  if (!good) {
    ISEED = 0;
  }
  return ISEED;
}

/** --- WriteFile --------------------------------------------------------------
 **/
int WriteFile (const char* fname, const muint seed, const mfvec& DWT,
               const mfvec& SWT, const voblist& ARX, const objlist& T) {
  int    err = 0;
  muint  arc = 0;
  mfloat twt = 0.0;
  muint  tst;
  char  str[255];
  std::vector <mfvec> FL (DWT.size());
  std::ofstream f (fname, std::ios::out | std::ios::trunc);
  if (f.is_open()) {
    f << "BEGIN" << std::endl;
    for (muint i = 0; i < ARX.size(); i++) {
      arc += ARX[i].size();
    }
    for (muint i = 0; i < DWT.size(); i++) {
      twt += DWT[i];
    }
    std::sprintf (str, "GRAPH %010lu WITH %lu SINKS AND %lu SOURCES", seed,
                  DWT.size(), SWT.size());
    f << str << std::endl;
    if (equal (twt, std::floor (twt))) {
      std::sprintf (str, " %lu ARCS AND %lu TOTAL WEIGHT", arc, muint (twt));
    } else {
      std::sprintf (str, " %lu ARCS AND %.8f TOTAL WEIGHT", arc, twt);
    }
    f << str << std::endl;
    f << "DEMAND" << std::endl;
    for (muint it = 0; it < DWT.size(); it++) {
      if (equal (DWT[it], std::floor (DWT[it]))) {
        std::sprintf (str, " %lu %lu", it + 1, muint (DWT[it]));
      } else {
        std::sprintf (str, " %lu %.8f", it + 1, DWT[it]);
      }
      f << str << std::endl;
    }
    f << "ARCS" << std::endl;
    for (muint it = 0; it < ARX.size(); it++) {
      FL[it] = mfvec (ARX[it].size(), 0.0);
    }
    for (muint it = 0; it < T.size(); it++) {
      tst = 0;
      while (   (tst < ARX[muint (T[it].i)].size())
             && (ARX[muint (T[it].i)][tst].j != T[it].j)) {
        tst++;
      }
      if (tst < ARX[muint (T[it].i)].size()) {
        FL[muint (T[it].i)][tst] = T[it].c;
      } else {
        std::cout << "Overflow in flow calculation" << std::endl;
      }
    }
    for (muint i = 0; i < ARX.size(); i++) {
      for (muint j = 0; j < ARX[i].size(); j++) {
        if (equal (ARX[i][j].c, std::floor (ARX[i][j].c))) {
          if (equal (FL[i][j], std::floor (FL[i][j]))) {
            std::sprintf (str, " %lu %lu %lu %lu",
                          ARX[i][j].i + 1, muint (ARX[i][j].j) + DWT.size() + 1,
                          muint (ARX[i][j].c), muint (FL[i][j]));
          } else {
            std::sprintf (str, " %lu %lu %lu %.8f",
                          ARX[i][j].i + 1, muint (ARX[i][j].j) + DWT.size() + 1,
                          muint (ARX[i][j].c), FL[i][j]);
          }
        } else {
          if (equal (FL[i][j], std::floor (FL[i][j]))) {
            std::sprintf (str, " %lu %lu %.8f %lu", ARX[i][j].i + 1,
                          muint (ARX[i][j].j) + DWT.size() + 1,
                          ARX[i][j].c, muint (FL[i][j]));
          } else {
            std::sprintf (str, " %lu %lu %.8f %.8f", ARX[i][j].i + 1,
              muint (ARX[i][j].j) + DWT.size() + 1, ARX[i][j].c, FL[i][j]);
          }
        }
        f << str << std::endl;
      }
    }
    f << "SUPPLY" << std::endl;
    for (muint it = 0; it < SWT.size(); it++) {
      if (equal (SWT[it], std::floor (SWT[it]))) {
        std::sprintf (str, " %lu %lu", it + DWT.size() + 1, muint (SWT[it]));
      } else {
        std::sprintf (str, " %lu %.8f", it + DWT.size() + 1, SWT[it]);
      }
      f << str << std::endl;
    }
    f << "END" << std::endl;
    f.close();
  }
  return err;
}

/// ----------------------------------------------------------------------------
