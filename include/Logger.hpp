//
// Created by musheng on 9/5/25.
//

#ifndef SORENLIB_LOGGER_HPP
#define SORENLIB_LOGGER_HPP
#include <memory>
#include <string>
#include <mutex>
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

			explicit Logger(const std::string &log_file = std::string(), Level lowest_level = TRACE);
			~Logger();
			void trace(const char *message, ...);
			void debug(const char *message, ...);
			void info(const char *message, ...);
			void warn(const char *message, ...);
			void error(const char *message, ...);
			void fatal(const char *message, ...);
		private:
			std::unique_ptr<LogDestination> log_destination_;
			Level lowest_level_;

			static std::string getTimeStamp();
			static std::string formatString(const char *fmt, va_list args) ;
			void log(Level log_level, const char *message, va_list args) const noexcept;
			static std::mutex &mutex();
	};
} // SorenLib

#endif //SORENLIB_LOGGER_HPP