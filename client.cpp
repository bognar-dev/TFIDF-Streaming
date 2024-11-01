#include <iostream>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

int main() {
    try {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve("localhost", "8080").begin();

        boost::asio::ip::tcp::socket socket(io_context);
        socket.connect(endpoint);

        // Send a sample song data
        nlohmann::json song_data = {
            {"id", "123456789"},
            {"title", "My Song"},
            {"artist", "John Doe"},
            {"lyrics", "This is the song lyrics."}
        };
        std::string request = song_data.dump() + "\n";
        boost::asio::write(socket, boost::asio::buffer(request));

        // Read the response
        std::array<char, 128> buf;
        boost::system::error_code error;
        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof) {
            std::cout << "Connection closed by server" << std::endl;
        } else if (error) {
            throw boost::system::system_error(error);
        } else {
            std::string response(buf.data(), len);
            std::cout << "Server response: " << response << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}