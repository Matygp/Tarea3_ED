// src/main.cpp
#include <iostream>
#include <chrono>
#include <iomanip>
#include "../include/TweetData.h"
#include "../include/openhash.h"
#include "../include/ClosedHashTable.h"
#include "../include/HashFunctions.h"

using namespace std;

int main() {
    string dataset_path = "data/auspol2019.csv"; 
    
    cout << "=========================================================" << endl;
    cout << "   BENCHMARK DE TABLAS HASH - ELECCIONES AUSPOL 2019" << endl;
    cout << "=========================================================\n" << endl;

    cout << "-> Cargando el dataset en memoria..." << endl;
    vector<TweetData> tweets = loadDataset(dataset_path);
    
    if (tweets.empty()) {
        cout << "[ERROR] El dataset esta vacio o no se encontro el archivo." << endl;
        return 1;
    }
    cout << "-> Dataset cargado con exito. Total de tweets: " << tweets.size() << "\n" << endl;

    // ========================================================================
    // 1. OPEN HASHING (Encadenamiento)
    // ========================================================================
    size_t open_table_size = 300007; 
    OpenHash table_open(open_table_size, hash_djb2);

    cout << "[1] Ejecutando insercion en Open HashTable..." << endl;
    
    auto start_open = chrono::high_resolution_clock::now();
    for (const auto& tweet : tweets) {
        table_open.insert(tweet.user_screen_name);
    }
    auto end_open = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration_open = end_open - start_open;
    
    cout << "    - Tiempo de insercion : " << duration_open.count() << " ms." << endl;
    cout << "    - Memoria aproximada  : " << table_open.get_memory_usage() / 1024 << " KB.\n" << endl;

    // ========================================================================
    // 2. CLOSED HASHING (Direccionamiento Abierto)
    // ========================================================================
    size_t closed_table_size = 300000; 
    ClosedHashTable table_closed(closed_table_size, QUADRATIC_PROBING, hash_djb2, hash_sdbm);

    cout << "[2] Ejecutando insercion en Closed HashTable (Quadratic Probing)..." << endl;
    
    auto start_closed = chrono::high_resolution_clock::now();
    for (const auto& tweet : tweets) {
        table_closed.insert(tweet.user_screen_name);
    }
    auto end_closed = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration_closed = end_closed - start_closed;
    
    cout << "    - Tiempo de insercion : " << duration_closed.count() << " ms." << endl;
    cout << "    - Memoria aproximada  : " << table_closed.get_memory_usage() / 1024 << " KB.\n" << endl;

    // ========================================================================
    // 3. PRUEBA DE VALIDACIÓN (Integridad de los datos)
    // ========================================================================

    string test_user = "nparmar1957"; 
    
    cout << "=========================================================" << endl;
    cout << "   VALIDACION DE CONTEO PARA USUARIO: @" << test_user << endl;
    cout << "=========================================================" << endl;
    
    int count_open = table_open.get_count(test_user);
    int count_closed = table_closed.get_count(test_user);

    cout << left << setw(25) << "Estructura" << " | Tweets contados" << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << left << setw(25) << "OpenHash"   << " | " << count_open << endl;
    cout << left << setw(25) << "ClosedHash" << " | " << count_closed << endl;

    if (count_open == count_closed && count_open > 0) {
        cout << "\n[EXITO] Ambas estructuras registran la misma cantidad de tweets." << endl;
    } else {
        cout << "\n[ADVERTENCIA] Hay una discrepancia en los conteos o el usuario no existe." << endl;
    }

    return 0;
}