cmake_minimum_required(VERSION 3.19)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_GENERATOR "MinGW Makefiles")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_TESTS "Build test program" OFF)
option(BUILD_CLI "Build CLI program" OFF)
if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES arm64)
endif()

# Compiler
if (WIN32)
	if (NOT DEFINED CMAKE_C_COMPILER)
		set(CMAKE_C_COMPILER "C:/TDM-GCC-64/bin/gcc.exe")
	endif()
	if (NOT DEFINED CMAKE_CXX_COMPILER)
    		set(CMAKE_CXX_COMPILER "C:/TDM-GCC-64/bin/g++.exe")
	endif()
elseif(APPLE)
    # macOS specific compiler settings (if needed)
    # Set the default C and C++ compiler for macOS, if not already defined
    if (NOT DEFINED CMAKE_C_COMPILER)
        set(CMAKE_C_COMPILER "/usr/bin/gcc") #Careful, this might be Apple clang compiler
    endif()
    if (NOT DEFINED CMAKE_CXX_COMPILER)
        set(CMAKE_CXX_COMPILER "/usr/bin/g++")
    endif()
endif()
set(CMAKE_CXX_FLAGS "-fPIC")

# Global variables
add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)
if (WIN32)
	add_definitions(-D__WINDOWS__)
elseif(UNIX AND NOT APPLE)
	add_definitions(-D__UNIX__)
elseif(APPLE)
    add_definitions(-D__APPLE__)
endif()
add_definitions(-DMAKEDLL)
if (NOT DEFINED MULTIFUNGIBLE_NETWORK)
    set(MULTIFUNGIBLE_NETWORK Testnet)
endif()
add_definitions(-DMULTIFUNGIBLE_NETWORK=${MULTIFUNGIBLE_NETWORK})
if (NOT DEFINED __SIMULATE__)
    set(__SIMULATE__ false)
endif()
add_definitions(-D__SIMULATE__=${__SIMULATE__})
if (NOT DEFINED MULTIFUNGIBLE_CONFIG_FILE)
    set(MULTIFUNGIBLE_CONFIG_FILE ./config.toml)
endif()
add_definitions(-DMULTIFUNGIBLE_CONFIG_FILE=${MULTIFUNGIBLE_CONFIG_FILE})
if (NOT DEFINED TEST_ROOT_PATH)
    set(TEST_ROOT_PATH "../tests")
endif()
add_definitions(-DTEST_ROOT_PATH=${TEST_ROOT_PATH})

# Project name
project(Multifungible)

# Main root files
set (SOURCES_ROOT methodsDLL.cpp)

# Source files
set (MyDLL_SRC 
    src/transaction/transaction.cpp
    src/transaction/signer.cpp
    src/transaction/messagesigner.cpp
    src/transaction/esdt.cpp
    src/transaction/gas_estimator.cpp
    src/transaction/token_payment.cpp
    src/transaction/payload_builder.cpp
    src/transaction/itransaction_builder.cpp
    src/transaction/transaction_builders.cpp
    src/transaction/transaction_factory.cpp
    src/smartcontracts/sc_arguments.cpp
    src/smartcontracts/contract_call.cpp
    src/internal/biguint.cpp
    src/account/account.cpp
    src/account/address.cpp
    src/filehandler/ifile.cpp
    src/filehandler/pemreader.cpp
    src/filehandler/keyfilereader.cpp
    src/wrappers/jsonwrapper.h
    src/wrappers/httpwrapper.h
    src/wrappers/cryptosignwrapper.h src/wrappers/cryptosignwrapper.cpp
    src/provider/apiresponse.h
    src/provider/proxyprovider.cpp
    src/provider/data/data_transaction.cpp
    src/provider/data/networkconfig.cpp
	src/cliconfig.cpp
    src/Multifungible.cpp
    src/Wallet.cpp
    src/WalletProvider.cpp
    src/WrapperKeyFile.cpp
    src/WrapperProxyProvider.cpp
    src/WrapperTransactionFactory.cpp
    src/transaction/signer.cpp
)

