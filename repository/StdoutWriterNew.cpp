#include "StdoutWriterNew.hpp"

namespace app_repositories
{

    StdoutWriterNew::StdoutWriterNew()
    {
        std::cout << "Init StdoutWriter" << std::endl;
    };

    StdoutWriterNew::~StdoutWriterNew()
    {
        std::cout << "Destruct StdoutWriter" << std::endl;
    };

    void StdoutWriterNew::write(std::string s)
    {
        std::cout << s;
    }
}
