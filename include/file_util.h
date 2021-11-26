#pragma once

#include <string>

namespace bsg2 {
int dir_exists(const char* const path);
std::string find_full_dir(std::string dir);
}