#include <iostream>
#include <string>

class ResourceNotFoundException : public std::exception
{
private:
    /* data */
    char *message_;

public:
    ResourceNotFoundException(char *message);
    const char *what();
};
