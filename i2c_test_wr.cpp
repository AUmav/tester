
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

    char data[bufSize];
    data[0] = 1;
    data[1] = 2;
    
    while (1)
    {
        int numWrite = write(fd, data, bufSize);
        if (numWrite != bufSize)
            printf("Couldn't write whole buffer of data, errorcode: %d\n", errno);

        data[1]++;
        if (data[1] == 11) {
            data[1] = 0;
        }

        int numRead = read(fd, &data, bufSize);
        if (numRead != bufSize)
            printf("Couldn't read whole buffer of data, errorcode: %d\n", errno);
        else {
            printf("Numbers: %d.%d\n", data[0], data[1]);
        }

        sleep(1);




    }


    return 0;
}