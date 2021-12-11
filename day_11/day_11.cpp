#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>

struct IntPairHasher
{
    size_t operator()(const std::pair<int, int> &s) const
    {
        return std::hash<int>()(s.first) ^ (std::hash<int>()(s.second) << 1);
    }
};

class DumboSquid
{
    int grid_width;
    int total_flashes = 0;
    int curr_flashes = 0;
    std::vector<std::vector<int>> grid;
    std::stack<std::pair<int, int>> to_affect_surrounding;
    std::unordered_set<std::pair<int, int>, IntPairHasher> flashing;

    void flash()
    {
        std::vector<std::pair<int, int>> surroundings;
        int col, row;
        while (to_affect_surrounding.size() > 0)
        {
            row = to_affect_surrounding.top().first;
            col = to_affect_surrounding.top().second;

            to_affect_surrounding.pop();

            surroundings = getNonFlashingSurrounding(row, col);

            for (std::pair<int, int> coor : surroundings)
            {
                if (++grid[coor.first][coor.second] > 9)
                {
                    to_affect_surrounding.push(coor);
                    flashing.insert(coor);
                    grid[coor.first][coor.second] = 0;
                    ++curr_flashes;
                }
            }
        }
    }

    std::vector<std::pair<int, int>> getNonFlashingSurrounding(int row, int col) const
    {
        std::vector<std::pair<int, int>> surroundings;
        if (row != 0 && flashing.find({row - 1, col}) == flashing.end())
            surroundings.push_back({row - 1, col});
        if (col != 0 && flashing.find({row, col - 1}) == flashing.end())
            surroundings.push_back({row, col - 1});
        if (row != (grid_width - 1) && flashing.find({row + 1, col}) == flashing.end())
            surroundings.push_back({row + 1, col});
        if (col != (grid_width - 1) && flashing.find({row, col + 1}) == flashing.end())
            surroundings.push_back({row, col + 1});

        if (row != 0 && col != 0 && flashing.find({row - 1, col - 1}) == flashing.end())
            surroundings.push_back({row - 1, col - 1});
        if (row != 0 && col != (grid_width - 1) && flashing.find({row - 1, col + 1}) == flashing.end())
            surroundings.push_back({row - 1, col + 1});
        if (row != (grid_width - 1) && col != 0 && flashing.find({row + 1, col - 1}) == flashing.end())
            surroundings.push_back({row + 1, col - 1});
        if (row != (grid_width - 1) && col != (grid_width - 1) && flashing.find({row + 1, col + 1}) == flashing.end())
            surroundings.push_back({row + 1, col + 1});

        return surroundings;
    }

public:
    DumboSquid(int grid_width, std::string file_name) : grid_width(grid_width)
    {
        std::string line;
        std::vector<int> curr_row;
        std::ifstream input_stream(file_name);

        while (getline(input_stream, line))
        {
            curr_row = {};
            for (char c : line)
            {
                curr_row.push_back(c - '0');
            }
            grid.push_back(curr_row);
        }

        input_stream.close();
    }

    void step()
    {
        to_affect_surrounding = {};
        flashing = {};
        curr_flashes = 0;
        for (uint row = 0; row < grid_width; ++row)
            for (uint col = 0; col < grid_width; ++col)
            {
                if (++grid[row][col] > 9)
                {
                    to_affect_surrounding.push({row, col});
                    flashing.insert({row, col});
                    grid[row][col] = 0;
                    ++curr_flashes;
                }
            }

        flash();
        total_flashes += curr_flashes;
    }

    int getNumOfTotalFlashes() const { return total_flashes; }

    int getNumOfCurrentFlashes() const { return curr_flashes; }

    void showGrid() const
    {
        for (uint row = 0; row < grid_width; ++row)
        {
            for (uint col = 0; col < grid_width; ++col)
            {
                std::cout << grid[row][col] << "\t";
            }
            std::cout << "\n";
        }
        std::cout << std::endl;
    }
};

int main()
{
    DumboSquid dumbosquid = DumboSquid(10, "input");

    int steps;
    for (steps = 0; steps < 100; ++steps)
        dumbosquid.step();

    std::cout << "Part 1:\t" << dumbosquid.getNumOfTotalFlashes() << std::endl;

    while (dumbosquid.getNumOfCurrentFlashes() != 100)
    {
        dumbosquid.step();
        ++steps;
    }

    std::cout << "Part 2:\t" << steps << std::endl;
}