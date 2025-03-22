#include <cluster/registery/Registery.hpp>
#include <bbt/core/log/Logger.hpp>
#include <bbt/network/EvThread.hpp>
#include <bbt/pollevent/Event.hpp>


class CustomRegistery : public cluster::Registery
{
public:
    CustomRegistery() = default;
    virtual ~CustomRegistery() = default;

    virtual void OnError(const bbt::core::errcode::Errcode& err) override
    {
        auto msg = bbt::core::clock::getnow_str() + "[error]" + err.CWhat();
        auto str = bbt::core::log::format_red(msg.c_str(), msg.size());
        std::cout << str << std::endl;
    }

    void OnInfo(const std::string& info) override
    {
        auto msg = bbt::core::clock::getnow_str() + "[info ]" + info;
        auto str = bbt::core::log::format_blue(msg.c_str(), msg.size());
        std::cout << str << std::endl;
    }
    void OnDebug(const std::string& debug) override
    {
        auto msg = bbt::core::clock::getnow_str() + "[debug]" + debug;
        auto str = bbt::core::log::format_green(msg.c_str(), msg.size());
        std::cout << str << std::endl;
    }
};

int main()
{
    auto evthread = std::make_shared<bbt::network::EvThread>(std::make_shared<bbt::pollevent::EventLoop>());
    auto registery_tcp = std::make_shared<CustomRegistery>();

    registery_tcp->Init(
        {"", 11021},
        {"", 11022},
        std::make_shared<bbt::network::TcpServer>(evthread),
        std::make_shared<bbt::network::TcpServer>(evthread),
        5000);

    registery_tcp->Start();
    auto event = evthread->RegisterEvent(0, bbt::pollevent::EventOpt::PERSIST, 
    [registery_tcp](int fd, short events, bbt::pollevent::EventId eventid) {
        registery_tcp->Update();
    });

    event->StartListen(1000);

    evthread->Start();
    evthread->Join();

    return 0;
}