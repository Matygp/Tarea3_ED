#include "../include/ClosedHashTable.hpp"

// Calcula la siguiente potencia de 2 (optimización para usar máscara de bits en lugar de módulo)
size_t ClosedHashTable::next_power_of_2(size_t n) const {
    size_t p = 1;
    while (p < n) p <<= 1;
    return p;
}

ClosedHashTable::ClosedHashTable(size_t size, CollisionStrategy strat, HashFunc h1, HashFunc h2) {
    table_size = next_power_of_2(size); // Forzamos base 2
    table.resize(table_size); 
    strategy = strat;
    hash_primary = h1;
    hash_secondary = h2;
    num_elements = 0;
}

// ========================================================================
// CONTROL DEL FACTOR DE CARGA Y REHASHING
// ========================================================================
void ClosedHashTable::rehash() {
    size_t old_size = table_size;
    table_size <<= 1; // Duplicamos manteniendo la potencia de 2
    
    std::vector<HashEntry> old_table = std::move(table);
    table.assign(table_size, HashEntry()); // Nueva tabla con status = EMPTY
    num_elements = 0; // Se recalculará en el insert_impl
    
    // Reinsertamos los elementos que ya estaban ocupados
    for (const auto& entry : old_table) {
        if (entry.status == OCCUPIED) {
            // Llamada polimórfica controlada
            switch (strategy) {
                case LINEAR_PROBING: insert_impl<LINEAR_PROBING>(entry.key, entry.count); break;
                case QUADRATIC_PROBING: insert_impl<QUADRATIC_PROBING>(entry.key, entry.count); break;
                case DOUBLE_HASHING: insert_impl<DOUBLE_HASHING>(entry.key, entry.count); break;
            }
        }
    }
}

// ========================================================================
// INSERCIÓN CON CERO-BRANCHING EN CICLO CRÍTICO (PLANTILLAS)
// ========================================================================
template <CollisionStrategy S>
void ClosedHashTable::insert_impl(const std::string& key, int count) {
    size_t index = hash_primary(key, table_size);
    size_t step = 1;
    
    // Si es Double Hashing, precalculamos el salto.
    if constexpr (S == DOUBLE_HASHING) {
        if (hash_secondary) {
            step = hash_secondary(key, table_size);
            // Si el tamaño de la tabla es 2^k, obligar al salto a ser impar 
            // garantiza coprimalidad absoluta. Previene ciclos infinitos.
            step |= 1; 
        }
    }

    size_t i = 0;
    while (i < table_size) {
        size_t probe_index = 0;
        
        // Uso de 'if constexpr' hace que el compilador descarte los caminos muertos.
        // La máscara '& (table_size - 1)' es el equivalente bitwise ultrarrápido del módulo '%'.
        if constexpr (S == LINEAR_PROBING) {
            probe_index = (index + i) & (table_size - 1);
        } else if constexpr (S == QUADRATIC_PROBING) {
            // Secuencia triangular (i^2 + i) / 2: Garantiza cubrir M slots si M es potencia de 2.
            probe_index = (index + (i * i + i) / 2) & (table_size - 1);
        } else if constexpr (S == DOUBLE_HASHING) {
            probe_index = (index + i * step) & (table_size - 1);
        }

        if (table[probe_index].status == EMPTY) {
            table[probe_index].key = key;
            table[probe_index].count = count;
            table[probe_index].status = OCCUPIED;
            num_elements++;
            return;
        } 
        else if (table[probe_index].key == key) {
            table[probe_index].count += count; // Acepta el conteo en casos de rehashing
            return;
        }
        i++;
    }
}

// Wrapper público
void ClosedHashTable::insert(const std::string& key) {
    // Controlamos el factor de carga (alpha >= 0.7)
    if (num_elements >= table_size * 0.7) {
        rehash();
    }

    // Resolvemos la estrategia una ÚNICA vez por inserción
    switch (strategy) {
        case LINEAR_PROBING: insert_impl<LINEAR_PROBING>(key, 1); break;
        case QUADRATIC_PROBING: insert_impl<QUADRATIC_PROBING>(key, 1); break;
        case DOUBLE_HASHING: insert_impl<DOUBLE_HASHING>(key, 1); break;
    }
}

// ========================================================================
// BÚSQUEDA OPTIMIZADA
// ========================================================================
template <CollisionStrategy S>
int ClosedHashTable::get_count_impl(const std::string& key) const {
    size_t index = hash_primary(key, table_size);
    size_t step = 1;
    
    if constexpr (S == DOUBLE_HASHING) {
        if (hash_secondary) {
            step = hash_secondary(key, table_size);
            step |= 1; // Fuerza salto impar
        }
    }

    size_t i = 0;
    while (i < table_size) {
        size_t probe_index = 0;
        
        if constexpr (S == LINEAR_PROBING) {
            probe_index = (index + i) & (table_size - 1);
        } else if constexpr (S == QUADRATIC_PROBING) {
            probe_index = (index + (i * i + i) / 2) & (table_size - 1);
        } else if constexpr (S == DOUBLE_HASHING) {
            probe_index = (index + i * step) & (table_size - 1);
        }

        if (table[probe_index].status == EMPTY) {
            return 0; // Si chocamos con un espacio vacío real, la llave no existe
        } else if (table[probe_index].key == key) {
            return table[probe_index].count;
        }
        i++;
    }
    return 0;
}

// Wrapper público
int ClosedHashTable::get_count(const std::string& key) const {
    switch (strategy) {
        case LINEAR_PROBING: return get_count_impl<LINEAR_PROBING>(key);
        case QUADRATIC_PROBING: return get_count_impl<QUADRATIC_PROBING>(key);
        case DOUBLE_HASHING: return get_count_impl<DOUBLE_HASHING>(key);
    }
    return 0;
}

// ========================================================================
// MEMORIA CON SSO-AWARENESS
// ========================================================================
size_t ClosedHashTable::get_memory_usage() const {
    // Memoria estática del objeto y los buckets continuos del vector
    size_t memory = sizeof(*this) + (table.capacity() * sizeof(HashEntry));
    
    // Iteramos para detectar alojamiento en Heap producto del Small String Optimization (SSO)
    for (const auto& entry : table) {
        if (entry.status == OCCUPIED) {
            size_t str_capacity = entry.key.capacity();
            // Si la capacidad de la string en bytes supera su propio tamaño de pila,
            // significa que la STL tuvo que hacer malloc/new para guardarla en el Heap.
            if (str_capacity * sizeof(char) > sizeof(std::string)) {
                memory += str_capacity * sizeof(char);
            }
        }
    }
    return memory;
}