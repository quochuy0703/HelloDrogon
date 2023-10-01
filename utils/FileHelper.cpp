#include "FileHelper.hpp"
#include "../errors/ResourceNotFoundException.hpp"

void app_helpers::file_helper::removeFile(const std::string &fileName)
{
    try
    {
        if (!std::filesystem::remove(fileName))
            throw ResourceNotFoundException("File not found!");
    }
    catch (std::filesystem::filesystem_error &ex)
    {
        std::cout << "Error FileHelper: " << ex.what() << std::endl;
        throw std::runtime_error("Error FileHelper: " + std::string(ex.what()));
    }
    catch (ResourceNotFoundException &ex)
    {
        std::cout << "Error FileHelper: " << ex.what() << std::endl;
        throw std::runtime_error("Error FileHelper: " + std::string(ex.what()));
    }
}