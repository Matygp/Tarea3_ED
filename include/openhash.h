#ifndef OPEN_HASH_HPP
#define OPEN_HASH_HPP

#include <string>
#include <vector>
#include <list>

// Definimos un tipo para la función hash: recibe el string y el tamaño de la tabla, devuelve el índice
typedef size_t (*HashFunction)(const std::string&, size_t);

// Estructura para almacenar la clave y su contador de tweets
struct HashNode {
    std::string key;
    int count;
    HashNode(const std::string& k) : key(k), count(1) {}
};

class OpenHash {
private:
    std::vector<std::list<HashNode>> table;
    size_t num_elements;
    size_t table_size;
    HashFunction hash_func; // Puntero a la función hash a utilizar

public:
    // Constructor: recibe el tamaño inicial y la función hash
    OpenHash(size_t size, HashFunction hf);

    // Método principal: inserta o actualiza el contador
    void insert(const std::string& key);

    // Método de utilidad para verificar cuántos tweets tiene un usuario
    int get_count(const std::string& key) const;

    // Método para estimar el uso de memoria (requerido para el informe)
    size_t get_memory_usage() const;
};

#endif