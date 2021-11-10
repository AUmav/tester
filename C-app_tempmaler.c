#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>



int main()
{
    char gpioLED0[] = "26";
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if(fd == -1)
        printf("Error: opening/making export file %s, errorcode: %d \n", gpioLED0, errno);
    
    
    int numWrite = write(fd, gpioLED0, strlen(gpioLED0) + 1);
    if (numWrite != (strlen(gpioLED0) + 1))
    {
        printf("Couldn't write whole buffer, error code:%d \n", errno);
        if (errno == 16)
            printf("Device or resource busy \n");
    }
    close(fd);

    fd = open("/sys/class/gpio/gpio26/direction", O_RDWR);
    if(fd == -1)
        printf("Couldn't open /direction file, error code:%d \n", errno);

    char dir[4];
    int numRead = read(fd, &dir, 4);
    if (numRead < 3)
        printf("Couldn't read whole buffer, error code:%d \n", errno);

    if (strcmp(dir, "in"))
        numWrite = write(fd, "out", 4);
        if (numWrite != 4)
            printf("Couldn't write whole buffer, error code:%d \n", errno);

    close(fd);

    fd = open("/sys/class/gpio/gpio26/value", O_RDWR);
        if(fd == -1)
            printf("Couldn't open /value file, error code: %d \n", errno);

    fd = open("/dev/i2c-1", O_RDONLY);
    if(fd == -1)
            printf("Couldn't open /i2c-1 file, error code: %d \n", errno);
    int err = ioctl(fd, 0x0703, 0x48); // i2cdev sys call (0x0703) to set I2C addr
        if (err != 0)
            printf("ioctl() returnerer fejl, errorno: %d \n", errno);
    
    char data[3];
    int tempWarning = 30;
    unsigned int warning = 0;

    char hjemmeside[] = "<html><body><h1>Temperaturen er: 20 C</h1></body></html>";

    while (1)
    {
        numRead = read(fd, &data, 2);
    if (numRead != 2)
        printf("could'nt read whole buffer of data, errorcode: %d\n", errno);

        sleep(1);
        if (data[1]== 0)
            printf("temperatur: %d.0\n", data[0]);
        else
            printf("temperatur: %d.5\n", data[0]);
    
    //HJEMMESIDE DEL
    sprintf(hjemmeside,"<html><body><h1>Temperaturen er: %d C</h1></body></html>", data[0]);

    //format
        fd = open("/www/pages/index.html", O_WRONLY);
    if(fd == -1)
        printf("Error: opening files index.html, errorcode: %d \n", errno);
    
    
    numWrite = write(fd, hjemmeside, strlen(hjemmeside) + 1);
    if (numWrite != (strlen(hjemmeside) + 1))
    {
        printf("Couldn't write whole buffer, error code:%d \n", errno);
        if (errno == 16)
            printf("Device or resource busy \n");
    }
    close(fd);
    //hjemmeside end

        




        if (data[0] > tempWarning && warning == 0)
        {
            warning = 1;
            char warningMsg[]="TEMPERATUREN ER NU OVER XX\n";
            sprintf(warningMsg,"TEMPERATUREN ER NU OVER %d\n", tempWarning);
            
            write(1, warningMsg, strlen(warningMsg) +1);

            fd = open("/sys/class/gpio/gpio26/value", O_RDWR);
            if(fd == -1)
                printf("Couldn't open /value file, error code: %d \n", errno);
            
            write(fd, "1", 2);
            if(fd == -1)
                printf("Couldn't write 1 to /value file, error code: %d \n", errno);    

            close(fd);
        }
        else if (data[0] < tempWarning && warning == 1)
        {
            warning = 0;

            char warningOffMsg[]="Temperaturen nu under XX igen\n";
            sprintf(warningOffMsg,"Temperaturen nu under %d igen\n", tempWarning);
            write(1, warningOffMsg, strlen(warningOffMsg) +1);

            fd = open("/sys/class/gpio/gpio26/value", O_RDWR);
            if(fd == -1)
                printf("Couldn't open /value file, error code: %d \n", errno);
            
            write(fd, "0", 2);
            if(fd == -1)
                printf("Couldn't write 1 to /value file, error code: %d \n", errno);
            close(fd);

        }

        fd = open("/dev/i2c-1", O_RDONLY);
            if(fd == -1)
                printf("Couldn't open /i2c-1 file, error code: %d \n", errno);
            err = ioctl(fd, 0x0703, 0x48); // i2cdev sys call (0x0703) to set I2C addr
            if (err != 0)
                printf("ioctl() returnerer fejl, errorno: %d \n", errno);

    }
    

    while (1) //toggler LED0 hvert sekund - Ikke i brug i resten af opgaverne
    {
        write(fd, "1", 2);
        if(fd == -1)
            printf("Couldn't write 1 to /value file, error code: %d \n", errno);
        sleep(1);
        write(fd, "0", 2);
        if(fd == -1)
            printf("Couldn't write 0 to /value file, error code: %d \n", errno);
        sleep(1);
    }

    
}

