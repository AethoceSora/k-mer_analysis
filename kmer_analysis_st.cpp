// C++程序实现 k-mer 分析

#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

// 定义 KmerMap 类型，用于保存 k-mer 信息和出现次数
typedef map<string, int> KmerMap;

// 函数定义：生成 k-mer
string GenerateKmer(string seq, int k)
{
    string kmer;
    for (int i = 0; i <= seq.length() - k; i++)
    {
        kmer = seq.substr(i, k);
        // 如果 k-mer 中含有其他字符，比如空格、回车等，则跳过这个 k-mer
        if (kmer.find(' ') != string::npos || kmer.find('\r') != string::npos || kmer.find('\n') != string::npos)
        {
            continue;
        }
        // 如果 k-mer 中含有小写字母，则转为大写字母
        for (int j = 0; j < kmer.length(); j++)
        {
            if (kmer[j] >= 'a' && kmer[j] <= 'z')
            {
                kmer[j] += 'A' - 'a';
            }
        }
        return kmer;
    }
    return ""; // 如果没有找到 k-mer，则返回空字符串
}

// 函数定义：统计 k-mer 数量
void CountKmer(string seq, int k, KmerMap &kmerMap)
{
    string kmer;
    for (int i = 0; i <= seq.length() - k; i++)
    {
        kmer = seq.substr(i, k);
        // 如果 k-mer 中含有其他字符，比如空格、回车等，则跳过这个 k-mer
        if (kmer.find(' ') != string::npos || kmer.find('\r') != string::npos || kmer.find('\n') != string::npos)
        {
            continue;
        }
        // 如果 k-mer 中含有小写字母，则转为大写字母
        for (int j = 0; j < kmer.length(); j++)
        {
            if (kmer[j] >= 'a' && kmer[j] <= 'z')
            {
                kmer[j] += 'A' - 'a';
            }
        }
        kmerMap[kmer]++; // 在 kmerMap 中统计该 k-mer 的出现次数
    }
}

int main(int argc, char *argv[])
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    if (argc < 3)
    {
        cout << "Usage: kmer_analysis <FASTA file> <k>" << endl;
        return 0;
    }

    // 读取命令行参数
    string fastaFileName(argv[1]);
    int k = atoi(argv[2]); // 将命令行参数中的 k 转为整数

    // 打开 FASTA 格式文件进行 k-mer 分析
    ifstream fastaFile(fastaFileName.c_str());
    if (!fastaFile.is_open())
    {
        cout << "Error: Unable to open file " << fastaFileName << endl;
        return 0;
    }

    // 定义 kmerMap，用于保存 k-mer 信息和出现次数
    KmerMap kmerMap;

    // 读取序列数据并分析 k-mer 出现次数
    string line;
    string seq = "";
    while (getline(fastaFile, line))
    {
        // 跳过注释行
        if (line[0] == '>')
        {
            if (seq != "")
            {
                CountKmer(seq, k, kmerMap); // 对上一条序列进行 k-mer 分析
                seq = "";
            }
        }
        else
        {
            seq += line;
        }
    }
    if (seq != "")
    {
        CountKmer(seq, k, kmerMap); // 分析最后一条序列的 k-mer
    }

    // 输出结果
    for (KmerMap::iterator it = kmerMap.begin(); it != kmerMap.end(); ++it)
    {
        cout << it->first << "\t" << it->second << endl;
    }

    return 1;
}
