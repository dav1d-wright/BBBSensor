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
#include "libpq-fe.h"

using namespace std;

int main(void)
{
	double dTemp;
	double dPress;
	double dHumid;
    PGconn     *conn;
    PGresult   *res;
	EError eError = gcBme280.init();

	if(eError == eErrOk)
	{
		cout << "Init successful!" << endl;

		while(eError == eErrOk)
		{
			eError = gcBme280.readUncompValues();

			if(eError == eErrOk)
			{
				eError = gcBme280.compensateValues();
				if(eError == eErrOk)
				{
					dTemp = gcBme280.getCompensatedTemperature();
					dPress = gcBme280.getCompensatedPressure();
					dHumid = gcBme280.getCompensatedHumidity();

					cout << "Temperature = " << dTemp << "°C | "
						<< "Pressure = " << dPress << "Pa | "
						<< "Humidity = " << dHumid << "%rH" << endl;
					sleep(1);
				}
			}
		}
	}

	return 0;
}
