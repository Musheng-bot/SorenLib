# SorenLib

这是一个常用组件的集合，包含一些对标准库的进一步封装，以及一些好用功能的封装

## Logger

待更新的内容：
- [ ] 日志对多个目标输出
- [ ] 日志自定义格式化
- [x] 日志添加线程ID，进程ID的提示
- [ ] 多线程环境的无锁优化
- [ ] 日志轮转，在日志文件大小达到上限时切换日志输出目标
- [ ] 异步写入，缓冲机制，异常处理
- [ ] 支持线程局部上下文
- [ ] 错误处理，单次日志的原子化
- [ ] 性能优化

## Thread

### 介绍

- 对标准库的进一步封装，提供更清晰的操作接口和更强的可控性和安全性
- 通过继承重写`run()`方法实现自定义线程任务

### 接口

| 函数原型                           |       函数名       |                 功能                 |                        参数                         |       返回值       |   访问权限    | 备注 |
|:-------------------------------|:---------------:|:----------------------------------:|:-------------------------------------------------:|:---------------:|:---------:|:--:|
| void start(int loop_times = 1) |      start      |             启动线程，开始执行              | loop_times : 重复执行需要的任务几次，默认一次，传入-1代表无限循环，传入0代表不执行 |        无        |  public   | 无  |
| void join()                    |      join       |             让线程在此处join             |                                                   |                 |  public   |    |
| void requestStop()             |   requestStop   |  请求线程终止，线程会在一次任务正常执行完毕，或者用户自定义处退出  |                                                   |                 |  public   |    |
| void detach()                  |     detach      |             让线程detach              |                                                   |                 |  public   |    |
| bool joinable() const          |    joinable     | 返回线程是否可以join，detach和未start时不可以join |                                                   |      bool       |  public   |
| std::thread::id getId() const  |      getId      |             返回线程的标识Id              |                                                   | std::thread::id |  public   |    |
| bool isRunning() const         |    isRunning    |            返回线程函数是否正在执行            |                                                   |      bool       |  public   |    |
| virtual void run() = 0         |       run       |  用于填写需要执行的任务，通过继承 Thread 类实现自定义任务  |                                                   |                 | protected |    |
| bool isStopRequested() const   | isStopRequested |  用于指示是否有请求终止线程，用于在run()函数中自定义退出节点  |                                                   |      bool       | protected |    |

### 使用样例：

```c++
#include <iostream>
#include <thread>
#include <sstream>
#include "Thread.hpp"

using namespace std::chrono_literals;
#define NUM_THREADS 10

class Task : public SorenLib::Thread {
	public:
		void run() override {
			std::cout << "Task started\n" << std::flush;
			std::stringstream ss;
			ss << "Thread id: " << getId() << '\n';
			std::cout << ss.str() << std::flush;
			std::cout << "Task ends\n" << std::flush;
		}
};

int main() {
	Task threads[NUM_THREADS];
	for (auto & thread : threads) {
		thread.start();
	}
	for (auto & thread : threads) {
		thread.join();
	}
	return 0;
}
```

