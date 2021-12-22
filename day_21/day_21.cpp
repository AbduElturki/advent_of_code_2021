#include <iostream>
#include <map>
#include <tuple>

template <typename T, typename U>
std::pair<T, U> operator+(const std::pair<T, U> &l, const std::pair<T, U> &r)
{
    return {l.first + r.first, l.second + r.second};
}

typedef std::tuple<bool, int, int, int, int> game_state;

std::pair<uint64_t, uint64_t> playQuantRound(bool is_player_1, int p1_pos, int p2_pos, int p1_score, int p2_score, int roll, std::map<game_state, std::pair<uint64_t, uint64_t>> &cache)
{
    if (is_player_1)
    {
        p1_pos = (p1_pos + roll - 1) % 10 + 1;
        p1_score += p1_pos;

        if (p1_score >= 21)
            return {1ULL, 0ULL};
    }

    else
    {
        p2_pos = (p2_pos + roll - 1) % 10 + 1;
        p2_score += p2_pos;

        if (p2_score >= 21)
            return {0ULL, 1ULL};
    }

    game_state curr_state({is_player_1, p1_pos, p2_pos, p1_score, p2_score});
    if (cache.find(curr_state) != cache.end())
        return cache[curr_state];

    std::pair<uint64_t, uint64_t> result({0, 0});
    for (uint roll_1 = 1; roll_1 <= 3; ++roll_1)
        for (uint roll_2 = 1; roll_2 <= 3; ++roll_2)
            for (uint roll_3 = 1; roll_3 <= 3; ++roll_3)
                result = result + playQuantRound(!is_player_1, p1_pos, p2_pos, p1_score, p2_score, roll_1 + roll_2 + roll_3, cache);

    cache[curr_state] = result;
    return result;
}

std::pair<uint64_t, uint64_t> playQuantumGame(int p1_pos, int p2_pos)
{
    std::map<game_state, std::pair<uint64_t, uint64_t>> cache;
    std::pair<uint64_t, uint64_t> result;

    for (uint roll_1 = 1; roll_1 <= 3; ++roll_1)
        for (uint roll_2 = 1; roll_2 <= 3; ++roll_2)
            for (uint roll_3 = 1; roll_3 <= 3; ++roll_3)
                result = result + playQuantRound(1, p1_pos, p2_pos, 0, 0, roll_1 + roll_2 + roll_3, cache);

    return result;
}

uint playSingleUniverseGame(uint p1_pos, uint p2_pos)
{
    uint p1_score = 0, p2_score = 0;

    uint curr_roll = 1;

    uint num_roll = 0;

    while (true)
    {
        p1_pos = p1_pos + curr_roll + (curr_roll + 1) + (curr_roll + 2);
        while (p1_pos > 10)
            p1_pos = p1_pos - 10;

        num_roll += 3;
        if (p1_score + p1_pos >= 1000)
            break;
        p1_score += p1_pos;

        curr_roll += 3;

        p2_pos += curr_roll + (curr_roll + 1) + (curr_roll + 2);
        while (p2_pos > 10)
        {
            p2_pos = p2_pos - 10;
        }

        num_roll += 3;
        if (p2_score + p2_pos >= 1000)
            break;
        p2_score += p2_pos;

        curr_roll += 3;
    }

    return std::min(p1_score, p2_score) * num_roll;
}

int main()
{
    uint player_pos_1 = 9;
    uint player_pos_2 = 3;

    uint score = playSingleUniverseGame(player_pos_1, player_pos_2);
    std::pair<uint64_t, uint64_t> result = playQuantumGame(player_pos_1, player_pos_2);

    std::cout << "Part 1:\t" << score << "\nPart 2:\t" << std::max(result.first, result.second) << std::endl;
}