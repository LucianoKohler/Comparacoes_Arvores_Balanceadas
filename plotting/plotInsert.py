import pandas as pd
import matplotlib.pyplot as plt

# Lê o CSV (separado por vírgula)
df = pd.read_csv("dataInsert.csv", header=None);

linhas = ["AVL"]
colunas = [1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000]

df.index = linhas
df.columns = colunas

plt.figure(figsize=(10, 6))
# Desenha cada linha com espessura suave
for label in df.index:
    plt.plot(df.columns, df.loc[label], linewidth=1.5, label=label)

# Títulos no estilo do exemplo
plt.title("INSERÇÃO:")
plt.suptitle("Análise dos dados")

# Labels dos eixos
plt.xlabel("Tamanho do conjunto", fontsize=12)
plt.ylabel("Média de iterações", fontsize=12)

# Grade suave
plt.grid(alpha=0.3)

# Legenda à esquerda
plt.legend(title="", loc="upper left", fontsize=12)

# Margens e espaçamento
plt.tight_layout()

plt.show()
