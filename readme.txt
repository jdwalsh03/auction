AUCTION ALGORITHMS IN C++ v1.0.0 : <readme.txt>

--------------------------------------------------------------------------------
Synopsis
--------------------------------------------------------------------------------

The auction method was developed by Dimitri Bertsekas in the late 1970s as a 
relaxation technique for solving integer-valued assignment problems. It
resembles a competitive bidding process, where unsatisfied persons (bidders)
attempt to claim the objects (lots) offering the best value. In 1989, Bertsekas
and David Castañón found a way to apply the auction method to  certain transport
problems. By transforming integer-valued transport problems into assignment
problems, the auction method can be extended to compute optimal transport
solutions. In 2016, J.D. Walsh and Luca Dieci generalized the work of Bertsekas
and Castañón, developing a more general auction method that can be applied
directly to real-valued transport problems.

All of these auction methods were described in detailed journal articles, and
software implementations of the original auction method for integer-valued
assignment are commonly available. However, implementations for integer-valued
transport are much more rare. Before now, no implementation of the general
auction method for real-valued transport has been published.

The AUCTION ALGORITHMS IN C++ software project was created to fill that gap. It
implements four auction algorithms for optimal transport. Three of the
algorithms solve integer-based transport problems by expanding them
into corresponding assignment problems and applying various optimizing
techniques to the standard auction algorithm. These three algorithms, described
by Bertsekas and Castañón, are:

  AUCTION     : standard auction algorithm without optimizations
  AUCTION-SO  : auction algorithm optimized for similar objects
  AUCTION-SOP : auction algorithm for similar objects and persons

The fourth algorithm solves real-valued transport problems without generating
assignment problems. This algorithm, described by Walsh and Dieci, is:

  General auction : transport-based auction algorithm

The general auction is so-named because it works on a broader class of problems
than the standard assignment-based auction.

The AUCTION ALGORITHMS IN C++ project is designed as a teaching tool, to help
readers of the journal articles (see "References" below) to understand how the
various auction algorithms can be implemented in practice. With this goal in
mind, the source code, provided in C++, favors clarity over efficiency. Error
checking is limited. The implementation relies heavily on the Standard Library
and a large number of (relatively) simple class objects.

That said, AUCTION ALGORITHMS is also a working project. The code has been
optimized over multiple iterations, and variations of it are currently being
applied to research in computational optimal transport. Using the file format
given in <graph/readme.txt> and the software provided as <auction.out>, one can
use the project as-is to exactly solve integer-valued transport problems and
approximate solutions to real-valued problems.

Those problems can be quite large; for example, see the graph template
<graph/test/R-5000-45000-31500000-99.txt>. Fully constructed, the graph file
includes 31.5 million arcs and requires 0.8 GB of storage space. Using a
2.27 GHz i5 processor, the general auction default settings can approximate the
solution (within 3e-8 percent) in less than a minute. The main limitation in
this case is storage; <graph.out> requires over 5 GB of memory to generate the
graph file, and <auction.out> uses 2.7 GB of memory when solving it.

This file is only one piece of the AUCTION ALGORITHMS IN C++ software project.
See the document <filelist.txt>, found in the parent folder of <graph>, for a
full list of the project files. If <filelist.txt> or any other file is missing,
go to <http://www.jdwalsh03.com/> to download the complete project.

--------------------------------------------------------------------------------
Motivation
--------------------------------------------------------------------------------

The AUCTION ALGORITHMS IN C++ project grew out of my own exploration of the
algorithms described by Bertsekas and Castañón. I found very few online examples
of software applying the auction method, and none that were clearly designed for
optimal transport. A few projects showed how to apply the auction algorithm for
integer-based assignment problems. The most helpful, for me, was the original
software by Dimitri Bertsekas, written using Fortran 77; see [1] in "References"
(below). However, it only covers the integer-based assignment problem.

In the end, I built my own implementation of the auction algorithms of Bertsekas
and Castañón, first in Python, and later in C++. The process of writing and
understanding those algorithms, and my desire to apply the auction method to
real-valued transport, led to the general auction method described in the Walsh
and Dieci paper; see [4] in "References" for more information.

I hope this project will save other researchers the work (and missteps) I went
through when translating the mathematics of the auction algorithms into
effective computer code. I also believe here is a lot be be gained from reading
the nitty-gritty details of each implementation, and comparing the performance
of the various algorithms. After all, learning how algorithms function on a
computer is a necessary prerequisite for understanding their underlying
mathematics.

