cmake -DBUILD_TESTS=ON  -DTEST_ROOT_PATH="../tests" -DMULTIFUNGIBLE_NETWORK=Local -D__SIMULATE__=false -DMULTIFUNGIBLE_CONFIG_FILE="./config folder/config.toml" .
cmake --build . 