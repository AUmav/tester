#pragma once
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ioctl.h>


class i2c_driver
{
public:
	i2c_driver();
	i2c_driver(unsigned char slaveAdr);
	void i2cInit(); //must run before any other functions in driver
	void i2cWrite(int byteOne);
	void i2cRead(unsigned char[] buffer);

private:
	int readBufSize_ = 4;
	int writeBufSize_ = 2;
	int fd_;
	unsigned char slaveAdr_;
	unsigned char wrData[2];
};
