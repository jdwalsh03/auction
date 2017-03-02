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
#include <cmath>       // std::floor, std::round
#include <cstdlib>     // exit, EXIT_FAILURE
#include <cstdio>      // std::sprintf
#include <fstream>     // std::ofstream
#include <iostream>    // std::cout, std::endl, std::ios::out, std::ios::trunc
#include <random>      // std::mt19937_64, std::uniform_int_distribution,
                       //   std::uniform_real_distribution
#include <set>         // std::set
#include <sstream>     // std::stringstream
#include <string>      // std::string, std::getline
#include <vector>      // std::vector
#include "glob.hpp"    // mfloat, mfvec, muint
#include "object.hpp"  // objlist, voblist

mfloat gEPS = std::sqrt(std::numeric_limits<mfloat>::epsilon());

char* GetOption (char ** begin, char ** end, const std::string & option);
char** GetOptionList (char ** begin, char ** end, const std::string & option,
                      const muint cnt);
bool OptionExists (char** begin, char** end, const std::string& option);
void Split (const std::string &s, std::vector<std::string> &elms);
void Split (const std::string &s, char dlm, std::vector<std::string> &elms);
muint ReadFile (const char* fname, muint& dsz, muint& ssz, muint& asz,
                muint& wmx, bool& rv);
int  WriteFile (const char* fname, const muint seed, const mfvec& DWT,
                const mfvec& SWT, const objlist& ARX);

/** --- main -------------------------------------------------------------------
 * program flags:
 *   -a # : number of arcs [integer # >= sinks + sources - 1]
 *   -c # : maximum cost [positive integer # >= 1]
 *   -d # : randomize using seed # [default: time-based seed]
 *   -i   : integer-valued graph
 *   -k # : number of sinks [positive integer]
 *   -l S : load graph characteristics from data file S
 *   -p # : percentage of arcs [real number 0 < # < 1]
 *   -r # : number of sources [positive integer]
 *   -s S : save graph to data file S
 *   -w # : maximum weight [positive integer # >= 1]
 *   -x   : return initial time-based seed and exit, ignoring other flags
 **/

