// https://compiler-explorer.com/z/Y7zrq9EPj
#include <chrono>
#include <dlfcn.h>
#include <exception>
#include <format>
#include <print>
#include <stacktrace>
#include <thread>
#include <typeinfo>

namespace ext
{
struct call_context
{
    std::string msg_;
    std::stacktrace stacktrace_;
    std::chrono::time_point<std::chrono::system_clock> timestamp_;
    std::thread::id tid_;

    call_context() noexcept
    {
    }

    call_context(const std::string& msg)
        : msg_(msg), stacktrace_(std::stacktrace::current()),
          timestamp_(std::chrono::system_clock::now()), tid_(std::this_thread::get_id())
    {
    }
    static call_context current(std::string msg = "")
    {
        return call_context(msg);
    }
};

inline thread_local call_context last_throw_context{};

} // namespace ext
template<> struct std::formatter<ext::call_context>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const ext::call_context& di, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "call_context(msg: {}, @{}, {}) stacktrace:\n{}", di.msg_,
                              di.timestamp_, di.tid_, di.stacktrace_);
    }
};

#ifdef __clang__
using __cxa_throw_type = void (*)(void*, std::type_info*, void (*)(void*));
#else
using __cxa_throw_type = void (*)(void*, void*, void (*)(void*));
#endif
static __cxa_throw_type original_cxa_throw{(__cxa_throw_type)dlsym(RTLD_NEXT, "__cxa_throw")};

#ifdef __clang__
extern "C" void __cxa_throw(void* thrown_exception, std::type_info* tinfo, void (*dest)(void*))
{
#else
extern "C" void __cxa_throw(void* thrown_exception, void* vp, void (*dest)(void*))
{
    std::type_info* tinfo = (std::type_info*)vp;
#endif
    ext::last_throw_context = ext::call_context::current("exception");

    original_cxa_throw(thrown_exception, tinfo, dest);
    abort();
}

int main()
{
    try
    {
        throw 123;
    }
    catch (...)
    {
        std::println("there was an exception context:\n{}", ext::last_throw_context);
    }
}
