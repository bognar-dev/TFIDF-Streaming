#include "StorageManager.h"
#include "PerformaceMonitor.h"
#include "MusicStreamProcessor.h"
#include "NetworkManager.h"

int main() {
    try {
        // Initialize components
        StorageManager storage("songs.db");
        PerformanceMonitor monitor;
        MusicStreamProcessor processor;
        NetworkManager network(processor, 8080);

        // Start network thread
        std::thread network_thread([&network]() { network.run(); });

        // Start processing thread
        std::thread processing_thread(
                [&processor]() { processor.process_stream(); }
        );

        // Wait for shutdown signal
        std::string input;
        std::getline(std::cin, input);

        // Clean shutdown
        processor.stop();
        processing_thread.join();
        network_thread.join();

        // Print final metrics
        monitor.print_report();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}