int main (int argc, char *argv[])
{
  muint   asz = 0;     // desired arc set size
  muint   cmx = 100;   // maximum arc cost
  muint   ssz = 5;     // desired negative vertex set size
  muint   dsz = 5;     // desired positive vertex set size
  bool    rv  = true;  // real-valued output?
  int     scc = 0;     // success / error output
  muint   seed = 0;    // random seed
  char    str[255];    // display string
  muint   wmx = 100;   // maximum vertex weight
  // Implement option flags
  char* lname = GetOption(argv, argv + argc, "-l");
  if (lname != 0) {
    seed = ReadFile (lname, dsz, ssz, asz, wmx, rv);
  }
  char* sdstr = GetOption(argv, argv + argc, "-d");
  if (sdstr == 0) {
    if (seed == 0) {
      std::random_device rd;
      seed = rd();
    }
  } else {
    seed = std::strtoul (sdstr, nullptr, 0);
  }
  if (OptionExists(argv, argv+argc, "-x")) {
    std::sprintf (str, " * seed = %lu", seed);
    std::cout << str << std::endl;
  } else {
    char* kstr = GetOption(argv, argv + argc, "-k");
    if (kstr != 0) {
      if (std::strtol (kstr, nullptr, 0) > 0) {
        dsz = std::strtoul (kstr, nullptr, 0);
      }
    }
    char* rstr = GetOption(argv, argv + argc, "-r");
    if (rstr != 0) {
      if (std::strtol (rstr, nullptr, 0) > 0) {
        ssz = std::strtoul (rstr, nullptr, 0);
      }
    }
    char* astr = GetOption(argv, argv + argc, "-a");
    if (astr != 0) {
      if (std::strtol (astr, nullptr, 0) > 0) {
        asz = std::strtoul (astr, nullptr, 0);
      }
    }
    char* wstr = GetOption(argv, argv + argc, "-w");
    if (wstr != 0) {
      if (std::strtol (wstr, nullptr, 0) >= 1) {
        wmx = std::strtoul (wstr, nullptr, 0);
      }
    }
    char* cstr = GetOption(argv, argv + argc, "-c");
    if (cstr != 0) {
      if (std::strtol (cstr, nullptr, 0) >= 1) {
        cmx = std::strtoul (cstr, nullptr, 0);
      }
    }
    if (asz == 0) {
      char* pstr = GetOption(argv, argv + argc, "-p");
      if (pstr != 0) {
        if (   (std::strtod (pstr, nullptr) >  0)
            && (std::strtod (pstr, nullptr) <= 1)) {
          asz = muint (std::round
                  (std::strtod (pstr, nullptr) * mfloat (dsz) * mfloat (ssz)));
        }
      }
    }
    if (asz == 0) {
      asz = muint (std::ceil (0.25 * mfloat (dsz) * mfloat (ssz)));
    }
    if (asz < dsz + ssz - 1) {
      asz = dsz + ssz - 1;
    }
    if (OptionExists(argv, argv+argc, "-i")) {
      rv = false;
    }
    char* fname = GetOption(argv, argv + argc, "-s");
    muint   act = 0;         // arc count
    objlist ARX;             // arc list
    muint   k;               // positive vertex index
    muint   l;               // negative vertex index
    bool    ps;              // positive vertex next?
    mfvec   SWT (ssz, 0.0);  // negative vertex weights
    mfvec   DWT (dsz, 0.0);  // positive vertex weights
    // INITIALIZE RANDOM GENERATORS
    std::mt19937_64 gen (seed);
    std::uniform_int_distribution <muint>  dsci (1, cmx);
    std::uniform_real_distribution<mfloat> dscr (1.0, mfloat (cmx));
    std::uniform_int_distribution <muint>  dstd (0, dsz - 1);
    std::uniform_int_distribution <muint>  dsti (1, wmx);
    std::uniform_int_distribution <muint>  dstp;
    std::uniform_real_distribution<mfloat> dstr (1.0, mfloat (wmx));
    std::uniform_int_distribution <muint>  dsts (0, ssz - 1);
    std::sprintf (str, " * seed = %lu", seed);
    std::cout << str << std::endl;
    // CONSTRUCT FULL ARC ARRAY WITH ALL EDGES HAVING ZERO WEIGHT
    ARX = objlist (dsz * ssz);
    for (muint i = 0; i < dsz; i++) {
      for (muint j = 0; j < ssz; j++) {
        ARX[i * ssz + j] = Object (0.0, i, j);
      }
    }
    std::cout << "Empty arc array constructed" << std::endl;
    // ENSURE AT LEAST ONE POSITIVE WEIGHT EDGE FOR EACH SINK
    for (muint it = 0; it < dsz; it++) {
      if (rv) {
        ARX[it * ssz + dsts (gen)].c = dstr (gen);
      } else {
        ARX[it * ssz + dsts (gen)].c = mfloat (dsti (gen));
      }
      ++act;
    }
    std::sprintf (str, "Sinks connected: %lu positive arcs", act);
    std::cout << str << std::endl;
    // ENSURE AT LEAST ONE POSITIVE WEIGHT EDGE FOR EACH SOURCE
    for (muint it = 0; it < ssz; it++) {
      k = 0;
      while ((k < dsz) && (ARX[k * ssz + it].c < gEPS)) {
        ++k;
      }
      if (k == dsz) {
        if (rv) {
          ARX[dstd (gen) * ssz + it].c = dstr (gen);
        } else {
          ARX[dstd (gen) * ssz + it].c = mfloat (dsti (gen));
        }
        ++act;
      }
    }
    std::sprintf (str, "Sources connected: %lu positive arcs", act);
    std::cout << str << std::endl;
    // ENSURE GRAPH IS CONNECTED, ADDING WEIGHTED EDGES AS NECESSARY
    muvec pth (1, 0);     // point thread
    muint pam = dsz - 1;  // positive amount of unassigned vertices
    muint nam = ssz;      // negative amount of unassigned vertices
    std::vector <bool> PNV (dsz, false); // positive vertex not available?
    std::vector <bool> NNV (ssz, false); // negative vertex not available?
    PNV[0] = true;
    do {
      k = 0;
      if (pth.size() % 2 == 0) {
        while ((k < dsz) && (PNV[k] || (ARX[k * ssz + pth.back()].c < gEPS))) {
          ++k;
        }
        if (k < ssz) {
          PNV[k] = true;
          --pam;
          pth.push_back (k);
        } else {
          pth.pop_back();
        }
      } else {
        while ((k < ssz) && (NNV[k] || (ARX[pth.back() * ssz + k].c < gEPS))) {
          ++k;
        }
        if (k < ssz) {
          NNV[k] = true;
          --nam;
          pth.push_back (k);
        } else {
          pth.pop_back();
        }
      }
      if ((pth.size() == 0) && ((pam > 0) || (nam > 0))) {
        if ((pam > 0) && (nam > 0)) {
          if (nam == ssz) {
            ps = false;
          } else {
            ps = (dsti (gen) % 2 == 0);
          }
        } else if (pam > 0) {
          ps = true;
        } else {
          ps = false;
        }
        if (ps && (nam == ssz)) {
          ps = false;
        }
        if (ps) {
          do {
            k = dstd (gen);
          } while (PNV[k]);
          do {
            l = dsts (gen);
          } while (!NNV[l]);
          --pam;
        } else {
          do {
            k = dstd (gen);
          } while (!PNV[k]);
          do {
            l = dsts (gen);
          } while (NNV[l]);
        }
        if (rv) {
          ARX[k * ssz + l].c = dstr (gen);
        } else {
          ARX[k * ssz + l].c = mfloat (dsti (gen));
        }
        ++act;
        PNV[k] = true;
        pth.push_back (k);
      }
    } while ((pam > 0) || (nam > 0));
    std::sprintf (str, "Graph connected: %lu positive arcs", act);
    std::cout << str << std::endl;
    // TOTAL UP WEIGHTS FOR DWT AND SWT
    for (muint i = 0; i < dsz; i++) {
      for (muint j = 0; j < ssz; j++) {
        DWT[i] += ARX[i * ssz + j].c;
        SWT[j] += ARX[i * ssz + j].c;
      }
    }
    std::cout << "Vertex weights assigned" << std::endl;
    // RANDOMLY REMOVE MEMBERS FROM ARX UNTIL ARX SIZE EQUALS ASZ
    muint tst;
    if (asz < act) {
      asz = act;
    }
    act  = ARX.size();
    while (ARX.size() > asz) {
      --act;
      dstp = std::uniform_int_distribution <muint>  (0, act);
      do {
        tst = dstp (gen);
        if (ARX[tst].c < gEPS) {
          ARX[tst] = ARX.back();
          ARX.pop_back();
        }
      } while (ARX.size() > act);
    }
    std::sort (ARX.begin(), ARX.end(), [](const Object& a, const Object& b)
      -> bool { return ((a.i < b.i) || ((a.i == b.i) && (a.j < b.j))); });
    std::sprintf (str, "Graph pruned: %lu arcs remaining", asz);
    std::cout << str << std::endl;
    // ASSIGN COSTS TO REMAINING ARCS
    for (muint it = 0; it < ARX.size(); it++) {
      if (rv) {
        ARX[it].c = dscr (gen);
      } else {
        ARX[it].c = mfloat (dsci (gen));
      }
    }
    std::cout << "Arc costs assigned" << std::endl;
    // WRITE GRAPH FILE
    if (fname == 0) {
      char dname[255];
      if (rv) {
        std::sprintf (dname, "R-%04lu-%04lu-%06lu-%02lu.txt",
                      ssz, dsz, asz, wmx);
      } else {
        std::sprintf (dname, "I-%04lu-%04lu-%06lu-%02lu.txt",
                      ssz, dsz, asz, wmx);
      }
      scc = WriteFile (dname, seed, DWT, SWT, ARX);
    } else {
      scc = WriteFile (fname, seed, DWT, SWT, ARX);
    }
    std::cout << "Graph file written" << std::endl;
  }
  return scc;
}

