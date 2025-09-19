#include <chrono>
#include <exception>
#include <print>
#include <random>

void funcZ0(int i)
{
    throw std::runtime_error("Error: " + std::to_string(i));
}
void funcZ1(int i)
{
    throw std::runtime_error("Error: " + std::to_string(i));
}
void funcB1(int i)
{
    funcZ0(i + 10);
}
void funcB2(int i)
{
    funcZ1(i - 10);
}
void funcA(int i)
{
    if (i < 50)
        funcB1(i);
    else
        funcB2(i);
}

template<typename T> int make_random()
{
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 random_engine(seed);
    std::uniform_int_distribution<int> distribution(0, 99);
    T randomNumber = distribution(random_engine);
    return randomNumber;
}

int main()
{
    try
    {
        auto randomNumber = make_random<int>();
        std::println("Random number: {} {}", randomNumber, randomNumber < 50);
        funcA(randomNumber);
    }
    catch (std::exception& e)
    {
        std::println("got exception: {}", e.what());
    }
}
