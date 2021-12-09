#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <algorithm>


struct SizePairHasher
{
    size_t operator()(const std::pair<size_t, size_t> &s) const {
        return std::hash<size_t>()(s.first) ^ ( std::hash<size_t>()(s.second) << 1);
    }
};

int BasinSizeDFS(const std::vector<std::vector<int>> &heightmap, std::unordered_set<std::pair<size_t, size_t>, SizePairHasher> &seen, size_t i, size_t j)
{

    if (seen.find({i,j}) != seen.end() || heightmap[i][j] == 9)
        return 0;

    seen.insert({i,j});
    int curr_size = 1;
    if ((i != 0) && (heightmap[i][j] < heightmap[i - 1][j]))
        curr_size += BasinSizeDFS(heightmap, seen, i - 1, j);

    if ((j != 0) && (heightmap[i][j] < heightmap[i][j - 1]))
        curr_size += BasinSizeDFS(heightmap, seen, i, j - 1);

    if ((j != heightmap[i].size() - 1) && (heightmap[i][j] < heightmap[i][j + 1]))
        curr_size += BasinSizeDFS(heightmap, seen, i, j + 1);

    if ((i != heightmap.size() - 1) && (heightmap[i][j] < heightmap[i + 1][j]))
        curr_size += BasinSizeDFS(heightmap, seen, i + 1, j);

    return curr_size;
}

std::vector<std::pair<size_t, size_t>> getLowLevels(const std::vector<std::vector<int>> & heightmap)
{
    std::vector<std::pair<size_t, size_t>> low_levels;

    for (size_t i = 0; i != heightmap.size(); ++i)
    {
        for (size_t j = 0; j != heightmap[i].size(); ++j)
        {
            bool is_low_level = true;
            // up
            if (i != 0)
                is_low_level &= heightmap[i][j] < heightmap[i - 1][j];
            //left
            if (j != 0)
                is_low_level &= heightmap[i][j] < heightmap[i][j - 1];
            //right
            if (j != heightmap[i].size() - 1)
                is_low_level &= heightmap[i][j] < heightmap[i][j + 1];
            // down
            if (i != heightmap.size() - 1)
                is_low_level &= heightmap[i][j] < heightmap[i + 1][j];

            if(is_low_level)
                low_levels.push_back({i,j});
        }
    }

    return low_levels;
}

std::vector<int> getBasinSizes(const std::vector<std::vector<int>> & heightmap, const std::vector<std::pair<size_t, size_t>> & locations)
{
    int i, j;
    std::vector<int> sizes;
    std::unordered_set<std::pair<size_t, size_t>, SizePairHasher> seen;
    for (const std::pair<size_t, size_t> & location: locations)
    {
        seen = {};
        i = location.first;
        j = location.second;
        sizes.push_back(BasinSizeDFS(heightmap, seen, i, j));
    }
    std::sort(sizes.rbegin(), sizes.rend());
    return sizes;
}

int getRiskLevel(const std::vector<std::vector<int>> & heightmap, const std::vector<std::pair<size_t, size_t>> & locations)
{
    int risk_level = 0;
    int i, j;
    for (const std::pair<size_t, size_t> & location: locations)
    {
        i = location.first;
        j = location.second;
        risk_level += 1 + heightmap[i][j];
    }
    return risk_level;
}

int main()
{
    std::vector<std::vector<int>> heightmap;

    std::ifstream input("input");

    std::string line;

    while (getline(input, line))
    {
        std::vector<int> curr_line;
        for (char c : line)
            curr_line.push_back(c - '0');
        heightmap.push_back(curr_line);
    }

    input.close();

    std::vector<std::pair<size_t, size_t>> low_level_locations = getLowLevels(heightmap);
    std::vector<int> basin_sizes = getBasinSizes(heightmap, low_level_locations);

    std::cout << "Part 1:\t" << getRiskLevel(heightmap, low_level_locations) << std::endl;
    std::cout << "Part 2:\t" << basin_sizes[0] * basin_sizes[1] * basin_sizes[2] << std::endl;
}