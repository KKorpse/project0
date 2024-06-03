# project0

### 运行方式
默认 ThreadPool, 9999 端口。

```
mkdir build && cd build
cmake ..
make
./MyProject
```


### 0. 目前不足

目前关注点在“能运行”，和完善可用的系统有一定差距，存在一定问题。

暂时没有统一错误处理机制，没有定义统一的错误码。

log 用的 std::cout ，定位问题信息较少。

类的抽象和封装不太完善（本来想把进程池和线程池统一接口，但是暂时没能做到）

单测数量较少，目前主要是一个框架和简单功能测试。

### 1. 线程池类实现

目录 ` src/thread_pool `

`Work` 是 std::function 的简单封装。

`WorkQueue` 用 std::queue 和 std::mutext 实现，线程安全，忙等型。

`ThreadPool` 目前只有线程数量这一个参数，主要有以下接口

* `Init()` 启动线程池，开启工作线程
* `Shutdown()` 关闭线程池
* `AddWork(std::unique_ptr<Work> pWork)` 添加一个任务（本质上就是传入一个函数指针和其对应的参数），后台工作线程以忙等模式运行，不断尝试从 work_queue 中读取任务并执行。

### 2. 进程池类实现

目录 ` src/process_pool ` ，进程池的实现不是特别优雅，比想象中复杂。

`Task`：目前做法是在 fork 前通过进程池的构造函数传入函数指针，指定该进程池的 task 函数。fork 后该函数指针及其地址在不同进程中依旧有效。

`CircleTaskQueue`：基于共享内存的环形任务队列。本质上是一个连续的共享内存空间，用于存储序列化为字符串的函数参数，所以支持不定长变量如 string。用信号量实现多进程安全，类似于生产者消费者。

`ProcessPool`：同线程池类似，主要以下接口
* Init()
* ShutDown()
* AddTask(const char *args, size_t size)：传入序列化后的函数参数和长度，比线程池更麻烦且定制性差。
* AddTask(const int fd)：传入文件描述符。文件描述符不能简单通过共享内存传输，需要用 Unix 域套接字，所以单独搞了一套机制。

### 3. Socket 类实现

`ServerSocket` 主要就是把 socket 做了个封装，主要以下接口：
* Init()：bind，listen，对某个端口进行监听。
* int Accept()：accept 的封装。
* static std::string ReadFrom(const int iClientSocket)：从 accept 获取的 fd 进行读取。
* static void SendTo(const std::string sMessage, const int iClientSocket)：回复指定 fd。

`ClientSocket` 类似，区别是无需监听端口。

### 4. EchoServer 实现，线程池版 & 进程池版

线程池版逻辑： 

单独开一个线程，使用 `ServerSocket` 监听指定端口。

对于 accept 到的 socket，封装成一个 work/task，交给线程池执行即可。

进程池版逻辑：

单独开一个线程，使用 `ServerSocket` 监听指定端口。

初始化 `ProcessPool` 对象，初始化时注册好对应的 task 函数。调用 `ProcessPool` 的 `Init()` 接口，fork 多个进程，开启进程池。

对于 accept 到的 socket，直接调用 `AddTask()` 交给进程池，会有一个进程通过 Unix 域套接字获取到该文件描述符，并且调用 task 执行 echo 操作。
