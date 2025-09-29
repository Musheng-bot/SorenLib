//
// Created by musheng on 9/29/25.
//

#ifndef SORENLIB_OPTIONAL_HPP
#define SORENLIB_OPTIONAL_HPP

#include <memory>

namespace SorenLib {
	class Null_t {
		public:
			operator bool() const {
				return false;
			}
	};

	template<typename T>
	class Nullable {
		public:
			Nullable():
				val_(nullptr){}
			Nullable(Null_t) :
				Nullable(){}
			Nullable(const T &val) :
				val_(std::make_unique<T>(val)){}
			~Nullable() = default;

			Nullable(const Nullable &other) {
				if (other) {
					val_ = std::make_unique<Nullable>(*other.val_);
				}
				else {
					val_ = nullptr;
				}
			}
			Nullable &operator=(const Nullable &other) {
				if (this != &other) {
					if (other) {
						val_ = std::make_unique<T>(*other.val_);
					}
					else {
						val_ = nullptr;
					}
				}
				return *this;
			}
			Nullable &operator=(const T &val) {
				if (!val_) {
					val_ = std::make_unique<T>(val);
				}
				else {
					*val_ = val;
				}
				return *this;
			}
			Nullable &operator=(Null_t) {
				val_ = nullptr;
				return *this;
			}

			Nullable(Nullable &&other) = default;
			Nullable &operator=(Nullable &&other) = default;
			Nullable &operator=(T &&val) {
				if (!val_) {
					val_ = std::make_unique<T>(std::move(val));
				}
				else {
					*val_ = std::move(val);
				}
				return *this;
			}


			T &get() {
				if (val_ == nullptr) {
					throw std::runtime_error("Trying to access an empty Nullable object.");
				}
				return *val_;
			}
			const T &get() const {
				if (val_ == nullptr) {
					throw std::runtime_error("Trying to access an empty Nullable object.");
				}
				return *val_;
			}

			bool operator==(const Nullable<T> &other) const {
				if (val_ == nullptr && other.val_ == nullptr) {
					return true;
				}
				if (val_ == nullptr || other.val_ == nullptr) {
					return false;
				}
				return *val_ == *other.val_;
			}
			bool operator!=(const Nullable<T> &other) const {
				return !(*this == other);
			}
			bool operator==(Null_t) const {
				return val_ == nullptr;
			}
			bool operator!=(Null_t) const {
				return val_ != nullptr;
			}
			operator bool() const {
				return val_ != nullptr;
			}
		private:
			std::unique_ptr<T> val_;
	};

	inline Null_t null;

} // SorenLib

#endif //SORENLIB_OPTIONAL_HPP