#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ioctl.h>

int main()
{
    int fd = open("/dev/i2c-1", O_RDWR);
    if (fd == -1)
        printf("Couldn't open /i2c-1 file, error code: %d \n", errno);
    int err = ioctl(fd, 0x0703, 0x08); // i2cdev sys call (0x0703) to set I2C addr
    if (err != 0)
        printf("ioctl() returns error, errorno: %d \n", errno);
    
    char data[3];

    while (1)
    {
        int numRead = read(fd, &data, 2);
        if (numRead != 2)
            printf("Could'nt read whole buffer of data, errorcode: %d\n", errno);
        else {
            printf("Numbers: %d.%d\n", data[0], data[1]);
        }
    }


	return 0;
}