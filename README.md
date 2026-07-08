## Integrantes del Grupo

* Bastián Pérez Aguayo 
* Matías García Padilla 
* Vicente Miranda Gómez 

# Estructuras de Datos — Entregable 3: Tablas Hash

Este proyecto implementa y compara el rendimiento y la escalabilidad de diferentes arquitecturas de tablas hash (hashing abierto, hashing cerrado con *Quadratic Probing* optimizado y `std::unordered_map` de la STL) utilizando un dataset real de las elecciones australianas del 2019 con más de $130.000$ tweets. El objetivo principal es evaluar la eficiencia temporal y espacial utilizando dos tipos de llaves: `user_id` y `user_screen_name`.

---

## Estructura del Proyecto

```text
Tarea3_ED/
├── data/
│   └── auspol2019.csv          # Dataset de tweets (133.136 filas procesadas)
├── include/
│   ├── ClosedHashTable.h       # Definición de la tabla hash con direccionamiento abierto
│   ├── HashFunctions.h         # Funciones hash primarias y secundarias (DJB2, SDBM)
│   ├── TweetData.h             # Estructura de datos del tweet y cabecera del parser CSV
│   └── openhash.h              # Definición de la tabla hash con encadenamiento
├── src/
│   ├── ClosedHashTable.cpp     # Implementación de la tabla cerrada y probing cuadrático
│   ├── HashFunctions.cpp       # Implementación matemática de hashes modulares
│   ├── TweetData.cpp           # Parser resistente a comas internas y saneamiento de strings
│   ├── main.cpp                # Suite automatizada de Benchmarking (20 iteraciones)
│   └── openhash.cpp            # Implementación de tabla abierta utilizando std::list
├── graficador.py               # Script de Python para análisis estadístico y gráficos
├── resultados_experimento.csv  # Dataset métrico generado por la suite de C++
└── README.md                   # Este archivo
```


---

## Requisitos del Entorno

### Entorno de Compilación (C++)

* Compilador con soporte para **C++17** o superior (`g++` recomendado).
* Sistema Operativo: GNU/Linux.

### Entorno de Análisis (Python)

El script de análisis requiere Python 3 y las siguientes dependencias de procesamiento científico:

```bash
pip install pandas matplotlib seaborn

```

---

## Instrucciones de Uso

Sigue este flujo de trabajo secuencial desde la raíz del proyecto (`Tarea3_ED/`) en tu terminal:

### 1. Compilación de Alto Rendimiento

Para realizar un estudio experimental confiable, compilamos activando el nivel de optimización máximo del compilador (`-O3`) para vectorización de ciclos y optimizaciones de registro, junto con el flag de inclusión `-Iinclude`:

```bash
g++ -O3 -Iinclude src/*.cpp -o main_hash

```

### 2. Ejecución del Banco de Pruebas (Benchmark)

Ejecuta el binario generado. Este proceso correrá de forma aislada la suite experimental automatizando **20 repeticiones completas** por cada tramo de $10.000$ tweets hasta completar el dataset, evaluando de forma cruzada ambas llaves en las 3 estructuras.

```bash
./main_hash

```

*Al finalizar, la consola mostrará la validación de integridad del conteo y habrá generado en la raíz el archivo estructurado `resultados_experimento.csv`.*

### 3. Generación de Gráficas Estadísticas

Ejecuta el script de Python para procesar el CSV, calcular las medias aritméticas y las desviaciones estándares de los tiempos, y exportar los reportes visuales de escalabilidad:

```bash
python graficador.py

```

---

## Outputs Generados

El script de Python generará automáticamente los siguientes archivos en la raíz del proyecto para ser incorporados de forma directa en el informe final en $\LaTeX$:

1. **`grafica_tiempo_screen_name.png`:** Curva de escalabilidad temporal (Tiempo en ms vs. Cantidad de tweets) utilizando la clave `user_screen_name`, detallando la desviación estándar de las 20 pasadas.
2. **`grafica_tiempo_user_id.png`:** Curva de escalabilidad temporal utilizando la clave analítica `user_id`.
3. **Reporte por Consola:** Resumen detallado del uso de memoria estática y dinámica de las tres estructuras en el límite superior de carga ($130.000$ registros), evidenciando el *trade-off* de espacio de la preasignación modular.

---





```

```
