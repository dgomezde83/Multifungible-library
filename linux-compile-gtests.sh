cmake -DBUILD_TESTS=ON  -DTEST_ROOT_PATH=".." -DMULTIFUNGIBLE_NETWORK=Testnet -D__SIMULATE__=false -DMULTIFUNGIBLE_CONFIG_FILE="./config folder/config.toml" .
cmake --build . 