#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit

pt = np.arange(0.5, 10.0, 0.01);

def func(x, a, b, c):
    return a*x**2 + b*x + c;



with open('effFit_low.txt') as f:
    lines = f.readlines()
    x    = [float(line.split()[0]) for line in lines]
    eff1 = [float(line.split()[1]) for line in lines]
    eff2 = [float(line.split()[2]) for line in lines]


subtract = [x1 - x2 for (x1, x2) in zip(eff1, eff2)]

params = curve_fit(func, x, subtract)

#fit = params[0][0]*pt**2 + params[0][1]*pt + params[0][2];
fit2 = 0.03836143*pt**(-0.7057044);
fit3 = 0.03836143*pt**(-0.7057044) + 0.001;
#fit3 = -0.008122365 + (77872.15 + 0.008122365)/(1 + (pt/2.222327e-12)**0.5337411);
fit4 = -0.009122365 + 77872.15/(1 + (pt/2.222327e-12)**0.5337411);

fig, ax = plt.subplots()
ax.scatter(x, subtract)
ax.plot(pt, fit2, label="fit2")
ax.plot(pt, fit4, label="fit3")
#ax.plot(pt, fit4)

ax.legend()

ax.set(xlabel='pT (GeV)', ylabel='Efficiency')
ax.grid()
#ax.set_xscale('log')
#ax.set_ylim([0.0,0.2])
#ax.set_xlim([0.1,300])
plt.show()
