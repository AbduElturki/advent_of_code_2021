#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

int findRoundedMedian(std::vector<int> v)
{
    int const count = static_cast<int>(v.size());
    if ( count % 2 == 0)
    {
        nth_element(v.begin(), v.begin() + count / 2, v.end());
        nth_element(v.begin(), v.begin() + (count - 1) / 2, v.end());

        return static_cast<int> (((v[(count - 1) / 2] + v[count / 2]) / 2.0) + 0.5f);
    }

    else
    {
        nth_element(v.begin(), v.begin() + count / 2, v.end());

        return static_cast<int>(v[count / 2]);
    }
}

int getMinMedianFuelConsumption(std::vector<int> crab_positions)
{
    int fuel = 0;
    int median_position = findRoundedMedian(crab_positions);
    for(int position: crab_positions)
        fuel += abs(position - median_position);
    return fuel;
}

int getMinMeanFuelConsumption(std::vector<int> crab_positions)
{
    float const count = static_cast<float>(crab_positions.size());
    int average_postion = static_cast<int>(std::reduce(crab_positions.begin(), crab_positions.end()) / count);

    int fuel = 0;
    for(int position: crab_positions)
    {
        int distance = abs(position - average_postion);
        fuel += static_cast<int> ((((distance + 1) * distance) / 2.0) + 0.5f);
    }
    return fuel;
}

int main()
{
    std::vector<int> crab_positions;
    std::string s;
    std::ifstream input("input");

    while (getline(input, s, ','))
        crab_positions.push_back(std::stoi(s));

    input.close();

    std::cout << "Part 1:\t" << getMinMedianFuelConsumption(crab_positions) << "\nPart 2:\t" << getMinMeanFuelConsumption(crab_positions) << std::endl;
}