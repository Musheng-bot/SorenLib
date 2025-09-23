//
// Created by musheng on 9/5/25.
//

#ifndef SORENLIB_LOGGER_HPP
#define SORENLIB_LOGGER_HPP
#include <string>
#include <vector>

#include "LogDestination.hpp"

namespace SorenLib {
	class Logger {
		public:
			enum Level {
				TRACE,
				DEBUG,
				INFO,
				WARN,
				ERROR,
				FATAL,
			};

			Logger(Destination destination, const std::string &log_file, std::string source, Level lowest_level = TRACE);

			~Logger();
			Logger(const Logger &);
			Logger &operator=(const Logger &);
			Logger(Logger &&) = default;
			Logger &operator=(Logger &&) = default;

			void trace(const char *message, ...) const;
			void debug(const char *message, ...) const;
			void info(const char *message, ...) const;
			void warn(const char *message, ...) const;
			void error(const char *message, ...) const;
			void fatal(const char *message, ...) const;

			void setLowestLevel(Level level);
			void setOutputFormat(std::string fmt);
			void setSource(std::string source);
			void setTimeFormat(std::string fmt);
			void addOutputDestination(Destination destination,const std::string &file);
			void removeOutputDestination(Destination destination, const std::string &file);
		private:
			Level lowest_level_;
			std::vector<ThreadSafeLogDestination> log_destinations_;
			std::string source_;
			std::string output_format_ = "[@l] [Process @p] [Thread @T] [@s] [@t] @m";
			std::string time_format_ = "%Y-%m-%d %H:%M:%S";

			[[nodiscard]] std::string getTimeStamp() const;
			static std::string getProcessId();
			static std::string getThreadId();
			static std::string formatOutputMessage(const char *fmt, va_list args) ;

			void log(Level log_level, const char *message, va_list args) const noexcept;
			Logger(std::string source, const std::vector<ThreadSafeLogDestination> &dests, Level lowest_level);
			[[nodiscard]] Logger clone(const std::string& source) const;
			void formatLog(std::string &log_line, Level log_level, const char *message, va_list args) const;

	};
} // SorenLib

#endif //SORENLIB_LOGGER_HPP