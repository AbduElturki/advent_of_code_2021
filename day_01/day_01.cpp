#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

int NumOfIncrease(const std::vector<int> &measurement, int windowSize = 1)
{
    int increases = 0;
    int prevWindow;
    int currWindow;
    for (int i = 0; i < ((measurement.size() - windowSize) + 1); i++)
    {
        currWindow = std::accumulate(measurement.begin() + i, measurement.begin() + i + windowSize, 0);
        if (i != 0 && currWindow > prevWindow)
        {
            increases++;
        }
        prevWindow = currWindow;
    }
    return increases;
}

int main()
{
    std::vector<int> measurements;

    std::ifstream input("input");

    int depth;

    while (input >> depth)
    {
        measurements.push_back(depth);
    }

    input.close();

    std::cout << "Part 1: \t" << NumOfIncrease(measurements, 1) << "\nPart 2: \t" << NumOfIncrease(measurements, 3) << std::endl;
}