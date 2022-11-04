#ifndef BDUI_LOG
#define BDUI_LOG

#include <source_location>
#include <string>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <array>
#include <atomic>
#include <fstream>

#include <error.hpp>

namespace BdUI {
	
	class Log {
	public:
		enum LogLevel {
			Fatal = 0,
			Error = 1,
			Warning = 2,
			Info = 3,
			Debug = 4,
		};

		Log();
		Log(LogLevel&& LevelMax);
		Log(std::string&& log_file);
		Log(LogLevel&& LevelMax, std::string&& log_file);
		~Log(){}

		/*
		@brief �ɶ�̬��ʾ���±���
		@brief these variable can be showed dynamically
		@param Line:\@line
		@param Column:\@column
		@param Message:\@msg
		@param FileName:\@file
		@param FunctionName:\@func
		*/
		std::array<std::atomic<const char*>, 5> Layout = {
			"[Fatal]:In @func,@msg",
			"[Error]:In @func,@msg",
			"[Warning]: In @func,@msg",
			"[Info]: @msg",
			"[Debug]: In @file : @func / Line: @line,Column: @column , @msg",
		};

		/*
			�����־����ȼ�
			the max level that the data is output;
		*/
		std::atomic<LogLevel> LevelMax = Debug;

		/*
		* @brief ������־�洢λ��
		* @param FilePath	��־�洢λ��,��Ϊstring.empty()�������������|the filepath log store,if string is empty it output to the command line
		*/
		void SetLogFile(std::string&& FilePath);
		void write(LogLevel Level, std::string msg, std::source_location&& source_location = std::source_location::current());
	private:
		typedef std::pair<std::string, std::source_location> Message;

		std::ostream* log_ostream = &std::cout;
		std::thread log_thread = std::thread(&Log::Logger,this);
		std::queue<std::pair<LogLevel,Message>> Queue;
		std::condition_variable condition;
		std::mutex Mutex;

		void Out(Message&&,LogLevel);
		void Logger();
	};
}
#endif