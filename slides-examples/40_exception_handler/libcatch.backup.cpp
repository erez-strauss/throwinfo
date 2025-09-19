// clang++-20 -W -Wall -std=c++23 -g -o capture_on_throw_process_on_catch
// capture_on_throw_process_on_catch.cpp  -lstdc++exp
// ../src/examples/capture_on_throw_process_on_catch.cpp

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

struct throw_info
{
    std::string msg;
    std::stacktrace st;
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    const std::type_info* type_info_{nullptr};
    std::string throw_type_name;
    void* exception_pointer;
    pid_t tid;

    std::string to_string()
    {
        std::string s;
        s = std::to_string(st);
        s += "\nthrow_type: ";
        // s += throw_type_name;

        int status;
        char* demangled_name = abi::__cxa_demangle(throw_type_name.c_str() /* tinfo->name()*/,
                                                   nullptr, nullptr, &status);
        if (status == 0)
        {
            s += demangled_name;

            free(demangled_name);
        }
        else
        {
            s += throw_type_name;
        }
        s += '\n';
        return s;
    }
};

static thread_local throw_info last_throw_info;

static thread_local std::stacktrace last_throw_stacktrace;

#if defined(__clang__) && !defined(EXT_USE_THROW_VOID_API)
using cxa_throw_type = void (*)(void*, std::type_info*, void (*)(void*));
extern "C" void __cxa_throw(void*, std::type_info*, void (*)(void*));
#else
using cxa_throw_type = void (*)(void*, void*, void (*)(void*));
extern "C" void __cxa_throw(void*, void*, void (*)(void*));
#endif

static cxa_throw_type original_cxa_throw{(cxa_throw_type)dlsym(RTLD_NEXT, "__cxa_throw")};

#if defined(__clang__) && !defined(EXT_USE_THROW_VOID_API)
extern "C" void __cxa_throw(void* exception_p, std::type_info* tinfo, void (*dest)(void*))
{
#else
extern "C" void __cxa_throw(void* exception_p, void* vp, void (*dest)(void*))
{
    std::type_info* tinfo = reinterpret_cast<std::type_info*>(vp);
#endif

    last_throw_info.timestamp = std::chrono::system_clock::now();
    last_throw_info.st = std::stacktrace::current();
    last_throw_info.type_info_ = tinfo;
    last_throw_info.throw_type_name = tinfo->name();
    last_throw_info.tid = gettid();
    last_throw_info.exception_pointer = exception_p;

    original_cxa_throw(exception_p, tinfo, dest);
    abort(); // Unreachable code.
}

static void* (*original_cxa_begin_catch)(void*){
    (void* (*)(void*))dlsym(RTLD_NEXT, "__cxa_begin_catch")};

extern "C" void*(__cxa_begin_catch)(void* ep)
{
    std::cerr << "Starting catch block for exception:\n" << last_throw_info.to_string() << '\n';
    if (*last_throw_info.type_info_ == typeid(int))
        std::cerr << "throw type int\n";
    else if (*last_throw_info.type_info_ == typeid(double))
        std::cerr << "throw type double: " << *(double*)ep << '\n';
    std::cerr << "End of Starting catch block sarting the catch block\n";

    return original_cxa_begin_catch(ep);
}

void setup() __attribute__((constructor(101)));
void setup()
{
    printf("Early init setting terminate handler\n");
    std::set_terminate([]() {
        std::cout << "Unhandled exception\n" << std::flush;
        if (last_throw_info.type_info_)
        {
            std::cerr << "Starting catch block for exception:\n"
                      << last_throw_info.to_string() << '\n';
            if (*last_throw_info.type_info_ == typeid(int))
                std::cerr << "throw type int\n";
            else if (*last_throw_info.type_info_ == typeid(double))
                std::cerr << "throw type double: " << *(double*)last_throw_info.exception_pointer
                          << '\n';
            std::cerr << "End of Starting catch block sarting the catch block\n";
        }

        std::abort();
    });
}

// void* early_init_ptr __attribute__((section(".init_array"))) { (void*)&setup
// };
