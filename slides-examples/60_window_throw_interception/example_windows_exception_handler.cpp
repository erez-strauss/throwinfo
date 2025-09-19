
// Compile flags:  /std:c++latest /EHsc  /Zi /Od  main.cpp kernel32.lib
// https://compiler-explorer.com/z/Eqcjnb5YE
// https://compiler-explorer.com/z/srevj1rxe
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

// Alternative implementation using thread-local storage and manual capture
template<typename F> auto with_stack_trace_capture(F&& func) -> decltype(func())
{
    try
    {
        return func();
    }
    catch (...)
    {
        // last_throw_stacktrace = std::stacktrace::current();
        throw; // Re-throw the exception
    }
}

void risky_function()
{
    throw std::runtime_error("Something went wrong!");
}

void risky_function_with_wrapper()
{
    with_stack_trace_capture(
        []() { throw std::runtime_error("Something went wrong with wrapper!"); });
}

void risky_int_throw()
{
    throw 42;
}

int main()
{
    std::cout << "Testing exception stack traces on Windows with MSVC\n\n";

    // Test 1: Manual stack trace capture
    try
    {
        risky_function();
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Caught exception: " << e.what() << std::endl;
        std::print("Stacktrace:\n{}\n", last_throw_stacktrace);
    }

    // Test 2: Using wrapper function
    try
    {
        risky_function_with_wrapper();
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Caught wrapped exception: " << e.what() << std::endl;
        std::print("Stacktrace:\n{}\n", last_throw_stacktrace);
    }

    // Test 3: Integer exception
    try
    {
        risky_int_throw();
    }
    catch (int i)
    {
        std::cout << "Caught integer exception: " << i << std::endl;
        std::print("Stacktrace:\n{}\n", last_throw_stacktrace);
    }

    return 0;
}
