import os
import numpy as np

# Dados de entrada
base_filename = "CondInic{}Caso{}ResultN=25.dat"
num_cond_ini = 10  # 1 a 10
num_casos = 8      # 1 a 8
desvioPadraoTodos = []
# Ler médias do arquivo MediasYOpts.dat
with open("MediasYOpts.dat", "r") as f:
    medias = [float(line.strip()) for line in f.readlines()]

# Para cada Caso Y, coletar os últimos valores
for caso in range(1, num_casos + 1):
    ultimos_valores = []
    
    for cond in range(1, num_cond_ini + 1):
        filename = base_filename.format(cond, caso)
        if not os.path.exists(filename):
            continue
        
        with open(filename, "r") as f:
            lines = f.readlines()
            if lines:
                ultimo_valor = float(lines[-1].strip())
                ultimos_valores.append(ultimo_valor)
    
    if not ultimos_valores:
        continue
    
    media_caso = medias[caso - 1]  # Assumindo que a linha Y corresponde ao Caso Y
    desvio_padrao = np.std(ultimos_valores, ddof=0)  # ddof=0 para população
    
    print(f"Caso {caso}:")
    print(f"  Média (YOpt): {media_caso}")
    print(f"  Últimos valores: {ultimos_valores}")
    print(f"  Desvio padrão: {desvio_padrao}\n")
    desvioPadraoTodos.append(desvio_padrao)
    # Salvar os desvios padrão em um arquivo
with open("DesviosPadraoN=25.dat", "w") as f:
    for desvio in desvioPadraoTodos:
        f.write(f"{desvio}\n") 