#include <iostream>
#include <fstream>
#include <vector>

int NumOfIncrease(const std::vector<int> &measurement, int windowSize = 1)
{
    int increases = 0;
    for (int i = windowSize; i < measurement.size(); i++)
    {
        // Since we only care about increases of sliding window, we can ignore the inbetween values.
        if (measurement[i] > measurement[i - windowSize])
        {
            increases++;
        }
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