#ifndef _FILE_LOG_H
#define _FILE_LOG_H
#include "stdio.h"

class FileLog
{
protected:
	FILE *f_log;
public:
	FileLog(const char *path)
	{
		f_log = fopen(path, "a+");
	}
	~FileLog()
	{
		if(f_log)
			fclose(f_log);
	}

	void WriteToLog(const char *str)
	{
		fputs(str, f_log);
	}
};

#endif
