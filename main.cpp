/*
 * main.cpp
 *
 *  Created on: 6 May 2016
 *      Author: wright
 */

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <Errors.h>
#include <CBME280.h>

using namespace std;

int main(void)
{
	if(gcBme280.init() == eErrOk)
	{

	}

	cout << "Hello remote Beaglebone!" << endl;

	FILE *LedHandle = NULL;

	const char* brightness = "/sys/class/leds/beaglebone:green:usr3/brightness";

	for(uint8_t i = 0; i < 10; i++)
	{
		if((LedHandle = fopen(brightness, "r+")) != NULL)
		{
			fwrite("1", sizeof(char), 1, LedHandle);
			fclose(LedHandle);
		}

		sleep(1);

		if((LedHandle = fopen(brightness, "r+")) != NULL)
		{
			fwrite("0", sizeof(char), 1, LedHandle);
			fclose(LedHandle);
		}
		sleep(1);
	}

	cout << "Goodbye remote Beaglebone!" << endl;

	return 0;
}
