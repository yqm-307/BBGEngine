## 现代C++代码风格

---
<br>

### 1、前言

&emsp;平时使用C++很多，所以阅读、使用过很多开源代码，在这些代码又有很多不同的风格，所以想要整理出一些自己对C++风格的观点。在我的认知里面C++代码风格不是一个简单的问题，不是一个代码缩进几格、用tab或者空格、换不换行这种肤浅的问题，而是接口提供风格、异常处理、参数传递等。

&emsp;为什么C++相对于其他主流语言会有这么多这样的问题？其中一部分和语言无关的问题不做讨论，更多的问题是C++支持了很多的编程范式，同时对于每种编程范式都做了妥协（给了很多兼容的语法），导致同一个功能可以有几十种实现方式；有很多的隐式行为，让人无法感知，需要推断或者运行时才可以暴露（这里就很吃经验、语法储备）。

&emsp;后面会就异常处理、函数风格、闭包风格、全局变量等方面来讨论一下不同风格的做法。

---
<br>


### 2、C++编程风格

<br>

&emsp;下面从我认为比较多差异的方向来阐述C++不同风格的处理方式。

<br>

#### （1）错误处理

&emsp;错误处理是现代语言都支持的功能，但是不同语言核心设计理念不同导致又有不同的方式进行处理。比如异常抛出、errcode、全局error等方式，还可以通过语言层面、代码层面使用不同的错误处理方式。
<br>

* 通过代码设计来进行错误处理

    &emsp;下面是摘取自hiredis的example部分代码：

    ``` c

    int main (int argc, char **argv) {
        signal(SIGPIPE, SIG_IGN);

        redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
        // 错误处理 begin
        if (c->err) {
            printf("Error: %s\n", c->errstr);
            return 1;
        }
        // 错误处理 end

        loop = aeCreateEventLoop(64);
        redisAeAttach(loop, c);
        redisAsyncSetConnectCallback(c,connectCallback);
        redisAsyncSetDisconnectCallback(c,disconnectCallback);
        redisAsyncCommand(c, NULL, NULL, "SET key %b", argv[argc-1], strlen(argv[argc-1]));
        redisAsyncCommand(c, getCallback, (char*)"end-1", "GET key");
        aeMain(loop);
        return 0;
    }

    ```

    &emsp;可以看出，这里的错误处理，是通过redisAsyncContext内部的一个err标志位来提供，并且这个结构体内部还有errno和errstr等来辅助处理错误问题的变量。这种就是通过代码设计来提供错误处理的一种方式，此类设计经常出现于一些C语言库中，可能是因为C语言没有从语言机制上提供错误处理的方式（如果有可能也是编译器拓展提供的）。

    &emsp;个人分析一下优缺点如下：
        <br>

    - 优点：
        
        - 对于语言没有要求。任何语言都可以实现这样的错误处理，不依赖语言机制。
        - 错误容易区分。因为这种实现让错误信息和对象、结构体绑定，于是对错误处理是和错误产生对象绑定的，处理的时候更容易进行找到具体错误产生的对象。
        - 容易理解。
 
    - 缺点：

        - 容易出问题。当对象在复杂环境中传递、处理时，错误信息不好维护；且对于处理者来说多线程环境也存在风险。所以有些库干脆不关心这些问题，他们把错误处理直接和操作绑定（合理，只有操作才会导致错误），操作由使用者保证其线程安全，这也是一种主流的处理方式。

<br>

* 通过全局标志位提供错误处理

    &emsp;这种处理方式我比较熟悉的就是linux系统库了，但是了解不够深入，上代码：

    ``` c

        pthread_t th1;
        int ret = pthread_create(&th1, NULL, thr_func, NULL);
        if (ret < 0) {
            // 现在基本上都是使用返回errcode了，所以这里用ret和errno都没问题，实际上推荐使用返回值，更安全
            printf("%s\n", strerror(errno));
            exit(1);
        }

    ```

    &emsp;这种处理方式最大的问题是线程安全，如果非线程安全就可能会有问题，不过可能errno的实现是thread local的所以没有这个问题。使用errno来做错误处理的例子见的比较少所以因为没有具体分析过（性能上返回int是低开销的），所以优缺点不做说明了。

