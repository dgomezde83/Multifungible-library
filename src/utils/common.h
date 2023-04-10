#ifndef ERD_COMMON_H
#define ERD_COMMON_H

#include "errors.h"
#include "stdexcept"
#include <filesystem>
#include <iostream>
#ifdef __WINDOWS__
#include <windows.h>
#endif

namespace util
{
template <typename T>
inline void checkParam(T const &param, T const &expectedParam, errorMessage const &error)
{
    if (param != expectedParam)
    {
        throw std::invalid_argument
                (error + "Expected: " + std::to_string(expectedParam) +", got: " + std::to_string(param));
    }
}

template <>
inline void checkParam<std::string>(std::string const &param, std::string const &expectedParam, errorMessage const &error)
{
    if (param != expectedParam)
    {
        throw std::invalid_argument
                (error + "Expected: " + expectedParam +", got: " + param);
    }
}


inline std::string getCanonicalRootPath(std::string const &path)
{
    // get the path to the current executable
    #ifdef __UNIX__
    std::filesystem::path executable_path = std::filesystem::canonical(std::filesystem::path("/proc/self/exe"));
    #elif __WINDOWS__
    char buf[MAX_PATH];
    GetModuleFileName(NULL, buf, MAX_PATH);
    std::filesystem::path executable_path(buf);
    #else
    return path;
    #endif
    std::cout << executable_path.string() << std::endl;
    // compute the path to the root
    std::filesystem::path root_path;
    for (auto& p : std::filesystem::recursive_directory_iterator(executable_path.parent_path())) {
        if (p.path().root_path() == p.path())
            root_path = p.path();
    }
    std::cout << root_path.string() << std::endl;
    // append the path to another file
    std::filesystem::path resulting_path = root_path / path;
    return resulting_path.string();
}

}

#endif
