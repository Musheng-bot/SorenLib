//
// Created by musheng on 9/5/25.
//

#ifndef SORENLIB_LOGDESTINATION_HPP
#define SORENLIB_LOGDESTINATION_HPP
#include <string>
#include <fstream>
#include <map>
#include <memory>
#include <mutex>

//LogDestination相关类只负责把数据写到指定的位置

namespace SorenLib {
	enum Destination {
		FILE,
		STDOUT,
		STDERR,
	};

	class LogDestination {
		public:
			LogDestination();
			virtual ~LogDestination();
			virtual void write(const std::string& message) = 0;
			[[nodiscard]] virtual std::unique_ptr<LogDestination> clone() const = 0;
	};

	class FileLogDestination final : public LogDestination {
		public:
			explicit FileLogDestination(std::string  file_name);
			~FileLogDestination() override;
			void write(const std::string& message) override;
			std::unique_ptr<LogDestination> clone() const override;
			const std::string &getLogFileName() const;
		private:
			std::fstream fout_;
			std::string file_name_;
	};

	class StdoutLogDestination : public LogDestination {
		public:
			StdoutLogDestination();
			~StdoutLogDestination() override;
			void write(const std::string& message) override;
			[[nodiscard]] std::unique_ptr<LogDestination> clone() const override;
	};

	class ErrorLogDestination final : public LogDestination {
		public:
			ErrorLogDestination();
			~ErrorLogDestination() override;
			void write(const std::string& message) override;
			[[nodiscard]] std::unique_ptr<LogDestination> clone() const override;
	};

	class ThreadSafeLogDestination {
		public:
			static ThreadSafeLogDestination getInstance(Destination dest, const std::string &file);
			~ThreadSafeLogDestination();
			ThreadSafeLogDestination(const ThreadSafeLogDestination &other);
			ThreadSafeLogDestination &operator=(const ThreadSafeLogDestination &other);
			ThreadSafeLogDestination(ThreadSafeLogDestination &&other) noexcept;
			ThreadSafeLogDestination &operator=(ThreadSafeLogDestination &&other) noexcept;

			ThreadSafeLogDestination clone() const;
			void write(const std::string& message) const;

		private:
			std::unique_ptr<LogDestination> impl_;
			mutable std::shared_ptr<std::mutex> mutex_;

			explicit ThreadSafeLogDestination(std::unique_ptr<LogDestination> impl, std::shared_ptr<std::mutex> mutex = std::make_shared<std::mutex>());
			static std::map<std::string, ThreadSafeLogDestination> &dests();
			static std::mutex &dests_mutex();
	};
} // SorenLib

#endif //SORENLIB_LOGDESTINATION_HPP