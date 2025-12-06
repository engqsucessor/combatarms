cmake_minimum_required(VERSION 3.15)
project(CombatArmsCabinFever VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(include)

# Common library for shared code
add_library(game_common STATIC
    src/common/Vector2.cpp
    src/common/Entity.cpp
    src/common/Player.cpp
    src/common/Zombie.cpp
    src/common/Weapon.cpp
    src/common/GameState.cpp
    src/common/Map.cpp
    src/common/Network.cpp
)

target_include_directories(game_common PUBLIC include)

# Client executable
add_executable(cabin_fever_client
    src/client/main.cpp
    src/client/Renderer.cpp
    src/client/InputHandler.cpp
    src/client/GameClient.cpp
)

target_link_libraries(cabin_fever_client
    game_common
    ${SDL2_LIBRARIES}
    Threads::Threads
)

# Server executable
add_executable(cabin_fever_server
    src/server/main.cpp
    src/server/GameServer.cpp
    src/server/AIController.cpp
    src/server/ZombieSpawner.cpp
)

target_link_libraries(cabin_fever_server
    game_common
    Threads::Threads
)

# Build configuration
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(game_common PRIVATE DEBUG_MODE)
endif()
