//
// Created by nikla on 29/10/2024.
//

#ifndef TFIDF_STREAMING_MUSICSTREAMPROCESSOR_H
#define TFIDF_STREAMING_MUSICSTREAMPROCESSOR_H
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
#include <iostream>
#include "IFIDFCalculator.h"
#include "SongDatabase.h"

class MusicStreamProcessor {
private:
    TFIDFCalculator calculator;
    SongDatabase database;
    std::queue<std::shared_ptr<Song>> song_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;
    bool running{true};

    void process_song(const std::shared_ptr<Song>& song);

public:
    void add_song_to_queue(std::shared_ptr<Song> song);

    void process_stream();

    void stop();
};


#endif //TFIDF_STREAMING_MUSICSTREAMPROCESSOR_H
