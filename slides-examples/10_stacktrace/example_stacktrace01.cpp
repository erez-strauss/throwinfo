#include <print>
#include <stacktrace>
#include <string>

using namespace std;

void funcC(const auto& param)
{
    string s = param + " C";
    println("C: {}", s);
    stacktrace strace{stacktrace::current()};
    println("C: stacktrace:\n{}", strace);

    throw s;
}

void funcB(const auto& param)
{
    string s = param + " B";
    println("B: {}", s);
    funcC(s);
}
void funcA(const auto& param)
{
    string s = param + " A";
    println("A: {}", s);
    funcB(s);
}

int main()
{
    try
    {
        const string s{"main"};
        funcA(s);
    }
    catch (string& se)
    {
        println("got string exception: '{}'", se);
    }
    catch (...)
    {
        println("got unknown exception");
    }
    return 0;
}
