//
// Created by nikla on 29/10/2024.
//

#include "NetworkManager.h"

NetworkManager::ClientSession::ClientSession(boost::asio::ip::tcp::socket s): socket(std::move(s)), buffer(8192) {}

NetworkManager::NetworkManager(MusicStreamProcessor &proc, unsigned short port): processor(proc),
                                                                                 acceptor(io_context,
                                                                                          boost::asio::ip::tcp::endpoint(
                                                                                              boost::asio::ip::tcp::v4(), port)) {
    start_accept();
}

void NetworkManager::start_accept() {
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

void NetworkManager::start_read(std::shared_ptr<ClientSession> session) {
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
                        [&, session](boost::system::error_code ec,
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
                        [&, session](boost::system::error_code ec,
                                     std::size_t /*length*/) {
                            if (!ec) {
                                start_read(session);
                            }
                        });
                }
            }
        });
}

void NetworkManager::run() {
    try {
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception in io_context.run(): " << e.what() << std::endl;
    }
}
