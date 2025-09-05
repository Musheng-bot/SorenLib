//
// Created by musheng on 9/5/25.
//

#include "Logger.hpp"
#include <cstdarg>
#include <cstring>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>
#include <unistd.h>
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
	const auto &log_level_error() {
		static const std::string log_level_error = "Over the highest level allowed!";
		return log_level_error;
	}
	const auto &file_open_error() {
		static const std::string file_open_error = "Fail to open the log file!";
		return file_open_error;
	}
	const auto &format_error() {
		static const std::string format_error = "Fail to format the string!";
		return format_error;
	}
	const auto &write_error() {
		static const std::string write_error = "Failed to write to the log file!";
		return write_error;
	}

	Logger::Logger(const Level lowest_level, const std::string &log_file) :
		lowest_level_(lowest_level) {
		if (log_file.empty()) {
			return;
		}

		fout_.open(log_file.c_str(), std::ofstream::out | std::ofstream::app | std::ofstream::binary);
		if (fout_.fail()) {
			std::cerr << file_open_error() << std::endl;
		}
	}

	Logger::~Logger() {
		fout_.close();
	}

	void Logger::trace(const char *message, ...)  {
		va_list args;
		va_start(args, message);
		log(TRACE, message, args);
		va_end(args);
	}
	void Logger::info(const char *message, ...)  {
		va_list args;
		va_start(args, message);
		log(INFO, message, args);
		va_end(args);
	}
	void Logger::warn(const char *message, ...)  {
		va_list args;
		va_start(args, message);
		log(WARN, message, args);
		va_end(args);
	}
	void Logger::error(const char *message, ...)  {
		va_list args;
		va_start(args, message);
		log(ERROR, message, args);
		va_end(args);
	}
	void Logger::fatal(const char *message, ...)  {
		va_list args;
		va_start(args, message);
		log(FATAL, message, args);
		va_end(args);
	}
	void Logger::debug(const char *message, ...)  {
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
			return format_error();
		}
		auto buffer = new char[length + 1];
		vsnprintf(buffer, length + 1, fmt, args);
		va_end(args_copy);
		std::string str(buffer);
		delete[] buffer;
		return str;
	}

	void Logger::log(const Level log_level, const char *message, va_list args) noexcept{
		std::unique_lock lock(mutex());
		if (!fout_.is_open()) {
			std::cerr << file_open_error() << std::endl;
			return;
		}
		if (log_level < lowest_level_) {
			std::cerr << log_level_error() << std::endl;
			return;
		}

		const std::string log_line =
			'[' + level_to_str().at(log_level) + "] " +
			'['	+ getTimeStamp() + "] " +
			formatString(message, args) + '\n';

		fout_ << log_line << std::flush;
		if (fout_.bad()) {
			std::cerr << write_error() << std::endl;
		}
	}

	std::mutex &Logger::mutex() {
		static std::mutex mtx;
		return mtx;
	}
} // SorenLib