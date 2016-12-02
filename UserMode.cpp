#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include "Timer.h"

#define TEST aTimes2
const size_t REPITITIONS = 10;
const char* IN_A = "a.bin";
const char* IN_B = "b.bin";
const char* IN_C = "c.bin";

const char* OUT = "out.bin";

const int IN_FLAGS = O_RDONLY;
const int OUT_FLAGS = O_WRONLY;

using namespace std;

void aTimes2(long long count)
{
    int fd_in = open(IN_A, IN_FLAGS);
    if (-1 == fd_in) {
        cerr << "Could not open file " << IN_A << endl;
        return;
    }

    int fd_out = open(OUT, OUT_FLAGS);
    if (-1 == fd_out) {
        cerr << "Could not open file " << OUT << endl;
        return;
    }

    for(long long i=0; i<count; i++) {
        int a;
        ssize_t bytesRead = read(fd_in, (void *)&a, sizeof(int));
        int b = a*2;
        ssize_t bytesWritten = write(fd_out, &b, sizeof(int));
    }

    close(fd_in);
    close(fd_out);
}

const long long G = 1000*1000*1000;

int main()
{
    for(long long count=G; count < 10*G; count+=G) {
        cout << count << "\t";

        double totalTime = 0.0;
        Timer timer;
        for(size_t i=0; i<REPITITIONS; i++) {
            timer.reset();
            TEST(count);
            double time = timer;

            totalTime += time;

            cout << time << "\n";
        }

        cout << (totalTime / REPITITIONS) << endl;
    }
    return 0;
}