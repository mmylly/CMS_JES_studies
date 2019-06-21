#!/usr/bin/python

import sys
import os

seeds=[840744607,431166825,11489507,859341684,719632152,384411333,90405435,297596781,620424940,829585206,
       350220548,862060943,865146589,11119376,706126850,761335296,286390445,408256820,447625541,368022699,
       281922559,852542479,509348179,175162098,688006297,512118632,676751467,212155085,158795947,68988051,
       258456879,625579469,146828216,582720998,226158642,439232438,366169042,745702146,412672564,177882235]

# Hard coded tune type
# 0: 4C from Herwig++ defaults (CTEQ6L1)
# 1: CUETP8S1 from CMS         (CTEQ6L1)
# 2: CUETP8M1 from CMS         (NNPDF2.3LO)
# 3: CUETP8M2 from CMS         (NNPDF3.0LO)
tune = 3

# Hard coded PDF choice
# 0: CTEQ6L1
# 1: use the default
pdf = 1

# CMS energy
# 0: 8  TeV
# 1: 13 TeV
# 2: 1.96 TeV
eScale = 1

ISR=True
FSR=True
MPI=True

weighted=False

#print "Initializing pythia8:"

# Read run settings from command line parameters
if len(sys.argv) != 5:
    print "    Incorrect amount of command line parameters"
    sys.exit()

tot_evts = int(sys.argv[1])
mode = int(sys.argv[2])
procs = int(sys.argv[3])
proc_id = int(sys.argv[4])

if tot_evts%procs!=0:
    print "    Number of processors and events should fit"
    sys.exit()

# Set a unique name for the run
name = ""
if mode == 0:
    name += "generic"
elif mode == 1:
    name += "dijet"
elif mode == 2:
    name += "gammajet"
elif mode == 3:
    name += "Zjet"
elif mode == 4:
    name += "ttbarjet"

name += "_"
name += str(tot_evts)

name += "_"
name += str(proc_id)

print name
name += ".cmnd"

f = open(name,'w')

f.write("Random:setSeed = on\n")
f.write("Random:seed = {}\n\n".format(seeds[proc_id]) )

f.write("Main:numberOfEvents = {}\n\n".format(tot_evts/procs))

f.write("! Generic settings\n")
f.write("Next:numberShowInfo = 0\n")
f.write("Next:numberShowProcess = 0\n")
f.write("Next:numberShowEvent = 0\n")
f.write("Next:numberCount = 0\n\n")

f.write("! PPbar beams\n")
f.write("Beams:idA = 2212\n")
f.write("Beams:idB = -2212\n\n")

f.write("! Set particles with long enough lifetimes to stable + photon radiation in lepton-pair decays\n")
f.write("ParticleDecays:allowPhotonRadiation = on\n");
f.write("ParticleDecays:limitTau0=on\n")
f.write("ParticleDecays:tau0Max=10.\n\n")

f.write("! Event weighting\n")
# Tranche 1
#f.write("PhaseSpace:pTHatMin = 80.\n")
#f.write("PhaseSpace:pTHatMax = 320.\n\n")
# Tranche 2
#f.write("PhaseSpace:pTHatMin = 180.\n")
#f.write("PhaseSpace:pTHatMax = 420.\n\n")
if weighted:
    f.write("PhaseSpace:bias2Selection = on\n")
    f.write("PhaseSpace:bias2SelectionPow = 4.5\n")
    f.write("PhaseSpace:bias2SelectionRef = 15.\n\n")

f.write("! CM energy\n")
if eScale==0:
    f.write("Beams:eCM = 8000.\n\n")
    #print "    CMS energy 8 TeV"
elif eScale==1:
    f.write("Beams:eCM = 13000.\n\n")
    #print "    CMS energy 13 TeV"
elif eScale==2:
    f.write("Beams:eCM = 1960.\n\n")
    #print "    CMS energy 1.96 TeV"

if mode==0:
    f.write("HardQCD:all = on\n")
    f.write("PhaseSpace:pTHatMin = 20.\n\n")
if mode==1:
    f.write("HardQCD:all = on\n")
