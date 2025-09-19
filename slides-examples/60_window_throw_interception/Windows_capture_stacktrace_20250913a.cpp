// https://compiler-explorer.com/z/3q1rPG85G
// Compile flags:  /std:c++latest /EHsc  /Zi /Od  main.cpp kernel32.lib
#include <exception>
#include <iostream>
#include <mutex>
#include <print>
#include <stacktrace>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <windows.h>

static thread_local std::stacktrace last_throw_stacktrace;
static std::mutex hook_mutex;
static bool hook_installed = false;

// Windows exception hook using SetUnhandledExceptionFilter and vectored
// exception handling
LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo)
{
    // Check if this is a C++ exception
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == 0xE06D7363) // MS C++ exception code
    {
        // Capture the stack trace at the point of exception
        last_throw_stacktrace = std::stacktrace::current();
    }
    // Continue with normal exception handling
    return EXCEPTION_CONTINUE_SEARCH;
}

// Alternative approach using structured exception handling
class StackTraceCapture
{
public:
    StackTraceCapture()
    {
        std::lock_guard<std::mutex> lock(hook_mutex);
        if (!hook_installed)
        {
            // Install vectored exception handler
            AddVectoredExceptionHandler(1, VectoredExceptionHandler);
            hook_installed = true;
        }
    }

    ~StackTraceCapture()
    {
        // Note: In a real application, you might want to remove the handler
        // RemoveVectoredExceptionHandler(handler);
    }
};

// Global instance to install the handler
static StackTraceCapture stack_trace_capture;

void code_throws_int()
{
    throw 43;
}
void code_throws_runtime_error()
{
    throw std::runtime_error("Wow Error, again?");
}

int main()
{
    std::println("Testing exception stack traces on Windows with MSVC\n");

    std::set_terminate([]() {
        std::println(std::cerr, "Unhandled exception\nStacktrace:\n{}\n", last_throw_stacktrace);
        std::abort();
    });

    try
    {
        code_throws_runtime_error();
    }
    catch (const std::runtime_error& e)
    {
        std::println("Caught exception: {}", e.what());
        std::println("Stacktrace:\n{}\n", last_throw_stacktrace);
    }

    try
    {
        // code_throws_int();
        code_throws_runtime_error();
    }
    catch (int i)
    {
        std::println("Caught integer exception: {}", i);
        std::print("Stacktrace:\n{}\n", last_throw_stacktrace);
    }
    catch (...)
    {
        std::println("Caught ... exception from");
        std::print("Stacktrace:\n{}\n", last_throw_stacktrace);
    }

    throw 3.14;

    return 0;
}