--JD

--------------------------------------------------------------------------------
Installation
--------------------------------------------------------------------------------

The project is ready to compile as-is, provided certain dependencies are
satisfied. Your computer should have the following installed:
  C++ Standard Library with C++11 support
    (originally compiled with libstdc++ version 4.9.2)
  g++ compiler version 4.7 or higher
    (originally compiled with g++ version 4.9.2)
  GNU Make
    (originally compiled using version 4.0)
See below for information on alternative development environments.

To install the full project in UNIX-based environments (e.g. Linux, MacOS,
Cygwin):

Open a terminal window in the top directory of the AUCTION ALGORITHMS IN C++
software project and run the installation script with the command:

  $ ./install.sh

(Depending on how the project was packaged, <install.sh> may need to be given 
permission to execute.) The install script will create the programs
<auction.out> and <graph/graph.out>, build a set of example graphs, and place
those graphs in the <test> folder.


To install the full project in more generic environments:

  1) Compile <graph.out> by entering the <graph> folder and calling make.
  2) Build one or more graphs:
     a) Choose a graph template XXX.txt from <graph/test/>
     b) In the <graph> folder, run the command ./graph.out -l XXX.txt
     c) Move the resulting file XXX.txt to a suitable location
        (this documentation assumes graphs are in <test>)
  3) Compile <auction.out> by calling make from the top directory.

--------------------------------------------------------------------------------
Installation Alternatives
--------------------------------------------------------------------------------

The program <auction.out> can be built directly by opening a terminal window in
the top directory and typing one of the following commands:
  make       : builds an optimized version of the executable <main.out>
  make check : checks code for various warning conditions and builds the
                 executable <main.out> with built-in debugging symbols
  make debug : builds the executable <main.out> with built-in debugging symbols
  make gprof : builds a custom version of the executable, <mainpg.out>, which
                 generates performance analysis data

Note: The project includes two additional (phony) commands that may be useful:
  make clean : deletes object files and dependency tracking files for a clean
                 recompile of <auction.out>
  make wipe  : deletes object files, dependency files, executables, and output
                 files from callgrind, gprof, and memtest

The software should compile in any development environment with C++11 support,
but the makefiles may require revision. For example, the project can be built
with clang++ version 3.8 by changing the compiler declarations (lines 4 and 5)
in the makefiles.

Additional graphs can be constructed using the program <graph/graph.out>; see
the file <graph/readme.txt> for details on this.

--------------------------------------------------------------------------------
Auction Program Examples
--------------------------------------------------------------------------------

Use the general auction method to compute the primal and dual solutions for the
graph given in the file <test/R-0500-4500-315000-09.txt>:

  $ ./auction.out -l ./test/R-0500-4500-315000-09.txt

Compute both solutions for the file <test/I-0100-0100-001400-09.txt> with each
one of the four auction algorithms:

  $ ./auction.out -l ./test/I-0100-0100-001400-09.txt -a -g -o -p

Apply the general auction method to <test/R-0100-0100-010000-99.txt>, using the
minimum epsilon value 1.0, with verbose command line output. Save the resulting
graph, including flows, to the file <result.txt>:

  $ ./auction.out -l ./test/R-0100-0100-010000-99.txt -s ./result.txt -v

--------------------------------------------------------------------------------
Auction Program <auction.out> Options
--------------------------------------------------------------------------------

  -a   : perform assignment auction
  -g   : perform general auction
  -l S : load and process graph file S
  -m # : maximum auction step size [requires # > 0]
  -n # : minimum auction step size [requires # > 0]
  -o   : perform extended (SO) auction
  -p   : perform extended (SOP) auction
  -r # : step size scaling rate [requires value 0 < # < 1]
  -s S : save graph results file S
  -v   : verbose output
  -vv  : very verbose output

--------------------------------------------------------------------------------
Auction Test Files
--------------------------------------------------------------------------------

The folder <test> contains example graphs, named with the format:
  A-BBBB-CCCC-DDDDDD-EE.txt
The format stands for:
  A      : either I for integer data (costs/weights) or R for real-valued data
  BBBB   : the number of supply (source) vertices in the graph
  CCCC   : the number of demand (sink) vertices in the graph
  DDDDDD : the number of arcs in the graph
  EE     : the largest arc weight used to initialize the graph

