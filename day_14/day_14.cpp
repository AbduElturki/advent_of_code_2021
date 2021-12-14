#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include <stack>

void updatePolyTemplate(std::map<std::string, uint64_t> &pair_count, const std::map<std::string, char> &pair_insertion, std::map<char, uint64_t> &counts)
{
    std::stack<std::pair<std::string, uint64_t>> to_add;
    std::string first_pair, second_pair;
    char inserted;
    for (auto it = pair_count.begin(); it != pair_count.end(); ++it)
    {
        if (pair_insertion.find(it->first) != pair_insertion.end())
        {
            inserted = pair_insertion.at(it->first);
            first_pair = {it->first[0], inserted};
            second_pair = {inserted, it->first[1]};

            counts[inserted] += it->second;
            to_add.push({first_pair, it->second});
            to_add.push({second_pair, it->second});

            it->second = 0ULL;
        }
    }

    while (to_add.size() > 0)
    {
        pair_count[to_add.top().first] += to_add.top().second;
        to_add.pop();
    }
}

uint64_t getScore(std::map<char, uint64_t> counts)
{
    auto ls = [](const std::pair<char, uint64_t> &p1, const std::pair<char, uint64_t> &p2) { return p1.second < p2.second; };

    auto max = std::max_element(counts.begin(), counts.end(), ls);
    auto min = std::min_element(counts.begin(), counts.end(), ls);

    return max->second - min->second;
}

int main()
{
    std::ifstream input("input");

    std::string line;

    std::map<char, uint64_t> counts;
    std::map<std::string, uint64_t> pair_count;

    getline(input, line);

    std::string::iterator first = line.begin(), second = ++line.begin();

    ++counts[*first];

    std::string curr_pair;
    while (second != line.end())
    {
        curr_pair = {*first, *second};
        pair_count[{*first, *second}]++;
        counts[*second]++;
        ++first;
        ++second;
    }

    getline(input, line);

    std::map<std::string, char> pair_insertion;
    while (getline(input, line))
    {
        pair_insertion[line.substr(0, 2)] = line[6];
    }

    input.close();

    uint steps = 10;
    while (steps-- > 0)
        updatePolyTemplate(pair_count, pair_insertion, counts);

    std::cout << "Part 1:\t" << getScore(counts) << "\nPart 2:\t";

    steps = 30;
    while (steps-- > 0)
        updatePolyTemplate(pair_count, pair_insertion, counts);

    std::cout << getScore(counts) << std::endl;
}