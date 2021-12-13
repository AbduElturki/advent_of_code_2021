#include <iostream>
#include <fstream>
#include <unordered_set>
#include <queue>
#include <string>

struct UIntPairHasher
{
    size_t operator()(const std::pair<uint, uint> &s) const
    {
        return std::hash<uint>()(s.first) ^ (std::hash<uint>()(s.second) << 1);
    }
};

class Origami
{
    std::unordered_set<std::pair<uint, uint>, UIntPairHasher> dots;
    std::queue<std::pair<char, uint>> folds;

    uint max_x = 0;
    uint max_y = 0;

    void foldByX(uint along)
    {
        std::unordered_set<std::pair<uint, uint>, UIntPairHasher> new_dots;
        uint difference;
        for (const auto &[x, y] : dots)
        {
            if (x < along)
            {
                new_dots.insert({x, y});
            }
            else
            {
                difference = x - along;
                new_dots.insert({along - difference, y});
            }
        }

        max_x = along - 1;
        dots = new_dots;
    }

    void foldByY(uint along)
    {
        std::unordered_set<std::pair<uint, uint>, UIntPairHasher> new_dots;
        uint difference;
        for (const auto &[x, y] : dots)
        {
            if (y < along)
            {
                new_dots.insert({x, y});
            }
            else
            {
                difference = y - along;
                new_dots.insert({x, along - difference});
            }
        }

        max_y = along - 1;
        dots = new_dots;
    }

public:
    Origami(std::unordered_set<std::pair<uint, uint>, UIntPairHasher> dots, std::queue<std::pair<char, uint>> folds, uint max_x, uint max_y)
        : dots(dots), folds(folds), max_x(max_x), max_y(max_y) {}

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

    size_t size() { return dots.size(); }

    void print() const
    {
        std::vector<std::string> paper(max_y + 1, std::string(max_x + 1, ' '));
        for (const auto &[x, y] : dots)
            paper[y][x] = '#';

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
    std::unordered_set<std::pair<uint, uint>, UIntPairHasher> dots;
    while (getline(input, line))
    {
        if (line.size() == 0)
            break;

        x = std::stoi(line.substr(0, line.find(",")));
        y = std::stoi(line.substr(line.find(",") + 1, line.size()));

        max_x = x > max_x ? x : max_x;
        max_y = y > max_y ? y : max_y;
        dots.insert({x, y});
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

    Origami origami(dots, folds, max_x, max_y);

    origami.fold();
    std::cout << "Part 1:\t" << origami.size() << "\nPart 2:" << std::endl;

    origami.foldAll();
    origami.print();
}