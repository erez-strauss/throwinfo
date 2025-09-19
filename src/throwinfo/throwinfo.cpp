
#include <throwinfo/throwinfo.h>

#if defined(__clang__) && !defined(EXT_USE_THROW_VOID_API)
using cxa_throw_type = void (*)(void*, std::type_info*, void (*)(void*));
extern "C" void __cxa_throw(void*, std::type_info*, void (*)(void*));
#else
using cxa_throw_type = void (*)(void*, void*, void (*)(void*));
extern "C" void __cxa_throw(void*, void*, void (*)(void*));
#endif

static cxa_throw_type original_cxa_throw{(cxa_throw_type)dlsym(RTLD_NEXT, "__cxa_throw")};
static void* (*original_cxa_begin_catch)(void*){
    (void* (*)(void*))dlsym(RTLD_NEXT, "__cxa_begin_catch")};

#if defined(__clang__) && !defined(EXT_USE_THROW_VOID_API)
extern "C" void __cxa_throw(void* exception_p, std::type_info* tinfo, void (*dest)(void*))
{
#else
extern "C" void __cxa_throw(void* exception_p, void* vp, void (*dest)(void*))
{
    std::type_info* tinfo = reinterpret_cast<std::type_info*>(vp);
#endif

    ext::last_throwinfo.capture(exception_p, tinfo);

    original_cxa_throw(exception_p, tinfo, dest);
    abort(); // std::unreachable() - is UB.
}

extern "C" void*(__cxa_begin_catch)(void* ep)
{
    ext::last_throwinfo.report(ep);

    return original_cxa_begin_catch(ep);
}

void setup() __attribute__((constructor(101)));
void setup()
{
    std::set_terminate([]() {
        std::println(std::cerr, "Unhandled exception\n");
        if (ext::last_throwinfo.type_info_)
        {
            std::println(std::cerr, "Starting catch block for exception: {}\n",
                         ext::last_throwinfo.to_string());
            if (*ext::last_throwinfo.type_info_ == typeid(std::exception))
                std::println(
                    std::cerr, "throw type exception: {}",
                    reinterpret_cast<const std::exception*>(ext::last_throwinfo.ep_)->what());
            std::println(std::cerr, "End of terminate handler");
        }

        std::abort();
    });
}
