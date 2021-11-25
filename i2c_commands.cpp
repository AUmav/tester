﻿#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ioctl.h>
#include "getch.h"

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


	char rdData[rdBufSize];
	char wrData[wrBufSize];
	wrData[0] = 0; //register byte? Kan give fejl, hvis den aendres
	while (1)
	{
		printf("Commands:\n1. Opfyld\n2.Kalibrer\n3.InitCleaning\n4.StartCleaning\n5.startRefill\n");

		//Getting user input
		printf("Input the command you want to send: ");
		scanf("%d", &wrData[1]);

		//Writing
		int numWrite = write(fd, wrData, wrBufSize);
		if (numWrite != wrBufSize)
			printf("Couldn't write whole buffer (%d) of data, errorcode: %d\n", numWrite, errno);

		/*int numRead = read(fd, &rdData, rdBufSize);
		if (numRead != rdBufSize) {
			printf("Couldn't read whole buffer of data, errorcode: %d\n", errno);
		}
		else {
				printf("Status code: %d || Data: %d\n", rdData[1], rdData[2]);
			}
		}*/
	}

	return 0;
}