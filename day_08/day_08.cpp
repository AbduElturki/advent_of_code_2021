#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <sstream>
#include <math.h>

struct Signal
{
    std::bitset<7> turned;
    uint count = 0;

    Signal(std::string s)
    {
        for (char c : s)
        {
            turned[c - 'a'] = true;
            count += 1;
        }
    }

    Signal() {}

    bool operator==(const Signal &rhs) const{
        return (rhs.turned == turned) && (rhs.count == count);
    }
};

struct SignalHasher
{
    size_t operator()(const Signal &s) const {
        return std::hash<std::bitset<7>>()(s.turned);
    }
};

int decodeSignal(const std::vector<Signal> &unique_siginal, const std::vector<Signal> & digits)
{
    /*
     aaaa  
    b    c 
    b    c 
     dddd  
    e    f 
    e    f 
     gggg 
    */

    std::vector<Signal> len_5; //2, 3 or 5
    std::vector<Signal> len_6; //0, 6 or 9

    std::unordered_map<Signal, int, SignalHasher> sig_to_int;
    std::unordered_map<int, Signal> int_to_sig;

    for(const Signal &s: unique_siginal)
    {
        switch(s.count){
            case 2:
                sig_to_int[s] = 1;
                int_to_sig[1] = s;
                break;
            case 3:
                sig_to_int[s] = 7;
                int_to_sig[7] = s;
                break;
            case 4:
                sig_to_int[s] = 4;
                int_to_sig[4] = s;
                break;
            case 5:
                len_5.push_back(s);
                break;
            case 6:
                len_6.push_back(s);
                break;
            case 7:
                sig_to_int[s] = 8;
                int_to_sig[8] = s;
                break;
        }
    }

    std::bitset<7> BD = (~int_to_sig[1].turned) & int_to_sig[4].turned;

    for(Signal s: len_5){
        bool has_CF = (s.turned & int_to_sig[1].turned) == int_to_sig[1].turned;
        bool has_BD = (s.turned & BD) == BD;
        if(has_CF)
        {
            sig_to_int[s] = 3;
            int_to_sig[3] = s;
        }
        else if (has_BD) 
        {
            sig_to_int[s] = 5;
            int_to_sig[5] = s;
        }
        else
        {
            sig_to_int[s] = 2;
            int_to_sig[2] = s;
        }
    }

    for(Signal s: len_6){
        bool has_CF = (s.turned & int_to_sig[1].turned) == int_to_sig[1].turned;
        bool has_BD = (s.turned & BD) == BD;
        if(has_CF && has_BD)
        {
            sig_to_int[s] = 9;
            int_to_sig[9] = s;
        }
        else if (has_CF && !has_BD) 
        {
            sig_to_int[s] = 0;
            int_to_sig[0] = s;
        }
        else if (!has_CF && has_BD) 
        {
            sig_to_int[s] = 6;
            int_to_sig[6] = s;
        }
    }

    int i = 3;
    int res = 0;
    for(Signal s: digits)
    {
        res += sig_to_int[s] * pow(10, i);
        i--;
    }
    return res;
}


int countSizeMatches(std::vector<std::vector<Signal>> digits_vector, std::set<uint> sizes)
{
    int res = 0;
    for (std::vector<Signal> digit_row : digits_vector)
        for (Signal digit : digit_row)
            if (sizes.find(digit.count) != sizes.end())
                res++;
    return res;
}

int main()
{
    std::vector<std::vector<Signal>> digit_signals;
    std::vector<std::vector<Signal>> unique_signals;
    std::ifstream input("input");

    std::string line, signal_pattern, digits;

    std::stringstream ss;
    std::string s;

    uint sum_decode = 0;
    while (getline(input, line))
    {
        size_t delimiter = line.find("|");

        std::vector<Signal> curr_unique_signal;

        signal_pattern = line.substr(0, delimiter);
        ss = std::stringstream(signal_pattern);
        while (ss >> s)
            curr_unique_signal.push_back(Signal(s));
        ss.clear();
        unique_signals.push_back(curr_unique_signal);

        std::vector<Signal> curr_digit_signal;
        digits = line.substr(delimiter + 1, line.size());
        ss = std::stringstream(digits);
        while (ss >> s)
            curr_digit_signal.push_back(Signal(s));
        ss.clear();

        sum_decode += decodeSignal(curr_unique_signal, curr_digit_signal);
        digit_signals.push_back(curr_digit_signal);
    }

    input.close();

    std::set<uint> sizes_to_compare{2, 3, 4, 7};

    std::cout << countSizeMatches(digit_signals, sizes_to_compare) << std::endl;
    std::cout << sum_decode << std::endl;
}