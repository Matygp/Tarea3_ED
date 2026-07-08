#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include "../include/TweetData.h"
#include "../include/openhash.h"
#include "../include/ClosedHashTable.h"
#include "../include/HashFunctions.h"

using namespace std;
using namespace std::chrono;

// Función utilitaria para estimar la memoria dinámica de std::unordered_map
size_t get_unordered_map_memory(const unordered_map<string, int>& umap) {
    size_t mem = sizeof(umap) + (umap.bucket_count() * sizeof(void*));
    for (const auto& pair : umap) {
        mem += sizeof(void*) * 2 + sizeof(pair);
        if (pair.first.capacity() * sizeof(char) > sizeof(string)) {
            mem += pair.first.capacity() * sizeof(char);
        }
    }
    return mem;
}

int main() {
    string dataset_path = "data/auspol2019.csv";
    
    cout << "=========================================================" << endl;
    cout << "   BENCHMARK & VALIDACION DE ESTRUCTURAS DE DATOS" << endl;
    cout << "=========================================================\n" << endl;

    cout << "-> Cargando el dataset en memoria..." << endl;
    vector<TweetData> tweets = loadDataset(dataset_path);
    
    if (tweets.empty()) {
        cerr << "[ERROR] Dataset vacio o archivo no encontrado." << endl;
        return 1;
    }
    cout << "-> Dataset cargado con exito. Total de tweets: " << tweets.size() << "\n" << endl;

    // Configuración del experimento automatizado
    const int NUM_EXPERIMENTOS = 20;
    const size_t STEP = 10000;
    size_t total_tweets = tweets.size();
    
    size_t table_size_open = 300007; 
    size_t table_size_closed = 300000; 

    // Guardar los resultados en el archivo CSV de forma silenciosa
    ofstream csv("resultados_experimento.csv");
    csv << "numero_experimento;dataset;clave;estructura_de_datos;cantidad_consultas;tiempo_ejecucion_ms;memoria_kb\n";

    cout << "[1] Iniciando bateria de experimentos (20 iteraciones)..." << endl;

    for (int exp = 1; exp <= NUM_EXPERIMENTOS; ++exp) {
        cout << "\r    -> Procesando Experimento " << exp << " / " << NUM_EXPERIMENTOS << "..." << flush;

        for (size_t n = STEP; n <= total_tweets; n += STEP) {
            for (int key_type = 0; key_type < 2; ++key_type) {
                string tipo_clave = (key_type == 0) ? "user_screen_name" : "user_id";

                // Abierto
                {
                    OpenHash table(table_size_open, hash_djb2);
                    auto start = high_resolution_clock::now();
                    for (size_t i = 0; i < n; ++i) {
                        table.insert((key_type == 0) ? tweets[i].user_screen_name : tweets[i].user_id);
                    }
                    auto end = high_resolution_clock::now();
                    csv << exp << ";auspol2019;" << tipo_clave << ";OpenHash;" << n << ";" 
                        << duration<double, milli>(end - start).count() << ";" << table.get_memory_usage() / 1024 << "\n";
                }

                // Cerrado
                {
                    ClosedHashTable table(table_size_closed, QUADRATIC_PROBING, hash_djb2, hash_sdbm);
                    auto start = high_resolution_clock::now();
                    for (size_t i = 0; i < n; ++i) {
                        table.insert((key_type == 0) ? tweets[i].user_screen_name : tweets[i].user_id);
                    }
                    auto end = high_resolution_clock::now();
                    csv << exp << ";auspol2019;" << tipo_clave << ";ClosedHash_Quadratic;" << n << ";" 
                        << duration<double, milli>(end - start).count() << ";" << table.get_memory_usage() / 1024 << "\n";
                }

                // STL Unordered Map
                {
                    unordered_map<string, int> stl_map;
                    auto start = high_resolution_clock::now();
                    for (size_t i = 0; i < n; ++i) {
                        stl_map[(key_type == 0) ? tweets[i].user_screen_name : tweets[i].user_id]++;
                    }
                    auto end = high_resolution_clock::now();
                    csv << exp << ";auspol2019;" << tipo_clave << ";unordered_map;" << n << ";" 
                        << duration<double, milli>(end - start).count() << ";" << get_unordered_map_memory(stl_map) / 1024 << "\n";
                }
            }
        }
    }
    csv.close();
    cout << "\n    [OK] Archivo 'resultados_experimento.csv' generado correctamente." << endl;

    // ========================================================================
    // SECCIÓN DE VALIDACIÓN (Tu código anterior incorporado)
    // ========================================================================
    cout << "\n[2] Generando instancias finales para validacion de integridad..." << endl;
    
    OpenHash v_open(table_size_open, hash_djb2);
    ClosedHashTable v_closed(table_size_closed, QUADRATIC_PROBING, hash_djb2, hash_sdbm);
    unordered_map<string, int> v_stl;

    // Inserción completa para la muestra final
    for (const auto& tweet : tweets) {
        v_open.insert(tweet.user_screen_name);
        v_closed.insert(tweet.user_screen_name);
        v_stl[tweet.user_screen_name]++;
    }

    string test_user = "nparmar1957"; 
    cout << "\n=========================================================" << endl;
    cout << "   VALIDACION DE CONTEO PARA USUARIO: @" << test_user << endl;
    cout << "=========================================================" << endl;
    
    int count_open = v_open.get_count(test_user);
    int count_closed = v_closed.get_count(test_user);
    int count_stl = v_stl[test_user];

    cout << left << setw(25) << "Estructura" << " | Tweets contados" << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << left << setw(25) << "OpenHash (Manual)"   << " | " << count_open << endl;
    cout << left << setw(25) << "ClosedHash (Manual)" << " | " << count_closed << endl;
    cout << left << setw(25) << "std::unordered_map"  << " | " << count_stl << endl;

    if (count_open == count_closed && count_closed == count_stl && count_open > 0) {
        cout << "\n[EXITO] Las tres estructuras son consistentes y registran el mismo conteo." << endl;
    } else {
        cout << "\n[ADVERTENCIA] Discrepancia en los conteos. Revisar lógica de colisiones." << endl;
    }

    return 0;
}