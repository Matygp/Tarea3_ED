#include "../include/HashFunctions.hpp"

// Algoritmo djb2, excelente para strings generales (nombres)
size_t hash_djb2(const std::string& key, size_t table_size) {
    size_t hash = 5381;
    for (char c : key) {
        //equivale a hash * 33 + c
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % table_size;
}

// Algoritmo FNV-1a, Muy rápido y con excelente dispersión para secuencias de caracteres (como IDs numéricos en texto)
size_t hash_fnv1a(const std::string& key, size_t table_size) {
    size_t hash = 2166136261u; //valor semilla FNV offset basis
    for (char c : key) {
        hash ^= (unsigned char)c;
        hash *= 16777619; //FNV prime
    }
    return hash % table_size;
}

// Función secundaria para Double Hashing
size_t hash_secondary(const std::string& key, size_t table_size) {
    /* 
     Para double hashing, la función secundaria NUNCA debe retornar 0.
     La fórmula clásica es R - (hash % R), donde R es un primo menor que el tamaño de la tabla
    */
    size_t R = 99991; //Se asume un número primo grande arbitrario
    size_t hash = 0;
    
    for (char c : key) {
        hash = (hash * 31 + c) % R;
    }
    
    size_t result = R - hash;
    return result % table_size; 
}