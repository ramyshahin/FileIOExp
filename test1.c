// test1.c 
// z = x+y

//#define DEBUG

static const char* IN_A = "/home/ramy/FileIOExp/a.bin";
static const char* IN_B = "/home/ramy/FileIOExp/b.bin";
static const char* IN_C = "/home/ramy/FileIOExp/c.bin";

static const char* OUT = "/home/ramy/FileIOExp/out.bin";

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

static int runTest(unsigned int count)
{
    FILE fin1;
    FILE fin2;
    FILE fout;
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

    fout = OPEN(OUT, OUT_FLAGS, S_IRWXU | S_IRWXG | S_IRWXO);
    if (!CHECK_FILE(fout)) {
        REPORT_ERROR("Could not open file ");
        return -1;
    }

    //printk(KERN_INFO "&a: %x", (void*)a);
    //printk(KERN_INFO "&b: %x", (void*)b);

    for(i=0; i<count;) {
        ssize_t bytesRead = READ(fin1, offset, a, BATCH_SIZE_BYTES);
        bytesRead = READ(fin2, offset, b, BATCH_SIZE_BYTES);
        size_t itemsRead = bytesRead / sizeof(int);

        //printk(KERN_INFO "bytes read %d", bytesRead);

        if (bytesRead == 0) {
            break;
        }

        for(j = 0; j<itemsRead; j++) {
            c[j] = b[j] + a[j];
        }

        ssize_t bytesWritten = WRITE(fout, offset, c, bytesRead);
        if (bytesWritten != bytesRead) {
            REPORT_ERROR("read-write count mismatch");
        }

        //DBGMSG("bytes written");

        offset += bytesRead;
        i += itemsRead;
    }

    FLUSH(fout);
    CLOSE(fin1);
    CLOSE(fin2);
    CLOSE(fout);
}