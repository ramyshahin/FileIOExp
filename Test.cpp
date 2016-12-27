#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include "Timer.h"

#define KERNEL_TEST

using namespace std;

#ifdef KERNEL_TEST
#include <fcntl.h>		/* open */
#include <sys/ioctl.h>	/* ioctl */
#include <errno.h>

#define IOCTL_RUN_TEST0 _IOR(MAJOR_NUM, 0, unsigned int)
#define IOCTL_RUN_TEST1 _IOR(MAJOR_NUM, 1, unsigned int)
#define IOCTL_RUN_TEST2 _IOR(MAJOR_NUM, 2, unsigned int)
#define TEST_IOCTL IOCTL_RUN_TEST2

#else
#include "test2.c"
#endif

const size_t REPITITIONS = 5;

const long long K = 1000;
const long long M = K*K;
const long long G = K*M;


#ifdef KERNEL_TEST
const char* DEVICE_FILE_NAME = "/dev/fpmod_dev";
#define MAJOR_NUM 100

void ioctl_run_test(int file_desc, unsigned int size)
{
	int ret_val;

	ret_val = ioctl(file_desc, TEST_IOCTL, size);

	if (ret_val < 0) {
		printf("TEST_IOCTL failed:%d\n", ret_val);
	}
}
#endif

int main()
{
#ifdef KERNEL_TEST

    int file_desc = open(DEVICE_FILE_NAME, 0);
	if (file_desc < 0) {
		printf("Can't open device file: %s\t errno: %d\n", DEVICE_FILE_NAME, errno);
		return (-1);
    }
#endif // KERNEL_TEST

    for(long long count=100*M; count <= 1*G; count+=100*M) {
        cout << count << "\t";

        double totalTime = 0.0;
        Timer timer;
        for(size_t i=0; i<REPITITIONS; i++) {
            timer.reset();
#ifdef KERNEL_TEST
            ioctl_run_test(file_desc, count);
#else
            runTest(count);
#endif

            double time = timer;

            totalTime += time;

            cout << time << "\t";
        }

        cout << (totalTime / REPITITIONS) << endl;
    }

#ifdef KERNEL_TEST
    close(file_desc);
#endif

    return 0;
}
