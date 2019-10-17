#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit

pt = np.arange(0.2, 300.0, 0.01);

def func(x, a, b):
    return a*x**2 + b*x + 0.02;

def func2(x, a):
    return a*x**2 + 0.02;


with open('hadron_fakerate_PF.txt') as f:
    lines = f.readlines()
    x  = [float(line.split()[0]) for line in lines]
    fr = [float(line.split()[1]) for line in lines]


params = curve_fit(func, x, fr)
#params = curve_fit(func2, x, fr)
#print(params);
#fit = params[0][0]*pt**2 + params[0][1]*pt + 0.02;
fit2 = 0.00046178*pt + 0.02;


fig, ax = plt.subplots()
ax.scatter(x, fr)
ax.plot(pt, fit2)

ax.set(xlabel='pT (GeV)', ylabel='Fakerate')
ax.grid()
ax.set_xscale('log')
#ax.set_ylim([0.0,0.2])
#ax.set_xlim([0.1,300])
plt.show()
