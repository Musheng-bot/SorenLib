//
// Created by musheng on 9/5/25.
//

#ifndef SORENLIB_TOPIC_HPP
#define SORENLIB_TOPIC_HPP

#include <condition_variable>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>
#include <mutex>

#include "Subscriber.hpp"

namespace SorenLib {

	template <typename MsgType>
	class Topic;

	template <typename MsgType>
	using TopicPtr = std::shared_ptr<Topic<MsgType>>;


	template <typename MsgType>
	class Topic {
		public:
			static TopicPtr<MsgType> createTopic(const std::string &name, const int queue_length) {
				if (!topics().contains(name)) {
					topics().emplace(name, std::make_shared<Topic>(queue_length));
				}
				return topics().at(name);
			}

			explicit Topic(const int queue_length):
					queue_length_(queue_length) {
				thread_ = std::thread([this]() {
					this->loopNotify();
				});
			}

			~Topic() {
				using namespace std::chrono_literals;
				terminate();
				if (thread_.joinable()) {
					thread_.join();
				}
			}
			void receiveMsg(const MsgType &msg){
				std::unique_lock lock(queue_mutex_);
				if (queue_.size() >= queue_length_) {
					queue_.pop();
				}
				queue_.push(msg);
			}
			void terminate() {
				stop_ = true;
			}
			void addSubscriber(SubscriberPtr<MsgType> &subscriber) {
				std::unique_lock lock(sub_mutex_);
				subscribers_.emplace_back(subscriber);
			}

		private:
			static std::unordered_map<std::string, TopicPtr<MsgType>> &topics() {
				static std::unordered_map<std::string, TopicPtr<MsgType>> ret;
				return ret;
			}

			void notify(const MsgType &msg) {
				for (auto &subscriber : subscribers_) {
					subscriber->receive(msg);
				}
			}
			void loopNotify();

			std::vector<SubscriberPtr<MsgType>> subscribers_;
			std::queue<MsgType> queue_;
			std::mutex queue_mutex_;
			std::mutex sub_mutex_;
			int queue_length_;
			std::atomic<bool> stop_{false};
			std::thread thread_;
	};

	template<typename MsgType>
	void Topic<MsgType>::loopNotify() {
		using namespace std::chrono_literals;
		while (!stop_) {
			if (!queue_.empty()) {
				MsgType msg;
				{
					std::unique_lock lock(queue_mutex_);
					msg = queue_.front();
					queue_.pop();
				}
				{
					std::unique_lock lock(sub_mutex_);
					notify(msg);
				}
			}
			std::this_thread::sleep_for(10ms);
		}
	}



} // SorenLib

#endif //SORENLIB_TOPIC_HPP