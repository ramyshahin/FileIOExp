// Timer.cpp

#include <ctime>

class Timer
{
public:
    Timer() :
        start(std::clock())
    {
    }

    operator double() const
    {
        return (std::clock() - start) / static_cast<double>(CLOCKS_PER_SEC);
    }

    void reset()
    {
        start = std::clock();
    }
    
private: 
    std::clock_t start;
};