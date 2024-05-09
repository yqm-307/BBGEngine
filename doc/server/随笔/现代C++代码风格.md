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

&emsp;不同的库作者提供接口的方式区别很大，尤其是有宏的存在，允许自定义一些奇怪的语法。反正我见过各种奇怪的接口实现。