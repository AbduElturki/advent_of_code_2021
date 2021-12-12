#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <unordered_set>

 void getPossiblePaths(std::unordered_set<std::string> &possible_paths,
                       std::string origin,
                       const std::unordered_map<std::string, std::vector<std::string>> &adj_map,
                       bool repeat_small_cave,
                       std::unordered_set<std::string> forbidden = {"start"},
                       std::string curr_path = "")
{
    if (curr_path.size())
        curr_path.push_back(',');
    curr_path += origin;

    std::unordered_set<std::string> forbidden_without_origin(forbidden);
    bool origin_small_cave = false;
    if (islower(origin[0]) && origin != "start")
    {
        forbidden.insert(origin);
        origin_small_cave = true;
    }

    if (adj_map.find(origin) != adj_map.end())
    {
        for (const std::string &destination : adj_map.find(origin)->second)
        {
            if (forbidden.find(destination) != forbidden.end())
                continue;

            if (destination == "end" && possible_paths.find(curr_path) == possible_paths.end())
            {
                possible_paths.insert(curr_path);
            }
            else if (repeat_small_cave && origin_small_cave)
            {
                getPossiblePaths(possible_paths, destination, adj_map, true, forbidden, curr_path);
                getPossiblePaths(possible_paths, destination, adj_map, false, forbidden_without_origin, curr_path);
            }
            else
                getPossiblePaths(possible_paths, destination, adj_map, repeat_small_cave, forbidden, curr_path);
        }
    }
}

int main()
{
    std::ifstream input("input");

    std::string line;
    std::string begining, end;

    std::unordered_map<std::string, std::vector<std::string>> adj_map;

    while (getline(input, line))
    {
        begining = line.substr(0, line.find("-"));
        end = line.substr(line.find("-") + 1, line.size());

        adj_map[begining].push_back(end);
        if (begining != "start" || end != "end")
            adj_map[end].push_back(begining);
    }

    input.close();

    std::unordered_set<std::string> paths = {};

    getPossiblePaths(paths, "start", adj_map, false);
    std::cout << "Part 1:\t" << paths.size() << std::endl;

    paths = {};
    getPossiblePaths(paths, "start", adj_map, true);
    std::cout << "Part 2:\t" << paths.size() << std::endl;
}