**Proyecto Final - Estructura y Diseño de Algoritmos**

## Descripción

Este proyecto implementa tres paradigmas fundamentales de Diseño de algoritmos sobre un dataset de Telco Customer Churn:

- **Módulo A (Divide y Vencerás):** MergeSort para ordenación + Búsqueda Binaria
- **Módulo B (Greedy):** Algoritmo de Kruskal para Árbol de Expansión Mínima (MST) con Union-Find
- **Módulo C (Programación Dinámica):** Solución del problema 0-1 Knapsack con tabulación y backtracking

## Estructura del Proyecto

```
ADA_PF_CORTES_ROLDAN/
├── README.md                           # Este archivo
├── data/
│   └── WA_Fn-UseC_-Telco-Customer-Churn.csv  # Dataset original (7043 registros)
├── results/
│   ├── solicitudes_ordenadas.csv       # Salida Módulo A (ordenado por tenure)
│   ├── busquedas_A.txt                 # Salida Módulo A (consultas binarias)
│   ├── mst_red.txt                     # Salida Módulo B (19 aristas del MST)
│   └── asignacion_bw.txt               # Salida Módulo C (solución Knapsack óptima)
├── src/
│   ├── main.cpp                        # Programa principal (orquesta los 3 módulos)
│   ├── parser.cpp / parser.hpp         # Carga y parsea CSV
│   ├── mergesort.cpp / mergesort.hpp   # Implementación MergeSort
│   ├── binary_search.cpp / binary_search.hpp  # Búsqueda binaria
│   ├── graph.cpp / graph.hpp           # Estructura de grafo
│   ├── kruskal.cpp / kruskal.hpp       # Algoritmo de Kruskal + Union-Find
│   └── knapsack.cpp / knapsack.hpp     # Mochila 0-1 (DP + backtracking)
└── bin/
    └── main.exe                        # Ejecutable compilado
```

## Requisitos

- **Compilador:** g++ con soporte C++17
- **Sistema Operativo:** Windows / Linux / macOS
- **Librerías:** Estándar de C++ (chrono, iostream, fstream, vector, algorithm)

## Compilación

Desde la carpeta raíz del proyecto:

```bash
g++ -std=c++17 -O2 src/main.cpp src/parser.cpp src/mergesort.cpp src/binary_search.cpp src/graph.cpp src/kruskal.cpp src/knapsack.cpp -o bin/main.exe
```

## Ejecución

```bash
./bin/main.exe
```

La ejecución procesa el dataset CSV y genera 4 archivos de salida en la carpeta `results/`.

## Descripción de Módulos

### Módulo A: Divide y Vencerás (MergeSort + Búsqueda Binaria)

- **Entrada:** Dataset CSV (7043 solicitudes de clientes)
- **Procesamiento:** Ordenación por `tenure` (meses de antigüedad) usando MergeSort O(n log n)
- **Búsqueda:** 5 consultas binarias sobre el array ordenado para encontrar solicitudes con tenure específico
- **Salida:** 
  - `solicitudes_ordenadas.csv` (todas las solicitudes ordenadas)
  - `busquedas_A.txt` (resultados de las 5 consultas binarias)
- **Complejidad:** O(n log n) ordenación, O(log n) por búsqueda

### Módulo B: Algoritmo Greedy (Kruskal + Union-Find)

- **Entrada:** Grafo con 20 nodos (agrupados del dataset) y 190 aristas
- **Procesamiento:** Algoritmo de Kruskal para encontrar Árbol de Expansión Mínima usando Union-Find
- **Salida:** `mst_red.txt` (19 aristas del MST con pesos)
- **Resultado:** 19 aristas, peso total 2393
- **Complejidad:** O(E log E) donde E=190

### Módulo C: Programación Dinámica (0-1 Knapsack)

- **Entrada:** Solicitudes ordenadas del Módulo A (primeras 50 con Churn=false)
- **Mapeo:**
  - Peso: `tenure` (meses)
  - Valor: `MonthlyCharges * 10` (centavos)
  - Capacidad: W = 500
- **Procesamiento:** 
  1. Tabulación: construye tabla DP de tamaño (n+1) × (W+1)
  2. Backtracking: recupera los ítems seleccionados
- **Salida:** `asignacion_bw.txt` (tabla completa + solución óptima)
- **Resultado:** Valor óptimo = 6912 centavos, 6 solicitudes seleccionadas, peso usado = 432
- **Complejidad:** O(nW) tiempo y espacio (pseudopolinomial)

## Dataset

**Fuente:** Kaggle - Telco Customer Churn  
**Registros:** 7043  
**Campos principales:**
- `tenure`: Meses de antigüedad (peso para Knapsack)
- `MonthlyCharges`: Cargo mensual en dólares (valor para Knapsack)
- `TotalCharges`: Cargos acumulados (usado internamente escalado)
- `Churn`: Indicador si el cliente se fue (filtro para Módulo C)
