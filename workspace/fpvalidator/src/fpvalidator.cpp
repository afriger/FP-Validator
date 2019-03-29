#include <stdio.h>
#include <iostream>
#include <direct.h>

#include "CSVRecordPrimaryLimits.h"
#include "DBLimits.h"
#include "Logger.h"
#include "Util.h"
#include "TargetData.h"
#include "INIFile.h"
#include "PrimaryLimitsNew.h"
#include "Family.h"

Logger* pLogger = NULL;

std::string GetCurrentWorkingDir(void)
{
	char buff[FILENAME_MAX];
	_getcwd(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}

int main()
{

	std::cout << "Wait .... " << std::endl;
	pLogger = Logger::getInstance();
	pLogger->Trace(true);
	INIFile ini("./ini.txt");
	ini.Ini();

	DBLimits db = Primary_Limits_new::Verify(ini);
	Primary_Limits_new::GetTargetData(ini, db);
	Family::Verify(ini);

	getchar();
	return 0;
}

int test()
{

	//	std::string path = GetCurrentWorkingDir();
	//	std::cout << "CurrentWorkingDir:" << path << std::endl;
	//	path += "\\";
	//	path += file_csv;

	char filename[] = "C:/Users/afriger/Sasha/dev/git/workspace/fpvalidator/Primary_Limits_new.csv";

	struct stat buf;

	if (!stat(filename, &buf))
	{
		char timeStr[100] = "";
		strftime(timeStr, 100, "%d-%m-%Y %H:%M:%S", localtime(&buf.st_mtime));
		printf("\nLast modified date and time = %s\n", timeStr);
	}
	else
	{
		printf("error getting atime\n");
	}

	return 0;
}
