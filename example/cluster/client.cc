#include <cluster/rpc/RpcClient.hpp>
#include <bbt/network/EvThread.hpp>
#include <bbt/core/log/Logger.hpp>

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

    client->Init({"127.0.0.1", 11021}, 3000);

    client->Start();

    while(1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        client->Update();
    }

    client->Stop();

};        