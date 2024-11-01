cmake_minimum_required(VERSION 3.22)
project(TFIDF_Streaming)

set(CMAKE_CXX_STANDARD 23)

add_executable(TFIDF_Streaming main.cpp
        Song.cpp
        Song.h
        IFIDFCalculator.cpp
        IFIDFCalculator.h
        SongDatabase.cpp
        SongDatabase.h
        MusicStreamProcessor.cpp
        MusicStreamProcessor.h
        TextPreProcessor.cpp
        TextPreProcessor.h
        StorageManager.cpp
        StorageManager.h
        PerformaceMonitor.cpp
        PerformaceMonitor.h
        NetworkManager.cpp
        NetworkManager.h)


find_package(SQLite3)
find_package(nlohmann_json 3.11.3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system asio)

if (Boost_FOUND)
    message(STATUS "Boost found")

endif ()
target_link_libraries(TFIDF_Streaming SQLite::SQLite3 nlohmann_json::nlohmann_json Boost::system Boost::asio)
add_executable(client client.cpp)
target_link_libraries(client Boost::system Boost::asio nlohmann_json::nlohmann_json)
