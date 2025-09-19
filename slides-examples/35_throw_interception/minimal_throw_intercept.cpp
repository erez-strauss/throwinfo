// clang++ only.
#include <dlfcn.h>
#include <print>
#include <typeinfo>

using __cxa_throw_type = void (*)(void*, std::type_info*, void (*)(void*));

// Pointer to store the original __cxa_throw function
static __cxa_throw_type original_cxa_throw{(__cxa_throw_type)dlsym(RTLD_NEXT, "__cxa_throw")};

// Our custom __cxa_throw implementation
extern "C" void __cxa_throw(void* thrown_exception, std::type_info* tinfo, void (*dest)(void*))
{
    std::println("Intercepted throw called: type: {}", tinfo->name());

    // Call the original __cxa_throw function to continue exception handling
    original_cxa_throw(thrown_exception, tinfo, dest);
}

int main()
{
    try
    {
        throw 123;
    }
    catch (...)
    {
        std::println("there was an exception");
    }
}
