#include "GameServer.h"
#include <iostream>
#include <csignal>
#include <atomic>

std::atomic<bool> keepRunning(true);

void signalHandler(int signal) {
    std::cout << "\nShutting down server..." << std::endl;
    keepRunning = false;
}

int main(int argc, char* argv[]) {
    std::cout << "==================================" << std::endl;
    std::cout << "  Combat Arms: Cabin Fever" << std::endl;
    std::cout << "  Dedicated Server" << std::endl;
    std::cout << "==================================" << std::endl;

    // Set up signal handler
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    int port = 7777;
    if (argc > 1) {
        port = std::atoi(argv[1]);
    }

    GameServer server(port);

    if (!server.start()) {
        std::cerr << "Failed to start server!" << std::endl;
        return 1;
    }

    // Run server
    while (keepRunning) {
        server.run();
    }

    server.stop();

    return 0;
}
