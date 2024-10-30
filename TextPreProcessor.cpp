//
// Created by nikla on 29/10/2024.
//

#include "TextPreProcessor.h"

std::vector<std::string> TextPreprocessor::preprocess(const std::string &text) {
    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string word;

    while (iss >> word) {

        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct),
                   word.end());


        if (stop_words.find(word) != stop_words.end()) {
            continue;
        }


        word = stem_word(word);

        if (!word.empty()) {
            tokens.push_back(word);
        }
    }
    return tokens;
}

std::string TextPreprocessor::stem_word(const std::string &word) {
    std::string result = word;
    for (const auto &[suffix, replacement]: stemming_rules) {
        if (result.length() > suffix.length() &&
            result.substr(result.length() - suffix.length()) == suffix) {
            result = result.substr(0, result.length() - suffix.length()) + replacement;
            break;
        }
    }
    return result;
}
