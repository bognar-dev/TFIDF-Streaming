//
// Created by nikla on 29/10/2024.
//

#ifndef TFIDF_STREAMING_PERFORMACEMONITOR_H
#define TFIDF_STREAMING_PERFORMACEMONITOR_H
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
#include "Song.h"

class PerformanceMonitor {
private:
    struct Metrics {
        size_t processed_songs{0};
        size_t total_terms{0};
        double avg_processing_time{0.0};
        std::chrono::system_clock::time_point last_update;

        std::unordered_map<std::string, size_t> term_frequencies;
        std::vector<double> processing_times;
    };

    Metrics metrics;
    std::mutex metrics_mutex;

public:
    class Timer {
    private:
        std::chrono::high_resolution_clock::time_point start;
        PerformanceMonitor& monitor;

    public:
        Timer(PerformanceMonitor& m)
                : start(std::chrono::high_resolution_clock::now()), monitor(m) {}

        ~Timer() {
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - start
            ).count();
            monitor.record_processing_time(duration);
        }
    };

    void update_metrics(const Song& song,
                        const std::unordered_map<std::string, double>& scores);

    void record_processing_time(double ms);

    void print_report();
};


#endif //TFIDF_STREAMING_PERFORMACEMONITOR_H
