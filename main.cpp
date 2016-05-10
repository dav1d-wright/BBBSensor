/*
 * main.cpp
 *
 *  Created on: 6 May 2016
 *      Author: wright
 */

#include <iostream>
#include <stdint.h>
#include <BME280.h>

using namespace std;

int main(void)
{
	BME280 cBme280(0, 0);

	if(cBme280.Init() == 0)
	{

	}

	cout << "!Hello hard float world!" << endl;
	return 0;
}
