cmake -DBUILD_TESTS=ON -DMULTIFUNGIBLE_NETWORK=Local -D__SIMULATE__=false -DMULTIFUNGIBLE_CONFIG_FILE="./config.toml" .
cmake --build . 