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
#include <chrono>

#include <error.hpp>

namespace BdUI {
	
	enum LogLevel {
		Fatal = 0,
		Error = 1,
		Warning = 2,
		Info = 3,
		Debug = 4,
	};

	class Log {
	public:
		

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
		@param Time:\@time
		*/
		std::array<std::atomic<const char*>, 5> Layout = {
			"[@time|Fatal]:In @func,@msg",
			"[@time|Error]:In @func,@msg",
			"[@time|Warning]: In @func,@msg",
			"[@time|Info]: @msg",
			"[@time|Debug]: In @file : @func / Line: @line,Column: @column , @msg",
		};

		/*
		@brief ʱ����ʾ��ʽ
		@brief Layout of the time;
		@param Year:\@year
		@param Month:\@month
		@param Day:\@day
		@param Hour:\@hour
		@param Minute:\@minute
		@param Second:\@second
		*/
		std::atomic<const char*> Time_Layout = "@year/@month/@day,@hour:@minute:@second";

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
		typedef std::pair<std::string, std::source_location> LogMessage;
		std::ostream* log_ostream = &std::cout;
		std::thread log_thread = std::thread(&Log::Logger,this);
		std::queue<std::pair<LogLevel,LogMessage>> Queue;
		std::condition_variable condition;
		std::mutex Mutex;

		std::string Time_Layout_Replace();
		virtual void DefaultProcess(LogMessage&&,LogLevel);
		void Logger();
	};
}
#endif