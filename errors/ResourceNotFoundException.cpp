#include "ResourceNotFoundException.hpp"

ResourceNotFoundException::ResourceNotFoundException(char *message)
{
    this->message_ = message;
}

const char *ResourceNotFoundException::what()
{
    return this->message_;
}