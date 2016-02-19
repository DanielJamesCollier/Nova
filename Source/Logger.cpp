#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Logger.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>


namespace Nova
{

	std::string Logger::m_log = "";
	std::string Logger::m_errorLog = "";
	bool        Logger::m_blockBegun = false;

	void Logger::InfoBlock(const std::string& blockHeader, const std::string& blockInfo, bool print_to_console)
	{
		std::string _block = "";
		
		_block = "\n\n\n\n-----------------------------------------------------------------\n"
			+ blockHeader + " *\n"
			+ "-----------------------------------------------------------------\n\n"
			+ blockInfo
			+ "\n\n-----------------------------------------------------------------\n";

		Logger::m_log += _block;

		if (print_to_console) std::cout << _block;
	}

	void Logger::InfoBlock(const std::string& block_header, const std::vector<char>& block_info, bool print_to_console)
	{
		std::string _block = "";

		_block = "\n\n\n\n-----------------------------------------------------------------\n* "
			+ block_header + " *\n"
			+ "-----------------------------------------------------------------\n\n";

		for (unsigned int i = 0; i < block_info.size(); i++)
		{
			_block += block_info[i];
		}

		_block += "\n\n-----------------------------------------------------------------\n";

		Logger::m_log += _block;

		if (print_to_console) std::cout << _block;
	}

	void Logger::InfoBlock(const std::string& block_header, const std::string& block_info_s, const std::vector<char>& block_info, bool print_to_console)
	{
		std::string _block = "";

		_block = "\n\n\n\n-----------------------------------------------------------------\n* "
			+ block_header + " *\n"
			+ "-----------------------------------------------------------------\n\n";

		_block += block_info_s + "\n";

		for (unsigned int i = 0; i < block_info.size(); i++)
		{
			_block += block_info[i];
		}

		_block += "\n\n-----------------------------------------------------------------\n";

		Logger::m_log += _block;

		if (print_to_console) std::cout << _block;
	}

	void Logger::InfoBlockBegin(const std::string& blockHeader)
	{
		std::string _block = "";

		_block = "\n\n\n\n-----------------------------------------------------------------\n* "
			+ blockHeader + " *\n"
			+ "-----------------------------------------------------------------\n\n";

		Logger::m_log += _block;

		std::cout << _block;
	}

	void Logger::InfoBlockMessage(const std::string& infoBlock)
	{
		std::string _block = "";

		for (unsigned int i = 0; i < infoBlock.size(); i++)
		{
			_block += infoBlock[i];
		}

		Logger::m_log += _block;

		std::cout << _block;
	}

	void Logger::InfoBlockEnd()
	{
		std::string _block = "";

		_block += "\n\n-----------------------------------------------------------------\n";

		Logger::m_log += _block;

		std::cout << _block;
	}

	void Logger::ErrorBlock(const std::string& block_header, const std::string& block_error, bool print_to_console)
	{
		std::string _block = "";

		_block = "\n\n\n\n-----------------------------------------------------------------\n* "
			+ block_header + " *\n"
			+ "-----------------------------------------------------------------\n\n"
			+ block_error
			+ "\n\n-----------------------------------------------------------------\n";

		Logger::m_errorLog += _block;

		if (print_to_console) std::cout << _block;
	}
	
	void Logger::ErrorBlock(const std::string& block_header, const std::vector<char>& block_error, bool print_to_console)
	{
		std::string _block = "";

		_block = "\n\n\n\n-----------------------------------------------------------------\n* "
			+ block_header + " *\n"
			+ "-----------------------------------------------------------------\n\n";

		for (unsigned int i = 0; i < block_error.size(); i++)
		{
			_block += block_error[i];
		}

		_block += "\n\n-----------------------------------------------------------------\n";

		Logger::m_errorLog += _block;

		if (print_to_console) std::cout << _block;
	}

	void Logger::ErrorBlock(const std::string& block_header, const std::string& block_error_s, const std::vector<char>& block_error, bool print_to_console)
	{
		std::string _block = "";

		_block = "\n\n\n\n-----------------------------------------------------------------\n* "
			+ block_header + " *\n"
			+ "-----------------------------------------------------------------\n\n";

		_block += block_error_s + "\n";

		for (unsigned int i = 0; i < block_error.size(); i++)
		{
			_block += block_error[i];
		}

		_block += "\n\n-----------------------------------------------------------------\n";

		Logger::m_errorLog += _block;

		if (print_to_console) std::cout << _block;
	}

	const std::string Logger::GetCurrentDateAndTime()
	{
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "date - [%d-%m-%Y] time - [%I-%M-%S]", timeinfo);
		std::string str(buffer);

		return buffer;
	}

	void Logger::DumpLogsToFile()
	{
		DumpInfoLogToFile();
		DumpErrorLogToFile();
	}

	void Logger::DumpInfoLogToFile()
	{
		std::ofstream output_file;

		std::string file_name = "[LOG] " + GetCurrentDateAndTime() + ".txt";

		output_file.open("Logs/" + file_name);

		if (output_file.fail()) Logger::ErrorBlock("File failed to open", file_name, true);

		output_file << m_log;
		output_file.close();
	}

	void Logger::DumpErrorLogToFile()
	{
		std::ofstream output_file;

		std::string file_name = "[ERROR_LOG] " + GetCurrentDateAndTime() + ".txt";

		output_file.open("Logs/" + file_name);

		if (output_file.fail()) Logger::ErrorBlock("File failed to open", file_name, true);

		output_file << m_errorLog;
		output_file.close();
	}
}
