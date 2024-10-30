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
find_package(Boost)
target_link_libraries(TFIFD_Streaming SQLite::SQLite3)
target_link_libraries(TFIFD_Streaming boost::boost)