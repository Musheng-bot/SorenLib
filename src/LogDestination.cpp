//
// Created by musheng on 9/5/25.
//

#include "LogDestination.hpp"

#include <iostream>
#include <mutex>
#include <utility>

namespace SorenLib {
	LogDestination::LogDestination() = default;
	LogDestination::~LogDestination() = default;

	FileLogDestination::FileLogDestination(std::string file_name):
		file_name_(std::move(file_name))
	{
		fout_.open(file_name_, std::ofstream::out | std::ofstream::app | std::ofstream::binary);
		if (fout_.fail()) {
			std::cerr << "Failed to open log file " << file_name_ << std::endl;
		}
	}

	FileLogDestination::~FileLogDestination() {
		if (fout_.is_open()) {
			fout_.close();
		}
	}

	void FileLogDestination::write(const std::string &message) {
		if (!fout_.is_open()) {
			std::cerr << "The log file " << file_name_ << " is not opened." << std::endl;
			return;
		}
		fout_ << message ;
	}

	std::unique_ptr<LogDestination> FileLogDestination::clone() const {
		return std::make_unique<FileLogDestination>(file_name_);
	}

	const std::string & FileLogDestination::getLogFileName() const {
		return file_name_;
	}

	StdoutLogDestination::StdoutLogDestination() = default;

	StdoutLogDestination::~StdoutLogDestination() = default;

	void StdoutLogDestination::write(const std::string &message) {
		std::cout << message ;
	}

	std::unique_ptr<LogDestination> StdoutLogDestination::clone() const {
		return std::make_unique<StdoutLogDestination>();
	}

	ErrorLogDestination::ErrorLogDestination() = default;

	ErrorLogDestination::~ErrorLogDestination() = default;

	void ErrorLogDestination::write(const std::string &message) {
		std::cerr << message ;
	}

	std::unique_ptr<LogDestination> ErrorLogDestination::clone() const {
		return std::make_unique<ErrorLogDestination>();
	}

	ThreadSafeLogDestination ThreadSafeLogDestination::getInstance(Destination dest, const std::string &file) {
		std::unique_lock lock(dests_mutex());
		std::string log = file;
		if (dest == STDOUT) {
			log = "stdout";
		}
		else if (dest == STDERR) {
			log = "stderr";
		}
		if (!dests().contains(log)) {
			dests().emplace(log, ThreadSafeLogDestination(std::make_unique<FileLogDestination>(log), FILE));
		}
		return dests().at(std::string(log)).clone();
	}

	ThreadSafeLogDestination::~ThreadSafeLogDestination() = default;

	ThreadSafeLogDestination ThreadSafeLogDestination::clone() const {
		return ThreadSafeLogDestination(impl_->clone(), dest_, mutex_);
	}

	ThreadSafeLogDestination::ThreadSafeLogDestination(const ThreadSafeLogDestination &other) :
		impl_(other.impl_->clone()),
		mutex_(other.mutex_),
		dest_(other.dest_){
	}

	ThreadSafeLogDestination & ThreadSafeLogDestination::operator=(const ThreadSafeLogDestination &other) {
		if (this == &other) {
			return *this;
		}
		*this = other.clone();
		return *this;
	}

	ThreadSafeLogDestination::ThreadSafeLogDestination(ThreadSafeLogDestination &&other) noexcept = default;

	ThreadSafeLogDestination & ThreadSafeLogDestination::operator=(ThreadSafeLogDestination &&other) noexcept = default;

	void ThreadSafeLogDestination::write(const std::string &message) const {
		std::unique_lock lock(*mutex_);
		this->impl_->write(message);
	}

	bool ThreadSafeLogDestination::operator==(const ThreadSafeLogDestination &other) const {
		if (dest_ != other.dest_) {
			return false;
		}
		if (dest_ == FILE) {
			auto ptr1 = dynamic_cast<FileLogDestination*>(this->impl_.get());
			auto ptr2 = dynamic_cast<FileLogDestination*>(other.impl_.get());
			return ptr1->getLogFileName() == ptr2->getLogFileName();
		}
		return true;
	}

	ThreadSafeLogDestination::ThreadSafeLogDestination( std::unique_ptr<LogDestination> impl,
														const Destination dest,
														std::shared_ptr<std::mutex> mutex) :
		impl_(std::move(impl)),
		mutex_(std::move(mutex)),
		dest_(dest){

	}

	std::map<std::string, ThreadSafeLogDestination> &ThreadSafeLogDestination::dests() {
		static std::map<std::string, ThreadSafeLogDestination> dests = {
			{"stdout", ThreadSafeLogDestination(std::make_unique<StdoutLogDestination>(), STDOUT)},
			{"stderr", ThreadSafeLogDestination(std::make_unique<ErrorLogDestination>(), STDERR)},
		};
		return dests;
	}

	std::mutex &ThreadSafeLogDestination::dests_mutex() {
		static std::mutex dests_mutex;
		return dests_mutex;
	}
} // SorenLib