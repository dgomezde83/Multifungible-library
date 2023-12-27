#include "utils/common.h"
#include <gtest/gtest.h>

TEST(GetCanonicalRootPathTest, SimpleFilename) {
    std::string filename = "myfile.txt";
    std::filesystem::path expectedPath = "/Users/danielgomez/Documents/Multifungible-library/bin/myfile.txt";
    
    ASSERT_EQ(util::getCanonicalRootPath(filename), expectedPath);
}

TEST(GetCanonicalRootPathTest, RelativePath) {
    std::string relativePath = "subdir/myfile.txt";
    std::filesystem::path expectedPath = "/Users/danielgomez/Documents/Multifungible-library/bin/subdir/myfile.txt";

    ASSERT_EQ(util::getCanonicalRootPath(relativePath), expectedPath);
}
TEST(GetCanonicalRootPathTest, RelativePath2) {
    std::string relativePath = "../subdir/myfile.txt";
    std::filesystem::path expectedPath = "/Users/danielgomez/Documents/Multifungible-library/subdir/myfile.txt";

    ASSERT_EQ(util::getCanonicalRootPath(relativePath), expectedPath);
}
TEST(GetCanonicalRootPathTest, RelativePath3) {
    std::string relativePath = "../../subdir/myfile.txt";
    std::filesystem::path expectedPath = "/Users/danielgomez/Documents/subdir/myfile.txt";

    ASSERT_EQ(util::getCanonicalRootPath(relativePath), expectedPath);
}
TEST(GetCanonicalRootPathTest, AbsolutePath) {
    std::string absolutePath = "/absolute/path/to/myfile.txt";
    
    ASSERT_EQ(util::getCanonicalRootPath(absolutePath), absolutePath);
}
TEST(GetCanonicalRootPathTest, PathWithCommonDirectories) {
    std::string complexPath = "Multifungible-library/tests/myfile.txt";
    std::filesystem::path expectedPath = "/Users/danielgomez/Documents/Multifungible-library/tests/myfile.txt";

    ASSERT_EQ(util::getCanonicalRootPath(complexPath), expectedPath);
}
TEST(GetCanonicalRootPathTest, PathWithCommonDirectories2) {
    std::string complexPath = "bin/tests/myfile.txt";
    std::filesystem::path expectedPath = "/Users/danielgomez/Documents/Multifungible-library/bin/tests/myfile.txt";

    ASSERT_EQ(util::getCanonicalRootPath(complexPath), expectedPath);
}
TEST(GetCanonicalRootPathTest, PathWithCommonDirectories3) {
    std::string complexPath = "Documents/Multifungible-library/tests/myfile.txt";
    std::filesystem::path expectedPath = "/Users/danielgomez/Documents/Multifungible-library/tests/myfile.txt";

    ASSERT_EQ(util::getCanonicalRootPath(complexPath), expectedPath);
}
TEST(GetCanonicalRootPathTest, PathWithCommonDirectories4) {
    std::string complexPath = "Documents/Myotherlibrary/tests/myfile.txt";
    std::filesystem::path expectedPath = "/Users/danielgomez/Documents/Myotherlibrary/tests/myfile.txt";

    ASSERT_EQ(util::getCanonicalRootPath(complexPath), expectedPath);
}
TEST(GetCanonicalRootPathTest, NonExistentFile) {
    std::string nonExistentFile = "nonexistentfile.txt";
    std::filesystem::path expectedPath = "/Users/danielgomez/Documents/Multifungible-library/bin/nonexistentfile.txt";

    ASSERT_EQ(util::getCanonicalRootPath(nonExistentFile), expectedPath);
}
