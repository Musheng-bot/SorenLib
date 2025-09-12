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
			void start(int loop_times = 1);
			void join();
			void requestStop();
			void detach();
			[[nodiscard]] bool joinable() const;
			[[nodiscard]] std::thread::id getId() const;
			[[nodiscard]] bool isRunning() const;

		protected:
			[[nodiscard]] bool isStopRequested() const;
			virtual void run() = 0;

		private:
			std::unique_ptr<std::thread> thread_ = std::make_unique<std::thread>();
			std::atomic<bool> running_{false};
			std::atomic<bool> stop_requested_{false};
			int loop_times_{1};

			void threadFunc();
			void resetStopRequest();

	};
} // SorenLib

#endif //SORENLIB_THREAD_HPP