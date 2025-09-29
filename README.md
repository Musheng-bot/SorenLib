# SorenLib

这是一个常用组件的集合，包含一些对标准库的进一步封装，以及一些好用功能的封装

## 安装

首先输入以下指令：

```shell
git clone https://github.com/Musheng-bot/SorenLib.git
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
target_link_libraries(main PUBLIC SorenLib)
```

这样就可以正常使用`SorenLib`库了。

---

## Logger

### 介绍

一个支持多线程环境的日志输出类，支持自定义输出格式，显示当前进程，线程，规定日志等级等功能

### 头文件

#### 接口文件

- `Logger.hpp`

#### 实现文件

- `LogDestination.hpp`
- `Logger.cpp`
- `LogDestination.cpp`

### 接口

- Logger类，用于日志记录
- Level枚举量，标识日志等级，一共有六个等级，从低到高`TRACE`, `DEBUG`, `INFO`, `WARN`, `ERROR`, `FATAL`六个枚举量。

#### 1. Logger 构造函数

| 项目       | 详情                                                                                                                                                           |
|----------|--------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **函数原型** | Logger(Destination destination, const std::string &log_file, std::string source, Level lowest_level = TRACE);                                                |
| **函数名**  | Logger                                                                                                                                                       |
| **功能**   | 创建一个日志对象                                                                                                                                                     |
| **参数**   | - `destination`: 日志输出目标（标准输出流、错误流、文件）<br>- `log_file`: 输出文件路径（仅`destination`为`FILE`时生效）<br>- `source`: 日志信息来源（用于记录标识）<br>- `lowest_level`: 最低日志等级（默认`TRACE`） |
| **返回值**  | 无                                                                                                                                                            |
| **访问权限** | public                                                                                                                                                       |
| **备注**   | 初始化日志对象的输出方式、来源标识和过滤等级                                                                                                                                       |


#### 2. trace 函数

| 项目       | 详情                                                     |
|----------|--------------------------------------------------------|
| **函数原型** | void Logger::trace(const char *fmt, ...)               |
| **函数名**  | trace                                                  |
| **功能**   | 发布 trace 等级日志                                          |
| **参数**   | - `fmt`: 消息格式（符合 C 语言格式规范）<br>- `...`: 不定量参数（与格式字符串匹配） |
| **返回值**  | 无                                                      |
| **访问权限** | public                                                 |
| **备注**   | 最低等级日志，通常用于详细调试信息                                      |


#### 3. debug 函数

| 项目       | 详情                                                     |
|----------|--------------------------------------------------------|
| **函数原型** | void Logger::debug(const char *fmt, ...)               |
| **函数名**  | debug                                                  |
| **功能**   | 发布 debug 等级日志                                          |
| **参数**   | - `fmt`: 消息格式（符合 C 语言格式规范）<br>- `...`: 不定量参数（与格式字符串匹配） |
| **返回值**  | 无                                                      |
| **访问权限** | public                                                 |
| **备注**   | 用于调试过程中的关键信息输出                                         |


#### 4. info 函数

| 项目       | 详情                                                     |
|----------|--------------------------------------------------------|
| **函数原型** | void Logger::info(const char *fmt, ...)                |
| **函数名**  | info                                                   |
| **功能**   | 发布 info 等级日志                                           |
| **参数**   | - `fmt`: 消息格式（符合 C 语言格式规范）<br>- `...`: 不定量参数（与格式字符串匹配） |
| **返回值**  | 无                                                      |
| **访问权限** | public                                                 |
| **备注**   | 用于记录程序运行的常规信息                                          |


#### 5. warn 函数

| 项目       | 详情                                                     |
|----------|--------------------------------------------------------|
| **函数原型** | void Logger::warn(const char *fmt, ...)                |
| **函数名**  | warn                                                   |
| **功能**   | 发布 warn 等级日志                                           |
| **参数**   | - `fmt`: 消息格式（符合 C 语言格式规范）<br>- `...`: 不定量参数（与格式字符串匹配） |
| **返回值**  | 无                                                      |
| **访问权限** | public                                                 |
| **备注**   | 用于警告可能的异常情况（不影响程序运行）                                   |


#### 6. error 函数

