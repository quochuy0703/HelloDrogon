#include "ResourceNotFoundException.hpp"

ResourceNotFoundException::ResourceNotFoundException(const char *message)
{
    this->message_ = message;
}

const char *ResourceNotFoundException::what()
{
    return this->message_;
}