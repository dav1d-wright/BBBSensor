/**----------------------------------------------------------------------------
	\file		CPgSql.h
	\author		D. Wright
	\version	0

	\brief Class that implements the interface to the PostgreSQL database.

	Long comment on this file.

  ----------------------------------------------------------------------------*/

#ifndef INC_CPGSQL_H_
#define INC_CPGSQL_H_

/*----------------------------------------------------------------------------*/

/*!	\defgroup Devices Higher functionality devices.
	\{
*/

/*!	\defgroup DEV_PGSQL PGSQL Device (PGSQL)
 	 * \brief The PGSQL device handles the interface to the PostgreSQL database.
	\{
*/

class CPgSql
{
private:
    PGconn *m_pCConn;
    PGresult *m_pCRes;
    string m_sDbName;
public:
    CPgSql(string asDbName);
	~CPgSql(void);
	EError init(void);
	EError createDb(void);
	EError createTable(void);
};


#endif /* INC_CPGSQL_H_ */
