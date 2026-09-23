#include <bits/stdc++.h>
using namespace std;

static const string STD_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const string URL_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

static int valOf(const string &alphabet, char c)
{
    for (int i = 0; i < 64; i++)
    {
        if (alphabet[i] == c)
            return i;
    }
    return -1;
}

static string encodeB64(const vector<int> &data, const string &alphabet, bool writePadding)
{
    // TODO: whole 3-byte groups (lesson 3), then the 1- or 2-byte tail (lesson 4),
    // then '=' characters ONLY when writePadding is true.
    // data holds ints 0-255; alphabet[v] is the character for the 6-bit value v.
    string out;
    int i = 0;
    while (i + 3 <= data.size())
    {
        int n = (data[i] << 16) | (data[i + 1] << 8) | data[i + 2];
        out += alphabet[(n >> 18) & 63];
        out += alphabet[(n >> 12) & 63];
        out += alphabet[(n >> 6) & 63];
        out += alphabet[n & 63];
        i += 3;
    }
    int rem = data.size() - i;
    if (rem == 1)
    {
        int n = data[i] << 16;
        out += alphabet[(n >> 18) & 63];
        out += alphabet[(n >> 12) & 63];

        if (writePadding)
            out += "==";
    }
    else if (rem == 2)
    {
        int n = (data[i] << 16) | (data[i + 1] << 8);

        out += alphabet[(n >> 18) & 63];
        out += alphabet[(n >> 12) & 63];
        out += alphabet[(n >> 6) & 63];

        if (writePadding)
            out += '=';
    }

    return out;
}

/* returns "" on success (bytes go into out), otherwise the reason word */
static string decodeB64(const string &text, const string &alphabet, bool paddingOptional, vector<int> &out)
{
    // TODO: the four checks in order (length, char, padding, bits), then lesson 5's group
    // decoder. Fill out with the bytes and return "" on success, or return the reason word
    // on the first failed check. valOf(alphabet, ch) is the 6-bit value, or -1. When
    // paddingOptional is true, a length % 4 of 3 or 2 implies 1 or 2 dropped pads.
    out.clear();
    if (!paddingOptional && text.size() % 4 != 0)
        return "length";
    if (paddingOptional && text.size() % 4 == 1)
        return "length";
    for (char c : text)
    {
        if (c != '=' && valOf(alphabet, c) == -1)
            return "char";
    }
    int total = 0;
    for (char c : text)
    {
        if (c == '=')
            total++;
    }
    int pads = 0;
    for (int i = (int)text.size() - 1;i >= 0 && text[i] == '=';i--)
    {
        pads++;
    }

    if (total > 0)
    {
        if (pads < 1 || pads > 2 ||
            total != pads ||
            text.size() % 4 != 0)
        {
            return "padding";
        }
    }
    else if (paddingOptional)
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
        int pos;

        if (total > 0)
            pos = (int)text.size() - pads - 1;
        else
            pos = (int)text.size() - 1;

        int v = valOf(alphabet, text[pos]);

        if (pads == 1 && (v & 3))
            return "bits";

        if (pads == 2 && (v & 15))
            return "bits";
    }
    string data = text;

    if (total == 0)
        data += string(pads, '=');

    for (size_t i = 0; i < data.size(); i += 4)
    {
        int v0 = data[i] == '=' ? 0 : valOf(alphabet, data[i]);
        int v1 = data[i + 1] == '=' ? 0 : valOf(alphabet, data[i + 1]);
        int v2 = data[i + 2] == '=' ? 0 : valOf(alphabet, data[i + 2]);
        int v3 = data[i + 3] == '=' ? 0 : valOf(alphabet, data[i + 3]);
        int n = (v0 << 18) |
                (v1 << 12) |
                (v2 << 6) |
                v3;
        out.push_back((n >> 16) & 255);
        if (i + 4 == data.size() && pads > 0)
        {
            if (pads == 1)out.push_back((n >> 8) & 255);
        }
        else
        {
            out.push_back((n >> 8) & 255);
            out.push_back(n & 255);
        }
    }
    return "";
}

static vector<int> toBytes(const string &h)
{
    vector<int> d;
    for (size_t i = 0; i + 2 <= h.size(); i += 2)
    {
        d.push_back((int)strtol(h.substr(i, 2).c_str(), NULL, 16));
    }
    return d;
}

static string toHex(const vector<int> &data)
{
    string s;
    char buf[8];
    for (size_t i = 0; i < data.size(); i++)
    {
        sprintf(buf, "%02x", data[i] & 255);
        s += buf;
    }
    return s;
}

static string run(const string &cmd, const string &arg)
{
    if (cmd == "ENCODE")
        return encodeB64(toBytes(arg), STD_ALPHABET, true);
    if (cmd == "ENCODE_URL")
        return encodeB64(toBytes(arg), URL_ALPHABET, false);
    if (cmd == "DECODE" || cmd == "DECODE_URL")
    {
        bool url = (cmd == "DECODE_URL");
        vector<int> data;
        string reason = decodeB64(arg, url ? URL_ALPHABET : STD_ALPHABET, url, data);
        if (reason.empty())
            return toHex(data);
        return "INVALID " + reason;
    }
    if (cmd == "ROUNDTRIP")
    {
        vector<int> original = toBytes(arg);
        string enc = encodeB64(original, STD_ALPHABET, true);
        vector<int> back;
        string reason = decodeB64(enc, STD_ALPHABET, false, back);
        bool ok = reason.empty() && back == original;
        return enc + (ok ? " OK" : " FAIL");
    }
    return "INVALID char";
}

int main()
{
    string line;
    string out;
    while (getline(cin, line))
    {
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);
        if (line.empty())
            continue;
        size_t sp = line.find(' ');
        string cmd = (sp == string::npos) ? line : line.substr(0, sp);
        string arg = (sp == string::npos) ? string("") : line.substr(sp + 1);
        out += run(cmd, arg) + "\n";
    }
    fputs(out.c_str(), stdout);
    return 0;
}