#    f.write("PhaseSpace:pTHatMin = 20.\n\n")
    f.write("PhaseSpace:pTHatMin = 200.\n\n")
if mode==2:
    f.write("PromptPhoton:qg2qgamma = on\n");
    f.write("PromptPhoton:qqbar2ggamma = on\n");
    f.write("PromptPhoton:gg2ggamma = on\n");
    f.write("PhaseSpace:pTHatMin = 20.\n\n");
#    f.write("PhaseSpace:pTHatMin = 150.\n\n")
#    f.write("PhaseSpace:pTHatMin = 200.\n\n");
if mode==3:
    f.write("! Produce only Z0's\n");
    f.write("WeakZ0:gmZmode = 2\n");
    f.write("WeakBosonAndParton:qqbar2gmZg = on\n");
    f.write("WeakBosonAndParton:qg2gmZq  = on\n");
    f.write("23:onMode = off\n");
    f.write("23:7:onMode = on\n");
    f.write("PhaseSpace:pTHatMin = 20.\n\n");
    f.write("PhaseSpace:mHatMin = 75.\n");
    f.write("PhaseSpace:pTHatMin = 100.\n\n")
    f.write("PhaseSpace:mHatMin = 75.\n");
if mode==4:
    f.write("Top::gg2ttbar = on\n");
    f.write("Top::qqbar2ttbar = on\n");
#    f.write("PhaseSpace:pTHatMin = 300.\n\n")

if mode>0:
    f.write("Tune:preferLHAPDF = 2\n")
    if tune==0:
        #print "    4C tune"
        f.write("! Tune (4C)\n")
        f.write("Tune:ee = 3\n")
        f.write("Tune:pp = 5\n")
    elif tune==1:
        #print "    CUETP8S1 tune (CTEQ6L1)"
        f.write("! CMS UE Tune CUETP8S1-CTEQ6L1\n")
        #f.write("Tune:pp = 15\n\n")
        f.write('Tune:ee 3\n')
        f.write('Tune:pp 5\n')
        f.write('MultipartonInteractions:pT0Ref=2.1006\n')
        f.write('MultipartonInteractions:ecmPow=0.21057\n')
        f.write('MultipartonInteractions:expPow=1.6089\n')
        f.write('MultipartonInteractions:a1=0.00\n')
        f.write('ColourReconnection:range=3.31257\n\n')
    elif tune==2:
        #print "    CUETP8M1 tune (Monash*)"
        f.write('! CMS UE Tune CUETP8M1-based on pythia8 monash-star tune\n')
        f.write('Tune:pp 14\n')
        f.write('Tune:ee 7\n')
        f.write('MultipartonInteractions:pT0Ref=2.4024\n')
        f.write('MultipartonInteractions:ecmPow=0.25208\n')
        f.write('MultipartonInteractions:expPow=1.6\n\n')
    elif tune==3:
        #print "    CUETP8M2 tune (Monash*)"
        f.write('! CMS UE Tune CUETP8M1-based on pythia8 monash-star tune\n')
        f.write('Tune:pp 14\n')
        f.write('Tune:ee 7\n')
        f.write('SpaceShower:alphaSvalue=0.1108\n')
        f.write('PDF:pSet=LHAPDF6:NNPDF30_lo_as_0130\n')
        f.write('ColourReconnection:range=5.706919e+00\n')
        f.write('MultipartonInteractions:pT0Ref=2.03434\n')
        f.write('MultipartonInteractions:ecmPow=0.25208\n')
        f.write('MultipartonInteractions:expPow=1.93260\n\n')

    if pdf==0:
        f.write("PDF:pSet = LHAPDF6:cteq6l1\n\n")

if not MPI:
    f.write("PartonLevel:MPI = off\n")
    #print "    WARNING: MPI has been turned off!"
if not ISR:
    f.write("PartonLevel:ISR = off\n")
    #print "    WARNING: ISR has been turned off!"
if not FSR:
    f.write("PartonLevel:FSR = off\n")
    #print "    WARNING: FSR has been turned off!"

f.close()
