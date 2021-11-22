#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>

static struct termios old, new1;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new1 = old; /* make new settings same as old settings */
    new1.c_lflag &= ~ICANON; /* disable buffered i/o */
    new1.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &new1); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

/* Read 1 character without echo */
char getch(void)
{
    return getch_(0);
}

/* Read 1 character with echo */
char getche(void)
{
    return getch_(1);
}

int main()
{
    int wrBufSize = 2;
    int rdBufSize = 3;

    int fd = open("/dev/i2c-1", O_RDWR);
    if (fd == -1)
        printf("Couldn't open /i2c-1 file, error code: %d \n", errno);
    int err = ioctl(fd, 0x0703, 0x08); // i2cdev sys call (0x0703) to set I2C addr
    //0x08 er adressen på I2C-slave! (PSoC)
    if (err != 0)
        printf("ioctl() returns error, errorno: %d \n", errno);


    unsigned char rdData[rdBufSize];
    unsigned char wrData[wrBufSize];
    wrData[0] = 0; //register byte? Kan give fejl, hvis den aendres
    while (1)
    {
        printf("Commands:\n1. Opfyld\n2.Kalibrer\n3.InitCleaning\n4.StartCleaning\n5.startRefill\n");

        //Getting user input
        printf("Input the command you want to send: ");
        scanf("%d", &wrData[1]);

        printf("Tryk 'x' for at stoppe");


        //Writing
        int numWrite = write(fd, wrData, wrBufSize);
        if (numWrite != wrBufSize)
            printf("Couldn't write whole buffer (%d) of data, errorcode: %d\n", numWrite, errno);

        char keyhit = '1';
        //Reading
        while (rdData[0] != 40 || keyhit != 'x') {
            int numRead = read(fd, &rdData, rdBufSize);
            if (numRead != rdBufSize) {
                printf("Couldn't read whole buffer of data, errorcode: %d\n", errno);
            }
            else {
                if (rdData[0] == 40) {
                    printf("Status code: %d || Data: %d\n", rdData[1], rdData[0]);
                }
            }
            keyhit = getch();
        }

        printf("Exiting\n");

        wrData[1] = 16;
        numWrite = write(fd, wrData, wrBufSize);
        if (numWrite != wrBufSize)
            printf("Couldn't write whole buffer (%d) of data, errorcode: %d\n", numWrite, errno);
        

        int numRead = read(fd, &rdData, rdBufSize);
        if (numRead != rdBufSize) {
            printf("Couldn't read whole buffer of data, errorcode: %d\n", errno);
        }
        else {
            printf("Status code: %d || Data: %d\n", rdData[1], rdData[2]);
        }
    }

    //comment
    return 0;
}