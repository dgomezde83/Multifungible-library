cmake -DBUILD_CLI=ON -DMULTIFUNGIBLE_NETWORK=Devnet -D__SIMULATE__=false -DMULTIFUNGIBLE_CONFIG_FILE="./config folder/config.toml" -DCMAKE_C_COMPILER="C:/TDM-GCC-64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/TDM-GCC-64/bin/g++.exe" -DLIBSODIUM_INCLUDE_PATH="C:/libsodium/include" -DLIBSODIUM_LIB_PATH="C:/libsodium/lib" -DOPENSSL_INCLUDE_PATH="C:/OpenSSL3.0.8/include" -DOPENSSL_LIB_PATH="C:/OpenSSL3.0.8/lib" . -G "MinGW Makefiles"
cmake --build . 