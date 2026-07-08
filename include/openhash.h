// include/openhash.h
#ifndef OPENHASH_H
#define OPENHASH_H

#include <string>
#include <vector>
#include <list>

struct HashNode {
    std::string key;
    int count;
    HashNode(const std::string& k, int c = 1) : key(k), count(c) {}
};

typedef size_t (*HashFunc)(const std::string&, size_t);

class OpenHash {
private:
    std::vector<std::list<HashNode>> table;
    size_t table_size;
    HashFunc hash_func;
    size_t num_elements;

public:
    OpenHash(size_t size, HashFunc hf);
    void insert(const std::string& key);
    int get_count(const std::string& key) const;
    size_t get_memory_usage() const;
};

#endif