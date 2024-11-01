//
// Created by nikla on 29/10/2024.
//

#ifndef TFIDF_STREAMING_NETWORKMANAGER_H
#define TFIDF_STREAMING_NETWORKMANAGER_H
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <algorithm>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
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
        explicit ClientSession(boost::asio::ip::tcp::socket s);
    };

public:
    NetworkManager(MusicStreamProcessor& proc, unsigned short port);

    void start_accept();

    void start_read(std::shared_ptr<ClientSession> session);

    void run();
};


#endif //TFIDF_STREAMING_NETWORKMANAGER_H
