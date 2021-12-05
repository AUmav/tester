#include "i2c_driver.h"

i2c_driver::i2c_driver()
{
	readBufSize_ = 4;
	writeBufSize_ = 2;
	slaveAdr_ = 0x08;
}

i2c_driver::i2c_driver(unsigned char slaveAdr)
{
	slaveAdr_ = slaveAdr;
}

void i2c_driver::i2cInit()
{
	fd_ = open("/dev/i2c-1", O_RDWR); //open and set read/write
	if (fd_ == -1)
		printf("Couldn't open /i2c-1 file, error code: %d \n", errno);
	int err = ioctl(fd_, 0x0703, slaveAdr_); // i2cdev sys call (0x0703) to set I2C addr
	//0x08 is addr on I2C-slave! (PSoC)
	if (err != 0)
		printf("ioctl() returns error, errorno: %d \n", errno);
	wrData[0] = 0; //important to keep wrData[0] at 0!
}

void i2c_driver::i2cWrite(int byteOne)
{
	wrData[1] = byteOne;

	int numWrite = write(fd_, wrData, writeBufSize_);
	if (numWrite != writeBufSize_)
		printf("Couldn't write whole buffer (%d) of data, errorcode: %d\n", numWrite, errno);
}

int i2c_driver::i2cRead()
{
	int numRead = read(fd_, &rdData, readBufSize_);
	if (numRead != readBufSize_)
		printf("Couldn't read whole buffer of data, errorcode: %d\n", errno);
}
