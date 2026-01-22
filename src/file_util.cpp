#include "file_util.h"

#include <sys/stat.h>

namespace bsg2 {
// Returns 1 if the directory exists.
// Returns 0 if the directory does not exist, or it exists but it is a file not a directory.
// Returns -1 if an error occured.
int dir_exists(const char* const path) {
    struct stat info;
    int statRC = stat(path, &info);
    if (statRC != 0) {
        if (errno == ENOENT) {
            // something along the path does not exist
            return 0;
        }
        if (errno == ENOTDIR) {
            // something in path prefix is not a dir
            return 0;
        }
        return -1;
    }

    return (info.st_mode & S_IFDIR) ? 1 : 0;
}

std::string find_full_dir(std::string dir) {
    static const unsigned int MAX_DIR_LEN = 400;
    while (true) {
        // Check if the assets directory exists. If not, go up a directory and check again.
        int result = dir_exists(dir.c_str());
        if (dir.size() > MAX_DIR_LEN) result = -1;
        switch (result) {
        case 1:
            // Directory exists.
            return dir;
        case 0:
            // The directory does not exist. Back up a directory.
            dir = "../" + dir;
            break;
        case -1:
            // No asset directory was found, return empty string.
            return "";
        }
    }
}
}