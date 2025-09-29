//
// Created by musheng on 9/28/25.
//

#ifndef SORENLIB_RESULT_HPP
#define SORENLIB_RESULT_HPP

#include <memory>
#include <cassert>

namespace SorenLib {
	template<typename ResultType, typename ErrorType>
	class Result {
		public:
			[[nodiscard]] bool isError() const {
				return error_ != nullptr;
			}
			Result() = default;
			Result(const ResultType &val, const ErrorType &err) :
				value_(val),
				error_(std::make_unique<ErrorType>(err)) {}
			Result(const ResultType &val) :
				value_(val) {}
			const ResultType &value() const {
				return value_;
			}
			const ErrorType &error() const {
				assert(isError() && "The result has no error!");
				return *error_;
			}
			operator bool() const {
				return isError();
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
			Result(const ErrorType &err) :
				error_(std::make_unique<ErrorType>(err)) {}
			const ErrorType &error() const {
				assert(isError() && "The result has no error!");
				return *error_;
			}
			operator bool() const {
				return isError();
			}
		private:
			std::unique_ptr<ErrorType> error_;
	};
} // SorenLib

#endif //SORENLIB_RESULT_HPP