import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
x = np.arange(-5,5,0.01)
y = np.arange(-5,5,0.01)

PI = 3.141592684
z = np.zeros((len(x),len(y)))
for i in range(len(x)):
    for j in range(len(y)):
        z[i][j] = 2+x[i]*x[i]+y[j]*y[j]-np.cos(2*PI*x[i])-np.cos(2*PI*y[j])

sns.heatmap(z,xticklabels=False,yticklabels=False,cmap="gnuplot")
plt.xlabel("x")
plt.ylabel("y")
plt.show()