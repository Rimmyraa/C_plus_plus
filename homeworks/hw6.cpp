#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {

    std::string path;

    std::cout << "Enter directory path: ";
    std::getline(std::cin, path);

    size_t txtCount = 0;
    size_t imgCount = 0;
    size_t exeCount = 0;
    size_t otherCount = 0;

    uintmax_t txtSize = 0;
    uintmax_t imgSize = 0;
    uintmax_t exeSize = 0;
    uintmax_t otherSize = 0;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(path)) {

            if (entry.is_regular_file()) {

                std::string ext = entry.path().extension().string();
                uintmax_t size = entry.file_size();

                if (ext == ".txt") {
                    txtCount++;
                    txtSize += size;
                }
                else if (ext == ".jpg" || ext == ".png" || ext == ".bmp") {
                    imgCount++;
                    imgSize += size;
                }
                else if (ext == ".exe") {
                    exeCount++;
                    exeSize += size;
                }
                else {
                    otherCount++;
                    otherSize += size;
                }
            }
        }

        std::cout << "\n=== Result ===\n";
        std::cout << "Text files (.txt): " << txtCount
                  << ", total size: " << txtSize << " bytes\n";

        std::cout << "Images (.jpg, .png, .bmp): " << imgCount
                  << ", total size: " << imgSize << " bytes\n";

        std::cout << "Executables (.exe): " << exeCount
                  << ", total size: " << exeSize << " bytes\n";

        std::cout << "Other files: " << otherCount
                  << ", total size: " << otherSize << " bytes\n";

    }
    catch (fs::filesystem_error& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

