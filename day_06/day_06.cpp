#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

uint64_t simulate(std::vector<uint64_t> timers, int days)
{
    while (days-- > 0)
    {
        uint64_t reproduce = timers[0];
        for (int i = 0; i < timers.size() - 1; ++i)
            timers[i] = timers[i + 1];

        timers[6] += reproduce;
        timers[8] = reproduce;
    }

    return std::accumulate(timers.begin(), timers.end(), uint64_t());
}

int main()
{
    std::ifstream input("input");
    std::string s;
    std::vector<std::string> time_str;

    getline(input, s);

    input.close();

    boost::split(time_str, s, boost::is_any_of(","));

    std::vector<uint64_t> fish_time(9, 0);
    for (std::string days_left : time_str)
        ++fish_time[boost::lexical_cast<uint64_t>(days_left)];

    std::cout << "Part 1:\t" << simulate(fish_time, 80) << "\nPart 2:\t" << simulate(fish_time, 256) << std::endl;
}