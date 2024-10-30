//
// Created by nikla on 29/10/2024.
//

#ifndef TFIDF_STREAMING_SONGDATABASE_H
#define TFIDF_STREAMING_SONGDATABASE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <memory>
#include <cmath>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Song.h"

class SongDatabase {
private:
    std::unordered_map<std::string, std::shared_ptr<Song>> songs;
    std::unordered_map<std::string,
    std::unordered_map<std::string, double>> tf_idf_scores;
    std::mutex mutex;
    static double calculate_cosine_similarity(
            const std::unordered_map<std::string, double>& scores1,
            const std::unordered_map<std::string, double>& scores2);

public:
    void add_song(const std::shared_ptr<Song>& song,
                  const std::unordered_map<std::string, double>& scores);

    std::vector<std::string> find_similar_songs(
            const std::string& song_id, size_t limit = 5);
};


#endif //TFIDF_STREAMING_SONGDATABASE_H
