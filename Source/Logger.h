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
	
		/*--------------------------*/

		static Logger& GetInstance()
		{
			static Logger instance;

			return instance;
		}

		Logger(Logger const&)   = delete;
		void operator=(Logger&) = delete;


		/*Debug Info Block Printers*/
		/*--------------------------*/
		void InfoBlock(const std::string& block_header, const std::string& block_info, bool print_to_console);
		void InfoBlock(const std::string& block_header, const std::vector<char>& block_info, bool print_to_console);
		void InfoBlock(const std::string& block_header, const std::string& block_info_s, const std::vector<char>& block_info, bool print_to_console);
		void InfoBlockBegin(const std::string& blockHeader);
		void InfoBlockMessage(const std::string& infoBlock);
		void InfoBlockEnd();
		/*--------------------------*/


		/*Debug Error Block Printers*/
		/*--------------------------*/
		void ErrorBlock(const std::string& block_header, const std::string& block_error, bool print_to_console);
		void ErrorBlock(const std::string& block_header, const std::vector<char>& block_error, bool print_to_console);
		void ErrorBlock(const std::string& block_header, const std::string& block_error_s, const std::vector<char>& block_error, bool print_to_console);
		/*static void ErrorBlockBegin();
		static void ErrorBlockMessage();
		static void ErrorBlockEnd();*/
		/*--------------------------*/


		/*Dump to file*/
		/*--------------------------*/
		void DumpLogsToFile();
		/*--------------------------*/

	private:
		Logger();

		/*private functions*/
		/*--------------------------*/
		void				 DumpInfoLogToFile();
		void			     DumpErrorLogToFile();
		const std::string GetCurrentDateAndTime();
		/*--------------------------*/


		std::string m_log;
		std::string m_errorLog;
		bool        m_blockBegun;

	};
}
#endif //LOGGER_HEADER

