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

params = curve_fit(func, x_fit, reso_fit)
print(params[0][0])
print(params[0][1])
print(params[0][2])
fit = params[0][0]*pt**2 + params[0][1]*pt + params[0][2];

params_mu = curve_fit(func, x_mu_fit, reso_mu_fit)
print(params_mu[0][0])
print(params_mu[0][1])
print(params_mu[0][2])
fit_mu = params_mu[0][0]*pt**2 + params_mu[0][1]*pt + params_mu[0][2];

fig, ax = plt.subplots()
ax.scatter(x_mu, reso_mu)
ax.scatter(x, reso)
ax.plot(pt, fit)
ax.plot(pt, reso_our)
ax.plot(pt, fit_mu)

ax.set(xlabel='pT (GeV)', ylabel='Resolution')
ax.grid()
ax.set_xscale('log')
ax.set_yscale('log')
ax.set_ylim([0.004,0.2])
ax.set_xlim([0.2,1000])
plt.show()
