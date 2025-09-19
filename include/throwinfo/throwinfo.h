#include <cxxabi.h>
#include <dlfcn.h>
#include <stacktrace>
#include <typeinfo>
#include <unistd.h>

#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <print>
#include <stdexcept>
#include <string>
#include <thread>

namespace ext
{

struct context
{
    std::string msg_;
    std::stacktrace stacktrace_;
    std::chrono::time_point<std::chrono::system_clock> timestamp_;
    std::thread::id thread_id_;
    pid_t tid_;

    context() noexcept
    {
    }
    context(const std::string& msg, int skip = 1)
        : msg_(msg), stacktrace_(std::stacktrace::current(skip)),
          timestamp_(std::chrono::system_clock::now()), thread_id_(std::this_thread::get_id()),
          tid_(gettid())
    {
    }
    static context current(std::string msg = "", int skip = 1)
    {
        return context(msg, skip);
    }
};

struct throwinfo
{
    context context_{};
    int throw_count_{0};
    const std::type_info* type_info_{nullptr};
    const void* ep_;

    void capture(void* exceptionp, std::type_info* type_info)
    {
        throw_count_ = ++active_throw_count;
        context_ = context::current("", 2);
        type_info_ = type_info;
        ep_ = exceptionp;
    }
    void report(void* ep) const
    {
        std::string type_name{};

        if (type_info_)
            type_name = type_info_->name();
        if (ep != ep_)
            type_name += " diff-pointers";
        std::println("throwinfo: throw_count: {} exception type: {}", throw_count_, type_name);
        std::println("context:\n{}", context_.stacktrace_);
    }
    std::string to_string();

    inline static thread_local uint64_t active_throw_count{0};
};

inline static thread_local throwinfo last_throwinfo{};

} // namespace ext

template<> struct std::formatter<ext::context>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const ext::context& di, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "context(msg: {}, @{}, {}) stacktrace:\n{}", di.msg_,
                              di.timestamp_, di.tid_, di.stacktrace_);
    }
};

template<> struct std::formatter<ext::throwinfo>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const ext::throwinfo& ti, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "throwinfo: throw_count {} {}", ti.throw_count_,
                              ti.context_);
    }
};
