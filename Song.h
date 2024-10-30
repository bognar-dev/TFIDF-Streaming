//
// Created by nikla on 29/10/2024.
//

#ifndef TFIDF_STREAMING_SONG_H
#define TFIDF_STREAMING_SONG_H


#include <string>
#include <chrono>

struct Song {
    std::string id;
    std::string title;
    std::string artist;
    std::string lyrics;
    std::chrono::system_clock::time_point timestamp;

    Song(const std::string& i, const std::string& t,
         const std::string& a, const std::string& l)
            : id(i), title(t), artist(a), lyrics(l),
              timestamp(std::chrono::system_clock::now()) {}
};


#endif //TFIDF_STREAMING_SONG_H