<br>

* 通过函数返回errcode来实现错误处理

    &emsp;这个也是不依赖语言机制的错误处理方式，和第一种errcode类似，但是要更进一步。通过定义一个Errcode类来承载错误信息，并且通过返回值的形式来处理。代码如下：


    &emsp;这是一个Errcode基类的实现：
    ``` cpp
    template<typename ErrType>
    class Errcode
    {
    public:
        Errcode();
        explicit Errcode(const std::string& msg, ErrType type);
        Errcode(const Errcode& other);
        Errcode(Errcode&& other);
        virtual ~Errcode();

        Errcode& operator=(const Errcode& other);
        Errcode& operator=(Errcode&& other);

        virtual const char*         CWhat() const = 0;
        virtual const std::string&  What() const = 0;
        virtual const ErrType&      Type() const = 0;

    protected:
        /* 设置信息文本 */
        void                SetMsg(const std::string& msg);
        void                SetMsg(std::string&& msg);
        /* 设置err type */
        void                SetErrType(ErrType err_type);

        const std::string&  GetMsg()        const;
        const char*         GetCMsg()       const;
        const ErrType&      GetErrType()    const;
    private:
        ErrType    m_err_type;
        std::string m_err_msg{""};
    };
    ```

    这是运用方法：
    ``` cpp
    void LuaScriptBaseComp::RegistInLuaEvent()
    {
        std::string path = std::regex_replace(std::regex_replace(m_script_path, std::regex(".lua"), ""), std::regex("//"), ".");

        auto err = m_vm->CallLuaFunction("CppCallRegist", 2, [](std::unique_ptr<bbt::cxxlua::detail::LuaStack>& stack)->std::optional<bbt::cxxlua::LuaErr>{
            bbt::cxxlua::Value msg;
            bbt::cxxlua::Value bsucc;
            auto pop_msg_err = stack->Pop(msg);
            if (pop_msg_err != std::nullopt)
                return pop_msg_err;
            
            auto pop_bsucc_err = stack->Pop(bsucc);
                return pop_bsucc_err;

            return std::nullopt;
        }, GetMemberId(), path.c_str());

        // 对函数返回的Errcode进行处理，这种处理方式和Golang默认的错误处理一样
        if (err != std::nullopt) {
            GAME_EXT1_LOG_ERROR(err.value().What().c_str());
            return;
        }
    }
    ```

    &emsp;这种错误处理方式的优缺点如下：

    - 优点

        - 语言无关。
        - 灵活。可以增加功能，因为是自己实现的。
        - 易实现。实现不足200行。
    
    - 缺点
        - 代码臃肿。毕竟要判断，但是其实也是合理的。
        - 构造对象和初始化对象需要分离。否则初始化对象失败无法通过errcode感知到，除非作为参数传入。
    
    &emsp;我最终选择以这种方式来作为我的库的主要处理方式，我觉得兼容性会比较好，如果我使用第一种、第二种方式都可以很简单的转换为Errcode方式来处理，代码如下：

    ``` cpp
    // errno处理错误
    Errcode foo() {
        int err = ::open("file.txt", O_CREAET | O_RDONLY);
        if (err != 0) {
            return Errcode(strerror(errno));
        }
    }

    // 以代码实现处理错误
    Errcode foo () {

        redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
        if (c->err) {
            printf("Error: %s\n", c->errstr);
            return Errcode(c->errstr);
        }
    }

    // 以异常处理
    Errcode foo() {
        try {
            foo();
        } catch(const std::CustomException& e) {
            return Errcode(e.what());
        }
    }

    ```

    &emsp;所以我使用Errcode的方式，就是因为很好兼容，不过好在我使用的大多数库都是以前两种形式，所以try catch的方式很少遇到，所以对try catch也不需要处理，这个等遇到再说吧。

