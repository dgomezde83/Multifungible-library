#ifndef ERD_COMMON_H
#define ERD_COMMON_H

#include "errors.h"
#include "stdexcept"
#include <filesystem>
#include <iostream>
#ifdef __WINDOWS__
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <limits.h> // for PATH_MAX
#include <stdlib.h> // for realpath
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

#if defined(__APPLE__)
//To get the path of the current executable on Apple (macOS) systems.
inline std::filesystem::path get_executable_path() {
    char pathbuf[PATH_MAX + 1];
    uint32_t bufsize = PATH_MAX;
    if (_NSGetExecutablePath(pathbuf, &bufsize) == 0) {
        char real_path[PATH_MAX];
        if (realpath(pathbuf, real_path) != nullptr) {
            return std::filesystem::path(real_path);
        }
    }
    return std::filesystem::path();
}
#endif

// Split a filesystem path into its individual components (directories, subdirectories, filename).
inline std::vector<std::filesystem::path> splitPath(const std::filesystem::path& path) {
    std::vector<std::filesystem::path> components;
    for (const auto& part : path) {
        components.push_back(part);
    }
    return components;
}

// To merge the executable path with a provided path, replacing part of the executable path with the provided path when they share common directories.
inline std::filesystem::path mergePaths(const std::filesystem::path& executablePath, const std::filesystem::path& providedPath) {
    std::vector<std::filesystem::path> execPathComponents = splitPath(executablePath);
    std::vector<std::filesystem::path> providedPathComponents = splitPath(providedPath);

    // Find the point where providedPath can replace a part of executablePath
    size_t replacementIndex = execPathComponents.size();
    bool t_found = false;
    for (size_t i = 0; i < providedPathComponents.size(); ++i) {
        if (t_found)
            {break;}
        for (std::vector<std::filesystem::path>::iterator it = execPathComponents.begin(); it != execPathComponents.end(); it++)
        {
            if (*it == providedPathComponents[i])
            {
                replacementIndex = std::distance(execPathComponents.begin(), it);
                t_found = true;
                break;
            }
        }
    }

    // Construct the resulting path
    std::filesystem::path result;
    for (size_t i = 0; i < replacementIndex; ++i) {
        result /= execPathComponents[i];
    }
    for (size_t i = 0; i < providedPathComponents.size(); ++i) {
        result /= providedPathComponents[i];
    }

    return result.lexically_normal(); // Normalize the path
}

inline std::string getCanonicalRootPath(std::string const &path)
{
    std::filesystem::path executable_path;

    // Retrieve the path of the current executable based on the OS
    #if defined(__UNIX__)
    executable_path = std::filesystem::canonical("/proc/self/exe");
    #elif defined(__APPLE__)
    executable_path = get_executable_path();
    #elif defined(__WINDOWS__)
    char buf[MAX_PATH];
    GetModuleFileName(NULL, buf, MAX_PATH);
    executable_path = std::filesystem::path(buf);
    #else
    return path; // If OS is not recognized, return the original path
    #endif
    // Remove the executable name from the canonical root path of the executable
    executable_path = executable_path.parent_path();
    //rest of the code, to be done
    // Merge the executable path with the provided path
    return mergePaths(executable_path, path);
}

}

#endif
