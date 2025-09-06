//
// Created by musheng on 9/5/25.
//

#include "Logger.hpp"
#include "Logger.hpp"
#include <cstdarg>
#include <cstring>
#include <fstream>
#include <map>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace SorenLib {
	const auto &level_to_str(Logger::Level level) {
		static const std::string level_to_str[] =
			{"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

		return level_to_str[static_cast<int>(level)];
	}

	Logger::Logger(const std::string &log_file, std::string source, const Level lowest_level) :
		source_(std::move(source)),
		lowest_level_(lowest_level) ,
		log_destination_(
			ThreadSafeLogDestination::getInstance(log_file)
		)
	{}

	Logger::~Logger() = default;

	Logger::Logger(const Logger &other):
		lowest_level_(other.lowest_level_),
		log_destination_(other.log_destination_.clone()),
		source_(other.source_) {
	}

	Logger & Logger::operator=(const Logger &other) {
		if (&other != this) {
			*this = other.clone(other.source_);
		}
		return *this;
	}

	Logger Logger::clone(const std::string& source) const {
		return {source, log_destination_.clone(), lowest_level_};
	}

	void Logger::trace(const char *message, ...) const {
		va_list args;
		va_start(args, message);
		log(TRACE, message, args);
		va_end(args);
	}
	void Logger::info(const char *message, ...) const {
		va_list args;
		va_start(args, message);
		log(INFO, message, args);
		va_end(args);
	}
	void Logger::warn(const char *message, ...) const {
		va_list args;
		va_start(args, message);
		log(WARN, message, args);
		va_end(args);
	}
	void Logger::error(const char *message, ...) const {
		va_list args;
		va_start(args, message);
		log(ERROR, message, args);
		va_end(args);
	}
	void Logger::fatal(const char *message, ...) const {
		va_list args;
		va_start(args, message);
		log(FATAL, message, args);
		va_end(args);
	}
	void Logger::debug(const char *message, ...) const {
		va_list args;
		va_start(args, message);
		log(DEBUG, message, args);
		va_end(args);
	}

	std::string Logger::getTimeStamp() {
		const auto now = std::chrono::system_clock::now();
		const auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

		// 分离秒和毫秒部分
		const auto epoch = now_ms.time_since_epoch();
		const auto secs = std::chrono::duration_cast<std::chrono::seconds>(epoch);
		const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(epoch) - secs;

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
		ss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S")
		   << "." << std::setw(3) << std::setfill('0') << ms.count();

		return ss.str();
	}

	std::string Logger::getProcessId() {
		return "Process " + std::to_string(getpid());
	}

	std::string Logger::getThreadId() {
#ifdef _WIN32
		return "Thread " + std::to_string(GetCurrentThreadId());
#else
		return "Thread " + std::to_string(syscall(SYS_gettid));
#endif
	}

	std::string Logger::formatString(const char *fmt, va_list args) {
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

		const std::string log_line =
			"[" + level_to_str(log_level) + "] " +
			"["	+ getTimeStamp() + "] " +
			"[" + getProcessId() + "] " +
			"[" + getThreadId() + "] " +
			"[" + source_ + "] " +
			formatString(message, args) + '\n';

		this->log_destination_.write(log_line);
	}

	Logger::Logger(std::string source, ThreadSafeLogDestination &&logger, const Level lowest_level) :
		source_(std::move(source)),
		lowest_level_(lowest_level),
		log_destination_(std::move(logger))
	{
	}
} // SorenLib