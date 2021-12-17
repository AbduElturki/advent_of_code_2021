#include <iostream>
#include <fstream>
#include <math.h>

std::pair<int, int> solveTrajectory(int x1, int x2, int y1, int y2)
{
    int count = 0;
    int max_y = 0;
    for (int vol_x = 0; vol_x <= x2; ++vol_x)
    {
        for (int vol_y = y1; vol_y <= -y1; ++vol_y)
        {
            int x = 0, y = 0;
            int tmp_max_y = 0;
            int curr_vol_x = vol_x, curr_vol_y = vol_y;
            while ((x < x1 || y > y2) && x <= x2 && y >= y1)
            {
                x += curr_vol_x;
                curr_vol_x -= curr_vol_x > 0;
                y += curr_vol_y;
                --curr_vol_y;
                tmp_max_y = std::max(tmp_max_y, y);
            }
            if (x >= x1 && x <= x2 && y >= y1 && y <= y2)
            {

                ++count;
                max_y = std::max(tmp_max_y, max_y);
            }
        }
    }
    return {count, max_y};
}

int main()
{
    std::ifstream input("input");

    std::string line;

    getline(input, line);

    input.close();

    int x1, x2, y1, y2;

    x1 = std::stoi(line.substr(line.find("=") + 1, line.find('.')));

    x2 = std::stoi(line.substr(line.find(".") + 2, line.find(',')));

    line = line.substr(line.find(','), line.size());

    y1 = std::stoi(line.substr(line.find("=") + 1, line.find('.')));
    y2 = std::stoi(line.substr(line.find(".") + 2, line.size()));


    auto [trajecotry_count, max_y] = solveTrajectory(x1,x2,y1,y2);
    std::cout << "Part 1:\t" << max_y << "\nPart 2:\t" << trajecotry_count << std::endl;
}