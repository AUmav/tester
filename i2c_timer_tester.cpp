#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "getch.h"

int main()
{
	int wrBufSize = 2;
	int rdBufSize = 4;

	int fd = open("/dev/i2c-1", O_RDWR);
	if (fd == -1)
		printf("Couldn't open /i2c-1 file, error code: %d \n", errno);
	int err = ioctl(fd, 0x0703, 0x08); // i2cdev sys call (0x0703) to set I2C addr
	//0x08 er adressen på I2C-slave! (PSoC)
	if (err != 0)
		printf("ioctl() returns error, errorno: %d \n", errno);


	char rdData[rdBufSize];
	unsigned char wrData[wrBufSize];
	wrData[0] = 0; //register byte? Kan give fejl, hvis den aendres

	while (1)
	{
		printf("\nCommands:\n1.startDispense\n2.checkDispense\n3.startCalibrate\n4.initCleaning\n5.startCleaning\n6.startRefill\n7.endRefill\n\n");

		//Getting user input
		printf("Input the command you want to send: ");
		scanf("%d", &wrData[1]);

		//Writing
		int numWrite = write(fd, wrData, wrBufSize);
		if (numWrite != wrBufSize)
			printf("Couldn't write whole buffer (%d) of data, errorcode: %d\n", numWrite, errno);

		if(wrData[1] == 1){
			do {
				int numRead = read(fd, &rdData, rdBufSize);
				if (numRead != rdBufSize) {
					printf("Couldn't read whole buffer of data, errorcode: %d\n", errno);
				}
				else {
					if (rdData[0] == 10) {
						if(rdData[1] == 1){
							printf("\nStatus code: %d\n", rdData[1]);
							printf("Sending command 2: checkDispense()");
							wrData[1] = 2;

							do{
								int numWrite = write(fd, wrData, wrBufSize);
								if (numWrite != wrBufSize)
									printf("Couldn't write whole buffer (%d) of data, errorcode: %d\n", numWrite, errno);

								do {
									int numRead = read(fd, &rdData, rdBufSize);
									if (numRead != rdBufSize) {
										printf("Couldn't read whole buffer of data, errorcode: %d\n", errno);
									}
									else {
										if (rdData[0] == 10) {
											if (rdData[1] == 4){
												printf("\nStatus code: %d || Time = %ds, %dcs\n", rdData[1], rdData[2], rdData[3]);
											}
											else{
												printf("\nStatus code: %d\n", rdData[1]);
											}
										}
									}
								} while (rdData[0] != 10);
							} while (rdData[1] == 5);
						}
						else{
							printf("\nStatus code: %d\n", rdData[1]);
						}
					}
				}
			} while (rdData[0] != 10);
		}

		else{
			do {
				int numRead = read(fd, &rdData, rdBufSize);
				if (numRead != rdBufSize) {
					printf("Couldn't read whole buffer of data, errorcode: %d\n", errno);
				}
				else {
					if (rdData[0] == 10) {
							printf("\nStatus code: %d\n", rdData[1]);
					}
				}
			} while (rdData[0] != 10);
		}
}
	return 0;
}
