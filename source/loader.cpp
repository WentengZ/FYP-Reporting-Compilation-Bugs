#ifndef INCLUDE_LOADER
#define INCLUDE_LOADER
#include "../include/loader.h"
#endif

void ListFilesAndDirectories(const fs::path& directoryPath, std::vector<std::string>& all_source_files, std::vector<std::string>& all_directories) {
    for (const auto& entry : fs::directory_iterator(directoryPath)){
        const fs::path& path = entry.path();
        if (fs::is_directory(path)) {
            all_directories.push_back(path);
            ListFilesAndDirectories(path, all_source_files, all_directories);
        } else if (fs::is_regular_file(path)) {
            if (CheckFileExtension(path, ".c") || CheckFileExtension(path, ".cpp") || CheckFileExtension(path, ".cc")){
                all_source_files.push_back(path);
            }
        }
    }
}

bool CheckFileExtension(const fs::path& filePath, const std::string& desiredExtension) {
    std::string extension = filePath.extension().string();

    // Convert the desired extension to lowercase for case-insensitive comparison
    std::string lowercaseDesiredExtension = desiredExtension;
    return (extension == lowercaseDesiredExtension);
}