For example, <R-0500-4500-315000-09.txt> indicates a graph with:
  R      : Real-valued costs and weights
  0500   : 500 source vertices
  4500   : 4500 sink vertices
  315000 : 315000 arcs (14% of the maximum possible)
  09     : initialized using arc weights in the range [1,9]

--------------------------------------------------------------------------------
Graph File Format
--------------------------------------------------------------------------------

Graph files are formatted as follows:

  BEGIN
  GRAPH <random seed> WITH <# of demand> SINKS AND <# of supply> SOURCES
   <# of arcs> ARCS AND <total demand/supply weight> TOTAL WEIGHT
  DEMAND
   <demand index> <demand weight>
   [... one line for each demand vertex ...]
  ARCS
   <demand index> <supply index> <arc cost> <solution flow>
   [... one line for each arc ...]
  SUPPLY
   <supply index> <weight>
   [... one line for each supply vertex ...]
  END

All values must be positive, including the <random seed>, and all values except
the costs, weights, and flows must be integers.

In an unsolved graph file (such as those in <test>), the <solution flow> value
for each arc equals zero. In a solved problem, the <solution flow> values are
equal to the flows given by the resulting transport map.

--------------------------------------------------------------------------------
Creating Additional Graphs
--------------------------------------------------------------------------------

The folder <graph> contains a sub-project for creating additional graphs. See
the file <graph/readme.txt> for details on its use.

--------------------------------------------------------------------------------
Auction API Reference
--------------------------------------------------------------------------------

For each auction algorithm, a single XXmap object is created and called, where
XX is:
  AP  : the AUCTION algorithm for optimal transport [1-3]
  GA  : the General Auction method for optimal transport [4]
  SO  : the AUCTION-SO algorithm for optimal transport [3]
  SOP : the AUCTION-SOP algorithm for optimal transport [3]

All of the XXmap objects require the following files in order to compile:
  ./include/glob.hpp   : global header (number types, global variables)
  ./include/object.hpp : Object structure definition
  ./include/XXbid.hpp  : header for the bidder(s)
  ./include/XXmap.hpp  : header for the transport map solver
  ./src/XXbid.cpp      : source for bidder(s)
  ./src/XXmap.cpp      : source for transport map solver

In addition to the files above, the GAmap object requires three extra files:
  ./include/claim.hpp  : Claim structure definition
  ./include/galot.hpp  : header for the general auction lot object(s)
  ./src/galot.cpp      : source for the general auction lot object(s)

For portability, the algorithms use custom number types:
  mfloat : floating point number
  mint   : signed integer
  muint  : unsigned integer
  mfvec  : vector of mfloats

The structure Object is a general-purpose object with three elements:
  c  : a number associated with the arc (i, j) - stored as mfloat
  i  : the index of a demand vertex - stored as muint
  j  : the index of a supply vertex - stored as muint
Two Object-based types are also defined:
  objlist : vector of Objects
  voblist : vector of objlists

The XXmap objects rely on the existence of three global variables, initialized
in main.cpp:
  gEPS : machine precision (epsilon) - stored as mfloat
  gINF : infinity - stored as mfloat
  gVBS : verbosity - stored as muint
           0 (standard), 1 (verbose), or 2 (very verbose)

Each auction algorithm has an identical interface. First, the constructor
XXmap() is called with six input variables:
  DWT : ordered list of demand weights - stored as mfvec
  SWT : ordered list of supply weights - stored as mfvec
  A   : array of arc costs - stored as std::vector <std::vector <Object> >
         each Object has a cost (c) associated with (i,j)
  MX  : maximum (i.e. initial) auction step size (epsilon) - stored as mfloat
  MN  : minimum (i.e. final) auction step size (epsilon) - stored as mfloat
  ST  : auction step size (< 1.0) - stored as mfloat

Next the XXmap object function Solve() is called with two input/output
variables:
  T   : simplified transport plan - stored as std::vector <Object>
         each Object has a flow (c) associated with (i,j)
  PR  : list of supply prices - stored as std::vector <mfloat>
          the order is identical to that of SWT
Both variables are cleared on start. If Solve() exits successfully the resulting
transport plan and supply prices are returned.

The XXmap object can be destroyed as soon as Solve is complete.

--------------------------------------------------------------------------------
Code Example
--------------------------------------------------------------------------------

In order to use the XXmap auction code, first the graph itself must be
initialized. Take the simplest example with a meaningful dual cost, a graph with
one demand vertex and two supply vertices:

      S0 <----- D0 -----> S1

