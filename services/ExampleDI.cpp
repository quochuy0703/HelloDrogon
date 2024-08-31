#pragma once
#include <boost/di.hpp>
#include <iostream>

namespace app_services
{
    class Writer
    {
    public:
        virtual void write(std::string s) = 0;
    };

    class StdoutWriter : public Writer
    {
    public:
        // Like "StdoutWriter() = default;" but also marks this constructor as the
        // one to use for injection.
        StdoutWriter()
        {
            std::cout << "Init StdoutWriter" << std::endl;
        };

        ~StdoutWriter()
        {
            std::cout << "Destruct StdoutWriter" << std::endl;
        };

        virtual void write(std::string s) override
        {
            std::cout << s;
        }
    };

    class Greeter
    {
    public:
        virtual void greet() = 0;
    };

    class GreeterImpl : public Greeter
    {
    private:
        std::shared_ptr<Writer> writer;

    public:
        // Like "GreeterImpl(Writer* writer) {...}" but also marks this constructor
        // as the one to use for injection.
        GreeterImpl(std::shared_ptr<Writer> writer) : writer(writer)
        {
            std::cout << "Init GreeterImpl" << std::endl;
        }

        ~GreeterImpl()
        {
            std::cout << "Destruct GreeterImpl" << std::endl;
        };

        virtual void greet() override
        {
            writer->write("Hello world!\n");
        }
    };
}
