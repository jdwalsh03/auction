AUCTION ALGORITHMS IN C++ v1.0.0 : <graph/readme.txt>

--------------------------------------------------------------------------------
Synopsis
--------------------------------------------------------------------------------

The GRAPH sub-project is part of the AUCTION ALGORITHMS IN C++ software project.
Using either command-line input or a graph template file, the program
<graph.out> constructs the weighted digraph for a feasible transport problem and
saves the full graph in a new file. The graph format is easy to customize, and
can be populated with either integer or real-valued data.

GRAPH exists to support the program <auction.out>, which runs various auction
algorithms on graph files. In order to fully explore the properties of the
auction algorithm, it is convenient to have a large supply of graphs with
various characteristics. The program <graph.out> can provide that supply.

This file is only one piece of the AUCTION ALGORITHMS IN C++ software project.
See the document <filelist.txt>, found in the parent folder of <graph>, for a
full list of the project files. If <filelist.txt> or any other file is missing,
go to <http://www.jdwalsh03.com/> to download the complete project.

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

The program <graph.out> can be built directly by opening a terminal window in
the <graph> directory and typing one of the following commands:
  make       : builds an optimized version of the executable <main.out>
  make check : checks code for various warning conditions and builds the
                 executable <main.out> with built-in debugging symbols
  make debug : builds the executable <main.out> with built-in debugging symbols
  make gprof : builds a custom version of the executable, <mainpg.out>, which
                 generates performance analysis data

Note: The project includes two additional (phony) commands that may be useful:
  make clean : deletes object files and dependency tracking files for a clean
                 recompile of <graph.out>
  make wipe  : deletes object files, dependency files, executables, and output
                 files from callgrind, gprof, and memtest

The software should compile in any development environment with C++11 support,
but the makefiles may require revision. For example, the project can be built
with clang++ version 3.8 by changing the compiler declarations (lines 4 and 5)
in the makefiles.

Additional graphs can be constructed using the program <graph/graph.out>; see
below for details on this.

--------------------------------------------------------------------------------
Graph Program Examples
--------------------------------------------------------------------------------

Generate an integer-valued graph with 1000 sources, 1000 sinks, and 500000 arcs,
using a maximum arc-weight of 99. Save the result with the default file name
<I-1000-1000-500000-99.txt>:

  $ ./graph.out -r 1000 -k 1000 -a 500000 -w 99 -i

Because no seed value is given, each instance of this command generates a
different random graph with the given characteristics.

Use the graph template <test/I-1000-1000-500000-99.txt> to generate an
integer-valued graph with the same characteristics as above: 1000 sources,
1000 sinks, and 500000 arcs, with a maximum arc-weight of 99. Save the result
with the default file name <I-1000-1000-500000-99.txt>:

  $ ./graph.out -l ./test/I-1000-1000-500000-99.txt

If the graph file <I-1000-1000-500000-99.txt> has already been created (for
example, by the previous command), this command overwrites it. Because the
template assigns a fixed seed value, 3090516045, each time this command is given
it generates the same graph.

Use the graph template <test/R-1000-1000-500000-99.txt> to generate a
real-valued graph with 1000 sources, 1000 sinks, and 500000 arcs, and a maximum
arc-weight of 99. Save the result with the custom file name <mygraph.txt>:

  $ ./graph.out -l ./test/R-1000-1000-500000-99.txt -s mygraph.txt

Because the template file assigns a seed value of 0, each instance of this
command generates a different random graph with the desired characteristics.

--------------------------------------------------------------------------------
Graph Program <graph.out> Options
--------------------------------------------------------------------------------

  -a # : number of arcs [integer # >= sinks + sources - 1]
  -c # : maximum cost [positive integer # >= 1]
  -d # : randomize using seed # [default: time-based seed]
  -i   : integer-valued graph
  -k # : number of sinks [positive integer]
  -l S : load graph characteristics from data file S
  -p # : percentage of arcs [real number 0 < # < 1]
  -r # : number of sources [positive integer]
  -s S : save graph to data file S
  -w # : maximum weight [positive integer # >= 1]
  -x   : return initial time-based seed and exit, ignoring other flags

--------------------------------------------------------------------------------
Graph Test Files
--------------------------------------------------------------------------------

The folder <test> contains sample graph templates, named with the format:
  A-BBBB-CCCC-DDDDDD-EE.txt
The format stands for:
  A      : either I for integer data (costs/weights) or R for real-valued data
  BBBB   : the number of supply (source) vertices in the graph
  CCCC   : the number of demand (sink) vertices in the graph
  DDDDDD : the number of arcs in the graph
  EE     : the largest arc weight used to initialize the graph

For example, <R-0500-4500-315000-09.txt> indicates a template with:
  R      : Real-valued costs and weights
  0500   : 500 source vertices
  4500   : 4500 sink vertices
  315000 : 315000 arcs (14% of the maximum possible)
  09     : initialized using arc weights in the range [1,9]

--------------------------------------------------------------------------------
Graph Template Format
--------------------------------------------------------------------------------

The graph template accepted by <graph.out> consists of four lines, formatted as
follows:

  BEGIN
  GRAPH <random seed> WITH <# of demand> SINKS AND <# of supply> SOURCES
   <# of arcs> ARCS AND <max arc-weight> [INTEGER or REAL] ARC-WEIGHT
  END

A non-zero <random seed> will be used to generate the graph output. If
<random seed> is zero, the program will generate a new seed value for each run.

All values must be integers, and all except the <random seed> must be positive.

As indicated, the word "ARC-WEIGHT" should be preceded with either "INTEGER" or
"REAL" -- this tells the program whether the graph should generate
integer-valued or real-valued data (i.e. costs and weights).

--------------------------------------------------------------------------------
Graph File Format
--------------------------------------------------------------------------------

The graph files resulting from <graph.out> are formatted as follows:

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

In an unsolved graph file, such as those given by <graph.out>, the
<solution flow> value for each arc equals zero. In the file for a solved graph,
such as those given by running <auction.out>, the <solution flow> values are
equal to the flows given by the resulting transport map.

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
