//
// Created by musheng on 9/5/25.
//

#ifndef SORENLIB_LOGDESTINATION_HPP
#define SORENLIB_LOGDESTINATION_HPP
#include <string>
#include <fstream>

namespace SorenLib {
	class LogDestination {
		public:
			LogDestination();
			virtual ~LogDestination();
			virtual void write(const std::string& message) = 0;
	};

	class FileLogDestination final : public LogDestination {
		public:
			explicit FileLogDestination(const std::string& file_name_);
			~FileLogDestination() override;
			void write(const std::string& message) override;\
		private:
			std::fstream fout_;
	};

	class StdoutLogDestination : public LogDestination {
		public:
			StdoutLogDestination();
			~StdoutLogDestination() override;
			void write(const std::string& message) override;
	};
} // SorenLib

#endif //SORENLIB_LOGDESTINATION_HPP