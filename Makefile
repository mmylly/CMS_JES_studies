###################################################################
## Makefile for building ROOT-fastjet analysis of simulated data ##
## Hannu Siikonen, 10.11.2015                                    ##
###################################################################

include config.mk

.SECONDEXPANSION:
.DELETE_ON_ERROR:
.PHONY: all clean run_herwig pythia6 run_pythia6 pythia8 run_pythia8 jetanalysis ptcut
VPATH= events:jetanalysis:lib:pythia6:pythia8:herwig:ptcut:greedy_pythia8:greedy_pythia6:simple_pythia8

INCLUDE := $(INCLUDE) -I.

# Parameter vals:
NUM_EVT := 600000
MODE := 3
NUM_PROC := 3
START_IDX := 0

all: gpythia8 spythia8 pythia8 pythia6 libHerwigTree.so jetanalysis ptcut

# Generic

## Particle

libPrtclEvent.so: PrtclEvent.o PrtclEventDict.o
	cd events && $(CXX) -fPIC -shared $(CXXFLAGS) PrtclEvent.o \
	PrtclEventDict.o -o ../lib/$@

PrtclEvent.o: PrtclEvent.cpp PrtclEvent.h
	cd events && $(CXX) PrtclEvent.cpp -fPIC -pthread $(INCLUDE) \
	$(CXXFLAGS) $(ROOT) -c -w -ldl -o $@

PrtclEventDict.o: PrtclDct.cc
	cd events && $(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOT_INCLUDE) -fPIC -pthread \
	-c ../events/PrtclDct.cc -o $@

PrtclDct.cc: PrtclEvent.h PrtclLinkDef.h
	cd events && $(CINTERPRET) -f $@ -c -p $(INCLUDE) $(CFLAGS) \
	PrtclEvent.h PrtclLinkDef.h && mv PrtclDct_rdict.pcm ../lib/.

## Jet

libJetEvent.so: JetEvent.o JetEventDict.o
	cd events && $(CXX) -fPIC -shared $(CXXFLAGS) JetEvent.o JetEventDict.o \
	-o ../lib/$@

JetEvent.o: JetEvent.cpp JetEvent.h
	cd events && $(CXX) -fPIC -pthread JetEvent.cpp $(INCLUDE) $(CXXFLAGS) \
	$(ROOT) -c -w -ldl -o $@

JetEventDict.o: JetDct.cc
	cd events && $(CXX) $(INCLUDE) $(ROOT_INCLUDE) $(CXXFLAGS) -fPIC -pthread \
	-c ../events/JetDct.cc -o $@

JetDct.cc: JetEvent.h JetLinkDef.h
	cd events && $(ROOTSYS)/bin/rootcint -f $@ -c -p $(INCLUDE) $(CFLAGS) \
	JetEvent.h JetLinkDef.h && mv JetDct_rdict.pcm ../lib/.

# Jet analysis

analysis_tmass: jetanalysis/main_tmass.cpp jetbase.o tmass.o libPrtclEvent.so libJetEvent.so
	cd jetanalysis && $(CXX) main_tmass.cpp jetbase.o tmass.o -L../lib -lPrtclEvent -lJetEvent -lQCDAwarePlugin -w \
	$(INCLUDE) $(CXXFLAGS) $(ROOT) $(FASTJET) -I. -ldl -o ../analysis.exe

analysis_vanilla: jetanalysis/main_vanilla.cpp jetbase.o libPrtclEvent.so libJetEvent.so
	cd jetanalysis && $(CXX) main_vanilla.cpp jetbase.o -L../lib -lPrtclEvent -lJetEvent -lQCDAwarePlugin -w \
	$(INCLUDE) $(CXXFLAGS) $(ROOT) $(FASTJET) -I. -ldl -o ../analysis.exe

analysis_standard: jetanalysis/main_standard.cpp jetbase.o analysis.o libPrtclEvent.so libJetEvent.so
	cd jetanalysis && $(CXX) main_standard.cpp jetbase.o analysis.o -L../lib -lPrtclEvent -lJetEvent -lQCDAwarePlugin -w \
	$(INCLUDE) $(CXXFLAGS) $(ROOT) $(FASTJET) -I. -ldl -o ../analysis.exe

analysis_plot: jetanalysis/main_standard.cpp jetbase.o jetplot.o libPrtclEvent.so libJetEvent.so
	cd jetanalysis && $(CXX) main_plot.cpp jetbase.o jetplot.o -L../lib -lPrtclEvent -lJetEvent -lQCDAwarePlugin -w \
	$(INCLUDE) $(CXXFLAGS) $(ROOT) $(FASTJET) -I. -ldl -o ../analysis.exe

