

#ifndef _SETTINGDB_H_
#define _SETTINGDB_H_

#define SQLITE_DATABASE_PATH	"/home/george/GK_SampleProvider/settings.db"


#define SQL_QUERY_LENGTH	(512)
#define MAXIMUM 			(1)
#define ROW_COUNT			(0)
#define MINIMUM 			(2)


int GetRegisterValue(int id, char* value);
int UpdateSetting(char *value);
#endif	/* _SETTINGDB_H_ */


