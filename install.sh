CURRENT_DIR=$(pwd)
INCLUDE_PATH=/usr/include
LIB_PATH=/usr/lib

echo "Getting updates and installing essentials..."
sudo apt-get update && sudo apt-get install build-essential

GCC_VERSION_CURRENT="$(gcc -dumpfullversion)"
GCC_VERSION_MIN_REQUIRED="9.4"
if ! [ "$(printf '%s\n' "$GCC_VERSION_MIN_REQUIRED" "$GCC_VERSION_CURRENT" | sort -V | head -n1)" = "$GCC_VERSION_MIN_REQUIRED" ]; then
       echo "Found GCC compiler version = ${GCC_VERSION_CURRENT}, which is less than min required = ${GCC_VERSION_MIN_REQUIRED}."
       echo "Install gcc version 9.4? (y/n)"
       read -r yn
       case $yn in
              [Yy]* ) sudo apt-get install gcc-9 || exit 1;;
              * ) echo "Aborting..." & exit 1;;
       esac
fi

echo "Installing libsodium library..."
sudo dpkg --configure -a
sudo apt install libsodium-dev || exit 1

echo "Updating submodules..."
git submodule update --init --recursive || exit 1

if ! dpkg -s libssl-dev > /dev/null; then
  echo "OpenSSL libssl-dev not installed, installing..."
  sudo apt-get install libssl-dev || exit 1
fi

echo "CMake"
if ! dpkg -s cmake > /dev/null; then
  echo "CMake not installed, installing..."
  sudo apt install cmake || sudo snap install cmake --classic || exit 1
fi

echo "Finished successfully !"