# Utils files
set(MyDLL_UTILS
    src/utils/base64.h src/utils/base64.cpp
    src/utils/bits.h src/utils/bits.cpp
    src/utils/hex.h src/utils/hex.cpp
    src/utils/params.h
    src/utils/errors.h
    src/utils/common.h
    src/utils/cfg.h
)

# AddressGen C files
set(MyDLL_ADDRESSGEN
    AddressGen/AddressGenerator.h
    AddressGen/AddressGenerator.c
    AddressGen/ElrondAddressGenerator.h
    AddressGen/ElrondAddressGenerator.c
    AddressGen/util.h
    AddressGen/util.c
)

# AddressGen C files only windows
set(MyDLL_ADDRESSGEN_WINDOWS
    AddressGen/winglue.h
    AddressGen/winglue.c
)

# Test files
set(MyDLL_TEST_FILES
    mainMultifungible.cpp
    src/UnitTests.cpp
    include/UnitTesting/UnitTestsInfo.h
    #src/UnitTesting/AddQuantityTests.cpp
    #src/UnitTesting/AddRolesTests.cpp
    #src/UnitTesting/AddURIVerification.cpp
    #src/UnitTesting/buildProofOfOwnershipOfKeyPair.cpp
    #src/UnitTesting/BurnQuantityTests.cpp
    #src/UnitTesting/getAddressTokenBalanceTest.cpp
    #src/UnitTesting/getCollectionPropertiesTests.cpp
    #src/UnitTesting/getEmittedCollectionsTest.cpp
    #src/UnitTesting/getIssuerAddressTest.cpp
    #src/UnitTesting/getOwnedTokenPropertiesTests.cpp
    #src/UnitTesting/getOwnedTokensTests.cpp
    #src/UnitTesting/getRolesAndAddressesTests.cpp
    #src/UnitTesting/getTokenPropertiesTests.cpp
    #src/UnitTesting/IssueNFTCollectionTests.cpp
    #src/UnitTesting/IssueNFTTokenTests.cpp
    #src/UnitTesting/IssueSFTCollectionTests.cpp
    #src/UnitTesting/IssueSFTTokenTests.cpp
    #src/UnitTesting/stopCreationTests.cpp
    #src/UnitTesting/TokenDecoderTests.cpp
    #src/UnitTesting/TransferCreationRoles.cpp
    #src/UnitTesting/transferOwnershipTests.cpp
    #src/UnitTesting/TransferTokenRoles.cpp
    #src/UnitTesting/UpdatePropertiesTests.cpp
    #src/UnitTesting/WalletGeneratorTests.cpp
    src/UnitTesting/WipeFreezeUnfreeze.cpp
    #src/UnitTesting/PauseUnpauseTransactionsTests.cpp
    #src/UnitTesting/RemoveRolesTests.cpp
    #src/UnitTesting/UpgradeAttributesTests.cpp
    #src/UnitTesting/transferEGLDTests.cpp
    src/UnitTesting/transferESDTTests.cpp
    #src/UnitTesting/issueESDTTest.cpp
)

set(SDK_TEST_FILES
    #tests/test_integration/test_proxyprovider/test_proxyprovider.cpp
    #tests/test_integration/test_wrappers/test_wrapper_client.cpp
    tests/test_src/test_account/test_account.cpp
    #tests/test_src/test_filehandler/test_filehandler.cpp
    tests/test_src/test_canonical_path/test_canonical_path.cpp
    tests/test_src/test_internal/test_internal.cpp
    tests/test_src/test_provider/test_apiresponse.cpp
    tests/test_src/test_provider/test_data_transaction.cpp
    tests/test_src/test_smart_contracts/test_smart_contracts.cpp
    tests/test_src/test_transaction/test_esdt.cpp
    tests/test_src/test_transaction/test_gas_estimator.cpp
    tests/test_src/test_transaction/test_message_signer.cpp
    tests/test_src/test_transaction/test_payload_builder.cpp
    tests/test_src/test_transaction/test_signer.cpp
    tests/test_src/test_transaction/test_token_payment.cpp
    tests/test_src/test_transaction/test_transaction_factory.cpp
    tests/test_src/test_transaction/test_transaction.cpp
    tests/test_src/test_utils/test_utils.cpp
    tests/test_src/test_wrappers/test_wrappers.cpp
)

