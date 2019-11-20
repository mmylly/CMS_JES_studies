#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit

pt = np.arange(0.2, 1000.0, 0.01);

reso_our = 0.01336 + 8.548e-5*pt;


def func(x, a, b, c):
    return a*x**2 + b*x + c;

def func2(x, a):
    return a*x**2 + 0.008;

with open('trkReso.txt') as f:
    lines = f.readlines()
    x    = [float(line.split()[0]) for line in lines]
    reso = [float(line.split()[1])/100. for line in lines]

with open('trkReso_fit.txt') as f:
    lines = f.readlines()
    x_fit    = [float(line.split()[0]) for line in lines]
    reso_fit = [float(line.split()[1])/100. for line in lines]

with open('mu_trkReso.txt') as f:
    lines = f.readlines()
    x_mu   = [float(line.split()[0]) for line in lines]
    reso_mu = [float(line.split()[1])/100. for line in lines]

with open('mu_trkReso_fit.txt') as f:
    lines = f.readlines()
    x_mu_fit    = [float(line.split()[0]) for line in lines]
    reso_mu_fit = [float(line.split()[1])/100. for line in lines]

PFreso = np.sqrt(1.02*1.02/pt + 0.065*0.065);

Ourreso = np.sqrt(1.49712*1.49712/pt + 8.92104e-02*8.92104e-02);
#  TF1* jerg_A = new TF1("jerg_A", "sqrt([0]*[0]/(x*x)+[1]*[1]/x+[2]*[2]) * (0.55*1.02900*(1-1.6758*pow(x/0.75,0.553456-1)) + 0.45*1.10286*(1-1.25613*pow(x/0.75,0.397034-1)))", 0, 1000);
#  jerg_A->SetParameters(9.59431e-05, 1.49712, 8.92104e-02);



params = curve_fit(func, x_fit, reso_fit)
print(params[0][0])
print(params[0][1])
print(params[0][2])
fit = (params[0][0]*pt**2 + params[0][1]*pt + params[0][2])*pt;

params_mu = curve_fit(func, x_mu_fit, reso_mu_fit)
print(params_mu[0][0])
print(params_mu[0][1])
print(params_mu[0][2])
fit_mu = params_mu[0][0]*pt**2 + params_mu[0][1]*pt + params_mu[0][2];

fig, ax = plt.subplots()
ax.plot(pt, PFreso,      label="PF params")
ax.plot(pt, Ourreso,      label="Our params")
ax.scatter(x,    reso)
ax.scatter(x_mu, reso_mu)
ax.plot(pt,      fit,      label="charged particle resolution")
ax.plot(pt,      fit_mu,   label="muon resolution"   )
ax.plot(pt,      reso_our, label="our pion sample resolution")

ax.legend()

ax.set(xlabel='pT (GeV)', ylabel='Resolution')
ax.grid()
ax.set_xscale('log')
ax.set_yscale('log')
#ax.set_ylim([0.004,0.2])
ax.set_xlim([0.2,1000])
plt.show()
