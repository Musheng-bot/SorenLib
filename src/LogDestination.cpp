//
// Created by musheng on 9/5/25.
//

#include "LogDestination.hpp"

#include <iostream>

namespace SorenLib {
	LogDestination::LogDestination() = default;
	LogDestination::~LogDestination() = default;

	FileLogDestination::FileLogDestination(const std::string &file_name_) {
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
			std::cerr << "The log file is not opened." << std::endl;
			return;
		}
		fout_ << message << std::flush;
	}

	StdoutLogDestination::StdoutLogDestination() = default;

	StdoutLogDestination::~StdoutLogDestination() = default;

	void StdoutLogDestination::write(const std::string &message) {
		std::cout << message << std::flush;
	}
} // SorenLib