| 项目       | 详情                                                     |
|----------|--------------------------------------------------------|
| **函数原型** | void Logger::error(const char *fmt, ...)               |
| **函数名**  | error                                                  |
| **功能**   | 发布 error 等级日志                                          |
| **参数**   | - `fmt`: 消息格式（符合 C 语言格式规范）<br>- `...`: 不定量参数（与格式字符串匹配） |
| **返回值**  | 无                                                      |
| **访问权限** | public                                                 |
| **备注**   | 用于记录程序运行中的错误（可能影响功能但不导致崩溃）                             |


#### 7. fatal 函数

| 项目       | 详情                                                     |
|----------|--------------------------------------------------------|
| **函数原型** | void Logger::fatal(const char *fmt, ...)               |
| **函数名**  | fatal                                                  |
| **功能**   | 发布 fatal 等级日志                                          |
| **参数**   | - `fmt`: 消息格式（符合 C 语言格式规范）<br>- `...`: 不定量参数（与格式字符串匹配） |
| **返回值**  | 无                                                      |
| **访问权限** | public                                                 |
| **备注**   | 用于记录致命错误（通常导致程序终止）                                     |


#### 8. setLowestLevel 函数

| 项目       | 详情                                        |
|----------|-------------------------------------------|
| **函数原型** | void Logger::setLowestLevel(Level level); |
| **函数名**  | setLowestLevel                            |
| **功能**   | 设置日志等级阈值，低于该等级的日志不会发布                     |
| **参数**   | - `level`: 日志等级（如`INFO`、`WARN`等）          |
| **返回值**  | 无                                         |
| **访问权限** | public                                    |
| **备注**   | 用于动态调整日志输出粒度（例如生产环境可设为`INFO`屏蔽调试日志）       |


#### 9. setOutputFormat 函数

| 项目       | 详情                                                                                                    |
|----------|-------------------------------------------------------------------------------------------------------|
| **函数原型** | void Logger::setOutputFormat(std::string fmt)                                                         |
| **函数名**  | setOutputFormat                                                                                       |
| **功能**   | 设置日志输出格式（包含消息、等级、进程ID等信息）                                                                             |
| **参数**   | - `fmt`: 格式字符串（支持特殊占位符）                                                                               |
| **返回值**  | 无                                                                                                     |
| **访问权限** | public                                                                                                |
| **备注**   | 支持的占位符:<br>- `@p`: 进程ID<br>- `@T`: 线程ID<br>- `@s`: 日志来源<br>- `@m`: 消息<br>- `@t`: 时间信息<br>- `@l`: 日志等级 |

#### 10. setSource函数

| 项目       | 详情                                         |
|----------|--------------------------------------------|
| **函数原型** | void Logger::setSource(std::string source) |
| **函数名**  | setSource                                  |
| **功能**   | 设置日志信息来源                                   |
| **参数**   | - `source`: 日志信息来源                         |
| **返回值**  | 无                                          |
| **访问权限** | public                                     |
| **备注**   |                                            |

#### 11. setTimeFormat函数

| 项目       | 详情                                          |
|----------|---------------------------------------------|
| **函数原型** | void Logger::setTimeFormat(std::string fmt) |
| **函数名**  | setTimeFormat                               |
| **功能**   | 设置日志时间信息格式                                  |
| **参数**   | - `fmt`: 日志时间信息格式                           |
| **返回值**  | 无                                           |
| **访问权限** | public                                      |
| **备注**   | 关于格式化字符串可以参考[标准库时间格式化信息](TimeFormatInfo.md) |

#### 12. addOutputDestination函数

