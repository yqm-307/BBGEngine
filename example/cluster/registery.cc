#include <cluster/registery/Registery.hpp>


class CustomRegistery : public cluster::Registery
{
public:
    CustomRegistery() = default;
    virtual ~CustomRegistery() = default;

    virtual void OnError(const bbt::errcode::Errcode& err) override
    {
        std::cout << "[error][Registery] " << err.CWhat() << std::endl;
    }

    void OnInfo(const std::string& info) override
    {
        std::cout << "[Info] [Registery] " << info << std::endl;
    }
    void OnDebug(const std::string& debug) override
    {
        std::cout << "[debug][Registery] " << debug << std::endl;
    }
};

int main()
{
    auto registery_tcp = std::make_shared<CustomRegistery>();

    registery_tcp->Init(bbt::net::IPAddress{"127.0.0.1", 10021}, 5000);

    registery_tcp->Start();

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}