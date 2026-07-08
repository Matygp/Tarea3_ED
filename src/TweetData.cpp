// src/TweetData.cpp
#include "../include/TweetData.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

// Saneamiento para eliminar espacios, saltos de línea y comillas residuales
static std::string sanearString(const std::string& str) {
    if (str.empty()) return str;
    size_t primero = 0;
    size_t ultimo = str.length() - 1;
    
    while (primero <= ultimo && (std::isspace(static_cast<unsigned char>(str[primero])) || str[primero] == '"')) {
        primero++;
    }
    while (ultimo >= primero && (std::isspace(static_cast<unsigned char>(str[ultimo])) || str[ultimo] == '"')) {
        ultimo--;
    }
    return (primero <= ultimo) ? str.substr(primero, ultimo - primero + 1) : "";
}

std::vector<TweetData> loadDataset(const std::string& filepath) {
    std::vector<TweetData> tweets;
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filepath << std::endl;
        return tweets;
    }

    std::string line;
    std::getline(file, line); // Saltar cabecera

    // Parseo resistente a comas (,) internas dentro del texto del tweet (full_text)
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<std::string> columns;
        std::string current_field;
        bool in_quotes = false;
        
        for (size_t i = 0; i < line.length(); ++i) {
            char c = line[i];
            if (c == '"') {
                in_quotes = !in_quotes; // Alternar estado de comillas
            } else if (c == ',' && !in_quotes) {
                // Coma fuera de comillas = separador de columna
                columns.push_back(sanearString(current_field));
                current_field.clear();
            } else {
                current_field += c;
            }
        }
        columns.push_back(sanearString(current_field)); // Última columna de la fila

        // Según el formato: 5 -> user_id, 7 -> user_screen_name
        if (columns.size() >= 8) {
            TweetData tweet;
            tweet.user_id = columns[5];
            tweet.user_screen_name = columns[7];
            tweets.push_back(tweet);
        }
    }
    return tweets;
}