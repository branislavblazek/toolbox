// make stream && ./stream sprava_enc.txt [sprava.txt]
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <array>
using namespace std;

constexpr int M = 26; // A-Z
constexpr long KEY = 0; // random seed is the key
constexpr bool DECIPHER = true;

long randx;

void stream_seed(long s)
{
    randx = s;
}

float stream_rand()
{
    // returns random number in <0, 1)
    long a = 8121;
    long b = 28411;
    long c = 134456;
    randx = (a * randx + b) % c;

    return (float) randx / c;
}

int stream_next()
{
    // random number 0 to 25 with uniform distribution
    return (int) (M * stream_rand());
}

int ctoi(char c)
{
    return c - 'A';
}

char itoc(int n)
{
    return 'A' + n;
}

int mod(int x)
{
    return ((x % M) + M) % M;
}

string stream_encrypt(long key, const string& plaintext)
{
    stream_seed(key);
    string result = "";

    for (unsigned char uc : plaintext)
    {
        char c = toupper(uc);

        if (c < 'A' || c > 'Z')
        {
            result += c;
            continue;
        }

        result += itoc(mod(ctoi(c) + stream_next()));
    }

    return result;
}

string stream_decrypt(long key, const string& ciphertext)
{
    stream_seed(key);
    string result = "";

    for (unsigned char uc : ciphertext)
    {
        char c = toupper(uc);

        if (c < 'A' || c > 'Z')
        {
            result += c;
            continue;
        }

        result += itoc(mod(ctoi(c) - stream_next()));
    }

    return result;
}

string read_all(istream& in)
{
    return string(istreambuf_iterator<char>(in), istreambuf_iterator<char>());
}

array<double, M> get_frequencies(const string& text)
{
    array<double, M> frequencies{};
    int count = 0;

    for (char letter : text)
    {
        int i = ctoi(letter);
        if (i >= 0 && i < M) 
        {
            frequencies[i]++;
            count++;
        }
    }

    if (count > 0)
    {
        for (double& f : frequencies)
            f /= count;
    }

    return frequencies;
}

double get_score(const string& text)
{
    static constexpr double lang_frequencies[M] = {0.11, 0.01, 0.04, 0.04, 0.08, 0.0, 0.0, 0.02, 0.08, 0.02, 0.04, 0.04, 0.03, 0.07, 0.1, 0.03, 0.0, 0.05, 0.06, 0.05, 0.03, 0.05, 0.0, 0.0, 0.03, 0.03};
    const array<double, M> text_frequencies = get_frequencies(text);

    double score = 0;

    for (int j = 0; j < M; j++)
    {
        score += lang_frequencies[j] * text_frequencies[j];
    }


    return score;
}

int main(int argc, char* argv[])
{
    ifstream input_file;
    ofstream output_file;
    istream* input = &cin;
    ostream* output = &cout;

    if (argc >= 2)
    {
        input_file.open(argv[1]);
        if (!input_file)
        {
            cerr << "Cannot open input file: " << argv[1] << "\n";
            return 1;
        }
        input = &input_file;
    }

    if (argc >= 3)
    {
        output_file.open(argv[2]);
        if (!output_file)
        {
            cerr << "Cannot open output file: " << argv[2] << "\n";
            return 2;
        }
        output = &output_file;
    }

    string text = read_all(*input);
    // *output << (DECIPHER ? stream_decrypt(KEY, text) : stream_encrypt(KEY, text));

    string prefix = text.substr(0, 200);
    long best_key = 0;
    double best_score = 0;

    for (int i = 0; i < 134456; i++)
    {
        double score = get_score(stream_decrypt(i, prefix));

        if (score > best_score)
        {
            best_score = score;
            best_key = i;
        }
    }

    cout << "KEY: " << best_key << endl;
    cout << stream_decrypt(best_key, text) << endl;

    return 0;
}
