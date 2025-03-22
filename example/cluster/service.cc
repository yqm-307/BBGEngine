#include <cluster/rpc/RpcServer.hpp>
#include <bbt/network/EvThread.hpp>
#include <bbt/pollevent/Event.hpp>
#include <bbt/core/log/Logger.hpp>

class CustomService : public cluster::RpcServer
{
public:
    CustomService(std::shared_ptr<bbt::network::EvThread> evthread):
        cluster::RpcServer(evthread)
    {
    };
    virtual ~CustomService() = default;

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
    auto node = std::make_shared<CustomService>(evthread);

    node->Init({"127.0.0.1", 10021}, {"127.0.0.1", 11021}, 5000);
    auto err = node->Start();
    if (err != std::nullopt) {
        std::cout << "start failed: " << err->CWhat() << std::endl;
        return -1;
    }

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        node->Update();
    }

    node->Stop();
}