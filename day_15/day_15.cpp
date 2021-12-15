#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <tuple>

struct IntPairHasher
{
    size_t operator()(const std::pair<int, int> &s) const
    {
        return std::hash<int>()(s.first) ^ (std::hash<int>()(s.second) << 1);
    }
};

bool isValidLocation(std::vector<std::vector<int>> risk_levels, int row, int col, uint factor)
{
    return (row >= 0) && (row < (risk_levels.size() * factor)) && (col >= 0) && (col < (risk_levels[0].size() * factor));
}

int getTotalLowRisk(std::vector<std::vector<int>> risk_levels, uint factor)
{
    std::unordered_map<std::pair<int, int>, int, IntPairHasher> costs;
    std::priority_queue<std::tuple<int, int, int>> cost_queue;

    constexpr int dx[4] = {0, 0, -1, 1};
    constexpr int dy[4] = {-1, 1, 0, 0};

    costs[{0, 0}] = 0;
    cost_queue.push({0, 0, 0});

    int cost, new_cost, cost_at_curr_loc;
    int next_x, next_y;
    int warp_x, warp_y;
    while (cost_queue.size() > 0)
    {
        auto [cost, x, y] = cost_queue.top();
        if (x == ((risk_levels.size() * factor) - 1) && y == (risk_levels[0].size() * factor) - 1)
        {
            return -cost;
        }

        cost_queue.pop();

        for (int dir = 0; dir < 4; ++dir)
        {
            next_x = x + dx[dir];
            next_y = y + dy[dir];
            if (isValidLocation(risk_levels, next_x, next_y, factor))
            {
                cost_at_curr_loc = 0;
                new_cost = -cost;

                warp_x = next_x;
                while (warp_x >= risk_levels.size())
                {
                    cost_at_curr_loc++;
                    warp_x -= risk_levels.size();
                }

                warp_y = next_y;
                while (warp_y >= risk_levels[0].size())
                {
                    cost_at_curr_loc++;
                    warp_y -= risk_levels[0].size();
                }

                cost_at_curr_loc += risk_levels[warp_x][warp_y];

                while (cost_at_curr_loc > 9)
                    cost_at_curr_loc -= 9;

                new_cost += cost_at_curr_loc;

                if (costs.find({next_x, next_y}) != costs.end() && costs.at({next_x, next_y}) <= new_cost)
                    continue;

                costs[{next_x, next_y}] = new_cost;
                cost_queue.push({-new_cost, next_x, next_y});
            }
        }
    }
    return -1;
}

int main()
{
    std::ifstream input("input");

    std::vector<std::vector<int>> risk_levels;

    std::string line;

    std::vector<int> vec_int;
    while (getline(input, line))
    {
        vec_int = {};
        for (char c : line)
            vec_int.push_back(c - '0');

        risk_levels.push_back(vec_int);
    }

    input.close();

    std::cout << "Part 1:\t" << getTotalLowRisk(risk_levels, 1) << "\nPart 2:\t" << getTotalLowRisk(risk_levels, 5) << std::endl;
}