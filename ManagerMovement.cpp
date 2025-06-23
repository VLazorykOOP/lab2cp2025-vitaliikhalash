#include <iostream>
#include <thread>
#include <mutex>
#include <cmath>
#include <vector>

const double PI = 3.14159265358979323846;

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
// It simulates continuous circular movement for a manager
[[noreturn]] void simulateManagerMovement(int id, double radius, double velocity, std::mutex &coutMutex)
{
    double angle = 0.0;
    double angularVelocity = velocity / radius;
    const double updateInterval = 0.1;

    while (true)
    {
        double x = radius * cos(angle);
        double y = radius * sin(angle);

        {
            std::scoped_lock lock(coutMutex);
            std::cout << "Manager " << id << " at position ("
                      << x << ", " << y << ") with velocity = " << velocity << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::duration<double>(updateInterval));
        angle += angularVelocity * updateInterval;

        if (angle >= 2 * PI)
            angle -= 2 * PI;
    }
}

int main()
{
    int managerCount;
    std::cout << "Enter number of managers: ";
    validateInput(managerCount);

    double radius;
    std::cout << "Enter radius of circular movement: ";
    validateInput(radius);

    double velocity;
    std::cout << "Enter movement velocity: ";
    validateInput(velocity);

    std::mutex coutMutex;
    std::vector<std::thread> threads;
    for (int i = 1; i <= managerCount; ++i)
    {
        threads.emplace_back(simulateManagerMovement, i, radius, velocity, std::ref(coutMutex));
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    return 0;
}
