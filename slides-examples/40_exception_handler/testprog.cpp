// clang++-20 -W -Wall -std=c++23 -g -o capture_on_throw_process_on_catch
// capture_on_throw_process_on_catch.cpp  -lstdc++exp
// ../src/examples/capture_on_throw_process_on_catch.cpp

#include <cxxabi.h>
#include <dlfcn.h>
#include <stacktrace>
#include <typeinfo>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <print>
#include <stdexcept>
#include <string>

void risky_function()
{
    throw 3.1415;
    throw std::runtime_error("Something went wrong!");
}

int main()
{
    std::set_terminate([]() {
        std::cout << "Unhandled exception\n"; // << last_throw_info.to_string() <<
                                              // std::flush;
        exit(-1);                             // std::abort();
    });

    try
    {
        risky_function();
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Caught exception: " << e.what() << std::endl;
        // std::print("Stacktrace:\n{}", last_throw_stacktrace);
        // std::print("Stacktrace: {}ns\n{}", stackcapture_ns,
        // last_throw_stacktrace);
    }

    // You can also throw a different type of exception to see the interception
    try
    {
        std::string s{"abd"};
        s.at(5)++;
        throw (double)42.0; // Throw an integer
    }
    catch (int i)
    {
        std::cout << "Caught integer exception: " << i << std::endl;
        // std::print("Stacktrace: {}ns\n{}", stackcapture_ns,
        // last_throw_stacktrace);
    }

    throw 4.5; // Throw an integer
    return 0;
}
