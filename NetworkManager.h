//
// Created by nikla on 29/10/2024.
//

#ifndef TFIDF_STREAMING_NETWORKMANAGER_H
#define TFIDF_STREAMING_NETWORKMANAGER_H
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
#include "MusicStreamProcessor.h"

class NetworkManager {
private:
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor;
    MusicStreamProcessor& processor;

    struct ClientSession : std::enable_shared_from_this<ClientSession> {
        boost::asio::ip::tcp::socket socket;
        std::vector<char> buffer;

        ClientSession(boost::asio::ip::tcp::socket s)
                : socket(std::move(s)), buffer(8192) {}
    };

public:
    NetworkManager(MusicStreamProcessor& proc, unsigned short port)
            : processor(proc),
              acceptor(io_context,
                       boost::asio::ip::tcp::endpoint(
                               boost::asio::ip::tcp::v4(), port)) {
        start_accept();
    }

    void start_accept() {
        acceptor.async_accept(
                [this](boost::system::error_code ec,
                       boost::asio::ip::tcp::socket socket) {
                    if (!ec) {
                        auto session = std::make_shared<ClientSession>(
                                std::move(socket));
                        start_read(session);
                    }
                    start_accept();
                });
    }

    void start_read(std::shared_ptr<ClientSession> session) {
        session->socket.async_read_some(
                boost::asio::buffer(session->buffer),
                [this, session](boost::system::error_code ec, std::size_t length) {
                    if (!ec) {
                        // Process received data
                        std::string data(session->buffer.begin(),
                                         session->buffer.begin() + length);
                        try {
                            auto json = nlohmann::json::parse(data);
                            auto song = std::make_shared<Song>(
                                    json["id"],
                                    json["title"],
                                    json["artist"],
                                    json["lyrics"]
                            );
                            processor.add_song_to_queue(song);

                            // Send acknowledgment
                            std::string response = "{\"status\":\"success\"}\n";
                            boost::asio::async_write(
                                    session->socket,
                                    boost::asio::buffer(response),
                                    [session](boost::system::error_code ec,
                                              std::size_t /*length*/) {
                                        if (!ec) {
                                            start_read(session);
                                        }
                                    });
                        }
                        catch (const std::exception& e) {
                            // Handle error
                            std::string response = "{\"status\":\"error\",\"message\":\"" +
                                                   std::string(e.what()) + "\"}\n";
                            boost::asio::async_write(
                                    session->socket,
                                    boost::asio::buffer(response),
                                    [session](boost::system::error_code ec,
                                              std::size_t /*length*/) {
                                        if (!ec) {
                                            start_read(session);
                                        }
                                    });
                        }
                    }
                });
    }

    void run() {
        io_context.run();
    }
};


#endif //TFIDF_STREAMING_NETWORKMANAGER_H
