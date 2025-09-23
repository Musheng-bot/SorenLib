//
// Created by musheng on 9/5/25.
//

#include "Logger.hpp"
#include <cstdarg>
#include <cstring>
#include <fstream>
#include <map>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define LOG(level, upper_level) 	\
	void Logger::level(const char *message, ...) const {\
		va_list args;\
		va_start(args, message);\
		log(upper_level, message, args);\
		va_end(args);\
	}

namespace SorenLib {
	const auto &level_to_str(Logger::Level level) {
		static const std::string level_to_str[] =
			{"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
		return level_to_str[static_cast<int>(level)];
	}

	Logger::Logger(Destination destination, const std::string &log_file, std::string source, const Level lowest_level) :
		lowest_level_(lowest_level),
		source_(std::move(source)) {
		log_destinations_.emplace_back(ThreadSafeLogDestination::getInstance(destination, log_file));
	}

	Logger::~Logger() = default;

	Logger::Logger(const Logger &other):
		lowest_level_(other.lowest_level_),
		log_destinations_(other.log_destinations_),
		source_(other.source_) {
	}

	Logger & Logger::operator=(const Logger &other) {
		if (&other != this) {
			*this = other.clone(other.source_);
		}
		return *this;
	}

	LOG(trace, TRACE)
	LOG(debug, DEBUG)
	LOG(info, INFO)
	LOG(warn, WARN)
	LOG(error, ERROR)
	LOG(fatal, FATAL)

	void Logger::setLowestLevel(Level level) {
		lowest_level_ = level;
	}

	void Logger::setOutputFormat(std::string fmt) {
		output_format_ = std::move(fmt);
	}

	void Logger::setSource(std::string source) {
		source_ = std::move(source);
	}

	void Logger::setTimeFormat(std::string fmt) {
		time_format_ = std::move(fmt);
	}

	void Logger::addOutputDestination(const Destination destination, const std::string &file) {
		auto dest = ThreadSafeLogDestination::getInstance(destination, file);
		log_destinations_.emplace_back(dest);
	}

	void Logger::removeOutputDestination(Destination destination, const std::string &file) {
		auto dest = ThreadSafeLogDestination::getInstance(destination, file);
		auto it = std::find(log_destinations_.begin(), log_destinations_.end(), dest);
		if (it != log_destinations_.end()) {
			log_destinations_.erase(it);
		}
	}

	std::string Logger::getTimeStamp() const {
		const auto now = std::chrono::system_clock::now();
		const auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

		// 分离秒和毫秒部分
		const auto epoch = now_ms.time_since_epoch();
		const auto secs = std::chrono::duration_cast<std::chrono::seconds>(epoch);
		[[maybe_unused]] const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(epoch) - secs;

		// 转换为time_t（秒级时间戳）
		std::time_t time = std::chrono::system_clock::to_time_t(now);

		// 线程安全处理：使用thread_local缓冲区存储tm结构
		thread_local std::tm tm_buf{};

#ifdef _WIN32
		// Windows平台使用localtime_s（C11标准，VS支持）
		if (localtime_s(&tm_buf, &time) != 0) {
			return "Invalid time";
		}
#else
		// POSIX平台（Linux/macOS）使用localtime_r
		if (localtime_r(&time, &tm_buf) == nullptr) {
			return "Invalid time";
		}
#endif

		// 格式化时间字符串
		std::stringstream ss;
		ss << std::put_time(&tm_buf, time_format_.c_str());

		return ss.str();
	}

	std::string Logger::getProcessId() {
#ifdef _WIN32
		return std::to_string(static_cast<int>(GetCurrentProcessId());
#else
		return std::to_string(getpid());
#endif
	}

	std::string Logger::getThreadId() {
#ifdef _WIN32
		return std::to_string(GetCurrentThreadId());
#else
		return std::to_string(syscall(SYS_gettid));
#endif
	}

	std::string Logger::formatOutputMessage(const char *fmt, va_list args) {
		va_list args_copy;
		va_copy(args_copy, args);
		const int length = vsnprintf(nullptr, 0, fmt, args_copy);
		va_end(args_copy);
		if (length < 0) {
			va_end(args);
			return "Fail to format the string!";
		}
		std::string buffer(length, '\0');
		vsnprintf(buffer.data(), length + 1, fmt, args);
		va_end(args_copy);
		return buffer;
	}

	void Logger::log(const Level log_level, const char *message, va_list args) const noexcept{
		if (log_level < lowest_level_) {
			return;
		}

		// const std::string log_line =
		// 	"[" + level_to_str(log_level) + "] " +
		// 	"["	+ getTimeStamp() + "] " +
		// 	"[" + getProcessId() + "] " +
		// 	"[" + getThreadId() + "] " +
		// 	"[" + source_ + "] " +
		// 	formatOutputMessage(message, args) + '\n';

		std::string line = this->output_format_;
		formatLog(line, log_level, message, args);
		line += '\n';
		for (const auto &destination : log_destinations_) {
			destination.write(line);
		}
	}

	Logger::Logger(std::string source, const std::vector<ThreadSafeLogDestination> &dests, const Level lowest_level) :
		lowest_level_(lowest_level),
		log_destinations_(dests),
		source_(std::move(source)){

	}

	Logger Logger::clone(const std::string& source) const {
		return {source, log_destinations_, lowest_level_};
	}

	void Logger::formatLog(std::string &log_line, const Level log_level, const char *message, va_list args) const {
		for (auto i = 0; i < log_line.size(); ++i) {
			if (log_line[i] == '\\') {
				if (i >= log_line.size() - 1) {
					log_line = "Message Wrong!\n";
					return;
				}

				if (log_line[i + 1] == '@') {
					log_line.replace(i, 2, "@");
				}
			}
			else if (log_line[i] == '@') {
				if (i >= log_line.size() - 1) {
					log_line = "Message Wrong!\n";
					return;
				}
				std::string to_replace = log_line.substr(i, 2);

				if (log_line[i + 1] == 'p') {
					to_replace = getProcessId();
				}
				else if (log_line[i + 1] == 'T') {
					to_replace = getThreadId();
				}
				else if (log_line[i + 1] == 's') {
					to_replace = source_;
				}
				else if (log_line[i + 1] == 'm') {
					to_replace = formatOutputMessage(message, args);
				}
				else if (log_line[i + 1] == 't') {
					to_replace = getTimeStamp();
				}
				else if (log_line[i + 1] == 'l') {
					to_replace = level_to_str(log_level);
				}
				log_line.replace(i, 2, to_replace);
				i += static_cast<int>(to_replace.size()) - 1;
			}
		}
	}
} // SorenLib