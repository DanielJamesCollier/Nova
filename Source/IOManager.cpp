#include "IOManager.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include  "Logger.h"
namespace Nova
{
	bool IOManager::DoesFileExist(const std::string& file_name)
	{
		std::ifstream f(file_name.c_str());
		if (f.good()) {
			f.close();
			return true;
		}
		else {
			Logger::ErrorBlock("IOManager Error", "File: " + file_name + "\nError: this file does not exist", true);
			return false;
		}
	}

	std::string IOManager::LoadFileText(const std::string& file_path)
	{
		std::string source;

		std::ifstream file(file_path);

		if (file.is_open())
		{
			std::string line;
			while (getline(file, line))
			{
				source += line + "\n";
			}
			file.close();
		}
		return source;
	}

	bool IOManager::dirExists(const std::string& dirName_in)
	{
		DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;  //something is wrong with your path!

		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return true;   // this is a directory!

		return false;    // this is not a directory!
	}

}
