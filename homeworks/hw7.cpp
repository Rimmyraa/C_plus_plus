#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <ranges>
#include <unordered_set>
#include <algorithm>

namespace fs = std::filesystem;

struct FileInfo
{
    fs::path path;
    size_t size;
    std::string extension;
};

int main()
{
    std::string directory;

    std::cout << "Enter directory path: ";
    std::getline(std::cin, directory);

    std::vector<FileInfo> files;

    try
    {
        for (const auto& entry : fs::recursive_directory_iterator(directory))
        {
            if (entry.is_regular_file())
            {
                FileInfo info;

                info.path = entry.path();
                info.size = entry.file_size();
                info.extension = entry.path().extension().string();

                std::ranges::transform(
                    info.extension,
                    info.extension.begin(),
                    [](unsigned char c)
                    {
                        return std::tolower(c);
                    });

                files.push_back(std::move(info));
            }
        }
    }
    catch (fs::filesystem_error& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

    int choice;

    std::cout << "\nChoose filter:\n";
    std::cout << "1 - Text files (.txt)\n";
    std::cout << "2 - Images (.jpg, .jpeg, .png, .bmp)\n";
    std::cout << "3 - Large files (> 1 MB)\n";
    std::cout << "Your choice: ";
    std::cin >> choice;

    std::cout << "\nResult:\n";

    if (choice == 1)
    {
        auto isTextFile = [](const FileInfo& f)
        {
            return f.extension == ".txt";
        };

        auto textFiles = files | std::views::filter(isTextFile);

        for (const auto& f : textFiles)
        {
            std::cout << f.path << " (" << f.size << " bytes)\n";
        }
    }
    else if (choice == 2)
    {
        auto isImageFile = [](const FileInfo& f)
        {
            static const std::unordered_set<std::string> exts{
                ".jpg", ".jpeg", ".png", ".bmp"
            };

            return exts.contains(f.extension);
        };

        auto imageFiles = files | std::views::filter(isImageFile);

        for (const auto& f : imageFiles)
        {
            std::cout << f.path << " (" << f.size << " bytes)\n";
        }
    }
    else if (choice == 3)
    {
        constexpr size_t MB = 1024 * 1024;

        auto largeFiles = files | std::views::filter(
            [](const FileInfo& f)
            {
                return f.size > MB;
            });

        for (const auto& f : largeFiles)
        {
            std::cout << f.path << " (" << f.size << " bytes)\n";
        }
    }
    else
    {
        std::cout << "Wrong choice\n";
    }

    return 0;
}

