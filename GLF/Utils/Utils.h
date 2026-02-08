#include <filesystem>
#include <fstream>
#include <iostream>

inline std::string ReadFileToString(const std::filesystem::path& filepath)
{
    std::ifstream file(filepath.string());
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file " << filepath << std::endl;
        return "";
    }

    std::ostringstream content_buffer;
    content_buffer << file.rdbuf();

    file.close();

    return content_buffer.str();
}
