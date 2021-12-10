#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stack>
#include <map>

uint getCorruptScore(const std::string &line, const std::map<char, char> &closing_to_opening, const std::map<char, int> &closing_points, std::stack<int> &stack)
{
    char c;
    uint points = 0;
    for (const char &c : line)
    {
        if (closing_points.find(c) != closing_points.end())
        {
            if ((stack.size() == static_cast<size_t>(0)) || (closing_to_opening.find(c)->second != stack.top()))
                points += closing_points.find(c)->second;

            stack.pop();
        }
        else
            stack.push(c);
    }

    return points;
}

uint64_t getIncompletenessScore(std::stack<int> stack, const std::map<char, int> & opening_points)
{
    uint64_t points = 0;
    while (stack.size() > static_cast<size_t>(0))
    {
        points = (5 * points) + opening_points.find(stack.top())->second;
        stack.pop();
    }
    return points;
}

int main()
{

    std::map<char, char> closing_to_opening = {{')', '('}, {']', '['}, {'}', '{'}, {'>', '<'}};
    std::map<char, int> closing_points = {{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};
    std::map<char, int> opening_points = {{'(', 1}, {'[', 2}, {'{', 3}, {'<', 4}};

    uint sum_corrupt_scores = 0;
    uint curr_corrupt_score = 0;

    std::vector<uint64_t> incompleteness_scores;

    std::ifstream input("input");
    std::string line;

    while (getline(input, line))
    {
        std::stack<int> starting_stack = {};

        curr_corrupt_score = getCorruptScore(line, closing_to_opening, closing_points, starting_stack);
        sum_corrupt_scores += curr_corrupt_score;

        if (curr_corrupt_score == 0) // If it isn't corrupt
            incompleteness_scores.push_back(getIncompletenessScore(starting_stack, opening_points));
    }

    input.close();

    size_t median_idx = incompleteness_scores.size()/2;
    std::nth_element(incompleteness_scores.begin(), incompleteness_scores.begin()+median_idx, incompleteness_scores.end());

    std::cout << "Part 1:\t" << sum_corrupt_scores << std::endl;
    std::cout << "Part 2:\t" << incompleteness_scores[median_idx] << std::endl;
}