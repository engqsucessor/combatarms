#pragma once

#include <string>
#include <vector>
#include <cstdint>

enum class PacketType : uint8_t {
    CONNECT_REQUEST,
    CONNECT_RESPONSE,
    DISCONNECT,
    PLAYER_INPUT,
    GAME_STATE_UPDATE,
    PLAYER_SHOOT,
    PLAYER_HIT,
    ZOMBIE_SPAWN,
    ZOMBIE_DIED,
    WAVE_START,
    WAVE_COMPLETE
};

struct Packet {
    PacketType type;
    std::vector<uint8_t> data;

    Packet(PacketType t) : type(t) {}
};

class NetworkManager {
public:
    virtual ~NetworkManager() = default;
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual bool sendPacket(const Packet& packet, const std::string& address = "") = 0;
    virtual bool receivePacket(Packet& packet, std::string& fromAddress) = 0;
};
