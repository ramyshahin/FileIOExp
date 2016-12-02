#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

const long long K = 1000;
const long long M = 1000 * K;
const long long G = 1000 * M;
const long long DATA_SET_SIZE = 10 * G;

using namespace std;

int main()
{
    srand(time(NULL));

    ios_base::openmode mode = ios_base::out | ios_base::binary;

    fstream a("a.bin", mode);
    fstream b("b.bin", mode);
    fstream c("c.bin", mode);
    fstream aTimes2("aTimes2.bin", mode);
    fstream aPlusb("aPlusb.bin", mode);
     
    for(long long i=0; i<DATA_SET_SIZE; i++) {
        int x = rand();
        int y = rand();
        int z = rand();
        int x2 = 2*x;
        int xy = x+y;

        a.write((char*)&x, sizeof(int));
        b.write((char*)&y, sizeof(int));
        c.write((char*)&z, sizeof(int));
        aTimes2.write((char*)&x2, sizeof(int));
        aPlusb.write((char*)&xy, sizeof(int));
    }

    return 0;
}
