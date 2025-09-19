#include <print>
#include <source_location>
#include <string>

template<> struct std::formatter<std::source_location> : std::formatter<std::string>
{
    template<typename FormatContext>
    auto format(const std::source_location& loc, FormatContext& ctx) const
    {
        std::string s = std::format("{}:{}:{} {}", loc.file_name(), loc.line(), loc.column(),
                                    loc.function_name());
        return std::formatter<std::string>::format(s, ctx);
    }
};

void report(const auto&& v, std::source_location sloc = std::source_location::current())
{
    std::println("report: {} from: {}", v, sloc);
}

void foo()
{
    std::source_location sloc{};
    std::println("foo: Source location: {}", sloc);
}

void bar()
{
    std::source_location sloc{std::source_location::current()};
    std::println("bar: Source location: {}", sloc);
}

int main()
{
    using namespace std::literals;
    foo();
    bar();
    report(4);
    report(5.6);
    report("this is a simple message, right"s);

    return 0;
}
