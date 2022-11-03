#include "log.hpp"

namespace BdUI {
	Log log = Log();

	Log::Log(LogLevel&& LevelMax = Debug) {
		this->LevelMax = LevelMax;
		log_thread.detach();
	}
	Log::Log(std::string&& log_file) {
		LevelMax = Debug;
		log_ofstream = new std::ofstream(log_file);
		log_thread.detach();
	}
	Log::Log(LogLevel&& LevelMax, std::string&& log_file) {
		this->LevelMax = LevelMax;
		log_ofstream = new std::ofstream(log_file);
		log_thread.detach();
	}
	Log::~Log() {
		delete log_ofstream;
	}

	void Log::SetLogFile(std::string&& file) {
		Mutex.lock();
		if (log_ofstream->is_open()) log_ofstream->close();
		log_ofstream->open(file);
		Mutex.unlock();
	}
	void Log::write(LogLevel level, std::string msg, std::source_location&& source_location) {
		Queue.push(std::pair<LogLevel, Message>{ level, { msg, source_location }});
		condition.notify_all();
	}

	void Log::Logger() {
		std::unique_lock<std::mutex> unique_lock(Mutex);
		while (true) {
			if (Queue.size() == 0) condition.wait(unique_lock);
			Out(std::move(Queue.front().second),std::move(Queue.front().first));
			Queue.pop();
		}
	}

	void Log::Out(Log::Message&& msg,LogLevel level) {
		if (level > LevelMax) return;
		std::string layout = Layout[level];
		layout.replace(layout.find("@func"), 5, msg.second.function_name());
		layout.replace(layout.find("@file"), 5, msg.second.file_name());
		layout.replace(layout.find("@line"), 5, std::to_string(msg.second.line()));
		layout.replace(layout.find("@column"), 7, std::to_string(msg.second.column()));
		layout.replace(layout.find("@msg"), 4, msg.first);
		*log_ofstream << layout << std::endl;
	}
}