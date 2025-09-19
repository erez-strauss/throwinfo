// Compile: g++ -std=c++23 -O2 -W -o simple-stacktrace-iteration_v2
// simple-stacktrace-iteration_v2.cpp -lstdc++exp
#include <iostream>
#include <print>
#include <stacktrace>
#include <string_view>

void print_current_stacktrace(std::stacktrace trace = std::stacktrace::current())
{
    std::print("=== Stack Trace ({} entries) ===\n", trace.size());
    std::string spaces_(4 * trace.size(), ' ');
    std::string_view spaces(spaces_);

    for (std::size_t i = trace.size() - 1; i < trace.size(); --i)
    {
        const auto& entry = trace[i];
        if (entry)
        {
            auto s = spaces.substr(0, 2 * (trace.size() - 1 - i));

            std::println("{}Frame {}:", s, i);
            std::println("{}Address: {:#x}", s, entry.native_handle());
            std::println("{}Description: {}", s, entry.description());

            if (!entry.source_file().empty())
            {
                std::println("{}Location: {}:{}", s, entry.source_file(), entry.source_line());
            }
        }
        else
        {
            std::println("Empty stack Entry");
        }

        std::println("");
    }

    std::print("=== End Stack Trace ===\n");
}

// Example usage with nested function calls to show stack depth

void function_c()
{
    std::print("In function_c, printing stacktrace:\n");
    print_current_stacktrace();
}

void function_b()
{
    std::print("In function_b, calling function_c\n");
    function_c();
}

void function_a()
{
    std::print("In function_a, calling function_b\n");
    function_b();
}

int main()
{
    std::println("Starting main, calling function_a");
    function_a();
}
