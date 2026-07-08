// src/openhash.cpp
#include "../include/openhash.h"

OpenHash::OpenHash(size_t size, HashFunc hf) {
    table_size = size;
    table.resize(table_size);
    hash_func = hf;
    num_elements = 0;
}

void OpenHash::insert(const std::string& key) {
    size_t index = hash_func(key, table_size);
    
    for (auto& node : table[index]) {
        if (node.key == key) {
            node.count++;
            return;
        }
    }
    table[index].emplace_back(key, 1);
    num_elements++;
}

int OpenHash::get_count(const std::string& key) const {
    size_t index = hash_func(key, table_size);
    
    for (const auto& node : table[index]) {
        if (node.key == key) {
            return node.count;
        }
    }
    return 0;
}

size_t OpenHash::get_memory_usage() const {
    // Memoria base del objeto + almacenamiento continuo del vector de buckets
    size_t memory = sizeof(*this) + (table.capacity() * sizeof(std::list<HashNode>));
    
    for (const auto& bucket : table) {
        for (const auto& node : bucket) {
            // Se calcula el tamaño real del nodo dinámico: payload + punteros next y prev
            memory += sizeof(HashNode) + (2 * sizeof(void*)); 
            
            // Manejo de Small String Optimization (SSO) para strings largos en el Heap
            if (node.key.capacity() * sizeof(char) > sizeof(std::string)) {
                memory += node.key.capacity() * sizeof(char); 
            }
        }
    }
    return memory;
}