* 通过C++异常机制来实现错误处理

    &emsp;C++提供了类似Java、C#等语言try catch的方式来实现错误处理，实例代码如下：

    ``` cpp
    #include <iostream>
    #include <exception>

    // 继承标准库中提供的异常基类，并实现一个自定义异常类
    class CustomException:
        public std::exception
    {
    public:
        CustomException(const std::string& msg):msg(msg) {}
        virtual const char* what() const throw()
        { 
            return msg.c_str();
        }
    private:    
        std::string msg;
    };

    void foo()
    {
        throw CustomException("this is a error!");
    }

    int main()
    {
        // 通过 try catch 捕获异常，并进行处理
        try {
            foo();
        } catch (const CustomException& e) {
            std::cout << e.what() << std::endl;
        }
    }
    ```

    &emsp;这种处理方式就是语言机制提供的处理方式。缺点就是和语言绑定比较死，如果语言提供这个机制才可以使用。好处就是成体系的，对于业务上的处理可以进行分类处理，但是对于内存相关的问题是无法处理的，比如访问空指针仍然会导致进程崩溃。

    &emsp;在以前我是考虑使用这种错误处理方式为主的，因为感觉比较现代。后续感觉C++主流的库都是C库占得比较多，所以感觉使用try catch并不是很好（需要接入一层）。另一个原因就是try catch有点鸡肋，因为标准库中如果抛出异常的地方大概率是自己得代码逻辑出现了问题，这种问题如果catch住了这个错误，如果自己写的代码没有对这个错误的容错性处理，那么可能会导致后续错误不断，即所谓的掩盖更大的错误，所以就感觉不是很好。

---
<br>


#### （2）接口风格
<br>

&emsp;不同的库作者提供接口的方式区别很大，尤其是有宏的存在，允许自定义一些奇怪的语法。反正我见过各种奇怪的接口实现。下面分享一下见过的几个常见库的接口提供方式，比如自定义宏、宏函数、模板、面向对象等。

<br>

* 自定义宏

    &emsp;个人感觉是最神奇、最毒瘤的api提供方式。目前用过的库里面使用这种自定义宏来提供功能的有两个库Boost.Test和libco，且看示例如下：

    &emsp;这是Boost.Test写测试代码的例程，具体测试内容不必深究，主要是两个函数BOOST_AUTO_TEST_SUITE、BOOST_AUTO_TEST_CASE等几个很特殊的非关键字。

    ``` cpp

    BOOST_AUTO_TEST_SUITE(AabbTest)

    /**
    * aabb盒碰撞检测 
    */
    BOOST_AUTO_TEST_CASE(t_aabb_check_test)
    {
        using namespace share::ecs::aoi;
        // 重叠检测
        AABBBox a;
        AABBBox b;
        AABBBox c;
        AABBBox d;
        AABBBox e;
        Box_Init(a, 1, 0, 1, 0, 1, 0);
        Box_Init(b, 1, 0, 1, 0, 2, 0);
        Box_Init(c, 3, 0, 3, 0, 3, 0);
        Box_Init(d, 2, 1, 1, 0, 1, 0);
        Box_Init(e, 2, 1, 2, 1, 2, 1);
        bool rlt1 = AABBCheck(a, a);
        BOOST_CHECK_MESSAGE(rlt1, "重叠测试失败");

        bool rlt2 = AABBCheck(a, b);
        BOOST_CHECK(rlt2);

        bool rlt3 = AABBCheck(a, c);
        BOOST_CHECK(rlt3);

        bool rlt4 = AABBCheck(a, d);
        BOOST_CHECK(!rlt4);

        bool rlt5 = AABBCheck(a, e);
        BOOST_CHECK(!rlt5);
    }

    BOOST_AUTO_TEST_SUITE_END()

    ```

    &emsp;其实这几个宏展开后，是做了注册操作。但是由于使用了宏，我们会认为它类似关键字一样的存在。这就是宏的特点，优点是很方便隐藏了很多细节，并且编译期就可以展开，不影响runtime性能；缺点是debug不方便，其次让代码变得不容易理解。

    &emsp;libco也是为了封装注册操作，下面是libco的例程：

    ``` cpp
    int main( int argc,char *argv[] )
    {
        vector< stCoClosure_t* > v;

        pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

        int total = 100;
        vector<int> v2;
        // 这里的宏
        co_ref( ref,total,v2,m);
        for(int i=0;i<10;i++)
        {
            // 这里的宏
            co_func( f,ref,i )
            {
                printf("ref.total %d i %d\n",ref.total,i );
                //lock
                pthread_mutex_lock(&ref.m);
                ref.v2.push_back( i );
                pthread_mutex_unlock(&ref.m);
                //unlock
            }
            co_func_end;
            v.push_back( new f( ref,i ) );
        }
        for(int i=0;i<2;i++)
        {
            // 这里的宏
            co_func( f2,i )
            {
                printf("i: %d\n",i);
                for(int j=0;j<2;j++)
                {
                    usleep( 1000 );
                    printf("i %d j %d\n",i,j);
                }
            }
            co_func_end;
            v.push_back( new f2( i ) );
        }

        batch_exec( v );
        printf("done\n");

        return 0;
    }
    ```

