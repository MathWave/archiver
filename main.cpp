#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>
#include <string>
#include <ctime>

struct node
{
    char letter;
    int amount;
    node* left;
    node* right;
    node(char let, int a, node* l, node* r) {letter = let; amount = a; left = l; right = r;}
};

typedef node* pnode;

int Position(std::vector<pnode> d, char letter)
{
    for (int i = 0; i < d.size(); i++)
        if (d[i]->letter == letter)
            return i;
    return -1;
}

bool Compare(pnode f, pnode s)
{
    return f->amount > s->amount;
}

std::string GetCode(pnode& root, std::string s, char &c)
{
    if (root->left)
        return GetCode(root->left, s + '0', c);
    else if (root->right)
        return GetCode(root->right, s + '1', c);
    else
    {
        c = root->letter;
        root = NULL;
        return s;
    }
}

int Power(int a)
{
    int res = 1;
    for (int i = 0; i < a; i++)
        res *= 2;
    return res;
}

unsigned char Byte(std::string s)
{
    unsigned char b = 0;
    for (int i = 0; i < 8; i++)
        if (s[i] == '1')
            b += Power(7 - i);
    return b;
}

std::string FromBinaryStringIntoByteString(std::string binary)
{
    std::string buffer = "";
    std::string result = "";
    for (int i = 0; i < binary.size() / 8; i++)
    {
        for (int j = 0; j < 8; j++)
            buffer += binary[i * 8 + j];
        result += Byte(buffer);
        buffer = "";
    }
    return result;
}

//Функция архивации

void EnCode()
{
    std::ifstream fin("Откуда берем");
    std::ofstream fout("Куда сохраняем");
    std::vector<std::string> full_string;
    std::string buff;
    std::vector<pnode> letters;
    while (getline(fin, buff))
        full_string.push_back(buff + '\n');
    full_string.back().pop_back();
    for (int i = 0; i < full_string.size(); i++)
        for (int j = 0; j < full_string[i].size(); j++)
        {
            int pos = Position(letters, full_string[i][j]);
            if (pos == -1)
            {
                pnode p = new node (full_string[i][j], 1, nullptr, nullptr);
                letters.push_back(p);
            }
            else
                letters[pos]->amount++;
        }
    while (letters.size() != 1)
    {
        std::sort(letters.begin(), letters.end(), Compare);
        pnode p = new node (NULL, letters[letters.size() - 1]->amount + letters[letters.size() - 2]->amount, letters[letters.size() - 2], letters[letters.size() - 1]);
        letters[letters.size() - 2] = p;
        letters.pop_back();
    }
    pnode tree = letters[0];
    std::map<char, std::string> dictionary;
    while (tree)
    {
        char c = NULL;
        std::string s = "";
        s = GetCode(tree, s, c);
        if (c)
            dictionary.insert({c, s});
    }
    for (auto it = dictionary.begin(); it != dictionary.end(); it++)
    {
        if (it->first == '\n')
            fout << "enter " << it->second << std::endl;
        else
            fout << it->first << " " << it->second << std::endl;
    }
    fout << "__";
    std::string binary_string = "";
    for (int i = 0; i < full_string.size(); i++)
        for (int j = 0; j < full_string[i].size(); j++)
            binary_string += dictionary[full_string[i][j]];
    int t = binary_string.size() % 8;
    if (t)
        for (int i = 0; i < 8 - t; i++)
            binary_string += '0';
    if (t)
        fout << 8 - t;
    fout << std::endl << FromBinaryStringIntoByteString(binary_string);
    fout.close();
}

std::vector<std::string> Split(std::string s)
{
    std::vector<std::string> v;
    std::string buffer = "";
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] != ' ')
            buffer += s[i];
        else
        {
            v.push_back(buffer);
            buffer = "";
        }
    }
    v.push_back(buffer);
    return v;
}

std::string FromCharIntoBinary(unsigned char c)
{
    std::string s = "";
    while (c > 0)
    {
        if (c % 2)
            s = '1' + s;
        else
            s = '0' + s;
        c /= 2;
    }
    int r = s.size() % 8;
    if (r or !s.size())
        for (int i = 0; i < 8 - r; i++)
            s = '0' + s;
    return s;
}

std::string FromStringIntoBinary(std::string s)
{
    std::string binary = "";
    for (int i = 0; i < s.size(); i++)
        binary += FromCharIntoBinary(s[i]);
    return binary;
}

//Функция извлечения

void DeCode()
{
    std::ifstream fin("Откуда берем");
    std::ofstream fout("Куда сохраняем");
    std::map<std::string, char> reversed_dictionary;
    std::string buffer;
    int t = 0;
    while (1)
    {
        getline(fin, buffer);
        std::vector<std::string> v = Split(buffer);
        if (v.size() == 1)
        {
            if (v[0].size() != 2)
                t = v[0].back() - 48;
            break;
        }
        else if (v.size() == 3)
            reversed_dictionary.insert({v[2], ' '});
        else if (v[0] == "enter")
            reversed_dictionary.insert({v[1], '\n'});
        else
            reversed_dictionary.insert({v[1], v[0][0]});
    }
    std::string full_string = "";
    while (getline(fin, buffer))
        full_string += FromStringIntoBinary(buffer + '\n');
    full_string.pop_back();
    for (int i = 0; i < t; i++)
        full_string.pop_back();
    int pointer = 0;
    while (pointer < full_string.size())
    {
        std::string buffer = "";
        while (!reversed_dictionary[buffer] and pointer < full_string.size())
        {
            buffer += full_string[pointer];
            pointer++;
        }
        fout << reversed_dictionary[buffer];
        buffer = "";
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    int start = time(0);
    EnCode();
    int end = time(0);
    std::cout << end - start << " seconds for Encoding" << std::endl;
    DeCode();
    std::cout << time(0) - end << " seconds for Decoding" << std::endl;
}
