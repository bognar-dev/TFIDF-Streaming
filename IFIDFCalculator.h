//
// Created by nikla on 29/10/2024.
//

#ifndef TFIDF_STREAMING_IFIDFCALCULATOR_H
#define TFIDF_STREAMING_IFIDFCALCULATOR_H


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

class TFIDFCalculator {
private:
    size_t document_count{0};
    std::unordered_map<std::string, std::set<std::string>> term_doc_frequencies;
    std::unordered_map<std::string,
            std::unordered_map<std::string, double>> term_frequencies;
    std::unordered_map<std::string, double> inverse_doc_frequencies;

    static std::vector<std::string> preprocess_text(const std::string &text);

    double calculate_tf(const std::string &term,
                        const std::vector<std::string> &terms) const;

    double calculate_idf(const std::string &term) const;

public:
    std::unordered_map<std::string, double> update_tf_idf(
            const std::shared_ptr<Song> &song);

    std::unordered_map<std::string, double> get_tf_idf_scores(
            const std::string &song_id) const;
};


#endif //TFIDF_STREAMING_IFIDFCALCULATOR_H
