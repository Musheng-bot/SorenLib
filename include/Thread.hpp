//
// Created by musheng on 9/5/25.
//

#ifndef SORENLIB_THREAD_HPP
#define SORENLIB_THREAD_HPP

#include <thread>
#include <memory>

namespace SorenLib {
	class Thread {
		public:
			Thread();
			virtual ~Thread();
			void start();
			void join();
			void detach();
			void stop();

			virtual void run() = 0;

		private:
			std::unique_ptr<std::thread> thread_;
			bool running_ = false;
	};
} // SorenLib

#endif //SORENLIB_THREAD_HPP