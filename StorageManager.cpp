//
// Created by nikla on 29/10/2024.
//

#include "StorageManager.h"

StorageManager::StorageManager(const std::string &path) : db_path(path) {
    int rc = sqlite3_open(path.c_str(), &db);
    if (rc) {
        throw std::runtime_error("Can't open database: " +
                                 std::string(sqlite3_errmsg(db)));
    }
    if (!tables_exist()) {
    init_tables();
}
}

bool StorageManager::tables_exist() {
    const char* sql = R"(
        SELECT name FROM sqlite_master WHERE type='table' AND name='songs';
    )";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    int rc = sqlite3_step(stmt);
    bool exists = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exists;
}

void StorageManager::init_tables() {
    const char* sql = R"(
            CREATE TABLE IF NOT EXISTS songs (
                id TEXT PRIMARY KEY,
                title TEXT,
                artist TEXT,
                lyrics TEXT,
                timestamp INTEGER
            );

            CREATE TABLE IF NOT EXISTS tf_idf_scores (
                song_id TEXT,
                term TEXT,
                score REAL,
                FOREIGN KEY(song_id) REFERENCES songs(id)
            );
        )";

    char* err_msg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        std::string error = err_msg;
        sqlite3_free(err_msg);
        throw std::runtime_error("SQL error: " + error);
    }
}

void StorageManager::save_song(const Song &song, const std::unordered_map<std::string, double> &scores) {
    // Begin transaction
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    try {
        // Save song
        sqlite3_stmt* stmt;
        const char* sql = "INSERT INTO songs VALUES (?, ?, ?, ?, ?)";
        sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

        sqlite3_bind_text(stmt, 1, song.id.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, song.title.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, song.artist.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, song.lyrics.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 5,
                           std::chrono::system_clock::to_time_t(song.timestamp));

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        // Save TF-IDF scores
        const char* score_sql =
                "INSERT INTO tf_idf_scores VALUES (?, ?, ?)";
        sqlite3_prepare_v2(db, score_sql, -1, &stmt, nullptr);

        for (const auto& [term, score] : scores) {
            sqlite3_bind_text(stmt, 1, song.id.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, term.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_double(stmt, 3, score);
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
        }

        sqlite3_finalize(stmt);

        // Commit transaction
        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    }
    catch (...) {
        // Rollback on error
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw;
    }
}
