#include <cluster/client/RpcClient.hpp>
#include <bbt/network/EvThread.hpp>
#include <bbt/core/log/Logger.hpp>
#include <bbt/pollevent/Event.hpp>

class CustomClient : public cluster::RpcClient
{
public:
    CustomClient(std::shared_ptr<bbt::network::EvThread> evthread):
        cluster::RpcClient(evthread)
    {
    };
    virtual ~CustomClient() = default;

    virtual void OnError(const util::errcode::Errcode& err) override
    {
        auto msg = bbt::core::clock::getnow_str() + "[error]" + err.What();
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
    auto client = std::make_shared<CustomClient>(evthread);

    client->Init({"127.0.0.1", 11022}, 3000);

    client->Start();

    auto event = evthread->RegisterEvent(0, bbt::pollevent::EventOpt::PERSIST, 
        [client](int fd, short events, bbt::pollevent::EventId eventid) {
            client->Update();
    });

    event->StartListen(1000);

    evthread->Start();
    evthread->Join();

};