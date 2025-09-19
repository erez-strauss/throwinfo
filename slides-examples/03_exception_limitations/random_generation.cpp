#include <chrono>
#include <exception>
#include <print>
#include <random>

int main()
{
    unsigned long long seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 random_engine(seed); // Mersenne Twister engine

    std::uniform_int_distribution<int> distribution(0, 99);
    int randomNumber = distribution(random_engine);
    std::println("Random number: {} {}", randomNumber, randomNumber < 50);

    return 0;
}
