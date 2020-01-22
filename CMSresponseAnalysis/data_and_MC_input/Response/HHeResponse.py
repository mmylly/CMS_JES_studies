#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit

x= np.arange(0.0, 1000.0, 0.01);

HHe = 1.123*(1-1.345*x**(0.421-1));



fig, ax = plt.subplots()
ax.plot(x,HHe)

ax.set(xlabel='pT (GeV)', ylabel='HHe response')
ax.grid()
ax.set_xscale('log')
ax.set_ylim([0,3])
ax.set_xlim([2,600])
plt.show()


