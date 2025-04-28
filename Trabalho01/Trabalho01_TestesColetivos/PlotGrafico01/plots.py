import numpy as np
import matplotlib.pyplot as plt

plt.rcParams.update({'font.size': 16})

for caso in range(1, 9):
    print(caso)
    data = np.loadtxt(f"CondInic1Caso{caso}ResultN=10.dat")
    plt.plot(data, label="N=10")
    data = np.loadtxt(f"CondInic1Caso{caso}ResultN=25.dat")
    plt.plot(data, label="N=25")
    data = np.loadtxt(f"CondInic1Caso{caso}ResultN=50.dat")
    plt.plot(data, label="N=50")
    data = np.loadtxt(f"CondInic1Caso{caso}ResultN=100.dat")
    plt.plot(data, label="N=100")

    plt.legend()
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Número de iterações")
    if(caso%2 == 0):
        plt.ylabel("Rosenbrock")
    else:
        plt.ylabel("Rastrigin")
    plt.suptitle(f"Prioridade para o ótimo individual - Caso {caso}")
    plt.show()