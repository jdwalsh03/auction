# Makefile
#
# Compiler: tested with g++ 4.9.2 and clang++ 3.8
CC        = g++
CXX       = g++
# Alternative compiler
# CC        = clang++-3.8
# CXX       = clang++-3.8

# General purpose flags
CXXFLAGS+= --std=c++11
# Object files
OBJ      = main.o
# Target executable(s)
TARGET   = graph.out
PTARGET  = graphpg.out

# Directories for dependencies and targets (separate by spaces)
VPATH    = src

# Directories for header files (other than /usr/include)
INCLUDES = -I./include

# Other Files (to delete for blank)
DETRITUS = callgrind.out.* gmon.out massif.out.*

# # Flags that cause errors in support programs (GDB and Valgrind)
# CXXFLAGS+= -fsanitize=address -fsanitize=leak

# # Flags that cause errors in GCC version 4.9.2 (Debian 4.9.2-10)
# CXXFLAGS+= -fsanitize=thread

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# name this target entry anything, but "default" or "all"
# are most commonly used (by convention)
#
default: CXXFLAGS+= -O3
default: all

# Compile the object files
all: $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET)  $^

# Link and build target with dependency testing and file generation
DEPDIR = dep
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDES) $(TARGET_ARCH) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

%.o : %.c $(DEPDIR)/%.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cc $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cxx $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;

include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))

# Optimization Test-specific flags (pg for gprof)
gprof: CXXFLAGS+= -pg -O3
gprof: CXXFLAGS+= -fno-omit-frame-pointer
gprof: $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(PTARGET)  $^

# Debug-specific flags (use for GDB, Callgrind, Kdbg)
debug: CXXFLAGS+= -ggdb -O0
debug: CXXFLAGS+= -fno-omit-frame-pointer
debug: all

# # Build error checking flags
check: CXXFLAGS+= -ggdb -O0
check: CXXFLAGS+= -fno-omit-frame-pointer
check: CXXFLAGS+= -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wfloat-equal
check: CXXFLAGS+= -Wconversion -Wcast-qual -Wcast-align
# # Runtime error checking / Hardening flags
check: CXXFLAGS+= -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
check: CXXFLAGS+= -D_FORTIFY_SOURCE=2 -fstack-protector
check: all

.PHONY : clean blank

# Recompile from scratch
clean: 
	$(RM) *.o *~ ./dep/*.Td

# Remove all non-source files from directory (subdirectories unchanged)
blank: 
	$(RM) *.o *~ ./dep/*.Td $(DETRITUS) $(TARGET) $(PTARGET)
