#include <cxxabi.h>
#include <dlfcn.h>
#include <typeinfo>

#include <iostream>
#include <string>

#if defined(ON_THROW_OLD_VOID_API) || !defined(__clang__)
using cxa_throw_type = void (*)(void*, void*, void (*)(void*));
extern "C" void __cxa_throw(void*, void*, void (*)(void*));
#else
using cxa_throw_type = void (*)(void*, std::type_info*, void (*)(void*));
extern "C" void __cxa_throw(void*, std::type_info*, void (*)(void*));
#endif

static cxa_throw_type original_cxa_throw{(cxa_throw_type)dlsym(RTLD_NEXT, "__cxa_throw")};

#if defined(ON_THROW_OLD_VOID_API) || !defined(__clang__)
extern "C" void __cxa_throw(void* exception_p, void* vp, void (*dest)(void*))
{
    std::type_info* tinfo = reinterpret_cast<std::type_info*>(vp);
#else
extern "C" void __cxa_throw(void* exception_p, std::type_info* tinfo, void (*dest)(void*))
{
#endif
    // Custom exception handling logic here
    std::cerr << "Intercepted __cxa_throw!" << std::endl;
    std::cerr << "Exception type: " << tinfo->name() << std::endl;

    // You can also attempt to demangle the type name for better readability
    int status;
    char* demangled_name = abi::__cxa_demangle(tinfo->name(), nullptr, nullptr, &status);
    if (status == 0)
    {
        std::cerr << "Demangled type: " << demangled_name << std::endl;
        free(demangled_name);
    }

    // Call the original __cxa_throw to continue the exception handling process
    // (It's crucial to call the original or handle the exception completely
    // to avoid unexpected program termination).
    std::cerr << "Calling original __cxa_throw..." << std::endl;
    // This is where you would call the "real" or original __cxa_throw
    // However, directly calling it is tricky because of the symbol resolution.
    // A common approach is to use linker wrappers or dlsym.
    // For this example, we'll demonstrate a simplified scenario,
    // where you might have to rely on a different mechanism to access the
    // original. In a real-world scenario, you might need to use techniques
    // similar to those described in link or other platform-specific methods for
    // hooking. For simplicity, we'll just simulate re-throwing the exception.

    // Simulate re-throwing the exception after logging
    // This will lead to std::terminate if no catch handler handles it later.
    // In a real system, you'd want a more robust way to forward to the original
    // handler.
    original_cxa_throw(exception_p, tinfo, dest);
    // throw;
    abort();
}

// main.cpp
#include <iostream>
#include <stdexcept>

void risky_function()
{
    throw std::runtime_error("Something went wrong!");
}

int main()
{
    try
    {
        risky_function();
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    // You can also throw a different type of exception to see the interception
    try
    {
        throw 42; // Throw an integer
    }
    catch (int i)
    {
        std::cout << "Caught integer exception: " << i << std::endl;
    }

    return 0;
}
