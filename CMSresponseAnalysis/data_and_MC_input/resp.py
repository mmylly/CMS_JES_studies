#!/usr/bin/env python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy
from scipy.optimize import curve_fit

x = np.arange(1, 1000.0, 0.01);

c1 = 1.044;
a1 = 1.293;
m1 = 0.543;

c2 = 1.041;
a2 = 1.342;
m2 = 0.532;




pi   = c1*(1-a1*x**(m1-1));
cat1 = c2*(1-a2*x**(m2-1)); 


fig, ax = plt.subplots()


ax.plot(x, pi, label="pi")
ax.plot(x, cat1, label="cat 1")

ax.legend()

ax.set(xlabel='E (GeV)', ylabel='Response')
ax.grid()
ax.set_xscale('log')
plt.show()
