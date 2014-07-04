

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <crypt.h>

#include "SettingDB.h"

/************************************************************************************************************/
/** \fn       int UpdateSetting(char *value)
* \brief  	This function update value in DB 
* \param	value          -   update DisplayName in database 
* \return	SUCCESS		- 0
* \return	FAILURE		- -1
* \note
*/
/************************************************************************************************************/
int UpdateSetting(char *value)
{
	sqlite3 *sqlHandle;

	sqlite3_stmt *sqlStmt_update;


	char sqlQuery[SQL_QUERY_LENGTH] = "";
	int status = -1;
	status = sqlite3_open_v2(SQLITE_DATABASE_PATH, &sqlHandle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	
	if (status != SQLITE_OK)
	return status;   

	
	sprintf(sqlQuery, "UPDATE settings SET Value= '%s' WHERE Id=1", value);
	
	status = sqlite3_prepare_v2(sqlHandle, sqlQuery, -1, &sqlStmt_update, 0);
	if (status != SQLITE_OK){
		sqlite3_close(sqlHandle);
		return status;
	}
	status = sqlite3_step(sqlStmt_update);
	if (status != SQLITE_DONE){
		sqlite3_close(sqlHandle);
		return status;
	}
	sqlite3_finalize(sqlStmt_update);
	status = 0;
	

	return status;
}


/************************************************************************************************************/
/** \fn       int GetRegisterValue(int id, i char* value)
* \brief  	This function retrieves RegisterValue details from  settings DB 
* \param	id           -   
* \param      value        -    
* \return	SUCCESS		-   0
* \return	FAILURE		-   -1
* \note
*/
/************************************************************************************************************/
int GetRegisterValue(int id, char* value)
{
	printf("--- GetRegisterValue called\n");
	sqlite3 *sqlHandle;
	sqlite3_stmt *sqlStmt;

	char sqlQuery[SQL_QUERY_LENGTH] = "";
	int status = -1;
	status = sqlite3_open_v2(SQLITE_DATABASE_PATH, &sqlHandle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	
	if (status != SQLITE_OK)
	    return status;   
	
	sqlite3_busy_timeout(sqlHandle, 5);
	sprintf(sqlQuery, "SELECT Value FROM settings WHERE Id =%d", id);
	
	status = sqlite3_prepare_v2(sqlHandle, sqlQuery, -1, &sqlStmt, 0);
	if (status != SQLITE_OK)
		return status;
		
	status = sqlite3_step(sqlStmt);
	if (status != SQLITE_ROW)
 	     return status;
 
	strcpy(value, (const char*)sqlite3_column_text(sqlStmt,0));
	printf("--- value : %s \n",value);
	status = 0;
	
	// Close the handle to free memory
	sqlite3_finalize(sqlStmt);
	sqlite3_close(sqlHandle);
	printf("--- GetRegisterValue exited\n");
    return status;
}
