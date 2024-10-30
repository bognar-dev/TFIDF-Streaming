//
// Created by nikla on 29/10/2024.
//

#ifndef TFIDF_STREAMING_STORAGEMANAGER_H
#define TFIDF_STREAMING_STORAGEMANAGER_H
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
#include <sqlite3.h>
#include "Song.h"

class StorageManager {
private:
    std::string db_path;
    sqlite3* db;
    bool tables_exist();
public:
    StorageManager(const std::string& path);

    void init_tables();


    void save_song(const Song& song,
                   const std::unordered_map<std::string, double>& scores);
};


#endif //TFIDF_STREAMING_STORAGEMANAGER_H
