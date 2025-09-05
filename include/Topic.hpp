//
// Created by musheng on 9/5/25.
//

#ifndef SORENLIB_TOPIC_HPP
#define SORENLIB_TOPIC_HPP

#include <memory>

namespace SorenLib {

	class TopicMsg {
		public:
			TopicMsg();
			virtual ~TopicMsg();
			virtual auto
	};

	class Topic {
		public:


		private:
			std::unique_ptr<TopicMsg> msg_;
	};
} // SorenLib

#endif //SORENLIB_TOPIC_HPP