#ifndef TWEET_DATA_HPP
#define TWEET_DATA_HPP

#include <string>
#include <vector>

//Estructura ligera para no saturar la memoria RAM
struct TweetData {
    std::string user_id;
    std::string user_screen_name;
};

//Declaración de la función que leerá el CSV
std::vector<TweetData> loadDataset(const std::string& filename);

#endif