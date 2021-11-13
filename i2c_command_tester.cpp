#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ioctl.h>

int main()
{
    int bufSize = 2;

    int fd = open("/dev/i2c-1", O_RDWR);
    if (fd == -1)
        printf("Couldn't open /i2c-1 file, error code: %d \n", errno);
    int err = ioctl(fd, 0x0703, 0x08); // i2cdev sys call (0x0703) to set I2C addr
    if (err != 0)
        printf("ioctl() returns error, errorno: %d \n", errno);


    unsigned char rdData[bufSize];
    unsigned char wrData[bufSize];
    wrData[0] = 1;

    while (1)
    {
        printf("Input the number of the command you want to send: ");
        printf("%d", wrData)

        int numWrite = write(fd, wrData, bufSize);
        if (numWrite != bufSize)
            printf("Couldn't write whole buffer of data, errorcode: %d\n", errno);

        int numRead = read(fd, &rdData, bufSize);
        if (numRead != bufSize)
            printf("Couldn't read whole buffer of data, errorcode: %d\n", errno);
        else {
            printf("Fluid level: %d || Placement: %d", rdData[0], rdData[1]);
        }
    }


    return 0;
}