#include "log.hpp"

namespace BdUI {
	Log log(Log::Debug);

	void Log::SetLevel(Log::Loglevel&& level) {
		Mutex.lock();
		Level = level;
		Mutex.unlock();
	}
	void Log::SetOfStream(const char* file) {
		Mutex.lock();
		if (log_ofstream.is_open()) log_ofstream.close();
		log_ofstream.open(file);
		Mutex.unlock();
	}
	void Log::CloseOfStream() {
		Mutex.lock();
		log_ofstream.close();
		Mutex.unlock();
	}
	void Log::SetLayout(const char* layout) {
		Mutex.lock();
		Layout = layout;
		Mutex.unlock();
	}

	void Log::write(Loglevel level, std::string msg, std::source_location&& source_location) {
		Mutex.lock();
		Queue.push({ level,msg,source_location });
		Mutex.unlock();
		condition.notify_all();
	}

	void Log::Logger() {
		std::unique_lock<std::mutex> unique_lock(Mutex);
		while (true) {
			if (Queue.size() == 0) condition.wait(unique_lock);
			Out(std::move(Queue.front()));
			Queue.pop();
		}
	}

	void Log::Out(Log::Message&& msg) {
		if (msg.Level > Level) return;
		const char* layout = Layout;
		for (unsigned char i = 0; i < strlen(layout); i++) {
			if (layout[i] == '\\' && layout[i + 1] == '@') {
				if (log_ofstream.is_open()) log_ofstream << '@';
				else std::clog << '@';
				i++;
				continue;
			}
			else if (layout[i] == '@') {
				i++;
				const char* var = layout + i;
				if (strncmp(var, "file", 4) == 0) {
					if (log_ofstream.is_open()) log_ofstream << msg.source_location.file_name();
					else std::clog << msg.source_location.file_name();
					i += 4;
				}
				else if (strncmp(var, "line", 4) == 0) {
					if (log_ofstream.is_open()) log_ofstream << msg.source_location.line();
					else std::clog << msg.source_location.line();
					i += 4;
				}
				else if (strncmp(var, "func", 4) == 0) {
					if (log_ofstream.is_open()) log_ofstream << msg.source_location.function_name();
					else std::clog << msg.source_location.function_name();
					i += 4;
				}
				else if (strncmp(var, "column", 6) == 0) {
					if (log_ofstream.is_open()) log_ofstream << msg.source_location.column();
					else std::clog << msg.source_location.column();
					i += 6;
				}
				else if (strncmp(var, "level", 5) == 0) {
					const char* level = nullptr;
					switch (msg.Level) {
						case Fatal: level = "Fatal"; break;
						case Error: level = "Error"; break;
						case Warning: level = "Warning"; break;
						case Info: level = "Info"; break;
						case Debug: level = "Debug"; break;
					}
					if (log_ofstream.is_open()) log_ofstream << level;
					else std::clog << level;
					i += 5;
				}
				else if (strncmp(var, "msg", 3) == 0) {
					if (log_ofstream.is_open()) log_ofstream << msg.msg;
					else std::clog << msg.msg;
					i += 3;
				}
				else throw error::Log::LayoutError();
			}
			if (log_ofstream.is_open()) log_ofstream << *(layout + i);
			else std::clog << *(layout + i);
		}
		if (log_ofstream.is_open()) log_ofstream << std::endl;
		else std::clog << std::endl;
	}
}