#!/bin/bash

# Build script for even-number-of-ones example
# This script compiles the code, runs the program, and builds the LaTeX output
# Usage: ./build.sh [clean]

set -e  # Exit on any error

# Function to clean directories
clean_directories() {
    echo "=== Cleaning even-number-of-ones example ==="
    
    # Clean build directory
    if [ -d "build" ]; then
        echo "Removing build directory..."
        rm -rf build
        echo "Build directory cleaned."
    else
        echo "Build directory does not exist."
    fi
    
    # Clean latex directory
    if [ -d "latex" ]; then
        echo "Removing latex directory..."
        rm -rf latex
        echo "LaTeX directory cleaned."
    else
        echo "LaTeX directory does not exist."
    fi
    
    echo "=== Cleanup completed! ==="
}

# Check if clean parameter is provided
if [ "$1" = "clean" ]; then
    clean_directories
    exit 0
fi

echo "=== Building even-number-of-ones example ==="

# Step 1: Configure and build the project
echo "Step 1: Configuring project with CMake..."
cmake -B build -S .

echo "Step 2: Building project..."
cmake --build build

# Step 2: Run the program to generate LaTeX files
echo "Step 3: Running the program to generate LaTeX files..."
./build/even_number_of_ones

# Step 3: Build the LaTeX output if latex directory exists
if [ -d "latex" ]; then
    echo "Step 4: Building LaTeX output..."
    cd latex
    make
    cd ..
    echo "LaTeX build completed!"
    
    # Display the generated files
    echo ""
    echo "Generated files:"
    ls -la latex/
else
    echo "Warning: latex directory not found. LaTeX files may not have been generated."
fi

echo ""
echo "=== Build process completed successfully! ==="
