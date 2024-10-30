//
// Created by nikla on 29/10/2024.
//

#include "SongDatabase.h"



double SongDatabase::calculate_cosine_similarity(const std::unordered_map<std::string, double> &scores1,
                                                 const std::unordered_map<std::string, double> &scores2) {
    double dot_product = 0.0;
    double mag1 = 0.0;
    double mag2 = 0.0;

    // Calculate dot product and magnitudes
    for (const auto& [term, score1] : scores1) {
        auto it = scores2.find(term);
        if (it != scores2.end()) {
            dot_product += score1 * it->second;
        }
        mag1 += score1 * score1;
    }

    for (const auto& [_, score2] : scores2) {
        mag2 += score2 * score2;
    }

    double magnitude = std::sqrt(mag1) * std::sqrt(mag2);
    return magnitude == 0.0 ? 0.0 : dot_product / magnitude;
}

void SongDatabase::add_song(const std::shared_ptr<Song> &song, const std::unordered_map<std::string, double> &scores) {
    std::lock_guard<std::mutex> lock(mutex);
    songs[song->id] = song;
    tf_idf_scores[song->id] = scores;
}

std::vector<std::string> SongDatabase::find_similar_songs(const std::string &song_id, size_t limit){
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<std::pair<std::string, double>> similarities;

    auto scores_it = tf_idf_scores.find(song_id);
    if (scores_it == tf_idf_scores.end()) {
        return {};
    }

    const auto& target_scores = scores_it->second;
    for (const auto& [other_id, other_scores] : tf_idf_scores) {
        if (other_id != song_id) {
            double similarity = calculate_cosine_similarity(
                    target_scores, other_scores);
            similarities.emplace_back(other_id, similarity);
        }
    }

    // Sort by similarity
    std::partial_sort(
            similarities.begin(),
            similarities.begin() + std::min(limit, similarities.size()),
            similarities.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; }
    );

    // Extract song IDs
    std::vector<std::string> result;
    result.reserve(std::min(limit, similarities.size()));
    for (size_t i = 0; i < limit && i < similarities.size(); ++i) {
        result.push_back(similarities[i].first);
    }
    return result;
}

