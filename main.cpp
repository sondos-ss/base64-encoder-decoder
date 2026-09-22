#include <bits/stdc++.h>
using namespace std;

// b64_size stores, for an input of n bytes, the character count in chars and
// the number of '=' characters in pads.
//
// TODO: chars = 4 characters for every 3-byte group, with the last partial
// group rounded UP to a whole group; pads = 0, 2 or 1, decided by n % 3.
// C++ hint: / on long long is already integer division and % is the
// remainder, so no floating point is needed.

static const string ALPHABET =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static void b64_size(long long n, long long &chars, long long &pads)
{
    (void)n;
    chars = 4 * ((n + 2) / 3);
    pads = (n % 3 == 0 ? 0 : 3 - n % 3);
}
// Return the alphabet character at index, or 0 when index is not 0..63.
static char alphabetChar(long index)
{
    // TODO: range-check index first, then return ALPHABET at that position.
    // Hint: ALPHABET[i] on a std::string does no bounds checking; use your own
    // 0..63 test rather than hoping the string complains.
    (void)index;
    if (index >= 0 && index <= 63)
        return ALPHABET[index];
    return 0; // the failure value; the driver below prints INVALID for it
}

// Return the 0..63 index of the character c, or -1 when c is not in ALPHABET.
static int alphabetValue(char c)
{
    // TODO: find the position of c among the 64 characters.
    // Hint: a for loop from 0 to 63 comparing ALPHABET[i] to c is enough; '='
    // is not in ALPHABET, so it must come back as -1.
    (void)c;
    if (ALPHABET.find(c) != string::npos)
        return ALPHABET.find(c);
    return -1; // the failure value; the driver below prints INVALID for it
}

string encode_group(unsigned b0, unsigned b1, unsigned b2)
{
    // TODO: pack b0, b1, b2 into one 24-bit number, cut four 6-bit fields out of
    // it (most significant first) and return the four B64 characters.
    // C++ hint: B64[i] is the character at index i; use <<, >>, | and & 63.
    string s = "";
    unsigned n = (b0 << 16) | (b1 << 8) | b2;
    char f0 = ALPHABET[(n >> 18) & 63];
    s += f0;
    char f1 = ALPHABET[(n >> 12) & 63];
    s += f1;
    char f2 = ALPHABET[(n >> 6) & 63];
    s += f2;
    char f3 = ALPHABET[n & 63];
    s += f3;
    return s;
}
// TODO: tail holds 1 or 2 byte values (0-255). Return the 4 characters.
static std::string encode_tail(const std::string &tailBytes)
{
    // tailBytes holds the leftover bytes as raw chars; use
    // (unsigned char)tailBytes[0] to read a value 0-255. Pack them with the
    // first byte most significant, shift so the bits sit at the top of a 24-bit
    // block, read off 2 fields (1 byte) or 3 fields (2 bytes) with (n >> k) & 63,
    // then a"ppend '=' until the block is 4 characters.
    string s = "";
    unsigned n = ((unsigned char)tailBytes[0]) << 16;

    if (tailBytes.size() == 2)
    {
        n |= ((unsigned char)tailBytes[1] << 8);
    }
    s += ALPHABET[(n >> 18) & 63];
    s += ALPHABET[(n >> 12) & 63];
    if (tailBytes.size() == 2)
        s += ALPHABET[(n >> 6) & 63];

    s += '=';
    if (tailBytes.size() == 1)
        s += "=";
    return s;
}

static int hexval(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return 0;
}

static std::string encode(const std::string &data)
{
    std::string out;
    size_t i = 0;
    while (i + 3 <= data.size())
    {
        out += encode_group((unsigned char)data[i], (unsigned char)data[i + 1],
                            (unsigned char)data[i + 2]);
        i += 3;
    }
    if (i < data.size())
    {
        out += encode_tail(data.substr(i));
    }
    return out;
}

static int char_value(char c)
{
    size_t p = ALPHABET.find(c);
    if (p == string::npos)
        return -1;
    return (int)p;
}

// Return the byte values (0-255) that this 4-character group carries.
static vector<int> decode_group(const string &group)
{
    // TODO: count the '=' characters in the group, turn each character into its
    // 6-bit value with char_value (use 0 for '='), pack the four values into one
    // 24-bit int, then cut three bytes out of it, most significant first, and
    // return the first (3 - pad) of them.
    // Hint: vector<int> three(...); three.resize(3 - pad); cuts the tail off.
    int pad = 0;

    for (char c : group)
        if (c == '=')
            pad++;

    int v0 = group[0] == '=' ? 0 : char_value(group[0]);
    int v1 = group[1] == '=' ? 0 : char_value(group[1]);
    int v2 = group[2] == '=' ? 0 : char_value(group[2]);
    int v3 = group[3] == '=' ? 0 : char_value(group[3]);

    int n = (v0 << 18) | (v1 << 12) | (v2 << 6) | v3;

    int b0 = (n >> 16) & 255;
    int b1 = (n >> 8) & 255;
    int b2 = n & 255;

    vector<int> three = {b0, b1, b2};
    three.resize(3 - pad);

    return three;
}

int main()
{
    string line;
    while (getline(cin, line))
    {
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
        {
            line.erase(line.size() - 1);
        }
        if (line.empty())
            continue;
        string hex;
        char buf[8];
        for (size_t i = 0; i + 4 <= line.size(); i += 4)
        {
            vector<int> bytes = decode_group(line.substr(i, 4));
            for (size_t j = 0; j < bytes.size(); j++)
            {
                // %02x prints one byte as exactly two lowercase hex digits.
                sprintf(buf, "%02x", bytes[j] & 255);
                hex += buf;
            }
        }
        printf("%s\n", hex.c_str());
    }
    return 0;
}
