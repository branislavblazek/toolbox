// single process start
// make CXXFLAGS=-O2 stream_rc4 && ./stream_rc4 -e -s 123 -in input.txt -out output.txt
// multi-process start (8)
// make -B CXXFLAGS=-O2 stream_rc4
// for k in 0 1 2 3 4 5 6 7; do
//     from=$((10000000 + k * 11250000))
//     ./stream_rc4 -in text1_enc.bin -from $from -to $((from + 11250000 - 1)) &
// done
// wait

#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
using namespace std;

uint8_t rc4_i, rc4_j;
array<uint8_t, 256> rc4_s;

array<uint8_t, 256> get_key(const string& passwd)
{
    // repeat the password to fill the whole 256-byte key
    array<uint8_t, 256> key;

    for (size_t i = 0; i < key.size(); i++)
    {
        key[i] = passwd[i % passwd.size()];
    }

    return key;
}

void rc4_init(const array<uint8_t, 256>& key)
{
    for (int i = 0; i < 256; i++)
    {
        rc4_s[i] = i;
    }

    for (int i = 0, j = 0; i < 256; i++)
    {
        j = (j + rc4_s[i] + key[i]) % 256;
        swap(rc4_s[i], rc4_s[j]);
    }

    rc4_i = 0;
    rc4_j = 0;
}

uint8_t rc4_rand()
{
    // returns random byte 0-255, uint8_t wraps around modulo 256 by itself
    rc4_i++;
    rc4_j += rc4_s[rc4_i];
    swap(rc4_s[rc4_i], rc4_s[rc4_j]);

    uint8_t t = rc4_s[rc4_i] + rc4_s[rc4_j];
    return rc4_s[t];
}

string rc4_encrypt(const string& passwd, const string& plaintext)
{
    rc4_init(get_key(passwd));
    string result = "";

    for (char p : plaintext)
    {
        result += (char) (p ^ rc4_rand()); // ^ is bitwise XOR
    }

    return result;
}

string rc4_decrypt(const string& passwd, const string& ciphertext)
{
    // XOR is its own inverse, so decryption is the same as encryption
    return rc4_encrypt(passwd, ciphertext);
}

bool looks_like_text(const string& passwd, const string& ciphertext, size_t n)
{
    rc4_init(get_key(passwd));
    int continuation = 0; // how many bytes we still expect

    for (size_t k = 0; k < n && k < ciphertext.size(); k++)
    {
        uint8_t b = (uint8_t) ciphertext[k] ^ rc4_rand();

        if (continuation > 0)
        {
            if (b < 0x80 || b > 0xBF) return false;
            continuation--;
        }
        else if (b >= 0x20 && b <= 0x7E) continue;          // printable ASCII
        else if (b == '\n' || b == '\r' || b == '\t') continue;
        else if (b >= 0xC2 && b <= 0xDF) continuation = 1;  // 2-byte char (á, č, ž, ...)
        else if (b >= 0xE0 && b <= 0xEF) continuation = 2;  // 3-byte char („, “, –, ...)
        else return false;
    }

    return continuation == 0;
}

bool read_file(const string& name, string& text)
{
    ifstream file(name, ios::binary);
    if (!file) return false;

    text.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
    return true;
}

bool write_file(const string& name, const string& text)
{
    ofstream file(name, ios::binary);
    if (!file) return false;

    file.write(text.data(), text.size());
    return (bool) file;
}

double seconds_since(chrono::steady_clock::time_point start)
{
    chrono::duration<double> elapsed = chrono::steady_clock::now() - start;
    return elapsed.count();
}

enum class Mode
{
    Encrypt,
    Decrypt
};

int main(int argc, char* argv[])
{
    auto start = chrono::steady_clock::now();
    Mode mode = Mode::Encrypt;
    string passwd = "123";
    string input_filename = "input.txt";
    string output_filename = "output.txt";
    long from = 10000000;
    long to = 99999999;

    for (int i = 1; i < argc; i++)
    {
        string param = argv[i];

        if (param == "-e") mode = Mode::Encrypt;
        else if (param == "-d") mode = Mode::Decrypt;
        else if (param == "-s" && i + 1 < argc) passwd = argv[++i];
        else if (param == "-in" && i + 1 < argc) input_filename = argv[++i];
        else if (param == "-out" && i + 1 < argc) output_filename = argv[++i];
        else if (param == "-from" && i + 1 < argc) from = stol(argv[++i]);
        else if (param == "-to" && i + 1 < argc) to = stol(argv[++i]);
    }

    if (passwd.empty())
    {
        cerr << "Password must not be empty\n";
        return 1;
    }

    string input;

    if (!read_file(input_filename, input))
    {
        cerr << "Cannot open input file: " << input_filename << "\n";
        return 1;
    }

    if (input.empty())
    {
        cerr << "Input text is empty\n";
        return 1;
    }

    for (long n = from; n <= to; n++)
    {
        string passwd = to_string(n);

        if (looks_like_text(passwd, input, 32))
        {
            cout << "TIME: " << seconds_since(start) << " s" << endl;
            cout << "PASSWORD: " << passwd << endl;
            cout << rc4_decrypt(passwd, input) << endl;
        }
    }
    // string output = mode == Mode::Encrypt ? rc4_encrypt(passwd, input) : rc4_decrypt(passwd, input);

    // output file is opened only after reading input, so -in and -out can be the same file
    // if (!write_file(output_filename, output))
    // {
    //     cerr << "Cannot write output file: " << output_filename << "\n";
    //     return 2;
    // }

    // TASK 1: 
    // TIME: 2.13921 s
    // PASSWORD: 12345678

    // TASK 2:
    // TIME: 0.474643 s
    // PASSWORD: 55555555

    // TASK 3:
    // TIME: 0.966634 s
    // PASSWORD: 89898989

    // TASK 4:
    // TIME: 5.76973 s
    // PASSWORD: 73081638

    return 0;
}
