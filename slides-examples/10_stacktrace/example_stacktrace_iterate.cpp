// Compile: g++ -g -std=c++23 -O2 -W -o simple-stacktrace-iteration
// simple-stacktrace-iteration.cpp -lstdc++exp
// https://compiler-explorer.com/z/Kqb9sWK4E
#include <iostream>
#include <print>
#include <stacktrace>
#include <string_view>

void print_current_stacktrace(unsigned indent = 2,
                              std::stacktrace trace = std::stacktrace::current())
{
    std::print("=== Stack Trace ({} entries) ===\n", trace.size());
    std::string spaces_((indent + 1) * trace.size(), ' ');
    std::string_view spaces(spaces_);

    for (std::size_t i = trace.size() - 1; i < trace.size(); --i)
    {
        const auto& entry = trace[i];
        auto s = spaces.substr(0, indent * (trace.size() - 1 - i));

        std::println("{}Frame {}:", s, i);
        std::println("{}Address: {:#x}", s, entry.native_handle());
        std::println("{}Description: {}", s, entry.description());

        if (!entry.source_file().empty())
        {
            std::println("{}Location: {}:{}", s, entry.source_file(), entry.source_line());
        }

        std::println("");
    }

    std::print("=== End Stack Trace ===\n");
}

// Example usage with nested function calls to show stack depth

void function_c()
{
    std::println("In function_c, printing stacktrace:");
    print_current_stacktrace(1);
}

void function_b()
{
    std::println("In function_b, calling function_c");
    function_c();
}

void function_a()
{
    std::println("In function_a, calling function_b");
    function_b();
}

int main()
{
    std::println("Starting main, calling function_a");
    function_a();
}
