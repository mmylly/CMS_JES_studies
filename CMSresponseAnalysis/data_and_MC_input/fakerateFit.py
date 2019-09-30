#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit

def func(x, a, b):
    return a*x**2 + b

with open('hadron_fakerate_PF.txt') as f:
    lines = f.readlines()
    x  = [float(line.split()[0]) for line in lines]
    fr = [float(line.split()[1]) for line in lines]


params = curve_fit(func, x, fr)

print(params[0][0])
fit = params[0][0]*x**2 + params[0][1];

fig, ax = plt.subplots()
ax.scatter(x, fr)
ax.plot(x, fit)

ax.set(xlabel='pT (GeV)', ylabel='Fakerate')
ax.grid()
ax.set_xscale('log')
#ax.set_ylim([0.0,0.2])
#ax.set_xlim([0.1,300])
plt.show()
