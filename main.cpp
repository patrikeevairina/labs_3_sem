#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <stack>

using namespace std;

static char cpu[30000];



struct Command
{
    int numOfRepeats;
    char com;
    Command()
        :numOfRepeats(1),com(0)
    {
    }
    Command (char a, int num = 1)
        :numOfRepeats(num), com(a)
    {
    }

};


void printVec(vector<Command> &vec)
{
    for (size_t i = 0; i < vec.size(); ++i)
    {
        //cout << "i = " << i << " ";
        cout << "{" << vec.at(i).com << "} " << vec.at(i).numOfRepeats << "; " << endl;
        //  cout << vec.at(i).com;
    }
    cout << endl;
}

void clearVec(vector<Command> &vec)
{
    for (size_t i = 0; i < vec.size()-2; ++i)
    {
        if ((vec.at(i).com == '[' && vec.at(i+2).com == ']')&&(vec.at(i+1).com == '+' || vec.at(i+1).com == '-'))
        {
            vec.at(i).com = '0';
            vec.erase(vec.begin()+i+1, vec.begin()+i+3);
            i+=2;
        }
    }
}


int main(int argc, char **argv)
{

    clock_t start = clock();
    vector<Command> acc;
    char ch_next;
    char ch_prev = 'p';
    int count_rep = 1;
    ifstream infile(argv[1]);
    while (infile)
    {
        infile.get(ch_next);
        if (ch_next == ch_prev)
        {
            ((acc.end()-1)->numOfRepeats)++;
        }
        else
        {
            acc.push_back(Command(ch_next, count_rep));
        }
        ch_prev = ch_next;
    }
    infile.close();
    clearVec(acc);
    //printVec(acc);

    char *buf = cpu;

    int st_p = 0;
    unsigned int count = 0;
    while (acc.at(count).com && (count < acc.size()-1))
    {
        try {
            switch (acc.at(count).com)
            {
            case '<': { buf = buf - acc.at(count).numOfRepeats; break;}
            case '>': { buf = buf + acc.at(count).numOfRepeats; break;}
            case '+': { (*buf) += acc.at(count).numOfRepeats; break;}
            case '-': { (*buf) -= acc.at(count).numOfRepeats; break;}
            case '.':
            {
                for (int k = 0; k < acc.at(count).numOfRepeats; ++k)
                    cout << (*buf);
                break;
            }
            case ',':
            {
                for (int k = 0; k < acc.at(count).numOfRepeats; ++k)
                    cin >> (*buf);
                break;
            }
            case '0':
            {
                (*buf) = 0;
                break;
            }
            case '[':
            {
                if(!(*buf))
                {
                    ++st_p;
                    while(st_p)
                    {
                        ++count;
                        if (acc[count].com == '[')
                            ++st_p;
                        if (acc[count].com == ']')
                            --st_p;
                    }
                }

                break;
            }
            case ']':
            {
                if(!(*buf))
                {}
                else
                {
                    if(acc[count].com == ']')
                        st_p++;
                    while(st_p)
                    {
                        --count;
                        if(acc[count].com == '[')
                            st_p--;
                        if(acc[count].com == ']')
                            st_p++;
                    }
                    --count;
                }
                break;
            }
            }
            count++;
        }
        catch(...)
        {

            cout << "fuck";
            return 9;
        }
    }
    cout << (float)(clock() - start)/CLOCKS_PER_SEC << endl;
    return 0;
}
