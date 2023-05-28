#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <cstdlib> // 用于使用 exit 函数
using namespace std;

string read_fasta(string file_name)
{ // 读入 fasta 文件并将序列拼接起来
    ifstream file(file_name.c_str());
    if (!file)
    { // 如果文件打开失败，则退出程序
        cerr << "Error: cannot open file " << file_name << endl;
        exit(1);
    }
    string line, sequence = "";
    while (getline(file, line))
    {
        if (line[0] != '>')
        { // 去掉注释行
            sequence += line;
        }
    }
    file.close();
    return sequence;
}

void kmer(string sequence, int k)
{ // 计算每个 k-mer 出现的次数
    unordered_map<string, int> freq;
    int n = sequence.length() - k + 1;
    for (int i = 0; i < n; i++)
    {
        string kmer = sequence.substr(i, k);
        freq[kmer]++;
    }
    for (auto it = freq.begin(); it != freq.end(); it++)
    {
        cout << it->first << " " << it->second << endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    { // 如果参数数量不正确，则退出程序
        cerr << "Usage: " << argv[0] << " <input_file> <k>" << endl;
        exit(1);
    }
    string file_name = argv[1];
    int k = atoi(argv[2]); // 将字符串转换为整型
    string sequence = read_fasta(file_name);
    kmer(sequence, k);
    return 0;
}
