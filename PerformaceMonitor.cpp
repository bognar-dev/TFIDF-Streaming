//
// Created by nikla on 29/10/2024.
//

#include "PerformaceMonitor.h"

void PerformanceMonitor::update_metrics(const Song &song, const std::unordered_map<std::string, double> &scores) {
    std::lock_guard<std::mutex> lock(metrics_mutex);

    metrics.processed_songs++;
    metrics.total_terms += scores.size();
    metrics.last_update = std::chrono::system_clock::now();

    for (const auto& [term, _] : scores) {
        metrics.term_frequencies[term]++;
    }
}

void PerformanceMonitor::record_processing_time(double ms) {
    std::lock_guard<std::mutex> lock(metrics_mutex);
    metrics.processing_times.push_back(ms);

    // Update average
    metrics.avg_processing_time = std::accumulate(
            metrics.processing_times.begin(),
            metrics.processing_times.end(), 0.0
    ) / metrics.processing_times.size();
}

void PerformanceMonitor::print_report() {
    std::lock_guard<std::mutex> lock(metrics_mutex);
    std::cout << "Performance Report:\n"
              << "Processed Songs: " << metrics.processed_songs << "\n"
              << "Total Terms: " << metrics.total_terms << "\n"
              << "Avg Processing Time: " << metrics.avg_processing_time
              << "ms\n"
              << "Most Common Terms:\n";

    std::vector<std::pair<std::string, size_t>> terms(
            metrics.term_frequencies.begin(),
            metrics.term_frequencies.end()
    );

    std::partial_sort(
            terms.begin(),
            terms.begin() + std::min(size_t(10), terms.size()),
            terms.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; }
    );

    for (size_t i = 0; i < std::min(size_t(10), terms.size()); ++i) {
        std::cout << terms[i].first << ": " << terms[i].second << "\n";
    }
}
