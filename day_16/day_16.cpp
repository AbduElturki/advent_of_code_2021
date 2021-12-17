#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <limits>

#define TRANSMISSION_SIZE 8192
#define WORD_LEN 4

struct Packet
{
    uint64_t version, type_id, value, size;

    std::vector<Packet> sub_packets;

    Packet(uint64_t version, uint64_t type_id, uint64_t value, uint64_t size) : version(version), type_id(type_id), value(value), size(size) {}
};

std::ostream &operator<<(std::ostream &os, const Packet &rhs)
{
    os << rhs.version << " " << rhs.type_id << " " << rhs.value;
    return os;
}

uint64_t popFrontBits(std::bitset<TRANSMISSION_SIZE> &bits, size_t num_of_bits)
{
    uint64_t result = 0;

    result = (bits >> (TRANSMISSION_SIZE - num_of_bits)).to_ullong();

    bits <<= num_of_bits;
    return result;
}

Packet parsePacketTree(std::bitset<TRANSMISSION_SIZE> &packet_transmission)
{
    uint64_t version, type_id, value, size;
    bool length_type, continue_signal;
    value = 0;
    version = popFrontBits(packet_transmission, 3);
    type_id = popFrontBits(packet_transmission, 3);
    std::vector<Packet> sub_packets = {};
    size = 6;

    if (type_id == 4)
    {
        do
        {
            continue_signal = popFrontBits(packet_transmission, 1);
            value <<= 4;
            value += popFrontBits(packet_transmission, 4);
            size += 5;

        } while (continue_signal == 1);
    }
    else
    {
        length_type = popFrontBits(packet_transmission, 1);
        size += 1;

        if (length_type)
        {
            value = popFrontBits(packet_transmission, 11);
            size += 11;

            uint64_t num_packets = 0;
            while ((num_packets < value) && packet_transmission.any())
            {
                Packet p = parsePacketTree(packet_transmission);
                size += p.size;
                sub_packets.push_back(p);
                ++num_packets;
            }
        }
        else
        {
            value = popFrontBits(packet_transmission, 15);
            size += 15;

            uint64_t sub_packets_len = 0;
            while ((sub_packets_len != value) && packet_transmission.any())
            {
                Packet p = parsePacketTree(packet_transmission);
                sub_packets_len += p.size;
                size += p.size;
                sub_packets.push_back(p);
            }
        }
    }

    Packet new_packet(version, type_id, value, size);
    new_packet.sub_packets = sub_packets;

    return new_packet;
}

uint64_t sumSubPacketsVersion(const Packet &packet)
{
    uint64_t res = packet.version;
    for (Packet sub_packet : packet.sub_packets)
        res += sumSubPacketsVersion(sub_packet);

    return res;
}

uint64_t evalPacket(const Packet &packet)
{
    uint64_t result;

    switch (packet.type_id)
    {
    case 0:
        result = 0ULL;
        for (const Packet &sub_packet : packet.sub_packets)
        {
            result += evalPacket(sub_packet);
        }
        break;
    case 1:
        result = 1ULL;
        for (const Packet &sub_packet : packet.sub_packets)
        {
            result = result * evalPacket(sub_packet);
        }

        break;
    case 2:
        result = std::numeric_limits<uint64_t>::max();
        for (const Packet &sub_packet : packet.sub_packets)
        {
            result = std::min(result, evalPacket(sub_packet));
        }
        break;
    case 3:
        result = 0ULL;
        for (const Packet &sub_packet : packet.sub_packets)
        {
            result = std::max(result, evalPacket(sub_packet));
        }
        break;

    case 4:
        result = packet.value;
        break;

    case 5:
        result = (evalPacket(packet.sub_packets[0]) > evalPacket(packet.sub_packets[1])) ? 1ULL : 0ULL;
        break;
    case 6:
        result = (evalPacket(packet.sub_packets[0]) < evalPacket(packet.sub_packets[1])) ? 1ULL : 0ULL;
        break;
    case 7:
        result = (evalPacket(packet.sub_packets[0]) == evalPacket(packet.sub_packets[1])) ? 1ULL : 0ULL;
        break;
    }
    return result;
}

int main()
{
    std::ifstream input("input");

    std::string packets_hex;

    getline(input, packets_hex);

    input.close();

    std::bitset<TRANSMISSION_SIZE> packet_transmission;

    uint64_t shift = TRANSMISSION_SIZE - WORD_LEN;
    for (const char &c : packets_hex)
    {
        if (c >= '0' && c <= '9')
            packet_transmission = packet_transmission | (std::bitset<TRANSMISSION_SIZE>(c - '0') << shift);
        else
            packet_transmission = packet_transmission | (std::bitset<TRANSMISSION_SIZE>((c - 'A') + 10ULL) << shift);

        shift -= WORD_LEN;
    }

    Packet top_packet = parsePacketTree(packet_transmission);

    std::cout << sumSubPacketsVersion(top_packet) << std::endl;
    std::cout << evalPacket(top_packet) << std::endl;
}