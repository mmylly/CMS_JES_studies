#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit

pt = np.arange(0.2, 5000.0, 0.01);

reso_our = 0.01336 + 8.548e-5*pt;

# single particle sample dP/|P| pT:n funktiona

pt_pion   = [481.313, 77.0767, 358.362, 208.733, 18.54];
p_pion    = [484.3, 84.6491, 359.724, 343.942, 59.3568];
reso_pion = [0.0536414, 0.0115157, 0.0406424, 0.0312065, 0.0183955]; 


def func(x, a, b, c):
    return a*x**2 + b*x + c;

def func90(x, a, b):
    return a*x**2 + b*x + 0.0190268;


with open('trkReso.txt') as f:
    lines = f.readlines()
    x    = [float(line.split()[0]) for line in lines]
    reso = [float(line.split()[1])/100. for line in lines]

with open('trkReso_fit.txt') as f:
    lines = f.readlines()
    x_fit    = [float(line.split()[0]) for line in lines]
    reso_fit = [float(line.split()[1])/100. for line in lines]

with open('trkReso_90.txt') as f:
    lines = f.readlines()
    x_90    = [float(line.split()[0]) for line in lines]
    reso_90 = [float(line.split()[1])/100. for line in lines]

with open('trkReso_90_fit.txt') as f:
    lines = f.readlines()
    x_90_fit    = [float(line.split()[0]) for line in lines]
    reso_90_fit = [float(line.split()[1])/100. for line in lines]

with open('mu_trkReso.txt') as f:
    lines = f.readlines()
    x_mu   = [float(line.split()[0]) for line in lines]
    reso_mu = [float(line.split()[1])/100. for line in lines]

with open('mu_trkReso_fit.txt') as f:
    lines = f.readlines()
    x_mu_fit    = [float(line.split()[0]) for line in lines]
    reso_mu_fit = [float(line.split()[1])/100. for line in lines]

with open('mu_fullsysreso.txt') as f:
    lines = f.readlines()
    x_mu_fs    = [float(line.split()[0]) for line in lines]
    reso_mu_fs = [float(line.split()[1]) for line in lines]

PFreso = np.sqrt(1.02*1.02/pt + 0.065*0.065);
Ourreso = np.sqrt(1.49712*1.49712/pt + 8.92104e-02*8.92104e-02);

mu_fullsys = 0.002579707*pt**0.4552789;
mu_fullsys2 = 0.06615472 - 0.05763629*np.exp(-0.002061568*pt);

params = curve_fit(func, x_fit, reso_fit)
print("68% params: ")
print(params[0][0])
print(params[0][1])
print(params[0][2], "\n")
fit  = params[0][0]*pt**2 + params[0][1]*pt + params[0][2];
fit2 = (params[0][0]*pt**2 + params[0][1]*pt + params[0][2])*pt;

#params_90 = curve_fit(func, x_90_fit, reso_90_fit)
#print("90% params: ")
#print(params_90[0][0])
#print(params_90[0][1])
#print(params_90[0][2], "\n")
#fit_90 = params_90[0][0]*pt**2 + params_90[0][1]*pt + params_90[0][2];

params_90 = curve_fit(func90, x_90_fit, reso_90_fit)
print("90% params: ")
print(params_90[0][0])
print(params_90[0][1])
print(0.0190268, "\n")
fit_90 = params_90[0][0]*pt**2 + params_90[0][1]*pt + 0.0190268;



params_mu = curve_fit(func, x_mu_fit, reso_mu_fit)
fit_mu = params_mu[0][0]*pt**2 + params_mu[0][1]*pt + params_mu[0][2];

fig, ax = plt.subplots()
ax.scatter(pt_pion, reso_pion, color="r", label="Single pion gun dP/|P| in function of pT")
#ax.plot(pt, PFreso,       label="PF params calo reso")
#ax.plot(pt, Ourreso,      label="Our params calo reso")
ax.scatter(x,    reso)
ax.scatter(x_90, reso_90)
ax.scatter(x_mu, reso_mu)
ax.scatter(x_mu_fs, reso_mu_fs)
ax.plot(pt, mu_fullsys, label="Full system resolution")
ax.plot(pt, mu_fullsys2, label="Full system resolution 2")

ax.plot(pt,      fit,      label="TRK paper charged particle reso")
ax.plot(pt,      fit_90,   label="TRK paper charged particle reso 90%")
#ax.plot(pt,      fit2,     label="TRK paper charged particle  abs reso")
ax.plot(pt,      fit_mu,   label="TRK paper muon reso"    )
ax.plot(pt,      reso_our, label="Our pion sample track reso")

#ax.scatter(p_pion, reso_pion, label="Single pion gun dP/|P| in function of P")


ax.legend()

ax.set(xlabel='pT or E (GeV)', ylabel='Resolution')
ax.grid()
ax.set_xscale('log')
ax.set_yscale('log')

ax.set_xlim([0.2,5000])
ax.set_ylim([0.01,100])
#ax.set_xlim([100,2000])
plt.show()






























