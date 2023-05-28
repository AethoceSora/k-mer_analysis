// C++11 多线程程序实现 k-mer 分析

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

// 定义 KmerMap 类型，用于保存 k-mer 信息和出现次数
typedef map<string, int> KmerMap;

// 全局变量定义：互斥锁和 KmerMap
KmerMap kmerMap;
mutex m;

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
void CountKmer(string seq, int k)
{
    string kmer;
    KmerMap localMap; // 本地 KmerMap，用于保存局部统计结果
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
        localMap[kmer]++; // 在本地 KmerMap 中统计该 k-mer 的出现次数
    }
    // 将本地 KmerMap 中的统计结果合并到全局 KmerMap 中
    lock_guard<mutex> lock(m); // 加锁，防止多个线程同时修改全局 KmerMap
    for (KmerMap::iterator it = localMap.begin(); it != localMap.end(); ++it)
    {
        kmerMap[it->first] += it->second;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cout << "Usage: kmer_analysis <FASTA file> <k> <threads>" << endl;
        return 0;
    }

    // 读取命令行参数
    string fastaFileName(argv[1]);
    int k = atoi(argv[2]);          // 将命令行参数中的 k 转为整数
    int numThreads = atoi(argv[3]); // 将命令行参数中的线程数转为整数

    // 打开 FASTA 格式文件进行 k-mer 分析
    ifstream fastaFile(fastaFileName.c_str());
    if (!fastaFile.is_open())
    {
        cout << "Error: Unable to open file " << fastaFileName << endl;
        return 0;
    }

    // 读取序列数据
    vector<string> seqList; // 保存所有的序列数据
    string line;
    string seq = "";
    while (getline(fastaFile, line))
    {
        if (line[0] == '>')
        {
            if (seq != "")
            {
                seqList.push_back(seq); // 将上一条序列加入序列列表中
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
        seqList.push_back(seq); // 将最后一个序列加入序列列表中
    }

    // 启动多个线程进行 k-mer 分析
    vector<thread> threads;
    int numSeqsPerThread = seqList.size() / numThreads; // 计算每个线程要处理的序列数
    for (int i = 0; i < numThreads; i++)
    {
        int startIndex = i * numSeqsPerThread;
        int endIndex = (i == numThreads - 1) ? seqList.size() : (i + 1) * numSeqsPerThread;
        threads.push_back(thread([&](int start, int end)
                                 {
            for (int j = start; j < end; j++) {
                CountKmer(seqList[j], k);
            } },
                                 startIndex, endIndex));
    }

    // 等待所有线程完成
    for (int i = 0; i < numThreads; i++)
    {
        threads[i].join();
    }

    // 输出结果
    for (KmerMap::iterator it = kmerMap.begin(); it != kmerMap.end(); ++it)
    {
        cout << it->first << "\t" << it->second << endl;
    }

    return 1;
}