# External files
set(MyDLL_EXTERNAL
    external/json/json.hpp
    external/http/httplib.h
    external/bech32/bech32.h external/bech32/bech32.cpp
    external/cliparser/cxxopts.hpp
    external/toml/toml.h external/toml/cpptoml.h
    external/aes_128_ctr/aes.c
    external/keccak/sha3.c
    external/bigint/integer.h external/bigint/integer.cpp
)

# Build executable for google tests
if (BUILD_TESTS)
    include_directories(include/UnitTesting)
    include_directories(tests/test_common)
    include_directories(include/boost_1_81_0)
    if (WIN32)
        add_executable(Multifungible ${SDK_TEST_FILES} ${MyDLL_TEST_FILES} ${SOURCES_ROOT} ${MyDLL_SRC} ${MyDLL_UTILS} ${MyDLL_ADDRESSGEN} ${MyDLL_ADDRESSGEN_WINDOWS} ${MyDLL_EXTERNAL})
    else()
        add_executable(Multifungible ${SDK_TEST_FILES} ${MyDLL_TEST_FILES} ${SOURCES_ROOT} ${MyDLL_SRC} ${MyDLL_UTILS} ${MyDLL_ADDRESSGEN} ${MyDLL_EXTERNAL})
    endif()
# Build executable for CLI
elseif(BUILD_CLI)
    if (WIN32)
        add_executable(Multifungible main_cli.cpp ${SOURCES_ROOT} ${MyDLL_SRC} ${MyDLL_UTILS} ${MyDLL_ADDRESSGEN} ${MyDLL_ADDRESSGEN_WINDOWS} ${MyDLL_EXTERNAL})
    elseif(UNIX OR APPLE)
        add_executable(Multifungible main_cli.cpp ${SOURCES_ROOT} ${MyDLL_SRC} ${MyDLL_UTILS} ${MyDLL_ADDRESSGEN} ${MyDLL_EXTERNAL})
    endif()
else()
# Build the shared library (DLL)
    if (WIN32)
        add_library(Multifungible SHARED ${SOURCES_ROOT} ${MyDLL_SRC} ${MyDLL_UTILS} ${MyDLL_ADDRESSGEN} ${MyDLL_ADDRESSGEN_WINDOWS} ${MyDLL_EXTERNAL})
    elseif(UNIX OR APPLE)
        add_library(Multifungible SHARED ${SOURCES_ROOT} ${MyDLL_SRC} ${MyDLL_UTILS} ${MyDLL_ADDRESSGEN} ${MyDLL_EXTERNAL})
    endif()
endif()


# Find pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Multifungible Threads::Threads)

# Add header files directories
include_directories(include)
include_directories(external)
include_directories(src)
include_directories(src/utils)
include_directories(src/wrappers)
include_directories(AddressGen)
if (UNIX AND NOT APPLE)
    set(LIBSODIUM_INCLUDE_PATH "/usr/local/include")
    set(LIBSODIUM_LIB_PATH "/usr/local/lib")
    find_library(LIBSODIUM_LIBRARY libsodium.so.23.3.0 ${LIBSODIUM_LIB_PATH} REQUIRED)
    find_package(OpenSSL REQUIRED)
    if(OPENSSL_FOUND)
        set(HTTPLIB_IS_USING_OPENSSL TRUE)
    endif()
    target_link_libraries(Multifungible crypto)
    target_link_libraries(Multifungible ssl)
    target_link_libraries(Multifungible sodium)
    if (BUILD_TESTS)
        find_package(GTest REQUIRED)
        target_link_libraries(Multifungible GTest::gtest)
    endif()