The following C++ code generates such a graph:

  /* graph variables */
  mfvec DWT (1);                           // demand vertex weight list
  mfvec SWT (2);                           // supply vertex weight list
  voblist ARX (1, objlist (2, Object()));  // arc cost list
  DWT[0] = 2.0;                            // demand vertex D0 with weight 2
  SWT[0] = 1.0;                            // supply vertex S0 with weight 1
  SWT[1] = 1.0;                            // supply vertex S1 with weight 1
  ARX[0][0] = Object (1.0, 0, 0);          // arc (D0,S0) with cost 1
  ARX[0][1] = Object (2.0, 0, 1);          // arc (D0,S1) with cost 2

Once the graph is prepared, GAmap can be called using the following C++ code:

  /* auction variables */
  mfloat MX  = 4.0;  // auction starting (maximum) step size
  mfloat MN  = 1.0;  // auction ending (minimum) step size
  mfloat SI = 0.25;  // auction step size increment
  /* solution variables */
  objlist T;         // transport map
  mfvec   PR;        // price list
  /* solve */
  GAmap gslv (DWT, SWT, ARX, MX, MN, SI);
  gslv.Solve (T, PR);

The resulting transport map can be used to compute the primal solution. Given an
arc list ARX and transport map T computed as above, the following C++ code
computes the primal solution:

  /* compute primal */
  muint  tst;         // test index
  mfloat psol = 0.0;  // primal solution
  for (muint it = 0; it < T.size(); it++) {
    tst = 0;
    while ((tst < ARX[T[it].i].size()) && (ARX[T[it].i][tst].j != T[it].j)) {
      tst++;
    }
    if (tst < ARX[T[it].i].size()) {
      psol += ARX[T[it].i][tst].c * T[it].c;
    }
  }

Given DWT, SWT, ARX, and the price list PR, the dual solution can be computed
using the following C++ code:

  /* compute dual */
  mfvec  M;           // expense array
  mfloat dsol = 0.0;  // dual solution
  for (muint i = 0; i < DWT.size(); i++) {
    M.clear();
    for (muint j = 0; j < ARX[i].size(); j++) {
      M.push_back (ARX[i][j].c - PR[ARX[i][j].j]);
    }
    dsol += DWT[i] * *std::max_element (M.begin(), M.end());
  }
  for (muint it = 0; it < SWT.size(); it++) {
    dsol += SWT[it] * PR[it];
  }

--------------------------------------------------------------------------------
Contact Information
--------------------------------------------------------------------------------

I am always interested in hearing how my software is used and how it might be
improved. If you have comments, bug reports, or questions, email them to me at
<math@jdwalsh03.com>.

--------------------------------------------------------------------------------
License
--------------------------------------------------------------------------------

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

--------------------------------------------------------------------------------
References
--------------------------------------------------------------------------------

[1] Bertsekas, D.P.: Auction and other algorithms for assignment problems.
URL http://www.mit.edu/~dimitrib/auction.txt

[2] Bertsekas, D.P.: Network Optimization: Continuous and Discrete Models.
Athena Scientific, Belmont, Massachusetts (1998)

[3] Bertsekas, D.P., Castañón, D.A.: The auction algorithm for the
transportation problem. Annals of Operations Research 20(1), 67–96 (1989)

[4] Walsh, J.D., Dieci, L.: General auction algorithm for real valued optimal
transport. Mathematical Programming [submitted] (2016)

--------------------------------------------------------------------------------
Acknowledgments
--------------------------------------------------------------------------------

This project would not have been possible without the advice and support of my
doctoral advisor, Luca Dieci of the Georgia Institute of Technology. Thank you,
Luca.

My interest in auction algorithms began during Fall of 2015, which I spent in
Norway with Michael Muskulus of the Norwegian University of Science and
Technology. Michael first gave me the idea of using auction methods to attack
optimal transport, and guided my early explorations of the algorithms.

Support from the Research Council of Norway (Norges forskningsråd) made my
semester with Michael possible. A Graduate Research Fellowship from the National
Science Foundation gave me the time to develop those initial explorations into
this project.

This material is based upon work supported by the National Science Foundation
Graduate Research Fellowship under Grant No. DGE-1148903. Any opinion,
findings, and conclusions or recommendations expressed in this material are
those of the author and do not necessarily reflect the views of the National
Science Foundation.

--------------------------------------------------------------------------------
