#include <iostream>
#include <chrono>
#include "../include/TweetData.h"
#include "../include/openhash.h"
#include "../include/HashFunctions.h"

using namespace std;

int main() {
    string dataset_path = "data/auspol2019.csv"; // ruta archivo
    
    cout << "- Cargando el dataset en memoria..." << endl;
    vector<TweetData> tweets = loadDataset(dataset_path);
    
    if (tweets.empty()) {
        cout << "El dataset esta vacio o no se encontro el archivo." << endl;
        return 1;
    }
    cout << "Dataset cargado con exito. Total de tweets: " << tweets.size() << endl;

    // Se prepara la tabla hash abierta
    // Se usa un numero primo cercano al doble de los tweets para tener un buen factor de carga
    size_t table_size = 300007; 
    
    // Se le pasa el tamaño y la funcion hash djb2 que programamos
    OpenHash table_screen_name(table_size, hash_djb2);

    cout << "\n2. Iniciando insercion en Open HashTable (Clave: user_screen_name)..." << endl;
    
    // Se inicia el cronómetro
    auto start = chrono::high_resolution_clock::now();
    
    //Se insertan todos los nombres de usuario
    for (const auto& tweet : tweets) {
        table_screen_name.insert(tweet.user_screen_name);
    }
    
    // Detiene el cronómetro
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    
    cout << "Insercion completada en: " << duration.count() << " milisegundos." << endl;
    cout << "Memoria aproximada de la tabla: " << table_screen_name.get_memory_usage() / 1024 << " KB." << endl;

    // Pequeña prueba de validación
    string test_user = "TaniaG"; //Podemos probar con un usuario real del dataset
    cout << "\nPrueba de busqueda: El usuario " << test_user 
         << " publico " << table_screen_name.get_count(test_user) << " tweets." << endl;

    return 0;
}