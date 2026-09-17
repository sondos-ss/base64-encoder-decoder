#include <bits/stdc++.h>
using namespace std;

// b64_size stores, for an input of n bytes, the character count in chars and
// the number of '=' characters in pads.
//
// TODO: chars = 4 characters for every 3-byte group, with the last partial
// group rounded UP to a whole group; pads = 0, 2 or 1, decided by n % 3.
// C++ hint: / on long long is already integer division and % is the
// remainder, so no floating point is needed.
static void b64_size(long long n, long long &chars, long long &pads) {
    (void)n;
    chars=4*((n+2)/3);
    pads=(n%3==0?0:3-n%3);
}

int main() {
    ios::sync_with_stdio(false);
    string line;
    while (getline(cin, line)) {
        size_t a = line.find_first_not_of(" \t\r\n");
        if (a == string::npos) continue;
        size_t b = line.find_last_not_of(" \t\r\n");
        long long n = atoll(line.substr(a, b - a + 1).c_str());
        long long chars = 0, pads = 0;
        b64_size(n, chars, pads);
        cout << chars << " " << pads << "\n";
    }
    return 0;
}