analysis_hard: jetanalysis/main_hard.cpp jetbase.o hardproc.o libPrtclEvent.so libJetEvent.so
	cd jetanalysis && $(CXX) main_hard.cpp jetbase.o hardproc.o -L../lib -lPrtclEvent -lJetEvent -lQCDAwarePlugin -w \
	$(INCLUDE) $(CXXFLAGS) $(ROOT) $(FASTJET) -I. -ldl -o ../analysis.exe

analysis_graphs: jetanalysis/main_histos.cpp jetbase.o histos.o libPrtclEvent.so libJetEvent.so
	cd jetanalysis && $(CXX) main_histos.cpp jetbase.o histos.o -L../lib -lPrtclEvent -lJetEvent -lQCDAwarePlugin -w \
	$(INCLUDE) $(CXXFLAGS) $(ROOT) $(FASTJET) -I. -ldl -o ../analysis.exe

analysis_test: jetanalysis/main_test.cpp jetbase.o test.o libPrtclEvent.so libJetEvent.so
	cd jetanalysis && $(CXX) main_test.cpp jetbase.o test.o -L../lib -lPrtclEvent -lJetEvent -lQCDAwarePlugin -w \
	$(INCLUDE) $(CXXFLAGS) $(ROOT) $(FASTJET) -I. -ldl -o ../analysis.exe




tmass.o: TMass.cpp jetbase.o
	cd jetanalysis && $(CXX) -c TMass.cpp jetbase.o -L../lib -lPrtclEvent -lJetEvent -lQCDAwarePlugin \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(FASTJET) -ldl -o $@

analysis.o: Analysis.cpp jetbase.o
	cd jetanalysis && $(CXX) -c Analysis.cpp jetbase.o -L../lib -lPrtclEvent -lJetEvent -lQCDAwarePlugin \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(FASTJET) -ldl -o $@

jetplot.o: JetPlot.cpp jetbase.o
	cd jetanalysis && $(CXX) -c JetPlot.cpp jetbase.o -L../lib -lPrtclEvent -lJetEvent -lQCDAwarePlugin \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(FASTJET) -ldl -o $@

hardproc.o: HardProc.cpp jetbase.o
	cd jetanalysis && $(CXX) -c HardProc.cpp jetbase.o -L../lib -lPrtclEvent-lJetEvent -lQCDAwarePlugin \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(FASTJET) -ldl -o $@

histos.o: Histograms.cpp jetbase.o
	cd jetanalysis && $(CXX) -c Histograms.cpp jetbase.o -L../lib -lPrtclEvent-lJetEvent -lQCDAwarePlugin \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(FASTJET) -ldl -o $@

test.o: Test.cpp jetbase.o
	cd jetanalysis && $(CXX) -c Test.cpp jetbase.o -L../lib -lPrtclEvent-lJetEvent -lQCDAwarePlugin \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(FASTJET) -ldl -o $@

jetbase.o: JetBase.cpp libJetEvent.so libPrtclEvent.so
	cd jetanalysis && $(CXX) -c JetBase.cpp -L../lib -lPrtclEvent -lJetEvent -lQCDAwarePlugin \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(FASTJET) -ldl -o $@



# Detector simulation

ptcut: ptcut/main.cpp ptcut.o libPrtclEvent.so
	cd ptcut && $(CXX) main.cpp ptcut.o -L../lib -lPrtclEvent -w \
	$(INCLUDE) $(CXXFLAGS) $(ROOT) $(FASTJET) -I. -ldl -o ../$@.exe

ptcut.o: PTCut.cpp libPrtclEvent.so
	cd ptcut && $(CXX) -c PTCut.cpp -L../lib -lPrtclEvent $(INCLUDE) \
	$(CXXFLAGS) -w $(ROOT) -ldl -o $@

# Pythia8

pythia8: pythia8/main.cpp pythia8.o libPrtclEvent.so
	cd pythia8 && $(CXX) main.cpp pythia8.o -w -L../lib -lPrtclEvent $(INCLUDE) \
	$(CXXFLAGS) $(ROOT) $(PYTHIA8) -ldl -o pythia8.exe

pythia8.o: Pythia8Tree.cpp libPrtclEvent.so
	cd pythia8 && $(CXX) -c Pythia8Tree.cpp -L../lib -lPrtclEvent \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(PYTHIA8) -ldl -o $@

run_pythia8:
	bash bash/pythia8.sh $(NUM_EVT) $(MODE) $(NUM_PROC) $(START_IDX)

