// supports both gcc and clang.
// https://compiler-explorer.com/z/TnsrcvWda
#include <dlfcn.h>
#include <print>
#include <typeinfo>

#ifdef __clang__
using __cxa_throw_type = void (*)(void*, std::type_info*, void (*)(void*));
#else
using __cxa_throw_type = void (*)(void*, void*, void (*)(void*));
#endif
// Pointer to store the original __cxa_throw function
static __cxa_throw_type original_cxa_throw{(__cxa_throw_type)dlsym(RTLD_NEXT, "__cxa_throw")};

// Our custom __cxa_throw implementation
#ifdef __clang__
extern "C" void __cxa_throw(void* thrown_exception, std::type_info* tinfo, void (*dest)(void*))
{
#else
extern "C" void __cxa_throw(void* thrown_exception, void* vp, void (*dest)(void*))
{
    std::type_info* tinfo = (std::type_info*)vp;
#endif
    std::println("Intercepted throw called: type: {}", tinfo->name());

    // Call the original __cxa_throw function to continue exception handling
    original_cxa_throw(thrown_exception, tinfo, dest);
    abort(); // as the __cxa_throw attribut is noreturn
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
