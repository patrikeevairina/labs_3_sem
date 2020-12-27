#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>
#include <vector>

using namespace std;

mutex forks[5];
mutex cout_mutex;

void print_msg(const string& msg)
{
    cout_mutex.lock();
    cout << msg << flush;
    cout_mutex.unlock();
}


void think(size_t id)
{
    ostringstream ostr;
    ostr << id << " is thinking about" << endl;
    print_msg(ostr.str());
    this_thread::sleep_for(chrono::seconds(rand()%5));
}
void got_hungry(size_t id)
{
    ostringstream ostr;
    ostr << id << " got hungry" << endl;
    print_msg(ostr.str());
}
void get_forks(size_t id, mutex& lf, mutex& rf)
{
    while(!lf.try_lock()) this_thread::yield();
    {
        ostringstream ostr;
        ostr << id << " got left fork! " << endl;
        print_msg(ostr.str());
    }

    // wait for left fork
    while(!rf.try_lock()) this_thread::yield();
    {
        ostringstream ostr;
        ostr << id << " got right fork! " << endl;
        print_msg(ostr.str());
    }
}
void eat(size_t id)
{
    {
        ostringstream ostr;
        ostr << id << " can eat!" << endl;
        print_msg(ostr.str());
    }
    this_thread::sleep_for(chrono::seconds(1));

    forks[ id ].unlock();
    forks[ id==0 ? 4 : id-1 ].unlock();

    {
        ostringstream ostr;
        ostr << id << " put forks on the table" << endl;
        print_msg(ostr.str());
    }
}
void Phil(size_t idx)

{
    mutex& left_fork  = forks[ idx==0 ? 4 : idx-1 ];
    mutex& right_fork = forks[ idx ];
    while(true)
    {
        think(idx);
        got_hungry(idx);
        get_forks(idx, left_fork, right_fork);
        eat(idx);
    }
}

int main()
{
    srand(time(0));
    vector<thread> philosophers;
    philosophers.emplace_back(thread(Phil,0));
    philosophers.emplace_back(thread(Phil,1));
    philosophers.emplace_back(thread(Phil,2));
    philosophers.emplace_back(thread(Phil,3));
    philosophers.emplace_back(thread(Phil,4));

    for(auto &t : philosophers) t.join();

    return 0;
}
