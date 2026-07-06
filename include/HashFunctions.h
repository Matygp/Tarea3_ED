#ifndef HASH_FUNCTIONS_HPP
#define HASH_FUNCTIONS_HPP

#include <string>
#include <cstddef> //size_t

// Función Hash ideal para user_screen_name (Algoritmo djb2)
size_t hash_djb2(const std::string& key, size_t table_size);

// Función Hash ideal para user_id tratado como texto (Algoritmo FNV-1a)
size_t hash_fnv1a(const std::string& key, size_t table_size);

// Función Hash secundaria (util para Double Hashing)
size_t hash_secondary(const std::string& key, size_t table_size);

#endif