| 项目       | 详情                                                                                 |
|----------|------------------------------------------------------------------------------------|
| **函数原型** | void Logger::addOutputDestination(Destination destination,const std::string &file) |
| **函数名**  | addOutputDestination                                                               |
| **功能**   | 增加日志输出目标                                                                           |
| **参数**   | - `destination`: 日志输出目标<br/>- `file`: 日志输出文件(仅`destination`为FILE时才会生效              |
| **返回值**  | 无                                                                                  |
| **访问权限** | public                                                                             |
| **备注**   |                                                                                    |

#### 13. removeOutputDestination函数

| 项目       | 详情                                                                                    |
|----------|---------------------------------------------------------------------------------------|
| **函数原型** | void Logger::removeOutputDestination(Destination destination,const std::string &file) |
| **函数名**  | removeOutputDestination                                                               |
| **功能**   | 去除一个日志输出目标                                                                            |
| **参数**   | - `destination`: 日志输出目标<br/>- `file`: 日志输出文件(仅`destination`为FILE时才会生效                 |
| **返回值**  | 无                                                                                     |
| **访问权限** | public                                                                                |
| **备注**   | 如果没有对应的目标删除，则不会发生任何事情                                                                 |


### 使用样例

### 待更新内容

- [x] 日志对多个目标输出
- [x] 日志自定义格式化
- [x] 日志添加线程ID，进程ID的提示
- [ ] 日志轮转，在日志文件大小达到上限时切换日志输出目标
- [x] 性能优化 -- 利用C++的流对象本身的性能，我只负责写入，性能通过流对象本身提高。

## Thread

### 介绍

- 对标准库的进一步封装，提供更清晰的操作接口和更强的可控性和安全性
- 通过继承重写`run()`方法实现自定义线程任务

### 头文件


#### 接口文件

- `Thread.hpp`

#### 实现文件

- `Thread.cpp`

### 接口

#### 1. start 函数
| 项目       | 详情                                           |
|----------|----------------------------------------------|
| **函数原型** | void Thread::start(int loop_times = 1)       |
| **函数名**  | start                                        |
| **功能**   | 启动线程，开始执行                                    |
| **参数**   | `loop_times`：重复执行任务的次数（默认1次；-1代表无限循环；0代表不执行） |
| **返回值**  | 无                                            |
| **访问权限** | public                                       |
| **备注**   | 无                                            |


 #### 2. join 函数
| 项目       | 详情                           |
|----------|------------------------------|
| **函数原型** | void Thread::join()          |
| **函数名**  | join                         |
| **功能**   | 让线程在此处join（阻塞当前线程，等待该线程执行完毕） |
| **参数**   | 无                            |
| **返回值**  | 无                            |
| **访问权限** | public                       |
| **备注**   | 无                            |


 #### 3. requestStop 函数
| 项目       | 详情                              |
|----------|---------------------------------|
| **函数原型** | void Thread::requestStop()      |
| **函数名**  | requestStop                     |
| **功能**   | 请求线程终止（线程会在一次任务正常执行完毕或用户自定义处退出） |
| **参数**   | 无                               |
| **返回值**  | 无                               |
| **访问权限** | public                          |
| **备注**   | 无                               |


 #### 4. detach 函数
| 项目       | 详情                                   |
|----------|--------------------------------------|
| **函数原型** | void Thread::detach()                |
| **函数名**  | detach                               |
| **功能**   | 让线程detach（分离线程，线程执行完毕后资源自动释放，无需join） |
| **参数**   | 无                                    |
| **返回值**  | 无                                    |
| **访问权限** | public                               |
| **备注**   | 无                                    |


 #### 5. joinable 函数
| 项目       | 详情                                     |
|----------|----------------------------------------|
| **函数原型** | bool Thread::joinable() const          |
| **函数名**  | joinable                               |
| **功能**   | 返回线程是否可以join（detach状态或未start状态下不可join） |
| **参数**   | 无                                      |
| **返回值**  | bool（`true`可join，`false`不可join）        |
| **访问权限** | public                                 |
| **备注**   | 无                                      |


 #### 6. getId 函数
| 项目       | 详情                                    |
|----------|---------------------------------------|
| **函数原型** | std::thread::id Thread::getId() const |
| **函数名**  | getId                                 |
| **功能**   | 返回线程的标识Id                             |
| **参数**   | 无                                     |
| **返回值**  | std::thread::id（线程唯一标识）               |
| **访问权限** | public                                |
| **备注**   | 无                                     |


 #### 7. isRunning 函数
| 项目       | 详情                             |
|----------|--------------------------------|
| **函数原型** | bool Thread::isRunning() const |
| **函数名**  | isRunning                      |
| **功能**   | 返回线程函数是否正在执行                   |
| **参数**   | 无                              |
| **返回值**  | bool（`true`正在执行，`false`未执行）    |
| **访问权限** | public                         |
| **备注**   | 无                              |


 #### 8. run 函数
| 项目       | 详情                              |
|----------|---------------------------------|
| **函数原型** | virtual void Thread::run() = 0  |
| **函数名**  | run                             |
| **功能**   | 用于填写需要执行的任务（通过继承Thread类实现自定义任务） |
| **参数**   | 无                               |
| **返回值**  | 无                               |
| **访问权限** | protected                       |
| **备注**   | 纯虚函数，子类必须重写实现具体任务逻辑             |


 #### 9. isStopRequested 函数
| 项目       | 详情                                   |
|----------|--------------------------------------|
| **函数原型** | bool Thread::isStopRequested() const |
| **函数名**  | isStopRequested                      |
| **功能**   | 指示是否有请求终止线程（用于在run()函数中自定义退出节点）      |
| **参数**   | 无                                    |
| **返回值**  | bool（`true`有终止请求，`false`无终止请求）       |
| **访问权限** | protected                            |
| **备注**   | 通常在run()的循环中调用，判断是否需要退出线程            |

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

### 待更新内容

已不再大规模更新

## Pub-sub model

### 介绍

- 是基于`发布者-订阅者模式`的相关实现。
- 支持`多线程`情况下的操作
- 接口实现`简单`，内存无需手动管理，足够`安全`

### 头文件

#### 接口文件

- `TopicInterface.hpp`

#### 实现文件

- `Topic.hpp`
- `Publisher.hpp`
- `Subscriber.hpp`

### 常用接口

#### 1. createPublisher 函数
| 项目       | 详情                                                                                                                                     |
|----------|----------------------------------------------------------------------------------------------------------------------------------------|
| **函数原型** | template \<typename MsgType\> <br/>PublisherPtr\<MsgType\> createPublisher(const std::string &topic_name, const int queue_length = 10) |
| **函数名**  | `createPublisher`                                                                                                                      |
| **功能**   | 创建发布者（用于向指定话题发布消息）                                                                                                                     |
| **参数**   | - `topic_name`：话题名（不同话题通过话题名区分标识）<br>- `queue_length`：话题消息队列长度（默认10，消息超过队列长度时舍弃旧消息、引入新消息）                                              |
| **返回值**  | `PublisherPtr<MsgType>`：储存发布者对象的 shared_ptr 共享指针                                                                                       |
| **访问权限** | public                                                                                                                                 |
| **备注**   | 模板函数，需指定消息类型 `MsgType`，匹配待发布消息的格式                                                                                                      |


#### 2. createSubscriber 函数
| 项目       | 详情                                                                                                                                                                                             |
|----------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **函数原型** | template \<typename MsgType\> <br />SubscriberPtr\<MsgType\> createSubscriber(const std::string &topic_name, typename Subscriber\<MsgType\>::callback_t callback, const int queue_length = 10) |
| **函数名**  | `createSubscriber`                                                                                                                                                                             |
| **功能**   | 创建话题订阅者（用于订阅指定话题并接收消息）                                                                                                                                                                         |
| **参数**   | - `topic_name`：话题名（需与发布者的话题名一致才能接收消息）<br>- `callback`：消息回调函数（类型为 `std::function<void(const MsgType &msg)>`，话题接收到消息后自动触发）<br>- `queue_length`：话题消息队列长度（默认10，消息超过队列长度时舍弃旧消息、引入新消息）               |
| **返回值**  | `SubscriberPtr<MsgType>`：储存订阅者对象的 shared_ptr 共享指针                                                                                                                                              |
| **访问权限** | public                                                                                                                                                                                         |
| **备注**   | 模板函数，需指定消息类型 `MsgType`，确保与订阅话题的消息格式匹配                                                                                                                                                          |


#### 3. publish 函数
| 项目       | 详情                                          |
|----------|---------------------------------------------|
| **函数原型** | void Publisher::publish(const MsgType &msg) |
| **函数名**  | `publish`                                   |
| **功能**   | 发布者发布信息（向已关联的话题推送消息）                        |
| **参数**   | `msg`：需要发布的信息（需与发布者的消息类型 `MsgType` 一致）      |
| **返回值**  | 无                                           |
| **访问权限** | public                                      |
| **备注**   | 需先通过 `createPublisher` 创建发布者对象，再调用此函数发布消息   |



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

### 待更新内容

已不再大规模更新

## Any类

### 介绍

- Any类是一个可以储存`任何数据`的类

### 头文件

#### 接口文件

- `Any.hpp`

### 接口

#### 1. Any构造函数

| 项目       | 详情                                           |
|----------|----------------------------------------------|
| **函数原型** | template\<typename T\><br>Any(const T &data) |
| **函数名**  | `Any`                                        |
| **功能**   | 构造一个Any类                                     |
| **参数**   | `data`: 需要储存的数据                              |
| **返回值**  | 无                                            |
| **访问权限** | public                                       |
| **备注**   | 这是一个可以隐式调用的函数                                |

#### 2. get函数

| 项目       | 详情                                              |
|----------|-------------------------------------------------|
| **函数原型** | template\<typename T\>const T &Any::get() const |
| **函数名**  | `get`                                           |
| **功能**   | 获取原始数据，不可以直接修改这个值                               |
| **参数**   |                                                 |
| **返回值**  | `const T &`: 原始数据的常量引用                          |
| **访问权限** | public                                          |
| **备注**   |                                                 |

#### 3. data函数

| 项目       | 详情                                        |
|----------|-------------------------------------------|
| **函数原型** | template\<typename T\><br>T &Any::data(); |
| **函数名**  | `data`                                    |
| **功能**   | 获取原始数据的引用，可以依此更改Any类数据                    |
| **参数**   |                                           |
| **返回值**  | `T &`: 原始数据的引用                            |
| **访问权限** | public                                    |
| **备注**   |                                           |

### 使用示例

```c++
#include <iostream>
#include "Any.hpp"


int main() {
	SorenLib::Any a = 1;
	std::cout << a.get<int>() << std::endl;
	SorenLib::Any a2 = std::string("hello");
	std::cout << a2.get<std::string>() << std::endl;
	a2.data<std::string>().append("hello");

	return 0;
}
```

### 待更新内容

已不再大规模更新

## Result

### 介绍

- 一个用于封装函数返回结果的模板类，支持同时存储正常返回值和错误信息
- 提供简洁的接口判断操作是否成功，并安全访问结果或错误信息
- 支持`void`类型的返回值场景（仅需要错误信息时）

### 头文件

#### 接口文件

- `Result.hpp`

### 接口

#### 1. 模板类定义

| 项目       | 详情                                                               |
|----------|------------------------------------------------------------------|
| **类原型**  | `template<typename ResultType, typename ErrorType> class Result` |
| **功能**   | 基础结果模板类，用于存储`ResultType`类型的返回值和`ErrorType`类型的错误信息                |
| **模板参数** | - `ResultType`: 正常返回值的类型<br>- `ErrorType`: 错误信息的类型               |
| **访问权限** | public                                                           |

#### 2. 特化类定义（void返回值）

| 项目       | 详情                                                           |
|----------|--------------------------------------------------------------|
| **类原型**  | `template<typename ErrorType> class Result<void, ErrorType>` |
| **功能**   | 针对`void`返回值的特化版本，仅存储`ErrorType`类型的错误信息                       |
| **模板参数** | - `ErrorType`: 错误信息的类型                                       |
| **访问权限** | public                                                       |

#### 3. isError 函数

| 项目       | 详情                                   |
|----------|--------------------------------------|
| **函数原型** | `[[nodiscard]] bool isError() const` |
| **函数名**  | isError                              |
| **功能**   | 判断操作是否发生错误                           |
| **参数**   | 无                                    |
| **返回值**  | `bool`（`true`表示有错误，`false`表示正常）      |
| **访问权限** | public                               |
| **备注**   | 使用`[[nodiscard]]`属性提示调用者检查返回值        |

#### 4. 构造函数（基础版本）

| 项目       | 详情                                                           |
|----------|--------------------------------------------------------------|
| **函数原型** | `Result(ResultType &&val, ErrorType &&err)`                  |
| **功能**   | 构造包含返回值和错误信息的Result对象（通常用于初始化时指定默认值）                         |
| **参数**   | - `val`: 正常返回值（右值引用）<br>- `err`: 错误信息（右值引用，内部通过unique_ptr管理） |
| **返回值**  | 无                                                            |
| **访问权限** | public                                                       |
| **备注**   | 提供默认构造函数（默认初始化值和空错误指针）                                       |

#### 5. 构造函数（void特化版本）

| 项目       | 详情                                   |
|----------|--------------------------------------|
| **函数原型** | `explicit Result(ErrorType &&err)`   |
| **功能**   | 构造仅包含错误信息的Result对象（void返回值场景）        |
| **参数**   | - `err`: 错误信息（右值引用，内部通过unique_ptr管理） |
| **返回值**  | 无                                    |
| **访问权限** | public                               |
| **备注**   | 提供默认构造函数（空错误指针）；使用`explicit`防止隐式转换   |

#### 6. value 函数

| 项目       | 详情                                |
|----------|-----------------------------------|
| **函数原型** | `const ResultType &value() const` |
| **函数名**  | value                             |
| **功能**   | 获取正常返回值的常量引用                      |
| **参数**   | 无                                 |
| **返回值**  | `const ResultType &`：返回值的常量引用     |
| **访问权限** | public                            |
| **备注**   | 仅基础版本提供，特化版本（void）无此函数            |

#### 7. error 函数

| 项目       | 详情                               |
|----------|----------------------------------|
| **函数原型** | `const ErrorType &error() const` |
| **函数名**  | error                            |
| **功能**   | 获取错误信息的常量引用                      |
| **参数**   | 无                                |
| **返回值**  | `const ErrorType &`：错误信息的常量引用    |
| **访问权限** | public                           |
| **备注**   | 调用前应通过`isError()`检查是否存在错误        |

### 使用示例

```c++
#include <iostream>
#include <string>
#include "Result.hpp"

// 基础类型使用示例
SorenLib::Result<int, std::string> divide(int a, int b) {
    if (b == 0) {
        return {0, "division by zero"}; // 返回错误信息
    }
    return {a / b, ""}; // 返回计算结果
}

// void类型使用示例
SorenLib::Result<void, std::string> validate(int value) {
    if (value < 0) {
        return SorenLib::Result<void, std::string>("value cannot be negative");
    }
    return {}; // 无错误
}

int main() {
    // 基础类型使用
    auto div_result = divide(10, 2);
    if (div_result.isError()) {
        std::cout << "Error: " << div_result.error() << std::endl;
    } else {
        std::cout << "Result: " << div_result.value() << std::endl;
    }

    // void类型使用
    auto val_result = validate(-5);
    if (val_result.isError()) {
        std::cout << "Validation error: " << val_result.error() << std::endl;
    } else {
        std::cout << "Validation successful" << std::endl;
    }

    return 0;
}
```

## Nullable

### 介绍

- 一个支持为空的模板类，可存储任意类型的数据或表示"空"状态
- 提供安全的访问接口和状态判断，避免空指针异常
- 支持值语义和移动语义，内存管理自动化

### 头文件

#### 接口文件

- `Nullable.hpp`

### 接口

#### 1. Null_t 类型

| 项目       | 详情                                   |
|----------|--------------------------------------|
| **类型定义** | `class Null_t`                       |
| **功能**   | 用于表示"空"状态的标记类型，可通过全局实例`null`使用       |
| **操作**   | 支持`bool`类型转换（始终返回`false`），用于判断是否为空状态 |
| **备注**   | 通常不直接使用，而是通过全局实例`SorenLib::null`表示空值 |


#### 2. Nullable 构造函数

| 项目       | 详情                       |
|----------|--------------------------|
| **函数原型** | `Nullable()`             |
| **功能**   | 默认构造函数，创建一个空的Nullable对象  |
| **参数**   | 无                        |
| **返回值**  | 无                        |
| **访问权限** | public                   |
| **备注**   | 初始状态为"空"（内部指针为`nullptr`） |


| 项目       | 详情                                 |
|----------|------------------------------------|
| **函数原型** | `Nullable(Null_t)`                 |
| **功能**   | 通过`null`标记构造空对象                    |
| **参数**   | `Null_t`：空标记（通常传入`SorenLib::null`） |
| **返回值**  | 无                                  |
| **访问权限** | public                             |
| **备注**   | 与默认构造函数效果相同，用于显式初始化为空              |


| 项目       | 详情                       |
|----------|--------------------------|
| **函数原型** | `Nullable(const T &val)` |
| **功能**   | 通过具体值构造Nullable对象        |
| **参数**   | `val`：需要存储的值（`T`类型的常量引用） |
| **返回值**  | 无                        |
| **访问权限** | public                   |
| **备注**   | 内部会创建`val`的副本进行存储        |


#### 3. 赋值运算符

| 项目       | 详情                                  |
|----------|-------------------------------------|
| **函数原型** | `Nullable &operator=(const T &val)` |
| **功能**   | 赋值具体值到Nullable对象                    |
| **参数**   | `val`：需要存储的值（`T`类型的常量引用）            |
| **返回值**  | 自身引用（`*this`）                       |
| **访问权限** | public                              |
| **备注**   | 若对象原为空，会创建新对象；否则更新已有值               |


| 项目       | 详情                                 |
|----------|------------------------------------|
| **函数原型** | `Nullable &operator=(Null_t)`      |
| **功能**   | 将对象设为空状态                           |
| **参数**   | `Null_t`：空标记（通常传入`SorenLib::null`） |
| **返回值**  | 自身引用（`*this`）                      |
| **访问权限** | public                             |
| **备注**   | 会释放内部存储的值（若存在）                     |


| 项目       | 详情                             |
|----------|--------------------------------|
| **函数原型** | `Nullable &operator=(T &&val)` |
| **功能**   | 通过右值引用赋值具体值                    |
| **参数**   | `val`：需要存储的右值（`T`类型的右值引用）      |
| **返回值**  | 自身引用（`*this`）                  |
| **访问权限** | public                         |
| **备注**   | 利用移动语义提高性能，适用于临时对象或可移动的资源      |


#### 4. get 函数

| 项目       | 详情                                            |
|----------|-----------------------------------------------|
| **函数原型** | `T &get()` / `const T &get() const`           |
| **函数名**  | get                                           |
| **功能**   | 获取存储的值的引用                                     |
| **参数**   | 无                                             |
| **返回值**  | 非常量/常量引用（`T &` / `const T &`）                 |
| **访问权限** | public                                        |
| **备注**   | 调用前需确保对象不为空（通过`operator bool()`判断），否则会触发未定义行为 |


#### 5. 比较运算符

| 项目       | 详情                                                |
|----------|---------------------------------------------------|
| **函数原型** | `bool operator==(const Nullable<T> &other) const` |
| **功能**   | 比较两个Nullable对象是否相等                                |
| **参数**   | `other`：另一个Nullable对象                             |
| **返回值**  | `bool`（均为空或存储的值相等时返回`true`，否则返回`false`）           |
| **访问权限** | public                                            |


| 项目       | 详情                                                |
|----------|---------------------------------------------------|
| **函数原型** | `bool operator!=(const Nullable<T> &other) const` |
| **功能**   | 比较两个Nullable对象是否不相等                               |
| **参数**   | `other`：另一个Nullable对象                             |
| **返回值**  | `bool`（`==`运算符的取反结果）                              |
| **访问权限** | public                                            |


| 项目       | 详情                                 |
|----------|------------------------------------|
| **函数原型** | `bool operator==(Null_t) const`    |
| **功能**   | 判断对象是否为空                           |
| **参数**   | `Null_t`：空标记（通常传入`SorenLib::null`） |
| **返回值**  | `bool`（为空时返回`true`，否则返回`false`）    |
| **访问权限** | public                             |


| 项目       | 详情                                 |
|----------|------------------------------------|
| **函数原型** | `bool operator!=(Null_t) const`    |
| **功能**   | 判断对象是否不为空                          |
| **参数**   | `Null_t`：空标记（通常传入`SorenLib::null`） |
| **返回值**  | `bool`（不为空时返回`true`，否则返回`false`）   |
| **访问权限** | public                             |


#### 6. 布尔转换运算符

| 项目       | 详情                                         |
|----------|--------------------------------------------|
| **函数原型** | `operator bool() const`                    |
| **功能**   | 判断对象是否存储了有效值（非空）                           |
| **参数**   | 无                                          |
| **返回值**  | `bool`（存储有效值时返回`true`，为空时返回`false`）        |
| **访问权限** | public                                     |
| **备注**   | 支持直接在条件语句中使用（如`if (nullable_obj) { ... }`） |


### 使用示例

```c++
#include <iostream>
#include "Nullable.hpp"

int main() {
    // 初始化空对象
    SorenLib::Nullable<int> num;
    if (num == SorenLib::null) {
        std::cout << "num is null" << std::endl;
    }

    // 赋值具体值
    num = 42;
    if (num != SorenLib::null) {
        std::cout << "num value: " << num.get() << std::endl;
    }

    // 移动赋值
    SorenLib::Nullable<std::string> str;
    str = std::string("hello");
    std::cout << "str value: " << str.get() << std::endl;

    // 设为空
    str = SorenLib::null;
    if (!str) {  // 利用bool转换运算符
        std::cout << "str is null" << std::endl;
    }

    // 复制操作
    SorenLib::Nullable<int> num2 = num;
    std::cout << "num2 value: " << num2.get() << std::endl;

    return 0;
}
```

### 待更新内容

暂无更新内容
