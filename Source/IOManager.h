#pragma once

#include <string>

namespace Nova
{
	class IOManager
	{
	public:
		IOManager() = delete;
		~IOManager() = delete;

		static bool DoesFileExist(const std::string& file_name);

		static std::string LoadFileText(const std::string& file_path);

		static bool dirExists(const std::string& dirName_in);

		

	};
}


