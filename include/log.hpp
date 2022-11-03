#ifndef BDUI_LOG
#define BDUI_LOG

#include <source_location>
#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <array>
#include <atomic>

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
		
		Log(LogLevel&& LevelMax = Debug);
		Log(std::string&& log_file);
		Log(LogLevel&& LevelMax, std::string&& log_file);
		~Log();

		/*
		@brief 可动态显示如下变量
		@brief these variable can be showed dynamically
		@param Line:\@line
		@param Column:\@column
		@param Message:\@msg
		@param FileName:\@file
		@param FunctionName:\@func
		*/
		std::array<std::atomic<std::string>, 5> Layout{
			std::string("[Fatal]:In @func,@msg"),
			std::string("[Error]:In @func,@msg"),
			std::string("[Warning]: In @func,@msg"),
			std::string("[Info]: @msg"),
			std::string("[Debug]: In @file : @func / Line: @line,Column: @column , @msg"),
		};
		/*
			最大日志输出等级
			the max level that the data is output;
		*/
		std::atomic<LogLevel> LevelMax;

		void SetLogFile(std::string&&);
		void write(LogLevel Level, std::string msg, std::source_location&& source_location = std::source_location::current());
	private:
		typedef std::pair<std::string, std::source_location> Message;

		std::ofstream* log_ofstream;
		std::thread log_thread;
		std::queue<std::pair<LogLevel,Message>> Queue;
		std::condition_variable condition;
		std::mutex Mutex;

		void Out(Message&&,LogLevel);
		void Logger();
	};
	extern Log log;
}
#endif