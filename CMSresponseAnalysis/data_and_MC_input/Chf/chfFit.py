#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit

pt = np.arange(0.0, 10000.0, 0.01);

def func(x, a, b, c):
    return a*x**2 + b*x + c;

#def func(x, a, b, c, d):
#    return a*x**3 + b*x**2 + c*x + d;

#def func(x, a, b, c, d):
#    return a + (b-a)/(1 + (x/c)**d);


with open('chfFit.txt') as f:
    lines = f.readlines()
    x  = [float(line.split()[0]) for line in lines]
    chf = [float(line.split()[1]) for line in lines]

with open('chf.txt') as f:
    lines = f.readlines()
    x2  = [float(line.split()[0]) for line in lines]
    chf2 = [float(line.split()[1]) for line in lines]

params = curve_fit(func, x, chf)
print(params[0][0])
print(params[0][1])
print(params[0][2])

fit = params[0][0]*pt**2 + params[0][1]*pt + params[0][2];



fig, ax = plt.subplots()
ax.scatter(x2, chf2)
ax.plot(pt,fit)

ax.set(xlabel='pT (GeV)', ylabel='chf')
ax.grid()
ax.set_xscale('log')
ax.set_ylim([0.0,1.0])
ax.set_xlim([10,10000])
plt.show()