elseif(WIN32)
    # Libsodium headers
    if (NOT DEFINED LIBSODIUM_INCLUDE_PATH)
        set(LIBSODIUM_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/sodium")
    endif()

    # Libsodium Libraries
    if (NOT DEFINED LIBSODIUM_LIB_PATH)
        set(LIBSODIUM_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/sodium")
    endif()

    # Openssl headers
    if (NOT DEFINED OPENSSL_INCLUDE_PATH)
        set(OPENSSL_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/openssl")
    endif()
    set(HTTPLIB_IS_USING_OPENSSL TRUE)
    include_directories(${OPENSSL_INCLUDE_PATH})

    # Openssl lib
    if (NOT DEFINED OPENSSL_LIB_PATH)
        set(OPENSSL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/openssl")
    endif()

    #Google tests headers and lib
    if (BUILD_TESTS)
        if (NOT DEFINED GOOGLETESTS_INCLUDE_PATH)
            set(GOOGLETESTS_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/")
        endif()
        if (NOT DEFINED GOOGLETESTS_LIB_PATH)
            set(GOOGLETESTS_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/googletest")
        endif()
        include_directories(${GOOGLETESTS_INCLUDE_PATH})
    endif ()

    # Include libcrypt32
    target_link_libraries(Multifungible crypt32)

    # Include libcrypto
    add_library(libcrypto STATIC IMPORTED)
    set_target_properties(libcrypto PROPERTIES IMPORTED_LOCATION "${OPENSSL_LIB_PATH}/libcrypto.lib")
    target_link_libraries(Multifungible libcrypto)

    # Include libssl
    add_library(libssl STATIC IMPORTED)
    set_target_properties(libssl PROPERTIES IMPORTED_LOCATION "${OPENSSL_LIB_PATH}/libssl.lib")
    target_link_libraries(Multifungible libssl)

    # Include libsodium
    add_library(libsodium STATIC IMPORTED)
    set_target_properties(libsodium PROPERTIES IMPORTED_LOCATION "${LIBSODIUM_LIB_PATH}/libsodium.a")
    target_link_libraries(Multifungible libsodium)

    # Include ws2_32
    target_link_libraries(Multifungible ws2_32)

    #if we are building the tests, add googletests
    if (BUILD_TESTS)
        add_library(libgtest STATIC IMPORTED)
        set_target_properties(libgtest PROPERTIES IMPORTED_LOCATION "${GOOGLETESTS_LIB_PATH}/libgtest.a")
        target_link_libraries(Multifungible libgtest)
    endif ()
elseif(APPLE) # macOS specific library settings
    # Libsodium headers
    if (NOT DEFINED LIBSODIUM_INCLUDE_PATH)
        find_path(LIBSODIUM_INCLUDE_PATH NAMES sodium.h PATHS /opt/homebrew/Cellar/libsodium/*/include NO_DEFAULT_PATH)
        if(NOT DEFINED LIBSODIUM_INCLUDE_PATH)
            message(FATAL_ERROR "sodium.h not found")
        endif()
    endif()

    # Set the paths for libsodium
    if (NOT DEFINED LIBSODIUM_LIB_PATH)
        find_path(LIBSODIUM_LIB_PATH NAMES libsodium.a PATHS /opt/homebrew/Cellar/libsodium/*/lib NO_DEFAULT_PATH)
        if(NOT DEFINED LIBSODIUM_LIB_PATH)
            message(FATAL_ERROR "libsodium.a not found")
        endif()
    endif()
    
    # Find the libsodium library
    add_library(libsodium STATIC IMPORTED)
    set_target_properties(libsodium PROPERTIES IMPORTED_LOCATION "${LIBSODIUM_LIB_PATH}/libsodium.a")
    target_link_libraries(Multifungible libsodium)

    # OpenSSL package
    find_package(OpenSSL REQUIRED)
    if(OPENSSL_FOUND)
        set(HTTPLIB_IS_USING_OPENSSL TRUE)
        include_directories(${OPENSSL_INCLUDE_DIR})
    endif()

    # Link libraries
    target_link_libraries(Multifungible ${OPENSSL_CRYPTO_LIBRARY})
    target_link_libraries(Multifungible ${OPENSSL_SSL_LIBRARY})
    target_link_libraries(Multifungible ${LIBSODIUM_LIBRARY})

    # Link Google Test for unit testing
    if (BUILD_TESTS)
        find_package(GTest REQUIRED)
        include_directories(${GTEST_INCLUDE_DIRS})
        target_link_libraries(Multifungible ${GTEST_BOTH_LIBRARIES})
    endif()
endif()
include_directories(${LIBSODIUM_INCLUDE_PATH})