import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Configuración estética para un informe formal
sns.set_theme(style="whitegrid")

# Cargar los datos
df = pd.read_csv('resultados_experimento.csv', sep=';')

# Calcular Promedio y Desviación Estándar agrupando por Clave, Estructura y Cantidad
stats = df.groupby(['clave', 'estructura_de_datos', 'cantidad_consultas']).agg(
    tiempo_promedio=('tiempo_ejecucion_ms', 'mean'),
    tiempo_std=('tiempo_ejecucion_ms', 'std'),
    memoria_kb=('memoria_kb', 'mean')
).reset_index()

# 1. Gráfica de Escalabilidad de Tiempo (user_screen_name)
plt.figure(figsize=(10, 6))
sns.lineplot(
    data=df[df['clave'] == 'user_screen_name'], 
    x='cantidad_consultas', y='tiempo_ejecucion_ms', 
    hue='estructura_de_datos', marker='o', errorbar='sd'
)
plt.title('Rendimiento de Inserción por Estructura (Clave: user_screen_name)')
plt.xlabel('Cantidad de Tweets Almacenados')
plt.ylabel('Tiempo de Ejecución (ms)')
plt.legend(title='Estructura de Datos')
plt.savefig('grafica_tiempo_screen_name.png', dpi=300, bbox_inches='tight')

# 2. Gráfica de Escalabilidad de Tiempo (user_id)
plt.figure(figsize=(10, 6))
sns.lineplot(
    data=df[df['clave'] == 'user_id'], 
    x='cantidad_consultas', y='tiempo_ejecucion_ms', 
    hue='estructura_de_datos', marker='s', errorbar='sd'
)
plt.title('Rendimiento de Inserción por Estructura (Clave: user_id)')
plt.xlabel('Cantidad de Tweets Almacenados')
plt.ylabel('Tiempo de Ejecución (ms)')
plt.legend(title='Estructura de Datos')
plt.savefig('grafica_tiempo_user_id.png', dpi=300, bbox_inches='tight')

# Imprimir tabla de Memoria (Para incluir en el documento)
print("=== ESPACIO EN MEMORIA PARA 130.000 TWEETS (KB) ===")
max_tweets = stats['cantidad_consultas'].max()
memoria_stats = stats[stats['cantidad_consultas'] == max_tweets][['clave', 'estructura_de_datos', 'memoria_kb']]
print(memoria_stats.to_string(index=False))