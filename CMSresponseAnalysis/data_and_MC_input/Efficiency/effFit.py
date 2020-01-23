#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit

pt = np.arange(10, 300.0, 0.01);

def func(x, a, b, c, d):
    return a*x**3 + b*x**2 + c*x + d;

def func2(x, a, b, c):
    return a*x**2 + b*x;


with open('effFit.txt') as f:
    lines = f.readlines()
    x    = [float(line.split()[0]) for line in lines]
    eff1 = [float(line.split()[1]) for line in lines]
    eff2 = [float(line.split()[2]) for line in lines]


subtract = [x1 - x2 for (x1, x2) in zip(eff1, eff2)]


print(subtract)

params = curve_fit(func2, x, subtract)

#fit = params[0][0]*pt**3 + params[0][1]*pt**2 + params[0][2]*pt + params[0][3];
fit = params[0][0]*pt**2 + params[0][1]*pt;
fit2 = 0.1245869 - 0.07823616/(1 + (pt/54.57317)**3.524974);
fit3 = 0.1381969 - 0.1606539/2**(pt/45.31742);
fit4 = 0.1252205 + (0.00214186 - 0.1252205)/(1 + (pt/185306.1)**1.664629)**552076.5;


print(params[0][0])
print(params[0][1])
print(params[0][2])

fig, ax = plt.subplots()
#ax.scatter(x, eff1)
#ax.scatter(x, eff2)
ax.scatter(x, subtract)
ax.plot(pt, fit)
ax.plot(pt, fit2)
ax.plot(pt, fit3)
ax.plot(pt, fit4)

#for i in range(len(eff)):
#  eff[i] += 0.00046178*x_eff[i] + 0.02;
#  print(x_eff[i] , eff[i])


ax.set(xlabel='pT (GeV)', ylabel='Efficiency')
ax.grid()
ax.set_xscale('log')
plt.show()
