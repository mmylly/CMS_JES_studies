#!/usr/bin/python

import sys

seeds=[840744607,431166825,11489507,859341684,719632152,384411333,90405435,297596781,620424940,829585206,
       350220548,862060943,865146589,11119376,706126850,761335296,286390445,408256820,447625541,368022699,
       281922559,852542479,509348179,175162098,688006297,512118632,676751467,212155085,158795947,68988051,
       258456879,625579469,146828216,582720998,226158642,439232438,366169042,745702146,412672564,177882235]

# Hard coded tune type
# 0: H7-UE-MMHT (default in Herwig 7)           (MMHT2014)
# 1: CUETHS1 from CMS (pre-Herwig7)             (CTEQ6L1)
# 2: CUETH_7_2 from CMS (pre-Herwig7)           (MSTW2008LO)
tune = 1

# Hard coded PDF choice
# 0: CTEQ6L1
# 1: CT10
# 2: MSTW2008LO
# 3: default
pdf = 0

# CMS energy
# 0: 8    TeV
# 1: 13   TeV
# 2: 1.96 TeV
eScale = 2

# Set ppbar=1 to produce ppbar collisions, otherwise pp
ppbar = 1

hepmc=False
ISR=True
FSR=True
MPI=True

#Weighting=False
Weighting=True

# Read run settings from command line parameters
if len(sys.argv) != 5:
    print "Incorrect amount of command line parameters"
    sys.exit()

tot_evts = int(sys.argv[1])
mode = int(sys.argv[2])
procs = int(sys.argv[3])
proc_id = int(sys.argv[4])

if tot_evts%procs!=0:
    print "Number of processors and events should fit"
    sys.exit()

# Set a unique name for the run
name = ""
if mode == 1:
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

name += ".in"
print name

f = open(name,'w')

f.write('################################################################\n')
f.write('# This file is based on the CUETHS1 Herwig++ tune from CMSSW,\n')
f.write('# which is based on the generic Herwig++ tune EE-5C for CTEQ6l1.\n')
f.write('# See CMSSW/Configuration/Generator/python Hpp configurations.\n')
f.write('################################################################\n\n')

f.write('################################\n')
f.write('# Setting up the event generator\n')
f.write('################################\n')
f.write('cd /Herwig/Generators\n')
f.write('set EventGenerator:RandomNumberGenerator:Seed {}\n'.format(seeds[proc_id]))
f.write('set EventGenerator:NumberOfEvents {}\n'.format(int(tot_evts/procs)))
f.write('set EventGenerator:DebugLevel 3\n')
f.write('set EventGenerator:UseStdout 0\n')
f.write('set EventGenerator:PrintEvent 10\n')
f.write('set EventGenerator:MaxErrors 10000\n')
f.write('set EventGenerator:EventHandler:StatLevel Full\n\n')

f.write('# Make some particles stable, according to their nominal lifetimes\n')
f.write('set /Herwig/Decays/DecayHandler:MaxLifeTime 10*mm\n\n') #Set ctau here

f.write('####################\n')
f.write('# Running parameters\n')
f.write('####################\n\n')

f.write('# LHAPDF settings\n')
f.write('cd /Herwig/Partons\n')
if pdf!=3:
    f.write('create ThePEG::LHAPDF customPDF ThePEGLHAPDF.so\n')

if pdf==0:
    f.write('set customPDF:PDFName cteq6l1\n')
elif pdf==1:
    f.write('set customPDF:PDFName CT10\n')
elif pdf==2:
    f.write('set customPDF:PDFName MSTW2008lo90cl\n')

if pdf!=3:
    f.write('set customPDF:RemnantHandler /Herwig/Partons/HadronRemnants\n')
    f.write('set /Herwig/Particles/p+:PDF customPDF\n')
    f.write('set /Herwig/Particles/pbar-:PDF customPDF\n\n')

f.write('# CM energy\n')
f.write('read snippets/PPCollider.in\n')
if ppbar==1:
    f.write('##################################################\n')
    f.write('# Tevatron physics parameters (override defaults) \n')
    f.write('##################################################\n')
    f.write('set /Herwig/Generators/EventGenerator:EventHandler:BeamB /Herwig/Particles/pbar-\n')
if eScale==0:
    f.write('set /Herwig/Generators/EventGenerator:EventHandler:LuminosityFunction:Energy 8000.0*GeV\n\n')
elif eScale==1:
    f.write('set /Herwig/Generators/EventGenerator:EventHandler:LuminosityFunction:Energy 13000.0*GeV\n\n')
elif eScale==2:
    f.write('set /Herwig/Generators/EventGenerator:EventHandler:LuminosityFunction:Energy 1960.0*GeV\n\n')

