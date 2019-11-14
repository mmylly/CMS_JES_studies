#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit

pt = np.arange(0.0, 1000.0, 0.01);

reso = 0.01336 + 10*8.548e-5*pt;



fig, ax = plt.subplots()
#ax.scatter(, chf2)
ax.plot(pt,reso)

ax.set(xlabel='pT_gen (GeV)', ylabel='resolution')
ax.grid()
#ax.set_xscale('log')
ax.set_ylim([0.0,1.0])
ax.set_xlim([10,1000])
plt.show()


