#!/bin/bash

# Check if Google Test is already installed
if ! dpkg -s libgtest-dev >/dev/null 2>&1; then
  echo "Google Test is not installed. Installing..."
  
  # Install Google Test
  sudo apt-get update
  sudo apt-get install -y libgtest-dev
  
  # Build and install Google Test from source
  cd /usr/src/gtest
  sudo cmake CMakeLists.txt
  sudo make
  sudo cp *.a /usr/lib
else
  echo "Google Test is already installed."
fi
