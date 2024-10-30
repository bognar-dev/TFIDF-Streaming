//
// Created by nikla on 29/10/2024.
//

#include "IFIDFCalculator.h"



std::vector<std::string> TFIDFCalculator::preprocess_text(const std::string &text) {
    std::vector<std::string> terms;
    std::stringstream ss(text);
    std::string term;

    while (ss >> term) {
        // Convert to lowercase
        std::transform(term.begin(), term.end(), term.begin(), ::tolower);
        // Remove punctuation
        term.erase(std::remove_if(term.begin(), term.end(), ::ispunct),
                   term.end());
        if (!term.empty()) {
            terms.push_back(term);
        }
    }
    return terms;
}

double TFIDFCalculator::calculate_tf(const std::string &term, const std::vector<std::string> &terms) const {
    auto count = std::count(terms.begin(), terms.end(), term);
    return static_cast<double>(count) / terms.size();
}

double TFIDFCalculator::calculate_idf(const std::string &term) const {
    auto docs_with_term = term_doc_frequencies.at(term).size();
    return std::log(static_cast<double>(document_count) /
                    (1 + docs_with_term));
}

std::unordered_map<std::string, double> TFIDFCalculator::update_tf_idf(const std::shared_ptr<Song> &song) {
    ++document_count;
    auto terms = preprocess_text(song->lyrics);
    std::unordered_map<std::string, double> scores;

    // Calculate TF for new document
    for (const auto& term : terms) {
        auto tf = calculate_tf(term, terms);
        term_frequencies[term][song->id] = tf;
        term_doc_frequencies[term].insert(song->id);

        // Update IDF for this term
        inverse_doc_frequencies[term] = calculate_idf(term);
    }

    return get_tf_idf_scores(song->id);
}

std::unordered_map<std::string, double> TFIDFCalculator::get_tf_idf_scores(const std::string &song_id) const {
    std::unordered_map<std::string, double> scores;

    for (const auto &[term, docs]: term_frequencies) {
        auto doc_it = docs.find(song_id);
        if (doc_it != docs.end()) {
            auto idf_it = inverse_doc_frequencies.find(term);
            if (idf_it != inverse_doc_frequencies.end()) {
                scores[term] = doc_it->second * idf_it->second;
            }
        }
    }
    return scores;
}
