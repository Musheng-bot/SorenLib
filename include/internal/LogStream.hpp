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
				GOOD = 0,
				ERROR = 1,
				FAIL = 2,
				Eof = 4
			};

		private:
	};
} // SorenLib

#endif //SORENLIB_LOGSTREAM_HPP