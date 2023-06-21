#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

bool CheckFileExtension(const fs::path& filePath, const std::string& desiredExtension);
void ListFilesAndDirectories(const fs::path& directoryPath, std::vector<std::string>& all_source_files, std::vector<std::string>& all_directories);

class Loader {
    private:
        std::vector<std::string> all_source_files;
        std::vector<std::string> all_directories;
    public:
        Loader(std::string rootPath) {
            if (fs::exists(rootPath) && fs::is_directory(rootPath)) {
                all_directories.push_back(rootPath);
                ListFilesAndDirectories(rootPath, all_source_files, all_directories);
            } else {
                std::cout << "Invalid directory path." << std::endl;
            }
        };
        std::vector<std::string> get_all_source_files(){return all_source_files;};
        std::vector<std::string> get_all_directories(){return all_directories;};
};
