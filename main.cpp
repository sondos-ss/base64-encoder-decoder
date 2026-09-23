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
static int charValue(char c)
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
// Lesson 5: decode an already-validated string to lowercase hex. The shifts
// and masks here are lesson 5's; leave this function as it is.
static string decodeHex(const string &s)
{
    string out;
    char buf[8];
    for (size_t i = 0; i < s.size(); i += 4)
    {
        int pads = 0;
        long n = 0;
        for (int j = 0; j < 4; j++)
        {
            if (s[i + j] == '=')
                pads++;
        }
        for (int j = 0; j < 4; j++)
        {
            int v = (s[i + j] == '=') ? 0 : charValue(s[i + j]);
            n = (n << 6) | v;
        }
        int bytes[3] = {(int)((n >> 16) & 255), (int)((n >> 8) & 255), (int)(n & 255)};
        for (int j = 0; j < 3 - pads; j++)
        {
            sprintf(buf, "%02x", bytes[j]);
            out += buf;
        }
    }
    return out;
}

static string validate(const string &s)
{
    // TODO: return "ok", or the first failing word of length, char, padding,
    // bits, in that order. Hint: s.size() gives the count and charValue(c)
    // returns -1 for a character outside the alphabet.
    if (s.size() % 4 != 0)
    {
        return "length";
    }
    for (char c : s)
    {
        if (c != '=' && charValue(c) == -1)
            return "char";
    }

    int pads = 0;

    for (int i = s.size() - 1; i >= 0 && s[i] == '='; i--)
    {
        pads++;
    }

    int total = 0;

    for (char c : s)
    {
        if (c == '=')
            total++;
    }

    if (pads > 2 || total != pads)
        return "padding";
    if (pads > 0)
    {
        int pos = (int)s.size() - pads - 1;
        int v = charValue(s[pos]);

        if (pads == 1 && (v & 3) != 0)
            return "bits";

        if (pads == 2 && (v & 15) != 0)
            return "bits";
    }

    return "ok";
}

static const string STD = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const string URL = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
static const string HEXDIGITS = "0123456789abcdef";

// Lesson 2's character lookup: 0-63, or -1 when c is not in this alphabet.
int alphabetIndex(const string &alphabet, char c)
{
    size_t p = alphabet.find(c);
    return p == string::npos ? -1 : (int)p;
}

// Lesson 4's finished standard encoder: '+', '/', and '=' padding.
string encodeStd(const vector<unsigned char> &data)
{
    string out;
    size_t i = 0;
    while (i + 3 <= data.size())
    {
        unsigned long v = ((unsigned long)data[i] << 16) | ((unsigned long)data[i + 1] << 8) | (unsigned long)data[i + 2];
        out += STD[(v >> 18) & 63];
        out += STD[(v >> 12) & 63];
        out += STD[(v >> 6) & 63];
        out += STD[v & 63];
        i += 3;
    }
    size_t rest = data.size() - i;
    if (rest == 1)
    {
        unsigned long v = (unsigned long)data[i] << 16;
        out += STD[(v >> 18) & 63];
        out += STD[(v >> 12) & 63];
        out += "==";
    }
    else if (rest == 2)
    {
        unsigned long v = ((unsigned long)data[i] << 16) | ((unsigned long)data[i + 1] << 8);
        out += STD[(v >> 18) & 63];
        out += STD[(v >> 12) & 63];
        out += STD[(v >> 6) & 63];
        out += '=';
    }
    return out;
}

// Lesson 5's group decoder. text is a multiple of 4 with its '=' written out.
vector<unsigned char> decodeGroups(const string &text, const string &alphabet)
{
    vector<unsigned char> out;
    for (size_t i = 0; i < text.size(); i += 4)
    {
        unsigned long v = 0;
        int pads = 0;
        for (int j = 0; j < 4; j++)
        {
            char c = text[i + j];
            if (c == '=')
            {
                pads++;
                v <<= 6;
            }
            else
            {
                v = (v << 6) | (unsigned long)alphabetIndex(alphabet, c);
            }
        }
        unsigned char triple[3] = {(unsigned char)((v >> 16) & 255), (unsigned char)((v >> 8) & 255), (unsigned char)(v & 255)};
        for (int k = 0; k < 3 - pads; k++)
            out.push_back(triple[k]);
    }
    return out;
}

string toHex(const vector<unsigned char> &b)
{
    string s;
    for (size_t i = 0; i < b.size(); i++)
    {
        s += HEXDIGITS[b[i] >> 4];
        s += HEXDIGITS[b[i] & 15];
    }
    return s;
}

int hexVal(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return 0;
}

vector<unsigned char> fromHex(const string &s)
{
    vector<unsigned char> out;
    for (size_t i = 0; i + 1 < s.size(); i += 2)
        out.push_back((unsigned char)(hexVal(s[i]) * 16 + hexVal(s[i + 1])));
    return out;
}

// Base64url: the same bits, index 62 as '-', index 63 as '_', and no '=' at all.
string encodeUrl(const vector<unsigned char> &data)
{
    // TODO: substitute the two characters and strip the padding.
    // Hint: walk the result of encodeStd rewriting '+' and '/', then erase the
    // trailing '=' with a while loop on out.back().
    string out = encodeStd(data);
    for (char &c : out)
    {
        if (c == '+')
            c = '-';
        else if (c == '/')
            c = '_';
    }
    while (!out.empty() && out.back() == '=')
        out.pop_back();
    return out;
}

// Return the decoded bytes as lowercase hex, or the word INVALID, a space, then one reason word.
string decodeUrl(const string &text)
{
    // TODO: lesson 6's four rules in lesson 6's order (length, char, padding, bits)
    // against URL, then recover the dropped pads from text.size() % 4 and call
    // decodeGroups(data + string(pads, '='), URL), then toHex.
    if (text.size() % 4 == 1)
        return "INVALID length";
    for (char c : text)
    {
        if (c != '=' && alphabetIndex(URL, c) == -1)
            return "INVALID char";
    }
    int pads = 0;
    int total = 0;

    for (char c : text)
    {
        if (c == '=')
            total++;
    }

    for (int i = (int)text.size() - 1;
         i >= 0 && text[i] == '=';
         i--)
    {
        pads++;
    }

    if (total > 0)
    {
        if (pads < 1 || pads > 2 ||
            text.size() % 4 != 0 ||
            total != pads)
        {
            return "INVALID padding";
        }
    }
    else
    {
        int rem = text.size() % 4;

        if (rem == 0)
            pads = 0;
        else if (rem == 2)
            pads = 2;
        else
            pads = 1;
    }
    if (pads > 0)
    {
        int pos = (int)text.size() - pads - 1;
        int v = alphabetIndex(URL, text[pos]);

        if (pads == 1 && v % 4 != 0)
            return "INVALID bits";

        if (pads == 2 && v % 16 != 0)
            return "INVALID bits";
    }

    string data = text;

    if (total == 0)
        data += string(pads, '=');

    return toHex(decodeGroups(data, URL));
}

int main()
{
    ios::sync_with_stdio(false);
    string line;
    while (getline(cin, line))
    {
        while (!line.empty() && (line.back() == '\n' || line.back() == '\r'))
            line.pop_back();
        if (line.empty())
            continue;
        size_t sp = line.find(' ');
        if (sp == string::npos)
            continue;
        string cmd = line.substr(0, sp);
        string arg = line.substr(sp + 1);
        if (cmd == "ENCODE_URL")
            cout << encodeUrl(fromHex(arg)) << "\n";
        else if (cmd == "DECODE_URL")
            cout << decodeUrl(arg) << "\n";
    }
    return 0;
}