<br>

* 宏函数

    &emsp;使用宏函数比较多的库，我是用过的就是libcurl、libevent和lua，libevent中有evutil_xx相关api都是以宏的方式提供，libcurl设置http协议头都是使用宏函数，lua api基本上都是宏函数。具体原因就是因为可以提高性能，因为预编译期就会展开，但是会带来代码膨胀的问题，不过如果是做服务器开发，linux服务器进程的text段完全不会溢出。所以也不用考虑膨胀问题。

* 模板库

    &emsp;前面说到的宏相关的主要是c库，也即是c库才需要通过宏来提高运行时性能。如果使用c++完全可以使用模板来提供库功能。使用模板同样有代码膨胀的问题，但是相对宏来说编译器可以支持类型系统、类型匹配、类型推断等功能，所以一些对模板元编程了解比较多的人会使用全头文件开发完全基于模板的库，这种库用起来不管怎么样，但是阅读起来及其困难。

    ``` cpp
    // 这是一个无锁队列模板库的示例
    #include "blockingconcurrentqueue.h"

    moodycamel::BlockingConcurrentQueue<int> q;
    std::thread producer([&]() {
        for (int i = 0; i != 100; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(i % 10));
            q.enqueue(i);
        }
    });
    std::thread consumer([&]() {
        for (int i = 0; i != 100; ++i) {
            int item;
            q.wait_dequeue(item);
            assert(item == i);
            
            if (q.wait_dequeue_timed(item, std::chrono::milliseconds(5))) {
                ++i;
                assert(item == i);
            }
        }
    });
    producer.join();
    consumer.join();

    assert(q.size_approx() == 0);
    ```

* 面向对象

    &emsp;对于C++库另一种提供功能的方式就是使用面向对象。这种库容易引入一些自己得概念，比如asio会引入executor、io_context、tcpservice等来提供功能，需要对库设计进行一定的理解，否则无法理解为什么这个库每个对象的关系。其次就是这种库需要对对象的生命周期敏感，即需要文档明确其各个对象的生命周期。

    ``` cpp

    int main()
    {
        asio::io_context io_context;
        asio::ip::tcp::resolver r(io_context);
        asio::ip::tcp::resolver::query q("www.yahoo.com", "http");

        asio::ip::tcp::socket socket(io_context);
        asio::error_code ec;
        asio::connect(socket, r.resolve(q), ec);
        return 0;
    }

    ```

<br>

&emsp; 上面几种调用方式，产生的究其原因有几点：

* C、C++ API提供的方式不同。一方面是C程序员喜欢使用函数、宏来提供接口，一些C转入C++的尤其喜欢使用宏。一些C++程序员比较喜欢现代C++技术（模板、全局常量等）来替代宏。还有一部分人喜欢将两种方式组合起来使用。

* 历史原因。一些库历史比较长远原本只需要兼容C，自从C++引入面向对象后，有些想要使用C库的，做了兼容性处理创造了适配层。

&emsp;不管怎么样，在一个项目里对外提供接口应该取其中的一种，否则对于使用者来说体验绝对是毁灭性。