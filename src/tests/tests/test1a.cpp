#include <exception>
#include <print>

void bar()
{
    throw std::runtime_error("Error 123");
}

void foo1()
{
    std::println("foo1");
    bar();
}
void foo2()
{
    std::println("foo2");
    bar();
}

int main(int argc, char**)
{
    try
    {
        if (argc % 2)
            foo1();
        else
            foo2();
    }
    catch (const std::runtime_error& e)
    {
        std::println("Caught exception: {}", e.what());
    }
}
