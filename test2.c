// test2.c 
// o1 = x+y
// o2 = y*z

//#define DEBUG

static const char* IN_A = "/home/ramy/FileIOExp/a.bin";
static const char* IN_B = "/home/ramy/FileIOExp/b.bin";
static const char* IN_C = "/home/ramy/FileIOExp/c.bin";

static const char* OUT1 = "/home/ramy/FileIOExp/out1.bin";
static const char* OUT2 = "/home/ramy/FileIOExp/out2.bin";

static const int IN_FLAGS = O_RDONLY | O_LARGEFILE; // | O_DIRECT;
static const int OUT_FLAGS = O_CREAT | O_WRONLY | O_LARGEFILE; // | O_DIRECT;

#ifdef KERNEL_TEST
#define FILE struct file*
#define OPEN file_open
#define CHECK_FILE(f) (f != 0)
#define REPORT_ERROR(e) (printk(KERN_INFO e))
#define READ(f, o, p, s) file_read(f, o, p, s)
#define WRITE(f, o, p, s) file_write(f, o, p, s)
#define CLOSE(f) file_close(f)
#define FLUSH(f) file_sync(f)
#else
#define FILE int
#define OPEN open
#define CHECK_FILE(f) (f != -1)
#define REPORT_ERROR(e) (cerr << e << endl)
#define READ(f, o, p, s) read(f, p, s)
#define WRITE(f, o, p, s) write(f, p, s)
#define CLOSE(f) close(f)
#define FLUSH(f) fsync(f)
#endif // KERNEL_TEST

#ifdef DEBUG
#define DBGMSG(e) REPORT_ERROR(e)
#else
#define DBGMSG(e)
#endif

#define BATCH_SIZE_BYTES  (4*1024)
#define BATCH_SIZE (BATCH_SIZE_BYTES / sizeof(int))
static int a[BATCH_SIZE] __attribute__((aligned(BATCH_SIZE_BYTES)));
static int b[BATCH_SIZE] __attribute__((aligned(BATCH_SIZE_BYTES)));
static int c[BATCH_SIZE] __attribute__((aligned(BATCH_SIZE_BYTES)));

static int o1[BATCH_SIZE] __attribute__((aligned(BATCH_SIZE_BYTES)));
static int o2[BATCH_SIZE] __attribute__((aligned(BATCH_SIZE_BYTES)));

static int runTest(unsigned int count)
{
    FILE fin1;
    FILE fin2;
    FILE fin3;
    FILE fout1;
    FILE fout2;

    unsigned long long offset = 0;
    long i = 0;
    long j = 0;

    fin1 = OPEN(IN_A, IN_FLAGS, 0);
    if (!CHECK_FILE(fin1)) {
        REPORT_ERROR("Could not open file ");
        return -1;
    }

    fin2 = OPEN(IN_B, IN_FLAGS, 0);
    if (!CHECK_FILE(fin2)) {
        REPORT_ERROR("Could not open file ");
        return -1;
    }

    fin3 = OPEN(IN_C, IN_FLAGS, 0);
    if (!CHECK_FILE(fin3)) {
        REPORT_ERROR("Could not open file ");
        return -1;
    }

    fout1 = OPEN(OUT1, OUT_FLAGS, S_IRWXU | S_IRWXG | S_IRWXO);
    if (!CHECK_FILE(fout1)) {
        REPORT_ERROR("Could not open file ");
        return -1;
    }

    fout2 = OPEN(OUT2, OUT_FLAGS, S_IRWXU | S_IRWXG | S_IRWXO);
    if (!CHECK_FILE(fout2)) {
        REPORT_ERROR("Could not open file ");
        return -1;
    }

    //printk(KERN_INFO "&a: %x", (void*)a);
    //printk(KERN_INFO "&b: %x", (void*)b);

    for(i=0; i<count;) {
        ssize_t bytesRead = READ(fin1, offset, a, BATCH_SIZE_BYTES);
        bytesRead = READ(fin2, offset, b, BATCH_SIZE_BYTES);
        bytesRead = READ(fin3, offset, c, BATCH_SIZE_BYTES);
        size_t itemsRead = bytesRead / sizeof(int);

        //printk(KERN_INFO "bytes read %d", bytesRead);

        if (bytesRead == 0) {
            break;
        }

        for(j = 0; j<itemsRead; j++) {
            o1[j] = b[j] + a[j];
            o2[j] = b[j] + c[j];
        }

        ssize_t bytesWritten = WRITE(fout1, offset, o1, bytesRead);
        if (bytesWritten != bytesRead) {
            REPORT_ERROR("read-write count mismatch");
        }

        bytesWritten = WRITE(fout2, offset, o2, bytesRead);
        if (bytesWritten != bytesRead) {
            REPORT_ERROR("read-write count mismatch");
        }

        //DBGMSG("bytes written");

        offset += bytesRead;
        i += itemsRead;
    }

    FLUSH(fout1);
    FLUSH(fout2);
    CLOSE(fin1);
    CLOSE(fin2);
    CLOSE(fin3);
    CLOSE(fout1);
    CLOSE(fout2);
}