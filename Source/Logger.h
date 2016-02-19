#pragma once
#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include <string>
#include <vector>
#include <GLEW/glew.h>

namespace Nova
{
	class Logger
	{
	public:

		/*Constructors & Destructor*/
		/*--------------------------*/
		Logger() = delete;
		~Logger()  = delete;
		/*--------------------------*/


		/*Debug Info Block Printers*/
		/*--------------------------*/
		static void InfoBlock(const std::string& block_header, const std::string& block_info, bool print_to_console);
		static void InfoBlock(const std::string& block_header, const std::vector<char>& block_info, bool print_to_console);
		static void InfoBlock(const std::string& block_header, const std::string& block_info_s, const std::vector<char>& block_info, bool print_to_console);
		static void InfoBlockBegin(const std::string& blockHeader);
		static void InfoBlockMessage(const std::string& infoBlock);
		static void InfoBlockEnd();
		/*--------------------------*/


		/*Debug Error Block Printers*/
		/*--------------------------*/
		static void ErrorBlock(const std::string& block_header, const std::string& block_error, bool print_to_console);
		static void ErrorBlock(const std::string& block_header, const std::vector<char>& block_error, bool print_to_console);
		static void ErrorBlock(const std::string& block_header, const std::string& block_error_s, const std::vector<char>& block_error, bool print_to_console);
		/*static void ErrorBlockBegin();
		static void ErrorBlockMessage();
		static void ErrorBlockEnd();*/
		/*--------------------------*/


		/*Dump to file*/
		/*--------------------------*/
		static void DumpLogsToFile();
		/*--------------------------*/

	private:


		/*private functions*/
		/*--------------------------*/
		static void				 DumpInfoLogToFile();
		static void			     DumpErrorLogToFile();
		static const std::string GetCurrentDateAndTime();
		/*--------------------------*/


		static std::string m_log;
		static std::string m_errorLog;
		static bool        m_blockBegun;

	};
}
#endif //LOGGER_HEADER

