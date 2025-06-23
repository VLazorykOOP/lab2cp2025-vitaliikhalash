#include <iostream>
#include <thread>
#include <mutex>
#include <random>

using namespace std;

const std::vector<std::string> DIRECTIONS = {
    "North", "North East", "East", "South East",
    "South", "South West", "West", "North West"};

template <typename T>
void validateInput(T &value)
{
    while (!(std::cin >> value) || value <= 0)
    {
        std::cout << "Invalid input. Please enter a positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// This function runs indefinitely and never returns
// It simulates continuous movement for a developer
[[noreturn]] void simulateDeveloperMovement(int id, double velocity, int intervalSeconds, std::mutex &coutMutex)
{
    std::random_device rd;
    std::mt19937 gen(rd() ^ std::hash<int>{}(id));
    auto dist = std::uniform_int_distribution(0, static_cast<int>(DIRECTIONS.size() - 1));

    while (true)
    {
        std::string direction = DIRECTIONS[dist(gen)];

        {
            std::scoped_lock lock(coutMutex);
            std::cout << "Developer " << id << " moving to " << direction
                      << " with velocity = " << velocity << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::duration<double>(intervalSeconds));
    }
}

int main()
{
    int developerCount;
    std::cout << "Enter number of developers: ";
    validateInput(developerCount);

    double velocity;
    std::cout << "Enter movement velocity: ";
    validateInput(velocity);

    int intervalSeconds;
    std::cout << "Enter direction change interval (seconds): ";
    validateInput(intervalSeconds);

    std::mutex coutMutex;
    std::vector<std::thread> threads;
    for (int i = 1; i <= developerCount; ++i)
    {
        threads.emplace_back(simulateDeveloperMovement, i, velocity, intervalSeconds, std::ref(coutMutex));
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    return 0;
}
