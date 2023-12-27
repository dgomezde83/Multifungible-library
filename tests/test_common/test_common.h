#ifndef ERDCPP_TEST_COMMON_H
#define ERDCPP_TEST_COMMON_H

#include "utils/common.h"

std::string getCanonicalTestDataPath(std::string const &file)
{
    return util::getCanonicalRootPath("Multifungible-library/tests/testData/" + file);
}

#endif //ERDCPP_TEST_COMMON_H
