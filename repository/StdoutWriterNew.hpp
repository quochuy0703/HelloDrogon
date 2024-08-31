#pragma once
#include "IWriterNew.hpp"
#include <iostream>

namespace app_repositories
{
    class StdoutWriterNew : public IWriterNew
    {
    public:
        // Like "StdoutWriter() = default;" but also marks this constructor as the
        // one to use for injection.
        StdoutWriterNew();

        ~StdoutWriterNew();

        virtual void write(std::string s) override;
    };
}
