//
// Created by musheng on 9/13/25.
//

#ifndef SORENLIB_TOPICINTERFACE_HPP
#define SORENLIB_TOPICINTERFACE_HPP

#include "internal/Publisher.hpp"

namespace SorenLib {

	template <typename MsgType>
	PublisherPtr<MsgType> createPublisher(const std::string &topic_name, const int queue_length = 10) {
		auto topic = Topic<MsgType>::createTopic(topic_name, queue_length);
		return std::make_shared<Publisher<MsgType>>(std::move(topic));
	}

	template <typename MsgType>
	SubscriberPtr<MsgType> createSubscriber(const std::string &topic_name, typename Subscriber<MsgType>::callback_t callback,const int queue_length = 10) {
		auto topic = Topic<MsgType>::createTopic(topic_name, queue_length);
		auto sub = std::make_shared<Subscriber<MsgType>>(std::move(callback));
		topic->addSubscriber(sub);
		return sub;
	}

}

#endif //SORENLIB_TOPICINTERFACE_HPP