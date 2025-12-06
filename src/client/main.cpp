#include "GameClient.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::cout << "==================================" << std::endl;
    std::cout << "  Combat Arms: Cabin Fever" << std::endl;
    std::cout << "  Client" << std::endl;
    std::cout << "==================================" << std::endl;

    std::string playerName = "Player";
    std::string serverAddress = "127.0.0.1";
    int serverPort = 7777;

    if (argc > 1) {
        playerName = argv[1];
    }
    if (argc > 2) {
        serverAddress = argv[2];
    }
    if (argc > 3) {
        serverPort = std::atoi(argv[3]);
    }

    std::cout << "Player name: " << playerName << std::endl;
    std::cout << "Server: " << serverAddress << ":" << serverPort << std::endl;

    GameClient client(playerName, serverAddress, serverPort);

    if (!client.initialize()) {
        std::cerr << "Failed to initialize client!" << std::endl;
        return 1;
    }

    std::cout << "\nControls:" << std::endl;
    std::cout << "  WASD / Arrow Keys - Move" << std::endl;
    std::cout << "  Mouse - Aim" << std::endl;
    std::cout << "  Left Click - Shoot" << std::endl;
    std::cout << "  R - Reload" << std::endl;
    std::cout << "  ESC - Quit" << std::endl;
    std::cout << "\nStarting game..." << std::endl;

    client.run();
    client.shutdown();

    return 0;
}
