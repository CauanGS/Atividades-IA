import numpy as np
import matplotlib.pyplot as plt

plt.rcParams.update({'font.size': 16})

data = np.loadtxt("resultados_N=5.dat")
plt.plot(data,label="N=5")
data = np.loadtxt("resultados_N=10.dat")
plt.plot(data,label="N=10")
data = np.loadtxt("resultados_N=25.dat")
plt.plot(data,label="N=25")
data = np.loadtxt("resultados_N=50.dat")
plt.plot(data,label="N=50")

plt.legend()
plt.xscale("log")
plt.yscale("log")
plt.xlabel("Número de iterações")
plt.ylabel("Função objetivo")
plt.suptitle("Prioridade para o ótimo individual")
plt.show()

data = np.loadtxt("resultadosb_N=5.dat")
plt.plot(data,label="N=5")
data = np.loadtxt("resultadosb_N=10.dat")
plt.plot(data,label="N=10")
data = np.loadtxt("resultadosb_N=25.dat")
plt.plot(data,label="N=25")
data = np.loadtxt("resultadosb_N=50.dat")
plt.plot(data,label="N=50")

plt.legend()
plt.xscale("log")
plt.yscale("log")
plt.xlabel("Número de iterações")
plt.ylabel("Função objetivo")
plt.suptitle("Prioridade para o ótimo do enxame")
plt.show()