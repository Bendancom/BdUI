#include "log.hpp"

namespace BdUI {
	Log::Log() {
		log_thread.detach();
	}
	Log::Log(LogLevel&& LevelMax) {
		this->LevelMax = LevelMax;
		log_thread.detach();
	}
	Log::Log(std::string&& log_file) {
		LevelMax = Debug;
		log_ostream = new std::ofstream(log_file);
		log_thread.detach();
	}
	Log::Log(LogLevel&& LevelMax, std::string&& log_file) {
		this->LevelMax = LevelMax;
		log_ostream = new std::ofstream(log_file);
		log_thread.detach();
	}

	void Log::SetLogFile(std::string&& file) {
		Mutex.lock();
		if (!file.empty()) log_ostream = new std::ofstream(file);
		else log_ostream = &std::cout;
		Mutex.unlock();
	}
	void Log::write(LogLevel level, std::string msg, std::source_location&& source_location) {
		Mutex.lock();
		Queue.push(std::pair<LogLevel, Message>{ level, { msg, source_location }});
		condition.notify_all();
		Mutex.unlock();
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
		std::string layout = Layout[level].load();
		layout.replace(layout.find("@func"), 5, msg.second.function_name());
		layout.replace(layout.find("@file"), 5, msg.second.file_name());
		layout.replace(layout.find("@line"), 5, std::to_string(msg.second.line()));
		layout.replace(layout.find("@column"), 7, std::to_string(msg.second.column()));
		layout.replace(layout.find("@msg"), 4, msg.first);
		if (log_ostream != nullptr) *log_ostream << layout << std::endl;
		else std::cout << layout << std::endl;
	}
}