# Pythia8 with jets

gpythia8: greedy_pythia8/main.cpp gpythia8.o
	cd greedy_pythia8 && $(CXX) main.cpp gpythia8.o -w -L../lib $(INCLUDE) \
	$(CXXFLAGS) $(ROOT) $(PYTHIA8) $(FASTJET) -ldl -o gpythia8.exe

gpythia8.o: Pythia8Jets.cpp greedy_settings.h
	cd greedy_pythia8 && $(CXX) -c Pythia8Jets.cpp -L../lib \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(PYTHIA8) -ldl -o $@

run_gpythia8:
	bash bash/gpythia8.sh $(NUM_EVT) $(MODE) $(NUM_PROC) $(START_IDX)

spythia8: simple_pythia8/main.cpp spythia8.o
	cd simple_pythia8 && $(CXX) main.cpp spythia8.o -w -L../lib $(INCLUDE) \
	$(CXXFLAGS) $(ROOT) $(PYTHIA8) $(FASTJET) -ldl -o spythia8.exe

spythia8.o: Pythia8Jets.cpp
	cd simple_pythia8 && $(CXX) -c Pythia8Jets.cpp -L../lib \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(PYTHIA8) -ldl -o $@

run_spythia8:
	bash bash/spythia8.sh $(NUM_EVT) $(MODE) $(NUM_PROC) $(START_IDX)

# Pythia6

pythia6: pythia6/main.cpp pythia6.o libPrtclEvent.so
	cd pythia6 && $(CXX) main.cpp pythia6.o -w -L../lib -lPrtclEvent $(INCLUDE) \
	$(CXXFLAGS) $(ROOT) $(LHAPDF) $(PYTHIA6) -ldl -o pythia6.exe

pythia6.o: Pythia6Tree.cpp libPrtclEvent.so
	cd pythia6 && $(CXX) -c Pythia6Tree.cpp -L../lib -lPrtclEvent \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(PYTHIA6) -ldl -o $@

run_pythia6:
	bash bash/pythia6.sh $(NUM_EVT) $(MODE) $(NUM_PROC)

# Pythia6 with jets

gpythia6: greedy_pythia6/main.cpp gpythia6.o
	cd greedy_pythia6 && $(CXX) main.cpp gpythia6.o -w -L../lib $(INCLUDE) \
	$(CXXFLAGS) $(ROOT) $(LHAPDF) $(PYTHIA6) $(FASTJET) -ldl -o gpythia6.exe

gpythia6.o: Pythia6Jets.cpp Pythia6Jets.h
	cd greedy_pythia6 && $(CXX) -c Pythia6Jets.cpp -L../lib \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(PYTHIA6) $(FASTJET) -ldl -o $@

run_gpythia6:
	bash bash/gpythia6.sh $(NUM_EVT) $(MODE) $(NUM_PROC)


# Herwig

libHerwigTree.so : HerwigTree.cpp HerwigTree.h libPrtclEvent.so
	cd herwig && $(CXX) -shared -fPIC HerwigTree.cpp \
	-L../lib -lPrtclEvent -isystem $(shell herwig-config --includedir) $(INCLUDE) $(ROOT_SAFE) $(CXXFLAGS) $(LDFLAGS) -o $@

run_herwig:
	bash bash/herwig.sh $(NUM_EVT) $(MODE) $(NUM_PROC)

libGHerwigTree.so : greedy_herwig7/HerwigTree.cpp greedy_herwig7/HerwigTree.h greedy_settings.h libPrtclEvent.so
	cd greedy_herwig7 && $(CXX) -shared -fPIC HerwigTree.cpp \
	-isystem $(shell herwig-config --includedir) $(FASTJET) $(INCLUDE) $(ROOT_SAFE) $(CXXFLAGS) $(LDFLAGS) -o $@

run_gherwig7:
	bash bash/gherwig.sh $(NUM_EVT) $(MODE) $(NUM_PROC)

doc_gherwig7:
	cd greedy_herwig7/doc && doxygen -u Doxyfile && doxygen Doxyfile

test : Test.C libPrtclEvent.so
	$(CXX) Test.C -Llib -lPrtclEvent \
	$(INCLUDE) $(CXXFLAGS) -w $(ROOT) $(FASTJET) $(LDFLAGS) -ldl -o $@

# Clean.
clean:
	rm -f *.log *.exe *.d *.so events/*.o jetsorting/*.o events/*Dct.* lib/* \
	herwig/*log herwig/*run herwig/*out herwig/*tex
