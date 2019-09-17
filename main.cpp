#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <chrono>

using namespace std;

string txt_to_string(const string filename);
int BF(const string text, const string pattern); // brute force
int KMP(const string text, const string pattern); // knuth-morris-pratt
int* failure_function(const string pattern); // failure function for KMP
int BM(const string text, const string pattern); // boyer-moore
int RK(const string text, const string pattern); // rabin-karp
bool prime(const unsigned int x);
unsigned int randomPrime(const unsigned int lowerLimit);
int* badCharTable(const string pattern);

int main(int argc, char *argv[])
{
    string text = txt_to_string(argv[1]);
    string pattern = txt_to_string(argv[2]);

    chrono::time_point< chrono::system_clock > startTime;
    chrono::duration< double, milli > elapsedTime;

    int result;

    unsigned int n = text.length();
    unsigned int m = pattern.length();

    cout << "Text Lenght: " << n << endl;
    cout << "Pattern Lenght: " << m << endl;
    cout << endl;

    // Brute Force

    startTime = chrono::system_clock::now();

    result = BM(text,pattern);

    elapsedTime = std::chrono::system_clock::now() - startTime;

    cout << "Brute Force:\n";

    if(result>-1)
        cout << "Found at index " << result << endl;
    else
        cout << "No match found\n";

    cout << "Elapsed time: " << elapsedTime.count() << " ms\n";

    cout << endl;

    // Knuth-Morris-Pratt

    startTime = chrono::system_clock::now();

    result = KMP(text,pattern);

    elapsedTime = std::chrono::system_clock::now() - startTime;

    cout << "Knuth-Morris-Pratt:\n";

    if(result>-1)
        cout << "Found at index " << result << endl;
    else
        cout << "No match found\n";

    cout << "Elapsed time: " << elapsedTime.count() << " ms\n";

    cout << endl;

    // Boyer-Moore

    startTime = chrono::system_clock::now();

    result = BM(text,pattern);

    elapsedTime = std::chrono::system_clock::now() - startTime;

    cout << "Boyer-Moore:\n";

    if(result>-1)
        cout << "Found at index " << result << endl;
    else
        cout << "No match found\n";

    cout << "Elapsed time: " << elapsedTime.count() << " ms\n";

    cout << endl;

    // Rabin-Karp

    startTime = chrono::system_clock::now();

    result = RK(text,pattern);

    elapsedTime = std::chrono::system_clock::now() - startTime;

    cout << "Rabin-Karp:\n";

    if(result>-1)
        cout << "Found at index " << result << endl;
    else
        cout << "No match found\n";

    cout << "Elapsed time: " << elapsedTime.count() << " ms\n";

    return 0;
}

string txt_to_string(const string filename)
{
    ifstream infile(filename);
    string result = "";
    string parser;

    while(std::getline(infile,parser))
    {
        result += parser;
    }

    return result;
}

int BF(const string text, const string pattern)
{
    unsigned int i = 0;

    while(i <= text.length() - pattern.length())
    {
        unsigned int j = 0;

        while( j < pattern.length())
        {
            if(text[i+j] == pattern[j])
                j++;

            else
                break;
        }

        if( j == pattern.length())
            return i+1;
        else
            i++;
    }

    return -1;
}

int* failure_function(const string pattern)
{
    int* F = new int[pattern.length()];
    F[0] = 0;
    unsigned int i = 1;
    unsigned int j = 0;

    while(i<pattern.length())
    {
        if(pattern[i] == pattern[j])
        {
            F[i] = j+1;
            i++;
            j++;
        }
        else if(j>0)
            j = F[j-1];
        else
        {
            F[i] = 0;
            i++;
        }
    }

    return F;
}

int KMP(const string text, const string pattern)
{
    int* F = failure_function(pattern);

    unsigned int i = 0;
    unsigned int j = 0;

    int result = -1;

    while(i < text.length())
    {
        if(text[i] == pattern[j])
        {
            if(j == pattern.length()-1)
            {
                result = i-j+1;
                break;
            }
            else
            {
                i++;
                j++;
            }
        }
        else
        {
            if(j>0)
                j = F[j-1];
            else
                i++;
        }
    }

    delete F;
    F = NULL;

    return result;
}

bool prime(const unsigned int x)
{
    for(unsigned i=2; i<=x/2; i++)
    {
        if(x%i==0)
            return false;
    }
    return true;
}

unsigned int randomPrime(const unsigned int lowerLimit)
{
    while(true)
    {
        unsigned int x = lowerLimit;
        if(prime(x))
            return x;
        x++;
    }
}

int mod (int a, int b)
{
   int x = a % b;
   if(x < 0)
     x+=b;
   return x;
}

int RK(const string text, const string pattern)
{
    unsigned int n = text.length();
    unsigned int m = pattern.length();

    int q = randomPrime(m);
    int c = 1;
    for(unsigned int i=0; i<m-1; i++)
        c = c*10;
    c = mod(c,q);

    int fp = 0;
    int ft = 0;

    for(unsigned int i=0; i<m; i++)
    {
        fp = (10*fp + pattern[i]);
        fp = mod(fp,q);
        ft = (10*ft + text[i]);
        ft= mod(ft,q);
    }

    for(unsigned int s=0; s<=n-m; s++)
    {
        if(fp==ft)
        {
            if(pattern.compare(text.substr(s,m))==0)
                return s+1;
        }

        ft = mod(((ft-text[s]*c)*10+text[s+m]),q);
    }

    return -1;
}

int* badCharTable(const string pattern)
{
    int* table = new int[256];

    for(int i=0; i<256; i++)
        table[i] = -1;

    for(unsigned int i=0; i<pattern.length(); i++)
    {
        table[(int)pattern[i]] = i;
    }

    return table;
}

int max(int x, int y)
{
    return (x>y) ? x : y;
}

int BM(const string text, const string pattern)
{
    int n = text.length();
    int m = pattern.length();

    int* table = badCharTable(pattern);

    int result = -1;
    int shift = 0;

    while(shift <= n-m)
    {
        int j = m-1;

        while(j>=0 && pattern[j]==text[shift+j])
            j--;

        if(j<0)
        {
            result = shift+1;
            break;
        }

        else
            shift += max(1, j - table[(int)text[shift+j]]);

    }

    delete table;
    table = NULL;

    return result;
}








