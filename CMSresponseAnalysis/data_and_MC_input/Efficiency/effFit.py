#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit


pt = np.arange(10, 300.0, 0.01);


def func(x, a, b, c):
    return a-b/2**(x/c)



with open('effFit.txt') as f:
    lines = f.readlines()
    x    = [float(line.split()[0]) for line in lines]
    eff1 = [float(line.split()[1]) for line in lines]
    eff2 = [float(line.split()[2]) for line in lines]


subtract = [x1 - x2 for (x1, x2) in zip(eff1, eff2)]

print(x)
print(subtract)

params = curve_fit(func, x, subtract)

fit =  0.1647249 - 0.1761053/2**(pt/49.75313);

fit3 = 0.1381969 - 0.1606539/2**(pt/45.31742);

print(params[0][0])
print(params[0][1])
print(params[0][2])

fig, ax = plt.subplots()
ax.scatter(x, subtract)
ax.plot(pt, fit)
ax.plot(pt, fit3)


ax.set(xlabel='pT (GeV)', ylabel='Efficiency')
ax.grid()
ax.set_xscale('log')
plt.show()
