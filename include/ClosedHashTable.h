#ifndef CLOSED_HASH_TABLE_HPP
#define CLOSED_HASH_TABLE_HPP

#include <string>
#include <vector>

// Enumerador para seleccionar la estrategia de colisión
enum CollisionStrategy {
    LINEAR_PROBING,
    QUADRATIC_PROBING,
    DOUBLE_HASHING
};

// Estados posibles para cada "bucket" de la tabla
enum SlotStatus {
    EMPTY,
    OCCUPIED
};

// Nodo de la tabla hash cerrada
struct HashEntry {
    std::string key;
    int count;
    SlotStatus status;
    
    // Constructor por defecto
    HashEntry() : key(""), count(0), status(EMPTY) {}
};

typedef size_t (*HashFunc)(const std::string&, size_t);

class ClosedHashTable {
private:
    std::vector<HashEntry> table;
    size_t table_size;
    size_t num_elements;
    CollisionStrategy strategy;
    
    HashFunc hash_primary;
    HashFunc hash_secondary;

    // Métodos privados para gestión interna
    size_t next_power_of_2(size_t n) const;
    void rehash();

    // Plantillas que resuelven la estrategia en tiempo de compilación (cero branching)
    template <CollisionStrategy S>
    void insert_impl(const std::string& key, int count = 1);

    template <CollisionStrategy S>
    int get_count_impl(const std::string& key) const;

public:
    ClosedHashTable(size_t size, CollisionStrategy strat, HashFunc h1, HashFunc h2 = nullptr);
    
    void insert(const std::string& key);
    int get_count(const std::string& key) const;
    size_t get_memory_usage() const;
};

#endif