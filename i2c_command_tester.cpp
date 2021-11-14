#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ioctl.h>

int main()
{
    int bufSize = 3;

    int fd = open("/dev/i2c-1", O_RDWR);
    if (fd == -1)
        printf("Couldn't open /i2c-1 file, error code: %d \n", errno);
    int err = ioctl(fd, 0x0703, 0x08); // i2cdev sys call (0x0703) to set I2C addr
    //0x08 er adressen på I2C-slave! (PSoC)
    if (err != 0)
        printf("ioctl() returns error, errorno: %d \n", errno);


    unsigned char rdData[bufSize];
    unsigned char wrData[bufSize];
    wrData[0] = 0; //register byte? Kan give fejl, hvis den aendres
    while (1)
    {
        //Getting user input
        printf("Input the integer (1 byte) you want to send: ");
        scanf("%d", &wrData[1]);
        printf("Second integer: ");
        scanf("%d", &wrData[2]);

        //Writing
        int numWrite = write(fd, wrData, bufSize);
        if (numWrite != bufSize)
            printf("Couldn't write whole buffer (%d) of data, errorcode: %d\n", numWrite, errno);


        //Reading
        int numRead = read(fd, &rdData, bufSize);
        if (numRead != bufSize)
            printf("Couldn't read whole buffer of data, errorcode: %d\n", errno);
        else {
            printf("Byte 1: %d || Byte 2: %d\n", rdData[0], rdData[1]);
        }
    }


    return 0;
}