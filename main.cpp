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


int main() {
    ios::sync_with_stdio(false);
    string line, out;
    while (getline(cin, line)) {
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n')) {
            line.erase(line.size() - 1);
        }
        if (line.empty()) continue;
        size_t sp = line.find(' ');
        string cmd = (sp == string::npos) ? line : line.substr(0, sp);
        string arg = (sp == string::npos) ? string("") : line.substr(sp + 1);
        if (cmd == "CHAR") {
            char *end = NULL;
            long n = strtol(arg.c_str(), &end, 10);
            if (arg.empty() || *end != '\0') {
                out += "INVALID\n";
                continue;
            }
            char ch = alphabetChar(n);
            if (ch == 0) {
                out += "INVALID\n";
            } else {
                out += ch;
                out += '\n';
            }
        } else if (cmd == "VAL") {
            int v = (arg.size() == 1) ? alphabetValue(arg[0]) : -1;
            if (v < 0) out += "INVALID\n";
            else {
                out += to_string(v);
                out += '\n';
            }
        }
    }
    cout << out;
    return 0;
}