f.write('# Intrinsic pT tune extrapolated to LHC energy\n')
if eScale==0:
    f.write('set /Herwig/Shower/ShowerHandler:IntrinsicPtGaussian 2.0*GeV\n')
elif eScale==1:
    f.write('set /Herwig/Shower/ShowerHandler:IntrinsicPtGaussian 2.2*GeV\n\n')
#elif eScale==2:
#    f.write('set /Herwig/Shower/ShowerHandler:IntrinsicPtGaussian 2.2*GeV\n\n')

f.write('# ptHat min\n')
if mode==1:
#    f.write('set /Herwig/Cuts/JetKtCut:MinKT 20.0*GeV\n\n')
    f.write('set /Herwig/Cuts/JetKtCut:MinKT 5.0*GeV\n\n')
#The below should be 2*MinKT according to some tutorial, but QCDCuts not found
#    f.write('set /Herwig/Cuts/QCDCuts:MHatMin 10.0*GeV\n\n')
elif mode==2:
    f.write('set /Herwig/Cuts/JetKtCut:MinKT 5.0*GeV\n\n')
    f.write('set /Herwig/Cuts/PhotonKtCut:MaxEta 1\n\n') #NEW

#    f.write('set /Herwig/Cuts/JetKtCut:MinKT 150.0*GeV\n\n')
elif mode==3:
    f.write('set /Herwig/Cuts/JetKtCut:MinKT 5.0*GeV\n\n') #Used to be 20.*GeV
elif mode==4:
#    f.write('set /Herwig/Cuts/JetKtCut:MinKT 30.0*GeV\n\n')
    f.write('set /Herwig/Cuts/JetKtCut:MinKT 5.0*GeV\n\n')

f.write('##############################################\n')
f.write('# Matrix Elements for hadron-hadron collisions\n')
f.write('##############################################\n\n')
f.write('# Event weighting scheme\n')
f.write('mkdir /Herwig/Weights\n')
f.write('cd /Herwig/Weights\n')
if Weighting:
    f.write('create ThePEG::ReweightMinPT reweightMinPT ReweightMinPT.so\n')
    f.write('set reweightMinPT:Power 4.5\n')
    f.write('set reweightMinPT:Scale 5*GeV\n') #(pThat min) used to be 10*GeV
    f.write('set reweightMinPT:OnlyColoured true\n\n')

f.write('# Set matrix element settings\n')
f.write('cd /Herwig/MatrixElements/\n')
if mode==1:
    f.write('insert SubProcess:MatrixElements[0] MEQCD2to2\n')
elif mode==2:
    f.write('insert SubProcess:MatrixElements[0] MEGammaJet\n')
elif mode==3:
    f.write('set MEZJet:ZDecay Muon\n')
    f.write('set MEZJet:GammaZ All\n')
    f.write('set /Herwig/Cuts/Cuts:MHatMin 75.0*GeV\n')
    f.write('insert SubProcess:MatrixElements[0] MEZJet\n')
elif mode==4:
    f.write('set MEHeavyQuark:QuarkType Top\n')
    f.write('set MEHeavyQuark:Process Pair\n')
    f.write('insert SubProcess:MatrixElements[0] MEHeavyQuark\n')
if Weighting:
    f.write('insert SubProcess:Preweights[0] /Herwig/Weights/reweightMinPT\n\n')
#f.write('set /Herwig/UnderlyingEvent/MPIHandler:IdenticalToUE 0\n')

f.write('# Save final particles and hardest subprocess particles\n')
f.write('cd /Herwig/Analysis\n')
f.write('create Herwig::HerwigTree jetAnalysis libGHerwigTree.so\n')
f.write('insert /Herwig/Generators/EventGenerator:AnalysisHandlers 0 jetAnalysis\n\n')


if hepmc:
    f.write('insert /Herwig/Generators/EventGenerator:AnalysisHandlers 1 HepMCFile\n')
    f.write('set /Herwig/Analysis/HepMCFile:PrintEvent 10\n')
    f.write('set /Herwig/Analysis/HepMCFile:Format GenEvent\n')
    f.write('set /Herwig/Analysis/HepMCFile:Units GeV_mm\n')

if not ISR:
    f.write("set /Herwig/Shower/SplittingGenerator:ISR No\n")
if not FSR:
    f.write("set /Herwig/Shower/SplittingGenerator:FSR No\n")
if not MPI:
    f.write("set /Herwig/Shower/ShowerHandler:MPIHandler NULL\n")

f.write('# For now saverun does not work with LHAPDF\n')
f.write('run {} /Herwig/Generators/EventGenerator\n'.format(name[0:-3]))

f.close()