/** --- GetOption --------------------------------------------------------------
 **/
char* GetOption (char ** begin, char ** end, const std::string & option) {
  char ** itr = std::find (begin, end, option);
  if (itr != end && ++itr != end) {
    return *itr;
  }
  return 0;
}

/** --- GetOptionList ----------------------------------------------------------
 **/
char** GetOptionList (char ** begin, char ** end, const std::string & option,
                     const muint cnt) {
  char ** itr = std::find (begin, end, option);
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
  return std::find (begin, end, option) != end;
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
muint ReadFile (const char* fname, muint& dsz, muint& ssz, muint& asz,
                muint& wmx, bool& rv) {
  muint  seed;
  bool good = false;
  std::ifstream f (fname);
  if (f.is_open()) {
    std::string line;
    if (std::getline (f, line)) {
      std::vector<std::string> elms;
      Split (line, elms);
      if (!elms.empty()) {
        bool  cont = true;
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
                    seed = std::stoul (elms[1], nullptr, 10);
                    if (elms[2].compare ("WITH") == 0) {
                      dsz = std::stoul (elms[3], nullptr, 10);
                      if (   (elms[4].compare ("SINKS") == 0)
                          && (elms[5].compare ("AND") == 0)) {
                        ssz = std::stoul (elms[6], nullptr, 10);
                        if (elms[7].compare ("SOURCES") == 0) {
                          cont = true;
                          pos = 1;
                        }
                      }
                    }
                  }
                  break;
                case 1:
                  if (elms.size() == 6) {
                    asz = std::stoul (elms[0], nullptr, 10);
                    if (   (elms[1].compare ("ARCS") == 0)
                        && (elms[2].compare ("AND") == 0)) {
                      wmx = std::stoul  (elms[3], nullptr);
                      if (elms[4].compare ("INTEGER") == 0) {
                        rv = false;
                      }
                      if (   ((!rv) || (elms[4].compare ("REAL") == 0))
                          && (elms[5].compare ("ARC-WEIGHT") == 0)) {
                        cont = true;
                        pos = 2;
                      }
                    }
                  }
                case 2:
                  if ((elms.size() == 1) && (elms[0].compare ("END") == 0)) {
                    good = true;
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
    seed = 0;
  }
  return seed;
}

/** --- WriteFile --------------------------------------------------------------
 **/
int WriteFile (const char* fname, const muint seed, const mfvec& DWT,
               const mfvec& SWT, const objlist& ARX) {
  int    err = 0;
  mfloat twt = 0.0;
  char  str[255];
  std::ofstream f (fname, std::ios::out | std::ios::trunc);
  if (f.is_open()) {
    f << "BEGIN" << std::endl;
    for (muint i = 0; i < DWT.size(); i++) {
      twt += DWT[i];
    }
    std::sprintf (str, "GRAPH %010lu WITH %lu SINKS AND %lu SOURCES", seed,
                  DWT.size(), SWT.size());
    f << str << std::endl;
    if (equal (twt, std::floor (twt))) {
      std::sprintf (str, " %lu ARCS AND %lu TOTAL WEIGHT", ARX.size(),
                    muint (twt));
    } else {
      std::sprintf (str, " %lu ARCS AND %.8f TOTAL WEIGHT", ARX.size(),
                    twt);
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
      if (equal (ARX[it].c, std::floor (ARX[it].c))) {
        std::sprintf (str, " %lu %lu %lu %lu",
                      ARX[it].i + 1, ARX[it].j + DWT.size() + 1,
                      muint (ARX[it].c), muint (0));
      } else {
        std::sprintf (str, " %lu %lu %.8f %lu", ARX[it].i + 1,
          ARX[it].j + DWT.size() + 1, ARX[it].c, muint (0));
      }
      f << str << std::endl;
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
