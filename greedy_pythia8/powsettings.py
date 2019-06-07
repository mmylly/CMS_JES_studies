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

if procs!=1:
    print "    Only one processor for POWHEG!"
    sys.exit()

# Set a unique name for the run
name = "powheg_ttbar"

print name
name += ".cmnd"

f = open(name,'w')

f.write("Random:setSeed = on\n")
f.write("Random:seed = {}\n\n".format(seeds[proc_id]) )

f.write("Main:numberOfEvents = 0\n")

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

if weighted:
    f.write("! Event weighting\n")
    # Tranche 1
    #f.write("PhaseSpace:pTHatMin = 80.\n")
    #f.write("PhaseSpace:pTHatMax = 320.\n\n")
    # Tranche 2
    #f.write("PhaseSpace:pTHatMin = 180.\n")
    #f.write("PhaseSpace:pTHatMax = 420.\n\n")
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
#f.write('!HadronLevel:All = off\n')

f.write('! Number of events to list and number of errors to allow.\n')
f.write('Next:numberShowLHA = 1\n')
f.write('Next:numberShowInfo = 1\n')
f.write('Next:numberShowProcess = 1\n')
f.write('Next:numberShowEvent = 1\n')
f.write('Main:timesAllowErrors = 10\n')

f.write('! List changed settings or particle data.\n')
f.write('Init:showChangedSettings = on\n')
f.write('Init:showChangedParticleData = off\n')

f.write('! Input file.\n')
f.write('Beams:frameType = 4\n')
f.write('! t tbar pair production.\n')
f.write('Beams:LHEF = ../1Mevts.lhe\n')

f.write('! Number of outgoing particles of POWHEG Born level process\n')
f.write('! (i.e. not counting additional POWHEG radiation)\n')
f.write('POWHEG:nFinal = 2\n')

f.write('! How vetoing is performed:\n')
f.write('!  0 - No vetoing is performed (userhooks are not loaded)\n')
f.write('!  1 - Showers are started at the kinematical limit.\n')
f.write('!      Emissions are vetoed if pTemt > pThard.\n')
f.write('!      See also POWHEG:vetoCount below\n')
f.write('POWHEG:veto = 1\n')
f.write('SpaceShower:pTmaxMatch = 2\n')
f.write('TimeShower:pTmaxMatch = 2\n')

f.write('! After vetoCount accepted emissions in a row, no more emissions\n')
f.write('! are checked. vetoCount = 0 means that no emissions are checked.\n')
f.write('! Use a very large value, e.g. 10000, to have all emissions checked.\n')
f.write('POWHEG:vetoCount = 3\n')

f.write('! Selection of pThard (note, for events where there is no\n')
f.write('! radiation, pThard is always set to be SCALUP):\n')
f.write('!  0 - pThard = SCALUP (of the LHA/LHEF standard)\n')
f.write('!  1 - the pT of the POWHEG emission is tested against all other\n')
f.write('!      incoming and outgoing partons, with the minimal value chosen\n')
f.write('!  2 - the pT of all final-state partons is tested against all other\n')
f.write('!      incoming and outgoing partons, with the minimal value chosen\n')
f.write('POWHEG:pThard = 2\n')

f.write('! Selection of pTemt:\n')
f.write('!  0 - pTemt is pT of the emitted parton w.r.t. radiating parton\n')
f.write('!  1 - pT of the emission is checked against all incoming and outgoing\n')
f.write('!      partons. pTemt is set to the minimum of these values\n')
f.write('!  2 - the pT of all final-state partons is tested against all other\n')
f.write('!      incoming and outgoing partons, with the minimal value chosen\n')
f.write('! WARNING: the choice here can give significant variations in the final\n')
f.write('! distributions, notably in the tail to large pT values.\n')
f.write('POWHEG:pTemt = 0\n')

f.write('! Selection of emitted parton for FSR\n')
f.write('!  0 - Pythia definition of emitted\n')
f.write('!  1 - Pythia definition of radiator\n')
f.write('!  2 - Random selection of emitted or radiator\n')
f.write('!  3 - Both are emitted and radiator are tried\n')
f.write('POWHEG:emitted = 0\n')

f.write('! pT definitions\n')
f.write('!  0 - POWHEG ISR pT definition is used for both ISR and FSR\n')
f.write('!  1 - POWHEG ISR pT and FSR d_ij definitions\n')
f.write('!  2 - Pythia definitions\n')
f.write('POWHEG:pTdef = 1\n')

f.write('! MPI vetoing\n')
f.write('!  0 - No MPI vetoing is done\n')
f.write('!  1 - When there is no radiation, MPIs with a scale above pT_1 are vetoed,\n')
f.write('!      else MPIs with a scale above (pT_1 + pT_2 + pT_3) / 2 are vetoed\n')
f.write('POWHEG:MPIveto = 0\n')
f.write('! Note that POWHEG:MPIveto = 1 should be combined with\n')
f.write('! MultipartonInteractions:pTmaxMatch = 2\n')

f.write('! QED vetoing\n')
f.write('!  0 - No QED vetoing is done for pTemt > 0.\n')
f.write('!  1 - QED vetoing is done for pTemt > 0.\n')
f.write('!  2 - QED vetoing is done for pTemt > 0.   If a photon is found\n')
f.write('!      with pT>pThard from the Born level process, the event is accepted\n')
f.write('!      and no further veto of this event is allowed (for any pTemt).\n')
f.write('POWHEG:QEDveto = 2\n')

f.close()
