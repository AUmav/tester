#include "i2c_driver.h"



int main()
{
	i2c_driver test;

	test.i2cInit();
	test.i2cWrite(3);
	test.i2cRead();

	return 0;
}
