#include <iostream>
#include <fstream>
#include <queue>
#include <string>

class Origami
{
    std::queue<std::pair<char, uint>> folds;
    std::vector<std::string> paper;
    uint num_dots = 0;

    void foldByX(uint along)
    {
        int difference;
        for (size_t y = 0; y < paper.size(); ++y)
        {
            for (size_t x = along + 1; x < paper[y].size(); ++x)
            {
                difference = x - along;
                if (paper[y][x] == '#')
                {
                    if (paper[y][along - difference] == '#')
                        --num_dots;
                    else
                        paper[y][along - difference] = '#';
                }
            }
            paper[y] = paper[y].substr(0, along);
        }
    }

    void foldByY(uint along)
    {
        int difference;
        for (size_t y = along + 1; y < paper.size(); ++y)
        {
            difference = y - along;
            for (size_t x = 0; x < paper[x].size(); ++x)
            {
                if (paper[y][x] == '#')
                {
                    if (paper[along - difference][x] == '#')
                        --num_dots;
                    else
                        paper[along - difference][x] = '#';
                }
            }
        }
        paper.resize(along);
    }

public:
    Origami(std::vector<std::pair<uint, uint>> dots_locations, std::queue<std::pair<char, uint>> folds, uint max_x, uint max_y)
        : folds(folds)
    {
        paper = std::vector<std::string>(max_y + 1, std::string(max_x + 1, ' '));
        for (auto [x, y] : dots_locations)
        {
            paper[y][x] = '#';
            ++num_dots;
        }
    }

    void fold()
    {
        if (folds.size() == 0)
            return;

        char direction;
        int along;

        direction = folds.front().first;
        along = folds.front().second;

        if (direction == 'y')
            foldByY(along);
        else
            foldByX(along);

        folds.pop();
    }

    void foldAll()
    {
        while (folds.size() != 0)
            fold();
    }

    int getNumOfDots() { return num_dots; }

    void print() const
    {
        for (const std::string &line : paper)
        {
            std::cout << line << "\n";
        }
        std::cout << std::endl;
    }
};

int main()
{
    std::ifstream input("input");

    std::string line;

    uint x, y;
    uint max_x = 0, max_y = 0;
    std::vector<std::pair<uint, uint>> dots_location;
    while (getline(input, line))
    {
        if (line.size() == 0)
            break;

        x = std::stoi(line.substr(0, line.find(",")));
        y = std::stoi(line.substr(line.find(",") + 1, line.size()));

        max_x = x > max_x ? x : max_x;
        max_y = y > max_y ? y : max_y;
        dots_location.push_back({x, y});
    }

    char fold;
    uint along;
    std::queue<std::pair<char, uint>> folds;
    while (getline(input, line))
    {
        fold = line[line.find("=") - 1];
        along = std::stoi(line.substr(line.find("=") + 1, line.size()));
        folds.push({fold, along});
    }

    input.close();

    Origami origami(dots_location, folds, max_x, max_y);

    origami.fold();
    std::cout << "Part 1:\t" << origami.getNumOfDots() << "\nPart 2:" << std::endl;

    origami.foldAll();
    origami.print();
}