#include <iostream>
#include <fstream>
#include <vector>

int getHorizontalPos(const std::vector<std::pair<std::string, int>> &movements, bool depth_mode = false)
{
    int depth = 0;
    int horizontal = 0;
    int aim = 0;
    for (std::pair<std::string, int> current : movements)
    {
        if (current.first == "forward")
        {
            horizontal += current.second;
            depth += (current.second * aim);
        }
        else if (current.first == "down")
        {
            aim += current.second;
        }
        else
        {
            aim -= current.second;
        }
    }
    return depth_mode ? depth * horizontal : aim * horizontal;
}

int main()
{
    std::vector<std::pair<std::string, int>> movements;

    std::string command;
    int value;

    std::ifstream input("input");

    while (input >> command && input >> value)
    {
        movements.push_back(std::make_pair(command, value));
    }
    input.close();
    std::cout << "Part 1:\t" << getHorizontalPos(movements) << "\nPart 2:\t" << getHorizontalPos(movements, true) << std::endl;
}