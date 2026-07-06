#include "../include/openhash.h"

OpenHash::OpenHash(size_t size, HashFunction hf) {
    table_size = size;
    table.resize(size); // Inicializamos el vector con la cantidad de buckets
    num_elements = 0;
    hash_func = hf;
}

void OpenHash::insert(const std::string& key) {
    // 1. Calculamos el índice usando la función hash provista
    size_t index = hash_func(key, table_size);
    
    // 2. Buscamos si la clave ya existe en la lista enlazada de ese bucket
    for (auto& node : table[index]) {
        if (node.key == key) {
            node.count++; // Si existe, H[k] = H[k] + 1
            return;
        }
    }
    
    // 3. Si no existe, agregamos un nuevo nodo al final de la lista
    table[index].push_back(HashNode(key)); // else H[k] = 1
    num_elements++;
}

int OpenHash::get_count(const std::string& key) const {
    size_t index = hash_func(key, table_size);
    for (const auto& node : table[index]) {
        if (node.key == key) {
            return node.count;
        }
    }
    return 0; // Si no se encuentra el usuario
}

size_t OpenHash::get_memory_usage() const {
    // Cálculo base de la instancia de la clase
    size_t mem_total = sizeof(*this);
    
    // Memoria ocupada por el vector principal
    mem_total += table.capacity() * sizeof(std::list<HashNode>);
    
    // Memoria ocupada por los nodos dentro de las listas enlazadas
    for (const auto& bucket : table) {
        // En una std::list, cada nodo guarda la data (HashNode) + 2 punteros (prev y next)
        mem_total += bucket.size() * (sizeof(HashNode) + 2 * sizeof(void*));
    }
    
    return mem_total;
}