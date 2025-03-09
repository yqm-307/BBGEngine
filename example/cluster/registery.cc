#include <cluster/registery/Registery.hpp>
#include <bbt/core/log/Logger.hpp>


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
    auto registery_tcp = std::make_shared<CustomRegistery>();

    registery_tcp->Init(bbt::core::net::IPAddress{"127.0.0.1", 10021}, 5000);

    registery_tcp->Start();

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        registery_tcp->Update();
    }

    registery_tcp->Stop();

    return 0;
}