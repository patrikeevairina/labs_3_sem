#include <iostream>
#include <future>
#include <thread>
#include <vector>
using namespace std;

#define SIZE_BITS 100000000

bool bits[SIZE_BITS];
int countBits(const int threads)
{
    vector<future<int>> s;
    long long count = 0;

    for(int i=0; i<threads; i++)
    {
        s.push_back(std::async(std::launch::async, [=]()
        {

            int c = 0;
            for (int j = i * SIZE_BITS / threads; j < SIZE_BITS * (i + 1) / threads; j++)
            {
                if (!bits[j])
                    c++;
            }
            return c;
        }));
    }

    for(int i=0;i<threads;i++)
        s[i].wait();

    for(int i=0;i<threads;i++)
        count += s[i].get();
    return count;
}

void countTime(const int threads)
{
    auto start = chrono::steady_clock::now();
    int count = countBits(threads);
    auto end = chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout <<"Count of threads: " << threads <<" Count: " << count <<" Elapsed time: " << elapsed_seconds.count() << "s" << endl;

}

int main()
{
    srand(time(nullptr));
    for(bool & bit : bits)
        bit = rand() % 2 == 1;
    for (int i=1; i<16;i++)
    {
        countTime(i);
    }
}
