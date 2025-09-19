#include <chrono>
#include <exception>
#include <format>
#include <functional>
#include <iostream>
#include <print>
#include <source_location>
#include <stacktrace>
#include <thread>
#include <utility>
#include <vector>

namespace ext
{
struct call_context
{
    std::string msg_;
    std::source_location loc_;
    std::stacktrace st_;
    std::chrono::time_point<std::chrono::system_clock> timestamp_;
    std::thread::id tid_;
    // unsigned throw_count;

    call_context() noexcept
    {
    }

    call_context(const std::string& msg, std::source_location loc = std::source_location::current())
        : msg_(msg), loc_(loc), st_(std::stacktrace::current()),
          timestamp_(std::chrono::system_clock::now()), tid_(std::this_thread::get_id())
    {
    }
};
} // namespace ext
template<> struct std::formatter<ext::call_context>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const ext::call_context& di, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(),
                              "call_context(msg: {}, loc: {}:{} {}, @{}, {}) stacktrace:\n{}",
                              di.msg_, di.loc_.file_name(), di.loc_.line(), di.loc_.function_name(),
                              di.timestamp_, di.tid_, di.st_);
    }
};

struct enriched_exception : public std::exception, public ext::call_context
{

    enriched_exception(auto& m) : std::exception(), ext::call_context(m)
    {
    }
    std::string ewhat() const
    {
        return std::format("{}", *static_cast<const ext::call_context*>(this));
    }
};

// using enriched_exception = yystd::expected<int, ext::call_context>;

int func_check(bool b = true)
{
    if (b)
        return 17;
    throw enriched_exception("error");
}

auto deep_n_info()
{
    return ext::call_context("hello call sequence");
}

auto f3()
{
    return deep_n_info();
}
auto f2a()
{
    return f3();
}
auto f2b()
{
    return f3();
}

auto testfunc(bool b = false)
{
    if (b)
        return f2a();
    return f2b();
}

int main()
{
    try
    {
        auto a = testfunc(true);
        auto b = testfunc();
        auto z = testfunc(false);

        auto c = func_check(false);
    }
    catch (enriched_exception& ee)
    {
        std::println("enriched exception: {}\n{}", ee.ewhat(), ee.what());
    }
    catch (std::exception& e)
    {
        std::println("exception: {}", e.what());
    }

    //
    //    std::println("a: {}", a);
    //    std::println("b: {}", b);

    //    if (c.has_value())
    //        std::print("c value: {}", c.value());
    //    else
    //        std::print("c error: {}", c.error());
}
