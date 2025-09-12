//
// Created by musheng on 9/6/25.
//

#ifndef SORENLIB_LOGSTREAM_HPP
#define SORENLIB_LOGSTREAM_HPP
#include <streambuf>

namespace SorenLib {
	template <typename T>
	class Stream {
		public:
			enum class StreamState : int {
				STATE_GOOD = 0,
				STATE_ERROR = 1,
				STATE_FAIL = 2,
				STATE_EOF = 4
			};



		private:
	};
} // SorenLib

#endif //SORENLIB_LOGSTREAM_HPP