//
// Created by musheng on 9/28/25.
//

#ifndef SORENLIB_RESULT_HPP
#define SORENLIB_RESULT_HPP

#include <memory>

namespace SorenLib {
	template<typename ResultType, typename ErrorType>
	class Result {
		public:
			[[nodiscard]] bool isError() const {
				return error_ != nullptr;
			}
			Result() = default;
			Result(ResultType &&val, ErrorType &&err) :
				value_(std::forward<ResultType>(val)),
				error_(std::make_unique<ErrorType>(std::forward<ErrorType>(err))) {}
			const ResultType &value() const {
				return value_;
			}
			const ErrorType &error() const {
				return *error_;
			}
		private:
			ResultType value_;
			std::unique_ptr<ErrorType> error_;
	};

	template<typename ErrorType>
	class Result<void, ErrorType> {
		public:
			[[nodiscard]] bool isError() const {
				return error_ != nullptr;
			}
			Result() = default;
			explicit Result(ErrorType &&err) :
				error_(std::make_unique<ErrorType>(std::forward<ErrorType>(err))) {}
			const ErrorType &error() const {
				return *error_;
			}
		private:
			std::unique_ptr<ErrorType> error_;

	};
} // SorenLib

#endif //SORENLIB_RESULT_HPP