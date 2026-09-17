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
static void b64_size(long long n, long long &chars, long long &pads) {
    (void)n;
    chars=4*((n+2)/3);
    pads=(n%3==0?0:3-n%3);
}
// Return the alphabet character at index, or 0 when index is not 0..63.
static char alphabetChar(long index) {
    // TODO: range-check index first, then return ALPHABET at that position.
    // Hint: ALPHABET[i] on a std::string does no bounds checking; use your own
    // 0..63 test rather than hoping the string complains.
    (void)index;
    if(index>=0&&index<=63)return ALPHABET[index];
    return 0;  // the failure value; the driver below prints INVALID for it
}

// Return the 0..63 index of the character c, or -1 when c is not in ALPHABET.
static int alphabetValue(char c) {
    // TODO: find the position of c among the 64 characters.
    // Hint: a for loop from 0 to 63 comparing ALPHABET[i] to c is enough; '='
    // is not in ALPHABET, so it must come back as -1.
    (void)c;
    if(ALPHABET.find(c)!=string::npos)return ALPHABET.find(c);
    return -1;  // the failure value; the driver below prints INVALID for it
}


string encode_group(unsigned b0, unsigned b1, unsigned b2)
{
    // TODO: pack b0, b1, b2 into one 24-bit number, cut four 6-bit fields out of
    // it (most significant first) and return the four B64 characters.
    // C++ hint: B64[i] is the character at index i; use <<, >>, | and & 63.
    string s="";
    int n=(b0<<16)|(b1<<8)|b2;
    char f0=ALPHABET[(n>>18)&63];
    s+=f0;
    char f1=ALPHABET[(n>>12)&63];
    s+=f1;
    char f2=ALPHABET[(n>>6)&63];
    s+=f2;
    char f3=ALPHABET[n&63];
    s+=f3;
    return s;
}

int main()
{
    ios::sync_with_stdio(false);
    string line;
    while (getline(cin, line)) {
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);
        if (line.empty()) continue;
        vector<unsigned> data;
        for (size_t i = 0; i + 1 < line.size(); i += 2)
            data.push_back((unsigned)strtoul(line.substr(i, 2).c_str(), NULL, 16));
        string out;
        for (size_t i = 0; i + 2 < data.size(); i += 3)
            out += encode_group(data[i], data[i + 1], data[i + 2]);
        cout << out << "\n";
    }
    return 0;
}
