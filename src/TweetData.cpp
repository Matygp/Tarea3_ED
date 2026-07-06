#include "../include/TweetData.h"
#include <iostream>
#include <fstream>

std::vector<TweetData> loadDataset(const std::string& filename) {
    std::vector<TweetData> data;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error, No se pudo abrir el archivo " << filename << "\n";
        return data;
    }

    std::string line, field;
    std::getline(file, line); // Se ignora la primera línea (cabecera)

    char c;
    bool inside_quotes = false;
    std::vector<std::string> row;
    
    while (file.get(c)) {
        if (c == '"') {
            inside_quotes = !inside_quotes;
        } else if (c == ',' && !inside_quotes) {
            row.push_back(field);
            field.clear();
        } else if (c == '\n' && !inside_quotes) {
            row.push_back(field);
            field.clear();
            
            // Indices: user_id es 5, user_screen_name es 7
            if (row.size() >= 8) {
                TweetData tweet;
                tweet.user_id = row[5];
                tweet.user_screen_name = row[7];
                data.push_back(tweet);
            }
            row.clear();
        } else {
            field += c;
        }
    }
    file.close();
    return data;
}