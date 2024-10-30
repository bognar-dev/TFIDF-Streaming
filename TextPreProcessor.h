//
// Created by nikla on 29/10/2024.
//

#ifndef TFIDF_STREAMING_TEXTPREPROCESSOR_H
#define TFIDF_STREAMING_TEXTPREPROCESSOR_H

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
#include <unordered_set>

class TextPreprocessor {
private:
    std::unordered_set<std::string> stop_words{
            "a", "an", "the", "in", "on", "at", "to", "for", "of", "with"

    };

    std::vector<std::pair<std::string, std::string>> stemming_rules{
            {"ing",  ""},
            {"ed",   ""},
            {"ly",   ""},
            {"ness", ""}

    };

public:
    std::string stem_word(const std::string &word);

    std::vector<std::string> preprocess(const std::string &text);
};

#endif //TFIDF_STREAMING_TEXTPREPROCESSOR_H
