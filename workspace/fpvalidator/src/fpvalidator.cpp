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

//const char* file_csv = "./Primary_Limits_new.csv";
Logger* pLogger = NULL;

std::string GetCurrentWorkingDir(void)
{
	char buff[FILENAME_MAX];
	_getcwd(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}

DBLimits PrimaryLimits(INIFile inifile)
{
	DBLimits db;
	std::string file_csv = inifile.getValue("path_limits");
	std::string original = inifile.getValue("original_limits");
	inifile.checkUpdate(file_csv, original);
	Primary_Limits_new reader(file_csv);

	std::vector<std::vector<std::string> > dataList = reader.getData();
	// Print the content of row by row on screen
	db.Counter(0);
	for (std::vector<std::string> record : dataList)
	{
		CSVRecordPrimaryLimits rec(record);
		db.Insert(rec);
	}
	//test trim data
	std::string type = inifile.getValue("type");
	std::string target = inifile.getValue("target");
	std::string tolerance = inifile.getValue("tolerance");
	size_t key = db.GetHash(type, target, tolerance);
	std::vector<CSVRecordPrimaryLimits> res;
	res = db.GetElements(key);
	res.size();
	std::cout << "KEYS COUNT: " << res.size() << std::endl;
	for (CSVRecordPrimaryLimits& r : res)
	{
		TargetData data = r.GetTargetData(0.051);
		std::cout << "data 0.051: [" << data.Value << "]" << data.Lo << ";" << data.Hi << ";" << data.Md;
		std::cout << '\n';
	}
	return db;
}

int main()
{

	std::cout << "Wait .... " << std::endl;
	pLogger = Logger::getInstance();
	pLogger->Trace(true);
	INIFile inifile("./ini.txt");
	inifile.Ini();

//	DBLimits db = PrimaryLimits(inifile);
//	std::cout << "PrimaryLimits number of doubles: " << db.Counter() << std::endl;

	std::string family_file = inifile.getValue("path_family");
	std::string family_original = inifile.getValue("original_family");
	inifile.checkUpdate(family_file,family_original);
	Family family(family_file);
	family.getData();
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
