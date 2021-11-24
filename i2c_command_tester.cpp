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

		//Writing
		int numWrite = write(fd, wrData, bufSize);
		if (numWrite != bufSize)
			printf("Couldn't write whole buffer (%d) of data, errorcode: %d\n", numWrite, errno);

		int numRead = read(fd, &rdData, bufSize);
		if (numRead != bufSize) {
			printf("Couldn't read whole buffer of data, errorcode: %d\n", errno);
		}
		else {
			if (rdData[0] == 40) {
				printf("Byte 1: %d || Byte 2: %d\n", rdData[0], rdData[1]);
			}
		}
	}
}
return 0;
}