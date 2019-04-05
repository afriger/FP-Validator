#include <stdio.h>
#include <iostream>
#include <direct.h>
#include "Logger.h"
#include "Validator.h"

Logger* pLogger = NULL;
const std::string file_ini = "./ini.txt";

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

	Validator validator(file_ini);
	validator.Validate();
	std::vector<ResultMessage> fatal = pLogger->GetMessages(_eRate::FATAL);
	std::vector<ResultMessage> warning = pLogger->GetMessages(_eRate::WARNING);
	std::cout << "RESULT: " << "FATAL:" << fatal.size() << "; WARNING:" << warning.size() << std::endl;

	//std::string cmd = validator.cmdPath();
	//system(cmd.c_str());
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
