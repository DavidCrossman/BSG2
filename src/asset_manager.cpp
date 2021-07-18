#include "asset_manager.h"

#include <sys/stat.h>

#include <iostream>

std::string ASSET_DIR_PREFIX;

// Returns 1 if if the directory exists.
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

void initialise_asset_managers() {
    ASSET_DIR_PREFIX = "assets/";
    while (true) {
        // Check if the assets directory exists. If not, go up a directory and check again.
        int result = dir_exists(ASSET_DIR_PREFIX.c_str());
        switch (result) {
            case 1:
                // Directory exists.
                return;
            case 0:
                // The directory does not exist. Back up a directory.
                ASSET_DIR_PREFIX = "../" + ASSET_DIR_PREFIX;
                break;
            case -1:
                // Error. Exit the program, no asset directory was found.
                std::cerr << "Could not find the assets directory. Exiting." << std::endl;
                std::exit(-1);
        }
    }
}

const std::string& get_asset_dir() { return ASSET_DIR_PREFIX; }
