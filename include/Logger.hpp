//
// Created by musheng on 9/5/25.
//

#ifndef SORENLIB_LOGGER_HPP
#define SORENLIB_LOGGER_HPP
#include <string>
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

			explicit Logger(const std::string &log_file = std::string(), std::string source = "Unknown", Level lowest_level = TRACE);
			~Logger();
			Logger(const Logger &);
			Logger &operator=(const Logger &);
			Logger(Logger &&) = default;
			Logger &operator=(Logger &&) = default;

			[[nodiscard]] Logger clone(const std::string& source) const;
			void trace(const char *message, ...) const;
			void debug(const char *message, ...) const;
			void info(const char *message, ...) const;
			void warn(const char *message, ...) const;
			void error(const char *message, ...) const;
			void fatal(const char *message, ...) const;
		private:
			Level lowest_level_;
			ThreadSafeLogDestination log_destination_;
			std::string source_;

			static std::string getTimeStamp();
			static std::string formatString(const char *fmt, va_list args) ;
			void log(Level log_level, const char *message, va_list args) const noexcept;
			Logger(std::string source, ThreadSafeLogDestination &&logger, Level lowest_level);
	};
} // SorenLib

#endif //SORENLIB_LOGGER_HPP