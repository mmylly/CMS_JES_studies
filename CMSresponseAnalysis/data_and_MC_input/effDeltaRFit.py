#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit

r = np.arange(0.001, 1.0, 0.0001);

def func(x, a, b, c, d):
    return a*x**3 + b*x**2 + c*x + d;



with open('effDeltaR.txt') as f:
    lines = f.readlines()
    x  = [float(line.split()[0]) for line in lines]
    eff = [float(line.split()[1]) for line in lines]

params = curve_fit(func, x, eff)

#fit = params[0][0]*r**3 + params[0][1]*r**2 + params[0][2]*r + params[0][3];

fig, ax = plt.subplots()
ax.scatter(x, eff)
#ax.plot(r,fit)

ax.set(xlabel='minDeltaR', ylabel='eff')
ax.grid()
ax.set_xscale('log')
#ax.set_ylim([0.0001,1.0])
ax.set_xlim([0.001,1.0])
plt.show()


