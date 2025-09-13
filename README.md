# SorenLib

这是一个常用组件的集合，包含一些对标准库的进一步封装，以及一些好用功能的封装

## 安装

首先输入以下指令：

```shell
git clone git@github.com:Musheng-bot/SorenLib.git
cd SorenLib
mkdir build
cd build
cmake ..
make
sudo cmake --install . --config Release
```

然后在`CMakeLists.txt`中添加如下内容

```cmake
find_package(SorenLib REQUIRED)

# 示例
add_executable(main main.cpp)
target_link_libraries(SorenLib PUBLIC SorenLib)
```

这样就可以正常使用`SorenLib`库了。

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
- [x] 性能优化 -- 利用C++的流对象本身的性能，我只负责写入，性能通过流对象本身提高。

## Thread

### 介绍

- 对标准库的进一步封装，提供更清晰的操作接口和更强的可控性和安全性
- 通过继承重写`run()`方法实现自定义线程任务

### 头文件

- `Thread.hpp`

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

## Pub-sub model

### 介绍

- 是基于`发布者-订阅者模式`的相关实现。
- 支持`多线程`情况下的操作
- 接口实现`简单`，内存无需手动管理，足够`安全`

### 头文件

- `TopicInterface.hpp`

### 常用接口

| 函数原型                                                                                                                                                                                          |        函数名         |   功能    |                                                                                    参数                                                                                    |                        返回值                        |  访问权限  | 备注 |
|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|:------------------:|:-------:|:------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|:-------------------------------------------------:|:------:|:--:|
| 	template \<typename MsgType\> <br/>PublisherPtr\<MsgType\> createPublisher(const std::string &topic_name, const int queue_length = 10)                                                       | `createPublisher`  |  创建发布者  |                                       `topic_name`: 话题名，不同话题之间主要靠话题名标识<br />`queue_length`: 话题消息队列长度，默认为10,传入的信息超过队列长度会舍弃旧信息，引入新信息                                       | `PublisherPtr<MsgType>`: 是储存发布者对象的shared_ptr共享指针  | public |    |
| template \<typename MsgType\> <br />SubscriberPtr\<MsgType\> createSubscriber(const std::string &topic_name, typename Subscriber\<MsgType\>::callback_t callback,const int queue_length = 10) | `createSubscriber` | 创建话题订阅者 | `topic_name`: 话题名，不同话题之间主要靠话题名标识<br />`callback`: 类型为`std::function<void(const MsgType &msg)`，是话题接收到信息后的回调函数<br />`queue_length`: 话题消息队列长度，默认为10,传入的信息超过队列长度会舍弃旧信息，引入新信息 | `SubscriberPtr<MsgType>`: 是储存订阅者对象的shared_ptr共享指针 | public |    |
| 	void Publisher::publish(const MsgType &msg)                                                                                                                                                  |     `publish`      | 发布者发布信息 |                                                                              `msg`: 需要发布的信息                                                                              |                                                   | public |    |



### 使用示例

```c++
#include <iostream>

#include "Thread.hpp"
#include "TopicInterface.hpp"

using namespace std::chrono_literals;

typedef struct {
	int x;
	int y;
} point_msg;

class Sub : public SorenLib::Thread {
	public:
		void run() override {

		}
	private:
		SorenLib::SubscriberPtr<point_msg> sub_ = SorenLib::createSubscriber<point_msg>("test", [](const point_msg& msg) {
			std::cout << msg.x << ", " << msg.y << std::endl;
		});
};

int main() {
	auto pub = SorenLib::createPublisher<point_msg>("test");
	Sub sub;
	sub.start();
	pub->publish(point_msg{1, 2});
	pub->publish(point_msg{2, 3});
	pub->publish(point_msg{3, 4});
	pub->publish(point_msg{4, 5});
	std::this_thread::sleep_for(100ms);
	sub.join();
	return 0;
}
```
