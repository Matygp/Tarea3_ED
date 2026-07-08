// src/HashFunctions.cpp
#include "../include/HashFunctions.h"

// DJB2 - Excelente distribución general para strings
size_t hash_djb2(const std::string& str, size_t table_size) {
    size_t hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % table_size;
}

// SDBM - Función secundaria para Double Hashing
size_t hash_sdbm(const std::string& str, size_t table_size) {
    size_t hash = 0;
    for (char c : str) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash % table_size;
}