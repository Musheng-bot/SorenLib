//
// Created by musheng on 9/5/25.
//

#ifndef SORENLIB_LOGGER_HPP
#define SORENLIB_LOGGER_HPP
#include <string>
#include <mutex>
#include <fstream>

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

			explicit Logger(Level lowest_level = FATAL, const std::string &log_file = std::string());
			~Logger();
			void trace(const char *message, ...);
			void debug(const char *message, ...);
			void info(const char *message, ...);
			void warn(const char *message, ...);
			void error(const char *message, ...);
			void fatal(const char *message, ...);
		private:
			std::fstream fout_;
			Level lowest_level_;

			static std::string getTimeStamp();
			static std::string formatString(const char *fmt, va_list args) ;
			void log(Level log_level, const char *message, va_list args) noexcept;
			static std::mutex &mutex();
	};
} // SorenLib

#endif //SORENLIB_LOGGER_HPP