#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <map>
#include <math.h>

class Card
{
    std::map<int, int> _num_map;
    std::vector<int> _num_vec;
    std::vector<int> _row_match;
    std::vector<int> _col_match;
    bool _is_playing = false;
    int _width;

public:

    Card(std::vector<int> nums) : _num_vec(nums)
    {
        _is_playing = true;
        _width = sqrt(nums.size());
        _row_match = std::vector<int>(_width, 0);
        _col_match = std::vector<int>(_width, 0);

        for (int i = 0; i < nums.size(); i++)
            _num_map[nums[i]] = i;
    }
    bool isIn(int num)
    {
        if (_num_map.find(num) != _num_map.end())
        {
            int index = _num_map[num];
            _num_vec[index] = -1;
            ++_row_match[floor(index / _width)];
            ++_col_match[index % _width];
            return true;
        }
        return false;
    }

    bool isBingo()
    {
        for (int i : _row_match)
            if (i == _width)
            {
                this->stopPlaying();
                return true;
            }
        for (int i : _col_match)
            if (i == _width)
            {

                this->stopPlaying();
                return true;
            }
        return false;
    }

    void stopPlaying()
    {
        _is_playing = false;
    }

    bool isPlaying() const
    {
        return _is_playing;
    }

    int getLeftoverSum() const
    {
        int leftover = 0;
        for (int i : _num_vec)
            if (i != -1)
                leftover += i;

        return leftover;
    }
};

std::vector<int> playBingo(const std::vector<int> &draws, std::vector<Card> &Cards)
{
    std::vector<int> scores;
    for (int draw : draws)
        for (Card &card : Cards)
            if (card.isPlaying() && card.isIn(draw) && card.isBingo())
                scores.push_back(card.getLeftoverSum() * draw);
    return scores;
}

int main()
{
    std::vector<int> draws;

    std::vector<Card> cards;

    std::ifstream input("input");

    // Read first line of draws, and ignore ','
    std::string s;
    std::getline(input, s);
    std::stringstream ss(s);

    for (int i; ss >> i;)
    {
        draws.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }

    std::getline(input, s); // ignore the space between draws and cards

    // Create flat vector of the card's number then create card class and then add it to vector of cards 
    std::vector<int> curr_card;
    while (std::getline(input, s))
    {
        if (s == "")
        {
            cards.push_back(Card(curr_card));
            curr_card = std::vector<int>();
        }
        else
        {
            ss = std::stringstream(s);
            int i;
            while (ss >> i)
            {
                curr_card.push_back(i);
            }
        }
    }

    cards.push_back(Card(curr_card)); // add last card to vector of cards


    std::vector<int> scores = playBingo(draws, cards);

    std::cout << "Part 1:\t" << scores.front() << "\nPart2:\t" << scores.back() << std::endl;
}