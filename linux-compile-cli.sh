cmake -DBUILD_CLI=ON -DTEST_ROOT_PATH="../tests" -DMULTIFUNGIBLE_NETWORK=Devnet -D__SIMULATE__=false -DMULTIFUNGIBLE_CONFIG_FILE="./config folder/config.toml" .
cmake --build . 