#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit

pt = np.arange(0.01, 1000.0, 0.01);

mu_trkReso = 4.66042e-07*pt**2+0.00010326*pt+0.0080762; # tracking paper
mu_FSreso = 0.002579707*pt**0.4552789; # mu full system reso?
mu_90reso = 2.006093e-05*pt**2+0.00103068*pt+0.0190268; # 90% parameters
mu_60reso = 2.77564e-06*pt**2+0.000124*pt+0.010373; # 60% parameters


fig, ax = plt.subplots()
ax.plot(pt, mu_trkReso, label="mu trk paper")
ax.plot(pt, mu_FSreso,  label="mu FS paper")
ax.plot(pt, mu_90reso,  label="mu 90% parameters")
ax.plot(pt, mu_60reso,  label="mu 60% parameters")

ax.legend()

ax.set(xlabel='pT_gen (GeV)', ylabel='resolution')
ax.grid()
ax.set_xscale('log')
ax.set_yscale('log')
ax.set_ylim([0.001,100.0])
ax.set_xlim([1,1000])
plt.savefig("mu_resolution.pdf")


