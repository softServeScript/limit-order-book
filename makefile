# ---------------------------------------------------------------
# MAKEFILE FOR THE LOB PROJECT
#   make          dev build (debug, slow, for fixing bugs)
#   make release  fast build
#   make bench    fast build for benchmark numbers
#   make check    shows where it found raylib
#   make clean    deletes build files
#
# You don't need to edit this file to add new .cpp files.
# Anything in this folder ending in .cpp is built automatically.
# ---------------------------------------------------------------

CXX    = g++
TARGET = lob.exe

# --- find raylib automatically -----------------------------------
# Looks inside ./raylib first, then C:/raylib (the Windows installer location).
# Header = raylib.h     Library = libraylib.a
RAY_H = $(firstword $(wildcard raylib/include/raylib.h raylib/src/raylib.h raylib/raylib.h C:/raylib/raylib/src/raylib.h))
RAY_A = $(firstword $(wildcard raylib/lib/libraylib.a raylib/src/libraylib.a raylib/libraylib.a C:/raylib/raylib/src/libraylib.a))

ifeq ($(RAY_H),)
$(warning raylib.h NOT FOUND. Put the raylib folder inside this project folder.)
RAY_INC = .
else
RAY_INC = $(patsubst %/,%,$(dir $(RAY_H)))
endif

ifeq ($(RAY_A),)
$(warning libraylib.a NOT FOUND. raylib has to be built (or use a prebuilt release) before this will link.)
RAY_LIB = .
else
RAY_LIB = $(patsubst %/,%,$(dir $(RAY_A)))
endif

# --- build settings ----------------------------------------------
OPT      = -g -O0
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra $(OPT) -I$(RAY_INC)
LDLIBS   = -L$(RAY_LIB) -lraylib -lopengl32 -lgdi32 -lwinmm

SRC = $(wildcard *.cpp) $(wildcard UI/*.cpp) $(wildcard Engine/*.cpp) $(wildcard RandomOrderGenerator/*.cpp)
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

all: $(TARGET)

# link: libraries (LDLIBS) must come AFTER the object files
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDLIBS)

# compile one .cpp into one .o  (-MMD -MP remembers which headers each file uses)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEP)

release: OPT = -O2 -DNDEBUG
release: clean $(TARGET)

bench: OPT = -O2 -DNDEBUG
bench: clean $(TARGET)

check:
	@echo raylib.h   : $(if $(RAY_H),$(RAY_H),NOT FOUND)
	@echo libraylib  : $(if $(RAY_A),$(RAY_A),NOT FOUND)

clean:
	rm -f $(OBJ) $(DEP) $(TARGET)

.PHONY: all release bench check clean