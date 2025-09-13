//
// Created by musheng on 9/5/25.
//

#include "Thread.hpp"
#include <chrono>
using namespace std::chrono_literals;

namespace SorenLib {
	Thread::Thread() = default;

	Thread::~Thread() {
		requestStop();
		if (joinable()) {
			join();
		}
		else {
			detach();
		}
	}

	void Thread::start(const int loop_times) {
		if (!isRunning()) {
			loop_times_ = loop_times;
			resetStopRequest();
			thread_ = std::make_unique<std::thread>([this]() {
				this->threadFunc();
			});
		}
	}

	void Thread::join() {
		if (joinable()) {
			thread_->join();
		}
	}

	void Thread::requestStop() {
		stop_requested_ = true;
	}

	void Thread::resetStopRequest() {
		stop_requested_ = false;
	}

	void Thread::detach() {
		if (joinable()) {
			thread_->detach();
		}
	}

	bool Thread::joinable() const {
		return thread_->joinable();
	}

	std::thread::id Thread::getId() const {
		return thread_->get_id();
	}

	bool Thread::isRunning() const {
		return running_;
	}

	bool Thread::isStopRequested() const {
		return stop_requested_;
	}

	void Thread::threadFunc() {
		running_ = true;
		while (!isStopRequested() && loop_times_) {
			run();
			--loop_times_;
			std::this_thread::sleep_for(10ms);
		}
		running_ = false;
	}
} // SorenLib