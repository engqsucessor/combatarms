#!/bin/bash

# Build script for Combat Arms: Cabin Fever

echo "=================================="
echo "  Building Cabin Fever"
echo "=================================="

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

cd build

# Run CMake
echo "Running CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

# Build
echo "Building..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo ""
echo "=================================="
echo "  Build successful!"
echo "=================================="
echo ""
echo "Run the client with: ./build/cabin_fever_client"
echo "Run the server with: ./build/cabin_fever_server"
