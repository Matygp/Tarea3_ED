// include/HashFunctions.h
#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

#include <string>
#include <cstddef>

size_t hash_djb2(const std::string& str, size_t table_size);
size_t hash_sdbm(const std::string& str, size_t table_size);

#endif