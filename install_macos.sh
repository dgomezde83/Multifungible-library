CURRENT_DIR=$(pwd)
INCLUDE_PATH=/usr/local/include
LIB_PATH=/usr/local/lib

echo "Updating Homebrew and installing essentials..."
brew update
brew install gcc

# Determine the installed GCC version
GCC_VERSION=$(brew list gcc --versions | grep gcc | sed 's/gcc@//g' | awk '{print $2}')
echo "Using GCC version ${GCC_VERSION}"

# Setting GCC command to use the specific version
GCC_COMMAND="gcc-${GCC_VERSION}"

echo "Installing libsodium library..."

# install libsodium
brew install libsodium || exit 1

echo "libsodium installation complete."


echo "Updating submodules..."
git submodule update --init --recursive || exit 1

if ! brew ls --versions openssl > /dev/null; then
  echo "OpenSSL libssl not installed, installing..."
  brew install openssl || exit 1
fi

echo "CMake"
if ! brew ls --versions cmake > /dev/null; then
  echo "CMake not installed, installing..."
  brew install cmake || exit 1
fi

echo "Finished successfully !"
