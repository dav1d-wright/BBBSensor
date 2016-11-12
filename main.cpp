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
#include <string>
using namespace std;

#include <Errors.h>
#include <CBME280.h>
#include "libpq-fe.h"
#include "CPgSql.h"


int main(void)
{
	double dTemp;
	double dPress;
	double dHumid;
	CPgSql cPgSqlDb("weatherman");

	EError eError = gcBme280.init();

	if(eError == eErrOk)
	{
		cout << "Init successful!" << endl;

		eError = cPgSqlDb.init();

		while(eError == eErrOk)
		{
			gcBme280.run();
		}
	}

	return 0;
}
