#include <cluster/registery/Registery.hpp>


class CustomRegistery : public cluster::Registery
{
public:
    CustomRegistery() = default;
    virtual ~CustomRegistery() = default;

    virtual void OnError(const bbt::errcode::Errcode& err) override
    {
        std::cout << "error: " << err.CWhat() << std::endl;
    }

    void OnInfo(const std::string& info) override
    {
        std::cout << "info: " << info << std::endl;
    }
};

int main()
{
    auto registery_tcp = std::make_shared<CustomRegistery>();

    registery_tcp->Init(bbt::net::IPAddress{"127.0.0.1", 10021}, 3000);

    registery_tcp->Start();

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}