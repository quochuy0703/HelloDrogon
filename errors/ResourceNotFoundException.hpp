#include <iostream>
#include <string>

class ResourceNotFoundException : public std::exception
{
private:
    /* data */
    const char *message_;

public:
    ResourceNotFoundException(const char *message);
    const char *what();
};