#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "getch.h"
#include <sys/time.h>
#include <sys/types.h>
#include "i2c_driver.h"

#define WAIT 10

int main()
{
// !! Inkluderer kode taget fra https://stackoverflow.com/questions/7226603/timeout-function
	fd_set          input_set;
	struct timeval  timeout;
	int             ready_for_reading = 0;
	int             read_bytes = 0;

	int command;
	unsigned char rdData[4];
	i2c_driver i2c;
	i2c.i2cInit();

	while (1)
	{
		printf("\nCommands:\n1.startDispense\n2.checkDispense\n3.startCalibrate\n4.initCleaning\n5.startCleaning\n6.startRefill\n7.endRefill\n\n");

		//Getting user input
		printf("Input the command you want to send: ");
		scanf("%d", &command);

		//Writing
		i2c.i2cWrite(command);

		if(command == 1){
			do {
				i2c.i2cRead(rdData);
				if (rdData[0] == 10) {
					if(rdData[1] == 1){
						printf("\nStatus code: %d\n", rdData[1]);
						printf("Sending command 2: checkDispense()");
						command = 2;

						do{
							i2c.i2cWrite(2);

							do {
								i2c.i2cRead(rdData);

								if (rdData[0] == 10) {
									if (rdData[1] == 4){
										printf("\nStatus code: %d || Time = %ds, %dcs\n", rdData[1], rdData[2], rdData[3]);
									}
									else{
										printf("\nStatus code: %d\n", rdData[1]);
									}
								}

							} while (rdData[0] != 10);
/// STOP !
////////
								/* Listening for input stream for any activity */
							FD_ZERO(&input_set);
							/* Listen to the input descriptor */
							FD_SET(STDIN_FILENO, &input_set);
							timeout.tv_sec = 0;    // 0 seconds
							timeout.tv_usec = WAIT;    // WAIT milliseconds
							ready_for_reading = select(1, &input_set, NULL, NULL, &timeout);

							if(ready_for_reading){
								printf("Stop button pressed\n");
								command = 16;
								i2c.i2cWrite(command);
								}
////////
////////
						} while (rdData[1] == 5);
					}
					else{
						printf("\nStatus code: %d\n", rdData[1]);
					}
				}
			} while (rdData[0] != 10);
		}

		else{
			do {
				i2c.i2cRead(rdData);
				if (rdData[0] == 10) {
					printf("\nStatus code: %d\n", rdData[1]);
				}
			} while (rdData[0] != 10);
		}
	}
	return 0;
}
