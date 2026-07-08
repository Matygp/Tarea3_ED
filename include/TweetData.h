// include/TweetData.h
#ifndef TWEETDATA_H
#define TWEETDATA_H

#include <string>
#include <vector>

struct TweetData {
    std::string user_screen_name;
    std::string user_id;
};

std::vector<TweetData> loadDataset(const std::string& filepath);

#endif