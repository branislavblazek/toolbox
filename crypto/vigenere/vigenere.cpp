// make vigenere && ./vigenere
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
using namespace std;

constexpr int M = 26; // A-Z

int ctoi(char c) 
{
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a';
    return M;
}

char itoc(int n)
{
    if (n >= 0 && n <= 25) return 'A' + n;
    return ' ';
}

int mod(int x)
{
    return ((x % M) + M) % M;
}

int add(int a, int b)
{
    return mod(a + b);
}

int sub(int a, int b)
{
    return mod(a - b);
}

vector<string> split_text(const string& text, int n)
{
    vector<string> groups(n);

    for (size_t i = 0; i < text.size(); i++)
        groups[i % n] += text[i];

    return groups;
}

array<double, M> get_frequencies(const string& text)
{
    array<double, M> frequencies{};
    int count = 0;

    for (char letter : text)
    {
        int i = ctoi(letter);
        if (i < M) 
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

char get_coincidence_index(const string& text, const string& lang)
{
    static constexpr double sk_letter_frequencies[M] = {0.11, 0.01, 0.04, 0.04, 0.08, 0.0, 0.0, 0.02, 0.08, 0.02, 0.04, 0.04, 0.03, 0.07, 0.1, 0.03, 0.0, 0.05, 0.06, 0.05, 0.03, 0.05, 0.0, 0.0, 0.03, 0.03};
    static constexpr double en_letter_frequencies[M] = {0.08, 0.01, 0.03, 0.04, 0.12, 0.02, 0.02, 0.06, 0.07, 0.0, 0.01, 0.04, 0.03, 0.07, 0.08, 0.02, 0.0, 0.06, 0.06, 0.09, 0.03, 0.01, 0.02, 0.0, 0.02, 0.0};
    
    const double* lang_frequencies = lang == "sk" ? sk_letter_frequencies : en_letter_frequencies;
    const array<double, M> text_frequencies = get_frequencies(text);

    double scores[M]{};

    for (int i = 0; i < M; i++)
    {
        double score = 0;

        for (int j = 0; j < M; j++)
        {
            score += lang_frequencies[j] * text_frequencies[(i + j) % M];
        }

        scores[i] = score;
    }

    double best_value = 0;
    int best_letter_index = 0;

    for (int i = 0; i < M; i++) {
        if (scores[i] > best_value) {
            best_value = scores[i];
            best_letter_index = i;
        }
    }

    return best_letter_index;
}

vector<int> vigenere_trigram_distances(const string& ciphertext)
{
    unordered_map<string, vector<int>> trigram_positions;
    vector<int> distances;

    for (int i = 0; i < ciphertext.size() - 2; i++)
    {
        string trigram = ciphertext.substr(i, 3);
        trigram_positions[trigram].push_back(i);
    }

    for (const auto& pair : trigram_positions)
    {
        const string& trigram = pair.first;
        const vector<int>& positions = pair.second;

        if (positions.size() < 2) continue;

        for (int i = 0; i < positions.size() - 1; i++)
        {
            int distance = positions[i + 1] - positions[i];
            distances.push_back(distance);
        }
    }

    return distances;
}

int vigenere_guess_key_length(const vector<int>& distances)
{
    const int min_key_length = 3;
    const int max_key_length = 25;
    vector<int> scores(max_key_length + 1, 0);

    for (int distance : distances)
    {
        for (int i = min_key_length; i <= max_key_length; i++)
        {
            if (distance % i == 0)
                scores[i]++;
        }
    }

    int best_score = 0;
    int best_key_length = 0;

    for (int i = min_key_length; i <= max_key_length; i++)
    {
        if (scores[i] > best_score) 
        {
            best_score = scores[i];
            best_key_length = i;
        }
    }

    return best_key_length;
}

string vigenere_crack_key(const string& ciphertext, int password_length)
{
    vector<string> groups = split_text(ciphertext, password_length);
    string result = "";

    for (const string& g : groups) {
        char index = get_coincidence_index(g, "sk");
        result += itoc(index);
    }

    return result;
}

string vigenere_encrypt(const string& key, const string& plaintext)
{
    int pwdIndex = 0;
    int pwdLen = key.size();
    string result = "";

    for (const char c : plaintext)
    {
        if (c < 'A' || c > 'Z')
        {
            result += c;
            continue;
        }

        char cipher_n = add(ctoi(c), ctoi(key[pwdIndex]));
        pwdIndex = (pwdIndex + 1) % pwdLen;
        result += itoc(cipher_n);
    }

    return result;
}

string vigenere_decrypt(const string& key, const string& ciphertext)
{
    int pwdIndex = 0;
    int pwdLen = key.size();
    string result = "";

    for (const char c : ciphertext)
    {
        if (c < 'A' || c > 'Z')
        {
            result += c;
            continue;
        }

        char plain_n = sub(ctoi(c), ctoi(key[pwdIndex]));
        pwdIndex = (pwdIndex + 1) % pwdLen;
        result += itoc(plain_n);
    }

    return result;
}

int main()
{
    string plaintext = "TOTO JE TAJNA SPRAVA";
    string ciphertext = "";
    string key = "HESLO";

    ciphertext = vigenere_encrypt(key, plaintext);
    cout << ciphertext << "\n";

    plaintext = vigenere_decrypt(key, ciphertext);
    cout << plaintext << "\n--\n";

    // TASK: Break the code!
    ciphertext              = "UYCKTLTAERJNNJBDAUOUYGIJNNZRCRSQOHGUOCSWSRSQOQOIYRODRJHNPYOEDLDXDVPWOZHRVFGTYACEJLRWOAZRVFQQZUOTOCFNFLFNNJBNHVHNXAIERVNWYJVTOKCEYJVJBLQNDHQQTLZNGYOONHHNLLUAAMBJSTSMZLFXUHGLIPZJTPBDRJHNPYOEDLDXDVPWOZHRDCCSIJOSTYCSIJNWARCENHNJKSOMETSAAUWWACFQNPHNNHVXDUMPEUSAAACASSCEDHBNHVXJZFYJ";
    vector<int> distances   = vigenere_trigram_distances(ciphertext);
    int key_length          = vigenere_guess_key_length(distances);
    key                     = vigenere_crack_key(ciphertext, key_length);
    plaintext               = vigenere_decrypt(key, ciphertext);
    // key = AHOJ
    // plaintext = UROBTE FREKVENCNU ANALYZU ANGLICKEHO A SLOVENSKEHO JAZYKA URCTE PRAVDEPODOBNOSTI VYSKYTOV JEDNOTLIVYCH ZNAKOV REFERENCNEHO TEXTU V ROZNYCH KODOVYCH ABECEDACH TELEGRAF NA TELEGRAF NA S MEDZEROU ASCII LATIN URCTE PRAVDEPODOBNOSTI DVOJIC AJ TROJIC ZNAKOV NA ZAKLADE MERANI NAVRHNITE NAHODNY GENERATOR SLOV DANEHOJAZYKA

    cout << plaintext << "\n";
    
    return 0;
}