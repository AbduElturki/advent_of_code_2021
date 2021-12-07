#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>


uint64_t simulate(std::vector<uint64_t> timers, int days)
{
    while (days-- > 0)
    {
        uint64_t reproduce = timers[0];
        std::rotate(timers.begin(), timers.begin() + 4, timers.end());

        timers[6] += reproduce;
    }

    return std::accumulate(timers.begin(), timers.end(), 0ULL);
}

int main()
{
    std::ifstream input("input");

    std::string s;
    std::vector<uint64_t> fish_time(9, 0);

    while(getline(input, s, ','))
        ++fish_time[std::stoi(s)];

    input.close();

    std::cout << "Part 1:\t" << simulate(fish_time, 80) << "\nPart 2:\t" << simulate(fish_time, 256) << std::endl;
}