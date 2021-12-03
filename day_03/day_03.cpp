#include <iostream>
#include <fstream>
#include <vector>

char BoyerMoore(const std::vector<int>::iterator start, const std::vector<int>::iterator end, int position)
{
    int curr_majority;
    int curr_count = 0;
    for (std::vector<int>::iterator it = start; it != end; ++it)
    {
        int curr_bit = (*it >> position) & 1;
        if (curr_count == 0)
        {
            curr_majority = curr_bit;
            curr_count = 1;
        }
        else if (curr_majority == curr_bit)
            curr_count++;
        else
            curr_count--;
    }
    return curr_count > 0 ? curr_majority : 1;
}

int getGammaValue(const std::vector<int>::iterator start, std::vector<int>::iterator end, int bit_width = 12)
{
    int gamma = 0;
    for (int i = 0; i < bit_width; i++)
    {
        char curr_majority = BoyerMoore(start, end, i);
        gamma |= curr_majority << i;
    }
    return gamma;
}

int elimination(std::vector<int> power, bool least_common = false, int bit_width = 12)
{
    int curr_bit = bit_width - 1;
    std::vector<int>::iterator pstart = power.begin();
    std::vector<int>::iterator pend = power.end();
    while ((pend - pstart) > 1)
    {
        char match = least_common ? ~BoyerMoore(pstart, pend, curr_bit) & 1 : BoyerMoore(pstart, pend, curr_bit);
        pend = std::remove_if(pstart, pend, [&match, &curr_bit](int curr_power)
                              { return ((curr_power >> curr_bit) & 1) != match; });
        curr_bit--;
    }
    return power[0];
}

int main()
{
    std::vector<int> power;

    std::ifstream input("input");

    std::string curr_power;
    while (input >> curr_power)
    {
        power.push_back(stoi(curr_power, 0, 2));
    }

    input.close();

    int bit_width = curr_power.size();

    int gamma = getGammaValue(power.begin(), power.end(), bit_width);
    int epsilon = ~gamma & ((1 << bit_width) - 1);

    std::cout << "Part 1:\t" << gamma * epsilon << std::endl;
    std::cout << "Part 2:\t" << elimination(power, false, bit_width) * elimination(power, true, bit_width) << std::endl;
}