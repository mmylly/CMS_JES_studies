# These may vary slightly depending on the system in use.

SHELL = /bin/sh

#######################################
# Using g++, the usual case:
CXX := g++-7
CFLAGS := #-no-pie
# Using clang, more hardcore debugging:
#CXX := clang++-6.0
#CFLAGS :=
#######################################
# -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unknown-pragmas
#CFLAGS:=$(CFLAGS) -g -Wall -Wextra -Werror -Wshadow -pedantic -m64
CFLAGS := $(CFLAGS) -g -Wall -Wextra -Wshadow -pedantic -m64
CXXFLAGS := $(CFLAGS) -std=c++11 -march=native -O2

#DEBUGFLAGS:= -Xlinker -v --verbose

#PYTHIA6:=-lEG -lEGPythia6 -L/afs/cern.ch/user/t/tmakela/private/install/pythia6/v6_428/lib -lPythia6
#PYTHIA6:=$(DEBUGFLAGS) -lEG -lEGPythia6 -L/home/user/install/pythia6/v6_428/lib -lPythia6
PYTHIA6:=-lEG -lEGPythia6 -L/home/mikael/Install/v6_428/lib -lPythia6
PYTHIA8:=$(shell pythia8-config --cflags) -L/home/mikael/Install/lib -lpythia8
#HERWIG:=-I$(shell herwig-config --cppflags --libdir --ldlibs --ldflags)
#HERWIG:=-I$(shell herwig-config --includedir)

# Commented this:
#HERWIG:=-I$(shell herwig-config --includedir) -L$(shell herwig-config --libdir) $(shell herwig-config --ldflags --ldlibs)

FASTJET:=$(shell fastjet-config --cxxflags --libs --plugins)
#FASTJET:=$(shell /home/user/install/bin/fastjet-config --cxxflags --libs --plugins)
ROOT:=$(shell root-config --cflags --glibs) -lGenVector
ROOT_INCLUDE:=$(shell root-config --cflags)
ROOT_SAFE:=$(shell root-config --glibs) -lGenVector -I$(shell root-config --incdir)
#ROOT_SAFE:=$(shell root-config --cflags --glibs) -lGenVector
CINTERPRET=$(ROOTSYS)/bin/rootcling
# ROOT 5:
#CINTERPRET=$(shell $(ROOTSYS)/bin/rootcint)
LHAPDF:=$(shell lhapdf-config --cflags --libs)

INCLUDE := -I./include

