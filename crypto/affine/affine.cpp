// make affine && ./affine
#include <iostream>
#include <string>
using namespace std;

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

int add(int a, int b)
{
    return (a + b) % 27;
}

int mul(int a, int b)
{
    int r = (a * b) % 27;

    return r < 0 ? r + 27 : r;
}

int inv(int a)
{
    for (int i = 0; i < 27; i++)
    {
        if (mul(a, i) == 1) return i;
    }

    return 0;
}

string affine_encrypt(int k1, int k2, string plaintext)
{
    // y = x * k1 + k2 mod 27
    string result = "";
    
    for (int i = 0; i < plaintext.length(); i++)
    {
        int local_n = ctoi(plaintext[i]);
        int cipher_n = add(mul(k1, local_n), k2);
        result += itoc(cipher_n);
    }

    return result;
}

string affine_decrypt(int k1, int k2, string ciphertext)
{
    // x = k1' * (y - k2) mod 27
    string plaintext = "";
    int k1c = inv(k1);

    for (int i = 0; i < ciphertext.length(); i++)
    {
        int local_n = ctoi(ciphertext[i]);
        int plain_n = mul(k1c, local_n - k2);
        plaintext += itoc(plain_n);
    }

    return plaintext;
}

int main() {
    string plaintext = "TOTO JE TAJNA SPRAVA";
    string ciphertext = "";

    int k1 = 5;
    int k2 = 15;

    ciphertext = affine_encrypt(k1, k2, plaintext);
    cout << ciphertext << "\n";

    string original = affine_decrypt(k1, k2, ciphertext);
    cout << original << "\n\n\n";

    // TASK: break the code!
    string code = "LIYGTOGDPOAUPDFQNVPVDAQV";
    int possible_k1[18] = {1, 2, 4, 5, 7, 8, 10, 11, 13, 14, 16, 17, 19, 20, 22, 23, 25, 26};

    // for (int k1 : possible_k1) {
    //     for (int k2 = 0; k2 <= 27; k2++) {
    //         cout << affine_decrypt(k1, k2, code) << " " << k1 << " " << k2 << "\n";
    //     }
    // }

    // k1 = 17, k2 = 5
    // plaintext = VYRIESIL SOM LAHKU ULOHU
    cout << affine_decrypt(17, 5, code) << "\n";
    
    return 0;
}
