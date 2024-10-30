//
// Created by nikla on 29/10/2024.
//

#include "MusicStreamProcessor.h"

void MusicStreamProcessor::process_song(const std::shared_ptr<Song> &song) {
    // Calculate TF-IDF scores
    auto tf_idf_scores = calculator.update_tf_idf(song);

    // Add to database
    database.add_song(song, tf_idf_scores);

    // Find similar songs
    auto similar_songs = database.find_similar_songs(song->id);

    // Log processing (replace with proper logging in production)
    std::cout << "Processed song: " << song->title << "\n";
    std::cout << "Found " << similar_songs.size() << " similar songs\n";
}

void MusicStreamProcessor::add_song_to_queue(std::shared_ptr<Song> song) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    song_queue.push(std::move(song));
    queue_cv.notify_one();
}

void MusicStreamProcessor::process_stream() {
    while (running) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue_cv.wait(lock, [this] {
            return !song_queue.empty() || !running;
        });

        if (!running) break;

        if (!song_queue.empty()) {
            auto song = song_queue.front();
            song_queue.pop();
            lock.unlock();

            process_song(song);
        }
    }
}

void MusicStreamProcessor::stop() {
    std::lock_guard<std::mutex> lock(queue_mutex);
    running = false;
    queue_cv.notify_all();
}
