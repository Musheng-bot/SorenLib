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

namespace SorenLib {
	const auto &level_to_str() {
		static const std::map<Logger::Level, std::string> level_to_str = {
			{Logger::TRACE, "TRACE"},
			{Logger::DEBUG, "DEBUG"},
			{Logger::INFO, "INFO"},
			{Logger::WARN, "WARN"},
			{Logger::ERROR, "ERROR"},
			{Logger::FATAL, "FATAL"},
		};
		return level_to_str;
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
		lowest_level_(TRACE),
		log_destination_(ThreadSafeLogDestination::getInstance(""))
	{
		*this = other.clone(other.source_);
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
		auto now = std::chrono::system_clock::now();
		auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

		auto epoch = now_ms.time_since_epoch();
		auto value = std::chrono::duration_cast<std::chrono::seconds>(epoch);
		std::time_t time = value.count();

		std::tm* local_time = std::localtime(&time);
		if (local_time == nullptr) {
			return "Invalid time"; // 处理转换失败的情况
		}

		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(epoch) % 1000;

		std::stringstream ss;
		ss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S")
		   << "." << std::setw(3) << std::setfill('0') << ms.count();

		return ss.str();
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
		auto buffer = new char[length + 1];
		vsnprintf(buffer, length + 1, fmt, args);
		va_end(args_copy);
		std::string str(buffer);
		delete[] buffer;
		return str;
	}

	void Logger::log(const Level log_level, const char *message, va_list args) const noexcept{
		if (log_level < lowest_level_) {
			std::cerr << "Over the highest level allowed!" << std::endl;
			return;
		}

		const std::string log_line =
			"[" + level_to_str().at(log_level) + "] " +
			"["	+ getTimeStamp() + "] " +
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