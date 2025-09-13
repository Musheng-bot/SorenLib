//
// Created by musheng on 9/13/25.
//

#ifndef SORENLIB_PUBLISHER_HPP
#define SORENLIB_PUBLISHER_HPP
#include "Topic.hpp"

namespace SorenLib {

	template <typename MsgType>
	class Publisher {
		public:
			explicit Publisher(TopicPtr<MsgType> topic):
				topic_(std::move(topic)){}

			~Publisher() = default;

			void publish(const MsgType &msg) {
				topic_->receiveMsg(msg);
			}

		private:
			TopicPtr<MsgType> topic_;
	};


	template <typename MsgType>
	using PublisherPtr = std::shared_ptr<Publisher<MsgType>>;
} // SorenLib

#endif //SORENLIB_PUBLISHER_HPP