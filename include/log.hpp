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
#include <xhash>

#include <error.hpp>

namespace BdUI {
	/*
	Layout :
	variable use '@' to identify,escape character is '\'{
		Level : level
		Line : line
		Column : column
		Message : msg
		file_name : file
		function_name : func
	}
	*/
	class Log {
	public:
		enum Loglevel {
			Fatal = 0,
			Error = 1,
			Warning = 2,
			Info = 3,
			Debug = 4,
		};
	private:
		struct Message {
			Loglevel Level;
			std::string msg;
			std::source_location source_location;
		};

		Loglevel Level = Debug;
		std::ofstream log_ofstream;
		const char* Layout = "[@level]: In @file : @func / Line: @line,Column: @column , Message: @msg";
		std::thread log_thread;
		std::queue<Message> Queue;
		std::condition_variable condition;
		std::mutex Mutex;

		void Out(Message&&);
		void Logger();
	public:
		Log(Loglevel&& level, const char* file) : Level(level), log_thread(std::thread(&Log::Logger, this)),log_ofstream(file) {
			log_thread.detach();
		}
		Log(Loglevel&& level, const char* file, const char* layout) : Level(level), Layout(layout),log_ofstream(file),
		log_thread(std::thread(&Log::Logger, this))
		{
			log_thread.detach();
		}
		Log(Loglevel&& level) : Level(level), log_thread(std::thread(&Log::Logger, this)){
			log_thread.detach();
		}
		~Log(){}


		void SetLayout(const char*);
		void SetOfStream(const char*);
		void CloseOfStream();
		void SetLevel(Loglevel&&);

		void write(Loglevel, std::string, std::source_location&& source_location = std::source_location::current());
	};
	extern Log log;
}
#endif