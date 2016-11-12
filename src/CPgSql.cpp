/**----------------------------------------------------------------------------
	\file		CPgSql.cpp
	\author		D. Wright
	\version	0

	\brief Class that implements the interface to the PostgreSQL database.

	Long comment on this file.

  ----------------------------------------------------------------------------*/

/*!	\defgroup Devices Higher functionality devices.
	\{
*/

/*!	\defgroup DEV_PGSQL PGSQL Device (PGSQL)
 	 * \brief The PGSQL device handles the interface to the PostgreSQL database.
	\{
*/

/*----------------------------------------------------------------------------*/
/* includes */
/*----------------------------------------------------------------------------*/
#include <Errors.h>
#include <string>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
using namespace std;

#include "libpq-fe.h"
#include <CPgSql.h>

/*----------------------------------------------------------------------------*/
/* definitions */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* implementations */
/*----------------------------------------------------------------------------*/
CPgSql::CPgSql(string asDbName):
		m_sDbName(asDbName)
{

}

CPgSql::~CPgSql(void)
{

}

EError CPgSql::init(void)
{
	EError eError = eErrOk;

	m_pCConn = PQconnectdb(m_sDbName.c_str());
	if (PQstatus(m_pCConn) != CONNECTION_OK)
	{
		cout << "Connection to DB " << m_sDbName << " failed. Maybe it doesn't exist yet!" << endl << "Let's create one." << endl;
		eError = this->createDb();
	}
	return eError;
}

EError CPgSql::createDb(void)
{
	EError eError = eErrOk;
	char sCmd[256];

	sprintf(sCmd, "CREATE_DATABASE %s OWNER postgres", m_sDbName.c_str());

	m_pCRes = PQexec(m_pCConn, sCmd);
	if(PQresultStatus(m_pCRes) != PGRES_COMMAND_OK)
	{
		perror("Creating DB failed.");
		eError = eErrPgSqlCreateDb;
	}

	return eError;
}

EError CPgSql::createTable(void)
{
	EError eError = eErrOk;
	string sCmd = "CREATE TABLE IF NOT EXISTS weatherdata\
			 (time TEXT, temperature double precision, pressure double precision, humidity double precision)";

	m_pCRes = PQexec(m_pCConn, sCmd.c_str());
	if(PQresultStatus(m_pCRes) != PGRES_COMMAND_OK)
	{
		perror("Creating Table failed.");
		eError = eErrPgSqlCreateTab;
	}

	return eError;
}
