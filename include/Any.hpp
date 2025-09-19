//
// Created by musheng on 9/19/25.
//

#ifndef SORENLIB_ANY_HPP
#define SORENLIB_ANY_HPP
#include <memory>
#include <cassert>

namespace SorenLib {
	class Any {
		public:
			Any() = default;
			~Any() = default;

			template <typename T>
			Any(T &&data):
				value_(std::make_unique<Derived<T>>(std::forward<T>(data))) {}

			template <typename T>
			Any &operator=(T &&data) {
				value_ = std::make_unique<Derived<T>>(std::forward<T>(data));
				return *this;
			}

			template <typename T>
			const T &get() const {
				auto ptr = dynamic_cast<Derived<T> *>(value_.get());
				assert(ptr != nullptr);
				return ptr->data;
			}

			template <typename T>
			T &data() {
				auto ptr = dynamic_cast<Derived<T> *>(value_.get());
				assert(ptr != nullptr);
				return ptr->data;
			}

		private:
			class Base {
				public:
					Base() = default;
					virtual ~Base() = default;
			};

			template<typename T>
			struct Derived final : public Base {
				T data;
				explicit Derived(T &&d) : data(std::forward<T>(d)) {}
				~Derived() override = default;
			};
			std::unique_ptr<Base> value_ = std::make_unique<Base>();
	};
} // SorenLib

#endif //SORENLIB_ANY_HPP