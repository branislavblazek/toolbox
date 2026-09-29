// make affine && ./affine
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

constexpr int M = 27; // A-Z + space

int ctoi(char c) 
{
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a';
    return 26;
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

int mul(int a, int b)
{
    return mod(a * b);
}

int gcd(int a, int b)
{
    while (b != 0)
    {
        int t = a % b;
        a = b;
        b = t;
    }

    return a;
}

void check_key(int k1)
{
    // k1 must be coprime with M, otherwise it has no inverse
    if (gcd(mod(k1), M) != 1) throw invalid_argument("k1 must be coprime with " + to_string(M));
}

int inv(int a)
{
    check_key(a);

    for (int i = 0; i < M; i++)
    {
        if (mul(a, i) == 1) return i;
    }

    return 0;
}

string affine_encrypt(int k1, int k2, const string& plaintext)
{
    // y = x * k1 + k2 mod M
    check_key(k1);
    string result = "";
    
    for (char c : plaintext)
    {
        int local_n = ctoi(c);
        int cipher_n = add(mul(k1, local_n), k2);
        result += itoc(cipher_n);
    }

    return result;
}

string affine_decrypt(int k1, int k2, const string& ciphertext)
{
    // x = k1' * (y - k2) mod M
    string result = "";
    int k1c = inv(k1);

    for (char c : ciphertext)
    {
        int local_n = ctoi(c);
        int plain_n = mul(k1c, local_n - k2);
        result += itoc(plain_n);
    }

    return result;
}

int main() {
    string plaintext = "TOTO JE TAJNA SPRAVA";
    string ciphertext = "";

    int k1 = 5;
    int k2 = 15;

    ciphertext = affine_encrypt(k1, k2, plaintext);
    cout << ciphertext << "\n";

    string original = affine_decrypt(k1, k2, ciphertext);
    cout << original << "\n--\n";

    // TASK: break the code!
    string code = "LIYGTOGDPOAUPDFQNVPVDAQV";
    int possible_k1[18] = {1, 2, 4, 5, 7, 8, 10, 11, 13, 14, 16, 17, 19, 20, 22, 23, 25, 26};

    // for (int k1 : possible_k1) {
    //     for (int k2 = 0; k2 < M; k2++) {
    //         cout << affine_decrypt(k1, k2, code) << " " << k1 << " " << k2 << "\n";
    //     }
    // }

    // k1 = 17, k2 = 5
    // plaintext = VYRIESIL SOM LAHKU ULOHU
    cout << affine_decrypt(17, 5, code) << "\n";
    
    return 0;
}
