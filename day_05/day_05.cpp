#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>

struct point
{
    int x;
    int y;

    point(int x, int y) : x(x), y(y){};
    bool operator==(const point &other) const { return other.x == x && other.y == y; };
    bool operator<(const point &other) const { return x < other.x || ((x == other.x) && y < other.y); }
};

std::ostream &operator<<(std::ostream &os, const point &p)
{
    os << "(" << p.x << "," << p.y << ")";
    return os;
};

struct pointHasher
{
    std::size_t operator()(const point &p) const
    {
        return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
    }
};

std::vector<int> getInbetweenNums(int num1, int num2)
{
    std::vector<int> res;
    if (num1 < num2)
        for (int i = num1; i <= num2; ++i)
            res.push_back(i);

    else
        for (int i = num1; i >= num2; --i)
            res.push_back(i);

    return res;
}

int countOverlap(const std::vector<std::pair<point, point>> &points_pair, int threshold = 2, bool diagonal = false)
{
    std::unordered_map<point, int, pointHasher> counts;
    for (const std::pair<point, point> &point_pair : points_pair)
    {
        point start_point = point_pair.first;
        point end_point = point_pair.second;

        // Horizontal points
        if (start_point.x == end_point.x)
            for (int y : getInbetweenNums(start_point.y, end_point.y))
                counts[point(start_point.x, y)]++;

        // Vertical points
        else if (start_point.y == end_point.y)
            for (int x : getInbetweenNums(start_point.x, end_point.x))
                counts[point(x, start_point.y)]++;

        // diagonal points
        else if (diagonal)
        {
            std::vector<int> xs = getInbetweenNums(start_point.x, end_point.x);
            std::vector<int> ys = getInbetweenNums(start_point.y, end_point.y);
            for (int i = 0; i < xs.size(); ++i)
                counts[point(xs[i], ys[i])]++;
        }
    }

    int res = 0;
    for (std::unordered_map<point, int>::iterator it = counts.begin(); it != counts.end(); ++it)
        if (it->second >= threshold)
            res++;
    return res;
}

int main()
{
    std::vector<std::pair<point, point>> points;

    std::ifstream input("input");
    std::string line;
    std::stringstream line_stream;
    int x1, y1, x2, y2;

    while (getline(input, line))
    {
        // format: x1,y1 -> x2,y2
        line.replace(line.find("->", 0), 2, " ");
        line.replace(line.find(",", 0), 1, " ");
        line.replace(line.find(",", 0), 1, " ");

        line_stream = std::stringstream(line);
        line_stream >> x1 >> y1 >> x2 >> y2;

        points.push_back(std::make_pair(point(x1, y1), point(x2, y2)));
    }
    input.close();

    std::cout << "Part 1:\t" << countOverlap(points, 2, false) << "\nPart 2:\t" << countOverlap(points, 2, true) << std::endl;
}