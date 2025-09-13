//
// Created by musheng on 9/13/25.
//

#ifndef SORENLIB_SUBSCRIBER_HPP
#define SORENLIB_SUBSCRIBER_HPP
#include <functional>
#include <memory>

namespace SorenLib {

	template <typename MsgType>
	class Subscriber {
		public:
			using callback_t = std::function<void(const MsgType &)>;
			explicit Subscriber(callback_t function):
				callback_(std::move(function)) {}

			~Subscriber() = default;

			void receive(const MsgType &msg) {
				callback_(msg);
			}

		private:
			callback_t callback_;
	};

	template <typename MsgType>
	using SubscriberPtr = std::shared_ptr<Subscriber<MsgType>>;
} // SorenLib

#endif //SORENLIB_SUBSCRIBER_HPP